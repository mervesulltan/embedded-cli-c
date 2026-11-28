#include "line_reader.h"

void lr_init(line_reader_t *lr)
{
    if (lr == NULL) {
        return;
    }

    rb_init(&lr->rx, lr->rx_storage, sizeof(lr->rx_storage));
    lr->line[0]    = '\0';
    lr->length     = 0;
    lr->overflowed = false;
}

bool lr_put_char(line_reader_t *lr, char c)
{
    if (lr == NULL) {
        return false;
    }
    return rb_push(&lr->rx, (uint8_t)c) == RB_OK;
}

lr_status_t lr_get_line(line_reader_t *lr, const char **out_line)
{
    uint8_t byte;

    if (lr == NULL || out_line == NULL) {
        return LR_NO_LINE;
    }

    while (rb_pop(&lr->rx, &byte) == RB_OK) {
        char c = (char)byte;

        if (c == '\r' || c == '\n') {
            /* End of line */
            if (lr->overflowed) {
                lr->length     = 0;
                lr->line[0]    = '\0';
                lr->overflowed = false;
                return LR_OVERFLOW;
            }
            if (lr->length == 0) {
                continue;               /* empty line (e.g. the \n of \r\n): ignore */
            }
            lr->line[lr->length] = '\0';
            lr->length = 0;             /* next call starts a new line */
            *out_line = lr->line;
            return LR_LINE_READY;
        }

        if (c == '\b' || c == 127) {
            /* Backspace: remove last character */
            if (lr->length > 0 && !lr->overflowed) {
                lr->length--;
            }
            continue;
        }

        /* Normal character */
        if (lr->length < LR_LINE_MAX) {
            lr->line[lr->length++] = c;
        } else {
            lr->overflowed = true;      /* too long: drop until Enter */
        }
    }

    return LR_NO_LINE;
}