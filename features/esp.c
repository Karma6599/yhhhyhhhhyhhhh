#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>

int evasion_key_is_visual_only(const char *name)
{
    if (name == NULL)
        return 0;

    if (strcmp(name, "espEnabled") == 0 ||
        strcmp(name, "espShowTracer") == 0 ||
        strcmp(name, "espTeamFilterEnemies") == 0 ||
        strcmp(name, "espTeamFilterSelf") == 0)
        return 1;

    return strcmp(name, "espTeamFilterTeammates") == 0;
}
