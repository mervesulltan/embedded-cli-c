#include "cli.h"
#include <string.h>

cli_status_t cli_dispatch(const cli_command_t *table, size_t table_size,
                          size_t argc, char *argv[])
{
    size_t i;

    if (table == NULL || argv == NULL) {
        return CLI_ERR_NULL;
    }
    if (argc == 0) {
        return CLI_ERR_EMPTY;
    }

    for (i = 0; i < table_size; i++) {
        if (strcmp(argv[0], table[i].name) == 0) {
            int rc = table[i].handler(argc, argv);
            return (rc == 0) ? CLI_OK : CLI_ERR_HANDLER;
        }
    }

    return CLI_ERR_UNKNOWN;
}