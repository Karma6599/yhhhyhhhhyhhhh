#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>

#define PROP_FEATURE 0
#define PROP_PARAM   1

extern int32_t evasion_get_requested(const char *name);
extern int32_t evasion_prop_kind(const char *name);
extern int32_t evasion_prop_dependency_gated(const char *name);

int evasion_prediction_alias_ok(const char *name)
{
    if (name == NULL)
        return 0;
    if (evasion_prop_kind(name) != PROP_FEATURE)
        return 0;
    return evasion_prop_dependency_gated(name) == 0;
}

int evasion_prediction_version_ok(uint32_t dep_flags, int predict_enabled)
{
    int32_t target_mode = evasion_get_requested("aopTargetMode");
    if (target_mode < 0 || target_mode >= 3)
        return 0;

    if (predict_enabled == 0)
        return 1;

    if ((dep_flags >> 2) & 1u) {
        int32_t version = evasion_get_requested("aopPredictVersion");
        if (version > 0 && version < 3)
            return 1;
    }
    return 0;
}

int evasion_prediction_dependency_ok(uint32_t dep_flags)
{
    return evasion_prediction_version_ok(
        dep_flags, (int)evasion_get_requested("aopPredictEnabled"));
}
