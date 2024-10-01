// Functional test for wstring_t. Derived from examples/test/

#include <locale.h>
#include <stdio.h>
#include <pklstr.h>

int main(int argc, char **argv) {
    setlocale(LC_ALL, "en_US.UTF_8");
    
    printf("Testing allocation...\n");
    wstring_result_t string_res = wstring_from(L"こんにちは世界", WCHAR_DEF_ALLOC);
    if (string_res.is_err) {
        fprintf(stderr, "Error occurred: %lu", (size_t) string_res.err.code);
        if (string_res.err.msg) {
            fprintf(stderr, " - %s", string_res.err.msg);
        }
        fprintf(stderr, "\n");
        return 1;
    }
    wstring_t string = string_res.ok;
    printf(
        "Result: { .len = %lu, .cap = %lu, .str = '%ls' }\n",
        string.len, string.cap, string.str
    );

    printf("Testing char append...\n");
    result_t char_append_res = wstring_append_char(&string, L'！');
    if (char_append_res.is_err) {
        fprintf(stderr, "Error occurred: %lu", (size_t) char_append_res.err.code);
        if (string_res.err.msg) {
            fprintf(stderr, " - %s", char_append_res.err.msg);
        }
        fprintf(stderr, "\n");
        return 1;
    }
    printf(
        "Result: { .len = %lu, .cap = %lu, .str = '%ls' }\n",
        string.len, string.cap, string.str
    );

    printf("Testing str append (no resize)...\n");
    result_t str_append_res1 = wstring_append_str(&string, L" More!");
    if (str_append_res1.is_err) {
        fprintf(stderr, "Error occurred: %lu", (size_t) str_append_res1.err.code);
        if (str_append_res1.err.msg) {
            fprintf(stderr, " - %s", str_append_res1.err.msg);
        }
        fprintf(stderr, "\n");
        return 1;
    }
    printf(
        "Result: { .len = %lu, .cap = %lu, .str = '%ls' }\n",
        string.len, string.cap, string.str
    );

    printf("Testing str append (double)...\n");
    result_t str_append_res2 = wstring_append_str(&string, L" More!");
    if (str_append_res2.is_err) {
        fprintf(stderr, "Error occurred: %lu", (size_t) str_append_res2.err.code);
        if (str_append_res2.err.msg) {
            fprintf(stderr, " - %s", str_append_res2.err.msg);
        }
        fprintf(stderr, "\n");
        return 1;
    }
    printf(
        "Result: { .len = %lu, .cap = %lu, .str = '%ls' }\n",
        string.len, string.cap, string.str
    );

    printf("Testing str append (more than double)...\n");
    result_t str_append_res3 = wstring_append_str(&string, L" Way Way Way Way Way Way More!");
    if (str_append_res3.is_err) {
        fprintf(stderr, "Error occurred: %lu", (size_t) str_append_res3.err.code);
        if (str_append_res3.err.msg) {
            fprintf(stderr, " - %s", str_append_res3.err.msg);
        }
        fprintf(stderr, "\n");
        return 1;
    }
    printf(
        "Result: { .len = %lu, .cap = %lu, .str = '%ls' }\n",
        string.len, string.cap, string.str
    );

    printf("Testing string append...\n");
    wstring_result_t string_res2 = wstring_from(L" Even More", WCHAR_DEF_ALLOC);
    if (string_res2.is_err) {
        fprintf(stderr, "Error occurred: %lu", (size_t) string_res2.err.code);
        if (string_res2.err.msg) {
            fprintf(stderr, " - %s", string_res2.err.msg);
        }
        fprintf(stderr, "\n");
        return 1;
    }
    wstring_t string2 = string_res2.ok;
    result_t string_append_res = wstring_append_string(&string, &string2);
    if (string_append_res.is_err) {
        fprintf(stderr, "Error occurred: %lu", (size_t) string_append_res.err.code);
        if (string_append_res.err.msg) {
            fprintf(stderr, " - %s", string_append_res.err.msg);
        }
        fprintf(stderr, "\n");
        return 1;
    }
    wstring_free(&string2);
    printf(
        "Result: { .len = %lu, .cap = %lu, .str = '%ls' }\n",
        string.len, string.cap, string.str
    );
    wstring_free(&string);

    printf("Resetting to a new string...\n");
    wstring_result_t string2_res = wstring_from(L"こんにちは世界！", WCHAR_DEF_ALLOC);
    if (string2_res.is_err) {
        fprintf(stderr, "Error occurred: %lu", (size_t) string2_res.err.code);
        if (string2_res.err.msg) {
            fprintf(stderr, " - %s", string2_res.err.msg);
        }
        fprintf(stderr, "\n");
        return 1;
    }
    wstring_t string3 = string2_res.ok;
    printf(
        "Result: { .len = %lu, .cap = %lu, .str = '%ls' }\n",
        string3.len, string3.cap, string3.str
    );

    printf("Inserting a char...\n");
    result_t char_ins_res = wstring_insert_char_at(&string3, L' ', 5);
    if (char_ins_res.is_err) {
        fprintf(stderr, "Error occurred: %lu", (size_t) char_ins_res.err.code);
        if (char_ins_res.err.msg) {
            fprintf(stderr, " - %s", char_ins_res.err.msg);
        }
        fprintf(stderr, "\n");
        return 1;
    }
    printf(
        "Result: { .len = %lu, .cap = %lu, .str = '%ls' }\n",
        string3.len, string3.cap, string3.str
    );

    printf("Inserting a str...\n");
    result_t str_ins_res = wstring_insert_str_at(&string3, L" beautiful", 6);
    if (str_ins_res.is_err) {
        fprintf(stderr, "Error occurred: %lu", (size_t) str_ins_res.err.code);
        if (str_ins_res.err.msg) {
            fprintf(stderr, " - %s", str_ins_res.err.msg);
        }
        fprintf(stderr, "\n");
        return 1;
    }
    printf(
        "Result: { .len = %lu, .cap = %lu, .str = '%ls' }\n",
        string3.len, string3.cap, string3.str
    );

    printf("Inserting a string...\n");
    wstring_result_t string4_res = wstring_from(L"It's a great day! ", WCHAR_DEF_ALLOC);
    if (string4_res.is_err) {
        fprintf(stderr, "Error occurred: %lu", (size_t) string4_res.err.code);
        if (string4_res.err.msg) {
            fprintf(stderr, " - %s", string4_res.err.msg);
        }
        fprintf(stderr, "\n");
        return 1;
    }
    wstring_t string4 = string4_res.ok;
    result_t string_ins_res = wstring_insert_string_at(&string3, &string4, 0);
    if (string_ins_res.is_err) {
        fprintf(stderr, "Error occurred: %lu", (size_t) string_ins_res.err.code);
        if (string_ins_res.err.msg) {
            fprintf(stderr, " - %s", string_ins_res.err.msg);
        }
        fprintf(stderr, "\n");
        return 1;
    }
    wstring_free(&string4);
    printf(
        "Result: { .len = %lu, .cap = %lu, .str = '%ls' }\n",
        string3.len, string3.cap, string3.str
    );

    printf("Removing a piece...\n");
    result_t rm_res = wstring_remove_at(&string3, 23, 11);
    if (rm_res.is_err) {
        fprintf(stderr, "Error occurred: %lu", (size_t) rm_res.err.code);
        if (rm_res.err.msg) {
            fprintf(stderr, " - %s", rm_res.err.msg);
        }
        fprintf(stderr, "\n");
        return 1;
    }
    printf(
        "Result: { .len = %lu, .cap = %lu, .str = '%ls' }\n",
        string3.len, string3.cap, string3.str
    );

    printf("Grab a substring...\n");
    wstring_result_t sub_res = wstring_substring(&string3, 18, 7);
    if (sub_res.is_err) {
        fprintf(stderr, "Error occurred: %lu", (size_t) sub_res.err.code);
        if (sub_res.err.msg) {
            fprintf(stderr, " - %s", sub_res.err.msg);
        }
        fprintf(stderr, "\n");
        return 1;
    }
    wstring_free(&string3);
    wstring_t sub = sub_res.ok;
    printf(
        "Result: { .len = %lu, .cap = %lu, .str = '%ls' }\n",
        sub.len, sub.cap, sub.str
    );

    printf("Index of char...\n");
    size_option_t char_ind_opt = wstring_index_of_char(&sub, L'ん');
    if (!char_ind_opt.is_some) {
        fprintf(stderr, "Couldn't find 'ん' in 'こんにちは世界'!\n");
        return 1;
    }
    printf("'ん' is index %lu of '%ls'\n", char_ind_opt.some, sub.str);

    printf("Index of str...\n");
    size_option_t str_ind_opt = wstring_index_of_str(&sub, L"は世");
    if (!str_ind_opt.is_some) {
        fprintf(stderr, "Couldn't find 'は世' in 'こんにちは世界'!\n");
        return 1;
    }
    printf("'は世' is index %lu of '%ls'\n", str_ind_opt.some, sub.str);

    wstring_free(&sub);
    return 0;
}

