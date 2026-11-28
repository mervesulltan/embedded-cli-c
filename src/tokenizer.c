#include "tokenizer.h"

static int is_space(char c)
{
    return c == ' ' || c == '\t';
}

tok_status_t tok_split(char *line, char *argv[], size_t max_args, size_t *argc)
{
    size_t n = 0;

    if (line == NULL || argv == NULL || argc == NULL) {
        return TOK_ERR_NULL;
    }

    *argc = 0;

    while (*line != '\0') {
        /* skip separators */
        while (is_space(*line)) {
            *line = '\0';
            line++;
        }
        if (*line == '\0') {
            break;
        }

        /* start of a word */
        if (n >= max_args) {
            return TOK_ERR_TOO_MANY;
        }
        argv[n++] = line;

        /* move to the end of the word */
        while (*line != '\0' && !is_space(*line)) {
            line++;
        }
    }

    *argc = n;
    return TOK_OK;
}