// Functional test for string_t

#include <stdio.h>
#include <pklstr.h>

int main(int argc, char **argv) {
    printf("Testing allocation...\n");
    string_result_t string_res = string_from("Hello, world", CHAR_DEF_ALLOC);
    if (string_res.is_err) {
        fprintf(stderr, "Error occurred: %lu", (size_t) string_res.err.code);
        if (string_res.err.msg) {
            fprintf(stderr, " - %s", string_res.err.msg);
        }
        fprintf(stderr, "\n");
        return 1;
    }
    string_t string = string_res.ok;
    printf(
        "Result: { .len = %lu, .cap = %lu, .str = '%s' }\n",
        string.len, string.cap, string.str
    );

    printf("Testing char append...\n");
    result_t char_append_res = string_append_char(&string, '!');
    if (char_append_res.is_err) {
        fprintf(stderr, "Error occurred: %lu", (size_t) char_append_res.err.code);
        if (string_res.err.msg) {
            fprintf(stderr, " - %s", char_append_res.err.msg);
        }
        fprintf(stderr, "\n");
        return 1;
    }
    printf(
        "Result: { .len = %lu, .cap = %lu, .str = '%s' }\n",
        string.len, string.cap, string.str
    );

    printf("Testing str append (no resize)...\n");
    result_t str_append_res1 = string_append_str(&string, " More!");
    if (str_append_res1.is_err) {
        fprintf(stderr, "Error occurred: %lu", (size_t) str_append_res1.err.code);
        if (str_append_res1.err.msg) {
            fprintf(stderr, " - %s", str_append_res1.err.msg);
        }
        fprintf(stderr, "\n");
        return 1;
    }
    printf(
        "Result: { .len = %lu, .cap = %lu, .str = '%s' }\n",
        string.len, string.cap, string.str
    );

    printf("Testing str append (double)...\n");
    result_t str_append_res2 = string_append_str(&string, " More!");
    if (str_append_res2.is_err) {
        fprintf(stderr, "Error occurred: %lu", (size_t) str_append_res2.err.code);
        if (str_append_res2.err.msg) {
            fprintf(stderr, " - %s", str_append_res2.err.msg);
        }
        fprintf(stderr, "\n");
        return 1;
    }
    printf(
        "Result: { .len = %lu, .cap = %lu, .str = '%s' }\n",
        string.len, string.cap, string.str
    );

    printf("Testing str append (more than double)...\n");
    result_t str_append_res3 = string_append_str(&string, " Way Way Way Way Way Way More!");
    if (str_append_res3.is_err) {
        fprintf(stderr, "Error occurred: %lu", (size_t) str_append_res3.err.code);
        if (str_append_res3.err.msg) {
            fprintf(stderr, " - %s", str_append_res3.err.msg);
        }
        fprintf(stderr, "\n");
        return 1;
    }
    printf(
        "Result: { .len = %lu, .cap = %lu, .str = '%s' }\n",
        string.len, string.cap, string.str
    );

    printf("Testing string append...\n");
    string_result_t string_res2 = string_from(" Even More", CHAR_DEF_ALLOC);
    if (string_res2.is_err) {
        fprintf(stderr, "Error occurred: %lu", (size_t) string_res2.err.code);
        if (string_res2.err.msg) {
            fprintf(stderr, " - %s", string_res2.err.msg);
        }
        fprintf(stderr, "\n");
        return 1;
    }
    string_t string2 = string_res2.ok;
    result_t string_append_res = string_append_string(&string, &string2);
    if (string_append_res.is_err) {
        fprintf(stderr, "Error occurred: %lu", (size_t) string_append_res.err.code);
        if (string_append_res.err.msg) {
            fprintf(stderr, " - %s", string_append_res.err.msg);
        }
        fprintf(stderr, "\n");
        return 1;
    }
    string_free(&string2);
    printf(
        "Result: { .len = %lu, .cap = %lu, .str = '%s' }\n",
        string.len, string.cap, string.str
    );

    string_free(&string);
    return 0;
}

