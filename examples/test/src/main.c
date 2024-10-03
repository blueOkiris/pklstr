// Functional test for string_t

#include <stdio.h>
#include <pklstr.h>

int main(int argc, char **argv) {
    printf("Testing allocation...\n");
    string_result_t string_res = string_from("Hello, world", CHAR_DEF_ALLOC);
    if (string_res.is_err) {
        fprintf(stderr, "Error occurred: %lu\n", (size_t) string_res.err);
        return 1;
    }
    string_t string = string_res.ok;
    printf(
        "Result: { .len = %lu, .cap = %lu, .str = '%s' }\n",
        string.len, string.cap, string.str
    );

    printf("Testing char append...\n");
    pklstr_err_option_t char_append_res = string_append_char(&string, '!');
    if (char_append_res.is_some) {
        fprintf(stderr, "Error occurred: %lu\n", (size_t) char_append_res.some);
        return 1;
    }
    printf(
        "Result: { .len = %lu, .cap = %lu, .str = '%s' }\n",
        string.len, string.cap, string.str
    );

    printf("Testing str append (no resize)...\n");
    pklstr_err_option_t str_append_res1 = string_append_str(&string, " More!");
    if (str_append_res1.is_some) {
        fprintf(stderr, "Error occurred: %lu\n", (size_t) str_append_res1.some);
        return 1;
    }
    printf(
        "Result: { .len = %lu, .cap = %lu, .str = '%s' }\n",
        string.len, string.cap, string.str
    );

    printf("Testing str append (double)...\n");
    pklstr_err_option_t str_append_res2 = string_append_str(&string, " More!");
    if (str_append_res2.is_some) {
        fprintf(stderr, "Error occurred: %lu\n", (size_t) str_append_res2.some);
        return 1;
    }
    printf(
        "Result: { .len = %lu, .cap = %lu, .str = '%s' }\n",
        string.len, string.cap, string.str
    );

    printf("Testing str append (more than double)...\n");
    pklstr_err_option_t str_append_res3 =
        string_append_str(&string, " Way Way Way Way Way Way More!");
    if (str_append_res3.is_some) {
        fprintf(stderr, "Error occurred: %lu\n", (size_t) str_append_res3.some);
        return 1;
    }
    printf(
        "Result: { .len = %lu, .cap = %lu, .str = '%s' }\n",
        string.len, string.cap, string.str
    );

    printf("Testing string append...\n");
    string_result_t string_res2 = string_from(" Even More", CHAR_DEF_ALLOC);
    if (string_res2.is_err) {
        fprintf(stderr, "Error occurred: %lu\n", (size_t) string_res2.err);
        return 1;
    }
    string_t string2 = string_res2.ok;
    pklstr_err_option_t string_append_res = string_append_string(&string, &string2);
    if (string_append_res.is_some) {
        fprintf(stderr, "Error occurred: %lu\n", (size_t) string_append_res.some);
        return 1;
    }
    string_free(&string2);
    printf(
        "Result: { .len = %lu, .cap = %lu, .str = '%s' }\n",
        string.len, string.cap, string.str
    );
    string_free(&string);

    printf("Resetting to a new string...\n");
    string_result_t string2_res = string_from("Hello world!", CHAR_DEF_ALLOC);
    if (string2_res.is_err) {
        fprintf(stderr, "Error occurred: %lu\n", (size_t) string2_res.err);
        return 1;
    }
    string_t string3 = string2_res.ok;
    printf(
        "Result: { .len = %lu, .cap = %lu, .str = '%s' }\n",
        string3.len, string3.cap, string3.str
    );

    printf("Inserting a char...\n");
    pklstr_err_option_t char_ins_res = string_insert_char_at(&string3, ',', 5);
    if (char_ins_res.is_some) {
        fprintf(stderr, "Error occurred: %lu\n", (size_t) char_ins_res.some);
        return 1;
    }
    printf(
        "Result: { .len = %lu, .cap = %lu, .str = '%s' }\n",
        string3.len, string3.cap, string3.str
    );

    printf("Inserting a str...\n");
    pklstr_err_option_t str_ins_res = string_insert_str_at(&string3, " beautiful", 6);
    if (str_ins_res.is_some) {
        fprintf(stderr, "Error occurred: %lu\n", (size_t) str_ins_res.some);
        return 1;
    }
    printf(
        "Result: { .len = %lu, .cap = %lu, .str = '%s' }\n",
        string3.len, string3.cap, string3.str
    );

    printf("Inserting a string...\n");
    string_result_t string4_res = string_from("It's a great day! ", CHAR_DEF_ALLOC);
    if (string4_res.is_err) {
        fprintf(stderr, "Error occurred: %lu\n", (size_t) string4_res.err);
        return 1;
    }
    string_t string4 = string4_res.ok;
    pklstr_err_option_t string_ins_res = string_insert_string_at(&string3, &string4, 0);
    if (string_ins_res.is_some) {
        fprintf(stderr, "Error occurred: %lu\n", (size_t) string_ins_res.some);
        return 1;
    }
    string_free(&string4);
    printf(
        "Result: { .len = %lu, .cap = %lu, .str = '%s' }\n",
        string3.len, string3.cap, string3.str
    );

    printf("Removing a piece...\n");
    pklstr_err_option_t rm_res = string_remove_at(&string3, 24, 10);
    if (rm_res.is_some) {
        fprintf(stderr, "Error occurred: %lu\n", (size_t) rm_res.some);
        return 1;
    }
    printf(
        "Result: { .len = %lu, .cap = %lu, .str = '%s' }\n",
        string3.len, string3.cap, string3.str
    );

    printf("Grab a substring...\n");
    string_result_t sub_res = string_substring(&string3, 18, 5);
    if (sub_res.is_err) {
        fprintf(stderr, "Error occurred: %lu\n", (size_t) sub_res.err);
        return 1;
    }
    string_free(&string3);
    string_t sub = sub_res.ok;
    printf(
        "Result: { .len = %lu, .cap = %lu, .str = '%s' }\n",
        sub.len, sub.cap, sub.str
    );

    printf("Index of char...\n");
    size_option_t char_ind_opt = string_index_of_char(&sub, 'e');
    if (!char_ind_opt.is_some) {
        fprintf(stderr, "Couldn't find 'e' in 'Hello'!\n");
        return 1;
    }
    printf("'e' is index %lu of '%s'\n", char_ind_opt.some, sub.str);

    printf("Index of str...\n");
    size_option_t str_ind_opt = string_index_of_str(&sub, "ll");
    if (!str_ind_opt.is_some) {
        fprintf(stderr, "Couldn't find 'll' in 'Hello'!\n");
        return 1;
    }
    printf("'ll' is index %lu of '%s'\n", str_ind_opt.some, sub.str);

    string_free(&sub);
    return 0;
}

