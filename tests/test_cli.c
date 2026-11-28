#include <stdio.h>
#include <stdlib.h>
#include "cli.h"

static int checks = 0;

#define CHECK(cond)                                                  \
    do {                                                             \
        checks++;                                                    \
        if (!(cond)) {                                               \
            printf("FAIL: %s (line %d)\n", #cond, __LINE__);         \
            exit(1);                                                 \
        }                                                            \
    } while (0)

/* Test state: records which handler ran and with how many args */
static int    last_called = 0;
static size_t last_argc   = 0;

static int handler_a(size_t argc, char *argv[])
{
    (void)argv;
    last_called = 1;
    last_argc   = argc;
    return 0;
}

static int handler_b(size_t argc, char *argv[])
{
    (void)argv;
    last_called = 2;
    last_argc   = argc;
    return 0;
}

static int handler_fail(size_t argc, char *argv[])
{
    (void)argc;
    (void)argv;
    last_called = 3;
    return 1;                        /* non-zero = error */
}

static const cli_command_t table[] = {
    { "aaa",  handler_a,    "first command"  },
    { "bbb",  handler_b,    "second command" },
    { "fail", handler_fail, "always fails"   },
};

#define TABLE_SIZE (sizeof(table) / sizeof(table[0]))

static void test_dispatch_finds_command(void)
{
    char *argv1[] = { "aaa" };
    char *argv2[] = { "bbb", "x", "y" };

    last_called = 0;
    CHECK(cli_dispatch(table, TABLE_SIZE, 1, argv1) == CLI_OK);
    CHECK(last_called == 1);
    CHECK(last_argc == 1);

    CHECK(cli_dispatch(table, TABLE_SIZE, 3, argv2) == CLI_OK);
    CHECK(last_called == 2);
    CHECK(last_argc == 3);
}

static void test_unknown_command(void)
{
    char *argv[] = { "nope" };

    last_called = 0;
    CHECK(cli_dispatch(table, TABLE_SIZE, 1, argv) == CLI_ERR_UNKNOWN);
    CHECK(last_called == 0);          /* no handler may run */
}

static void test_empty(void)
{
    char *argv[] = { "aaa" };

    last_called = 0;
    CHECK(cli_dispatch(table, TABLE_SIZE, 0, argv) == CLI_ERR_EMPTY);
    CHECK(last_called == 0);
}

static void test_handler_error(void)
{
    char *argv[] = { "fail" };

    CHECK(cli_dispatch(table, TABLE_SIZE, 1, argv) == CLI_ERR_HANDLER);
    CHECK(last_called == 3);
}

static void test_null(void)
{
    char *argv[] = { "aaa" };

    CHECK(cli_dispatch(NULL, TABLE_SIZE, 1, argv) == CLI_ERR_NULL);
    CHECK(cli_dispatch(table, TABLE_SIZE, 1, NULL) == CLI_ERR_NULL);
}

int main(void)
{
    test_dispatch_finds_command();
    test_unknown_command();
    test_empty();
    test_handler_error();
    test_null();

    printf("CLI: all tests passed (%d checks)\n", checks);
    return 0;
}