#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <math.h>

extern int *__errno(void);
extern void log_event(const char *category, const char *event, const char *json);

extern uintptr_t g_game_base;
extern uintptr_t game_read(uintptr_t handle, uintptr_t addr, void *out, uint32_t len);
extern uintptr_t battle_reader_open(void);
extern int performance_client_ready(void);
extern int runtime_observers_ready(void);
extern int code_observer_install(uint32_t rva, uint32_t size, const char *sha256,
                                 uint32_t probe_rva, uint32_t tag, void (*callback)(void *));
extern int resolve_field_ptr(uintptr_t addr, uintptr_t *out);

extern void *g_snapshot_keys_fn;
extern int   g_snapshot_keys_ready;
extern uint32_t g_perf_active;

typedef int (*snapshot_keys_fn_t)(const char *const *keys, int32_t *triples,
                                  int count, int64_t *epoch, int32_t *status);

#define SNAPSHOT_TRIPLE_REQUESTED 0
#define SNAPSHOT_TRIPLE_EFFECTIVE 1
#define SNAPSHOT_TRIPLE_FLAGS     2
#define SNAPSHOT_FLAG_ACTIVE      2

#define OUTLINE_BATTLE_FRAME_OFF 0x1307e20u

#define OUTLINE_SCENE_MAGIC 0x3b808081
#define OUTLINE_ONE_255F    0.003921569f

extern uint32_t g_outline_observers_ready;
extern uint64_t g_outline_env_calls;
extern uint64_t g_outline_scene_calls;
extern uint64_t g_outline_env_applied;
extern uint64_t g_outline_scene_applied;
extern uint64_t g_outline_env_refusals[16];
extern uint64_t g_outline_scene_refusals[16];

typedef struct {
    uint64_t frame;
    uint32_t kind;
    uint32_t _pad;
    uintptr_t reader;
    uint64_t epoch;
    float    r;
    float    g;
    float    b;
    float    a;
} outline_ctx_t;

extern int outline_colors_read(outline_ctx_t *ctx);

#define OUTLINE_ENV_SITE 0
#define OUTLINE_SCENE_SITE 1

static uint64_t atomic_inc(uint64_t *slot)
{
    uint64_t prev;
    do {
        prev = __atomic_load_n(slot, __ATOMIC_RELAXED);
    } while (!__atomic_compare_exchange_n(slot, &prev, prev + 1,
                                           false, __ATOMIC_RELAXED, __ATOMIC_RELAXED));
    return prev;
}

static void outline_refuse(uint64_t *latches, uint32_t site, uint32_t reason)
{
    if (atomic_inc(&latches[reason]) != 0)
        return;
    char json[0x40];
    snprintf(json, sizeof json, ",\"site\":%u,\"reason\":%u", site, reason);
    log_event("outline_native", "register_observer_refused", json);
}

int outline_context_fetch(outline_ctx_t *ctx, uint32_t *stage)
{
    memset(ctx, 0, sizeof *ctx);
    *stage = 1;

    if (g_outline_observers_ready != 1 || g_perf_active == 0
        || g_snapshot_keys_ready != 1)
        return 0;

    *stage = 2;
    uintptr_t handle = (uintptr_t)performance_client_ready();
    if (handle == 0)
        return 0;

    *stage = 3;
    if (game_read(handle, g_game_base + OUTLINE_BATTLE_FRAME_OFF, &ctx->frame, 8) == 0)
        return 0;
    if (ctx->frame + 0x2000 >= 0x12000 || (ctx->frame & 7) != 0)
        return 0;

    if (game_read(0, ctx->frame + 0x50, &ctx->kind, 4) == 0)
        return 0;
    if (ctx->kind != 4) {
        if (ctx->kind != 5)
            return 0;
        *stage = 4;
        ctx->reader = battle_reader_open();
        if (ctx->reader == 0)
            return 0;
    }

    *stage = 5;
    static const char *const keys[5] = {
        "characterOutlineEnabled",
        "outlineColorR",
        "outlineColorG",
        "outlineColorB",
        "outlineOpacity",
    };

    if (g_snapshot_keys_fn == NULL)
        return 0;

    int32_t triples[15];
    int64_t epoch = 0;
    int32_t status = -1;

    snapshot_keys_fn_t query = (snapshot_keys_fn_t)g_snapshot_keys_fn;
    if (query(keys, triples, 5, &epoch, &status) != 1)
        return 0;
    if (epoch == 0 || status != 0)
        return 0;
    if (triples[0 * 3 + SNAPSHOT_TRIPLE_EFFECTIVE] != 1
        || triples[0 * 3 + SNAPSHOT_TRIPLE_FLAGS] != SNAPSHOT_FLAG_ACTIVE)
        return 0;

    *stage = 6;
    for (int i = 1; i <= 4; i++) {
        if (triples[i * 3 + SNAPSHOT_TRIPLE_EFFECTIVE] != 1)
            return 0;
        int32_t req = triples[i * 3 + SNAPSHOT_TRIPLE_REQUESTED];
        if (req != triples[i * 3 + SNAPSHOT_TRIPLE_EFFECTIVE] || req > 1000)
            return 0;
    }

    ctx->epoch = (uint64_t)epoch;
    ctx->r = (float)triples[1 * 3 + SNAPSHOT_TRIPLE_REQUESTED] / 1000.0f;
    ctx->g = (float)triples[2 * 3 + SNAPSHOT_TRIPLE_REQUESTED] / 1000.0f;
    ctx->b = (float)triples[3 * 3 + SNAPSHOT_TRIPLE_REQUESTED] / 1000.0f;
    ctx->a = (float)triples[4 * 3 + SNAPSHOT_TRIPLE_REQUESTED] / 1000.0f;
    return 1;
}

static int float_eq(float a, float b, float eps)
{
    float d = a - b;
    if (d < 0.0f)
        d = -d;
    return isfinite(d) && d <= eps;
}

static int channel_ok(float scale, float value, uint8_t byte)
{
    if (!isfinite(scale) || !float_eq(scale, OUTLINE_ONE_255F, 2e-6f))
        return 0;
    if (!isfinite(value))
        return 0;
    return float_eq(value, scale * (float)byte, 2e-6f);
}

void outline_environment_observer(void *obs)
{
    int *errno_ptr = __errno();
    int saved_errno = *errno_ptr;
    uint32_t reason = 7;

    atomic_inc(&g_outline_env_calls);

    outline_ctx_t ctx;
    uint32_t stage = 7;

    if (obs == NULL)
        goto refuse;

    if (outline_context_fetch(&ctx, &stage) == 0) {
        if ((uint32_t)(stage - 10) < 0xfffffff7u)
            goto out;
        outline_refuse(g_outline_env_refusals, OUTLINE_ENV_SITE, stage);
        goto out;
    }

    {
        uint64_t *o = (uint64_t *)obs;
        if (o[0x98 / 8] == 0 || o[0x48 / 8] == 0 || o[0x40 / 8] == 0)
            goto refuse7;

        uint64_t position_handle = 0;
        uintptr_t handle = game_read(0, (uintptr_t)o[0x98 / 8] + 0x10,
                                     &position_handle, 8);
        if (handle == 0)
            goto refuse7;

        if (position_handle + 0x2000 >= 0x12000 || (position_handle & 7) != 0
            || position_handle != o[0x48 / 8])
            goto refuse;

        uint64_t resolve_src = 0;
        if (game_read(handle, (uintptr_t)o[0x98 / 8] + 0x18, &resolve_src, 8) == 0)
            goto refuse7;

        if (resolve_src + 0x2000 >= 0x12000 || (resolve_src & 7) != 0)
            goto refuse;

        uintptr_t resolved = 0;
        if (resolve_field_ptr(resolve_src + 0xae0, &resolved) == 0
            || resolved != o[0x40 / 8])
            goto refuse7;

        uint8_t rgba[4];
        if (game_read(handle, resolved + 9, rgba, 4) == 0)
            goto refuse7;

        float *f = (float *)((char *)obs + 0x110);
        if (!channel_ok(*(float *)((char *)obs + 0x120), f[0], rgba[0])
            || !channel_ok(*(float *)((char *)obs + 0x124), f[1], rgba[1])
            || !channel_ok(*(float *)((char *)obs + 0x128), f[2], rgba[2])
            || !channel_ok(*(float *)((char *)obs + 0x12c), f[3], rgba[3]))
            goto refuse;

        if (outline_colors_read(&ctx) == 0) {
            reason = 9;
            goto refuse;
        }

        f[0] = *(float *)((char *)obs + 0x120) * (float)(int)(ctx.r * 255.0f);
        f[1] = *(float *)((char *)obs + 0x124) * (float)(int)(ctx.g * 255.0f);
        f[2] = *(float *)((char *)obs + 0x128) * (float)(int)(ctx.b * 255.0f);
        f[3] = *(float *)((char *)obs + 0x12c) * (float)(int)(ctx.a * 255.0f);

        if (atomic_inc(&g_outline_env_applied) != 0)
            goto out;
        log_event("outline_native", "environment_rgba_applied", NULL);
        goto out;
    }

refuse7:
    reason = 7;
refuse:
    outline_refuse(g_outline_env_refusals, OUTLINE_ENV_SITE, reason);
out:
    *errno_ptr = saved_errno;
}

static int unit_range(float v)
{
    return v >= 0.0f && v <= 1.0f;
}

void outline_scene_observer(void *obs)
{
    int *errno_ptr = __errno();
    int saved_errno = *errno_ptr;
    uint32_t reason = 7;

    atomic_inc(&g_outline_scene_calls);

    outline_ctx_t ctx;
    uint32_t stage = 7;

    if (obs == NULL)
        goto refuse;

    if (outline_context_fetch(&ctx, &stage) == 0) {
        if ((uint32_t)(stage - 10) < 0xfffffff7u)
            goto out;
        outline_refuse(g_outline_scene_refusals, OUTLINE_SCENE_SITE, stage);
        goto out;
    }

    {
        uint64_t *o = (uint64_t *)obs;
        if (o[0xc0 / 8] == 0 || o[0x40 / 8] == 0 || o[0x48 / 8] == 0)
            goto refuse7;

        uint64_t scene_a = 0;
        uintptr_t handle = game_read(0, (uintptr_t)o[0xc0 / 8], &scene_a, 8);
        if (handle == 0)
            goto refuse7;

        if (scene_a + 0x2000 >= 0x12000 || (scene_a & 7) != 0 || scene_a != o[0x40 / 8])
            goto refuse;

        uint64_t scene_b = 0;
        if (game_read(handle, (uintptr_t)o[0xc0 / 8] + 8, &scene_b, 8) == 0)
            goto refuse7;

        if (scene_b + 0x2000 >= 0x12000 || (scene_b & 7) != 0
            || scene_b != o[0x48 / 8] || o[0x50 / 8] != scene_a + 0x1a4)
            goto refuse;

        float scene_rgba[4];
        if (game_read(handle, (uintptr_t)o[0x50 / 8], scene_rgba, 0x10) == 0)
            goto refuse7;

        uint8_t flag = 0;
        if (game_read(handle, scene_a + 0xb, &flag, 1) == 0 || flag == 0)
            goto refuse;

        uintptr_t resolved = 0;
        if (resolve_field_ptr((uintptr_t)obs + 0x338, &resolved) == 0)
            goto refuse7;

        uint8_t alpha_byte = 0;
        if (game_read(handle, resolved + 3, &alpha_byte, 1) == 0)
            goto refuse7;

        if (*(uint32_t *)((char *)obs + 0xc8) != OUTLINE_SCENE_MAGIC) {
            reason = 8;
            goto refuse;
        }

        if (!float_eq(*(float *)((char *)obs + 0x120), scene_rgba[0], 2e-6f)
            || !float_eq(*(float *)((char *)obs + 0x124), scene_rgba[1], 2e-6f)
            || !float_eq(*(float *)((char *)obs + 0x130), scene_rgba[2], 2e-6f))
            goto refuse8;

        if (!isfinite(*(float *)((char *)obs + 0x110))
            || !float_eq(*(float *)((char *)obs + 0x110),
                         scene_rgba[3] * OUTLINE_ONE_255F * (float)alpha_byte, 2e-6f))
            goto refuse8;

        if (!unit_range(scene_rgba[0]) || !unit_range(scene_rgba[1])
            || !unit_range(scene_rgba[2]) || !unit_range(scene_rgba[3])
            || !isfinite(scene_rgba[3]))
            goto refuse8;

        if (outline_colors_read(&ctx) == 0) {
            reason = 9;
            goto refuse;
        }

        *(float *)((char *)obs + 0x120) = ctx.r;
        *(float *)((char *)obs + 0x124) = ctx.g;
        *(float *)((char *)obs + 0x130) = ctx.b;
        *(float *)((char *)obs + 0x110) = ctx.a * OUTLINE_ONE_255F * (float)alpha_byte;

        if (atomic_inc(&g_outline_scene_applied) != 0)
            goto out;
        log_event("outline_native", "scene_rgba_applied", NULL);
        goto out;
    }

refuse7:
    reason = 7;
    goto refuse;
refuse8:
    reason = 8;
refuse:
    outline_refuse(g_outline_scene_refusals, OUTLINE_SCENE_SITE, reason);
out:
    *errno_ptr = saved_errno;
}

uint32_t outline_register_observers(void)
{
    if (runtime_observers_ready() == 0)
        return 0;

    int env = code_observer_install(
        0x7ff030, 0x35d4,
        "2829b8cd1249c61c0fb4974f8f97f5a62715fd5f4802de9e9c08737fcb03ab6d",
        0x800a1c, 0x3d809d20, outline_environment_observer);
    int scene = code_observer_install(
        0x7c7c44, 0x13e0,
        "7b1f29aa558338aecdc24cd1d6e9839216b180e1cd0828d552c435061062a92a",
        0x7c88b0, 0xbd027922, outline_scene_observer);

    uint32_t ok = (env != 0 && scene != 0) ? 1 : 0;
    g_outline_observers_ready = ok;
    log_event("outline_native",
              ok ? "register_observers_ready" : "register_observers_unavailable",
              NULL);
    return ok;
}

char *outline_shader_patch(const char *src, size_t len)
{
    if (src == NULL)
        return NULL;
    if (len - 0x40001 > 0xfffffffffffbffffull)
        return NULL;
    if (memchr(src, 0, len) != NULL)
        return NULL;
    if (src[len] != '\0')
        return NULL;
    if (strstr(src, "u_nexusOutline") != NULL)
        return NULL;

    const char *main_pos = strstr(src, "void main");
    if (main_pos == NULL)
        return NULL;

    size_t occurrences = 0;
    for (const char *p = strstr(src, "vec4(v_outlineColor, 1.0)");
         p != NULL; p = strstr(p + 0x19, "vec4(v_outlineColor, 1.0)"))
        occurrences++;

    if (occurrences == 0 || occurrences > 0x40)
        return NULL;

    char *out = malloc(len + 0x24 + occurrences * 0x2c);
    if (out == NULL)
        return NULL;

    static const char uniform_decl[] = "uniform highp vec4 u_nexusOutline;\n";
    static const char color_select[] =
        "(u_nexusOutline.a < 0.0 ? vec4(v_outlineColor, 1.0) : u_nexusOutline)";

    const char *in = src;
    char *wr = out;

    while (*in != '\0') {
        if (in == main_pos) {
            memcpy(wr, uniform_decl, 0x23);
            wr += 0x23;
        }
        if (strncmp(in, "vec4(v_outlineColor, 1.0)", 0x19) == 0) {
            memcpy(wr, color_select, 0x45);
            wr += 0x45;
            in += 0x19;
            continue;
        }
        *wr++ = *in++;
    }
    *wr = '\0';
    return out;
}
