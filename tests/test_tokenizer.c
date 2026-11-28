#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tokenizer.h"

static int checks = 0;

#define CHECK(cond)                                                  \
    do {                                                             \
        checks++;                                                    \
        if (!(cond)) {                                               \
            printf("FAIL: %s (line %d)\n", #cond, __LINE__);         \
            exit(1);                                                 \
        }                                                            \
    } while (0)

static void test_basic(void)
{
    char line[] = "set blink 500";
    char *argv[TOK_MAX_ARGS];
    size_t argc = 0;

    CHECK(tok_split(line, argv, TOK_MAX_ARGS, &argc) == TOK_OK);
    CHECK(argc == 3);
    CHECK(strcmp(argv[0], "set") == 0);
    CHECK(strcmp(argv[1], "blink") == 0);
    CHECK(strcmp(argv[2], "500") == 0);
}

static void test_extra_spaces(void)
{
    char line[] = "   led \t  on   ";
    char *argv[TOK_MAX_ARGS];
    size_t argc = 0;

    CHECK(tok_split(line, argv, TOK_MAX_ARGS, &argc) == TOK_OK);
    CHECK(argc == 2);
    CHECK(strcmp(argv[0], "led") == 0);
    CHECK(strcmp(argv[1], "on") == 0);
}

static void test_single_word(void)
{
    char line[] = "status";
    char *argv[TOK_MAX_ARGS];
    size_t argc = 0;

    CHECK(tok_split(line, argv, TOK_MAX_ARGS, &argc) == TOK_OK);
    CHECK(argc == 1);
    CHECK(strcmp(argv[0], "status") == 0);
}

static void test_empty_and_blank(void)
{
    char empty[] = "";
    char blank[] = "    ";
    char *argv[TOK_MAX_ARGS];
    size_t argc = 99;

    CHECK(tok_split(empty, argv, TOK_MAX_ARGS, &argc) == TOK_OK);
    CHECK(argc == 0);

    argc = 99;
    CHECK(tok_split(blank, argv, TOK_MAX_ARGS, &argc) == TOK_OK);
    CHECK(argc == 0);
}

static void test_too_many(void)
{
    char line[] = "a b c d";
    char *argv[3];
    size_t argc = 0;

    CHECK(tok_split(line, argv, 3, &argc) == TOK_ERR_TOO_MANY);
}

static void test_null(void)
{
    char line[] = "x";
    char *argv[TOK_MAX_ARGS];
    size_t argc = 0;

    CHECK(tok_split(NULL, argv, TOK_MAX_ARGS, &argc) == TOK_ERR_NULL);
    CHECK(tok_split(line, NULL, TOK_MAX_ARGS, &argc) == TOK_ERR_NULL);
    CHECK(tok_split(line, argv, TOK_MAX_ARGS, NULL) == TOK_ERR_NULL);
}

int main(void)
{
    test_basic();
    test_extra_spaces();
    test_single_word();
    test_empty_and_blank();
    test_too_many();
    test_null();

    printf("Tokenizer: all tests passed (%d checks)\n", checks);
    return 0;
}