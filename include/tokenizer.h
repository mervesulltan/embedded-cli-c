#ifndef TOKENIZER_H
#define TOKENIZER_H

#include <stddef.h>

#define TOK_MAX_ARGS 8

typedef enum {
    TOK_OK = 0,
    TOK_ERR_NULL,       /* NULL pointer passed */
    TOK_ERR_TOO_MANY    /* more than max_args tokens */
} tok_status_t;

/* Split `line` in place into words separated by spaces/tabs.
 * The line is modified: separators are replaced by '\0'.
 * argv[i] point into `line`. *argc receives the word count. */
tok_status_t tok_split(char *line, char *argv[], size_t max_args, size_t *argc);

#endif /* TOKENIZER_H */