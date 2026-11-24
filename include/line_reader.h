#ifndef LINE_READER_H
#define LINE_READER_H

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include "ring_buffer.h"

#define LR_LINE_MAX 64   /* max characters in one line, excluding the '\0' */

typedef enum {
    LR_NO_LINE = 0,   /* line not complete yet */
    LR_LINE_READY,    /* a full line is available */
    LR_OVERFLOW       /* line was too long and has been dropped */
} lr_status_t;

typedef struct {
    ring_buffer_t rx;                  /* incoming characters (e.g. from UART ISR) */
    uint8_t       rx_storage[128];     /* storage for the ring buffer */
    char          line[LR_LINE_MAX + 1]; /* line being built, always '\0' terminated */
    size_t        length;              /* characters currently in line */
    bool          overflowed;          /* true if the current line is too long */
} line_reader_t;

void        lr_init(line_reader_t *lr);

/* Feed one received character. Returns false if the rx buffer is full. */
bool        lr_put_char(line_reader_t *lr, char c);

/* Process buffered characters. When a full line is found, a pointer to it
 * is stored in *out_line (valid until the next call). */
lr_status_t lr_get_line(line_reader_t *lr, const char **out_line);

#endif /* LINE_READER_H */