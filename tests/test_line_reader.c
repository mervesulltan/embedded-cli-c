#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "line_reader.h"

static int checks = 0;

#define CHECK(cond)                                                  \
    do {                                                             \
        checks++;                                                    \
        if (!(cond)) {                                               \
            printf("FAIL: %s (line %d)\n", #cond, __LINE__);         \
            exit(1);                                                 \
        }                                                            \
    } while (0)

/* Helper: feed a whole string into the line reader */
static void feed(line_reader_t *lr, const char *s)
{
    while (*s != '\0') {
        lr_put_char(lr, *s);
        s++;
    }
}

static void test_simple_line(void)
{
    line_reader_t lr;
    const char *line = NULL;

    lr_init(&lr);
    feed(&lr, "led on\n");

    CHECK(lr_get_line(&lr, &line) == LR_LINE_READY);
    CHECK(strcmp(line, "led on") == 0);
    CHECK(lr_get_line(&lr, &line) == LR_NO_LINE);
}

static void test_incomplete_line(void)
{
    line_reader_t lr;
    const char *line = NULL;

    lr_init(&lr);
    feed(&lr, "led");
    CHECK(lr_get_line(&lr, &line) == LR_NO_LINE);   /* no Enter yet */

    feed(&lr, " on\n");                              /* rest arrives later */
    CHECK(lr_get_line(&lr, &line) == LR_LINE_READY);
    CHECK(strcmp(line, "led on") == 0);
}

static void test_crlf(void)
{
    line_reader_t lr;
    const char *line = NULL;

    lr_init(&lr);
    feed(&lr, "status\r\n");

    CHECK(lr_get_line(&lr, &line) == LR_LINE_READY);
    CHECK(strcmp(line, "status") == 0);
    CHECK(lr_get_line(&lr, &line) == LR_NO_LINE);    /* \n must not create an empty line */
}

static void test_two_lines(void)
{
    line_reader_t lr;
    const char *line = NULL;

    lr_init(&lr);
    feed(&lr, "one\ntwo\n");

    CHECK(lr_get_line(&lr, &line) == LR_LINE_READY);
    CHECK(strcmp(line, "one") == 0);
    CHECK(lr_get_line(&lr, &line) == LR_LINE_READY);
    CHECK(strcmp(line, "two") == 0);
    CHECK(lr_get_line(&lr, &line) == LR_NO_LINE);
}

static void test_backspace(void)
{
    line_reader_t lr;
    const char *line = NULL;

    lr_init(&lr);
    feed(&lr, "ledd\b on\n");        /* typo, then backspace */

    CHECK(lr_get_line(&lr, &line) == LR_LINE_READY);
    CHECK(strcmp(line, "led on") == 0);
}

static void test_overflow(void)
{
    line_reader_t lr;
    const char *line = NULL;
    int i;

    lr_init(&lr);
    for (i = 0; i < LR_LINE_MAX + 10; i++) {
        lr_put_char(&lr, 'a');
        /* drain regularly so the rx buffer does not fill up */
        CHECK(lr_get_line(&lr, &line) == LR_NO_LINE);
    }
    lr_put_char(&lr, '\n');
    CHECK(lr_get_line(&lr, &line) == LR_OVERFLOW);

    /* reader must work normally after an overflow */
    feed(&lr, "ok\n");
    CHECK(lr_get_line(&lr, &line) == LR_LINE_READY);
    CHECK(strcmp(line, "ok") == 0);
}

static void test_empty_lines_ignored(void)
{
    line_reader_t lr;
    const char *line = NULL;

    lr_init(&lr);
    feed(&lr, "\n\n\n");
    CHECK(lr_get_line(&lr, &line) == LR_NO_LINE);
}

int main(void)
{
    test_simple_line();
    test_incomplete_line();
    test_crlf();
    test_two_lines();
    test_backspace();
    test_overflow();
    test_empty_lines_ignored();

    printf("Line reader: all tests passed (%d checks)\n", checks);
    return 0;
}