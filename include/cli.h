#ifndef CLI_H
#define CLI_H

#include <stddef.h>

typedef enum {
    CLI_OK = 0,
    CLI_ERR_NULL,        /* NULL pointer passed */
    CLI_ERR_EMPTY,       /* no command given */
    CLI_ERR_UNKNOWN,     /* command not found in table */
    CLI_ERR_HANDLER      /* the command handler reported an error */
} cli_status_t;

/* A command handler receives the arguments (argv[0] is the command name)
 * and returns 0 on success or a non-zero value on error. */
typedef int (*cli_handler_t)(size_t argc, char *argv[]);

typedef struct {
    const char    *name;     /* command name typed by the user */
    cli_handler_t  handler;  /* function to call */
    const char    *help;     /* one-line description for the help command */
} cli_command_t;

/* Look up argv[0] in `table` and call its handler. */
cli_status_t cli_dispatch(const cli_command_t *table, size_t table_size,
                          size_t argc, char *argv[]);

#endif /* CLI_H */