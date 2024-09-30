// Display Hello, world! using string_t and wstring_t

#include <locale.h>
#include <stdio.h>
#include <pklstr.h>

int main(int argc, char **argv) {
    setlocale(LC_ALL, "en_US.UTF_8");

    string_result_t new_string_res = string_from("Hello, world!", CHAR_DEF_ALLOC);
    if (new_string_res.is_err) {
        switch (new_string_res.err.code) {
            case PKLSTR_ERR_ALLOC_FAILED:
                fprintf(stderr, "Error allocating space for string_t\n");
                return 1;
            default:
                fprintf(
                    stderr, "An unhandled error occurred: %lu",
                    (size_t) new_string_res.err.code
                );
                if (new_string_res.err.msg) {
                    fprintf(stderr, " - %s", new_string_res.err.msg);
                }
                fprintf(stderr, "\n");
                return 1;
        }
    }
    string_t new_string = new_string_res.ok;
    printf("English: %s\n", new_string.str);
    string_free(&new_string);

    wstring_result_t new_wstring_res = wstring_from(L"こんにちは世界！", WCHAR_DEF_ALLOC);
    if (new_wstring_res.is_err) {
        printf("Error!\n");
        switch (new_wstring_res.err.code) {
            case PKLSTR_ERR_ALLOC_FAILED:
                fprintf(stderr, "Error allocating space for wstring_t\n");
                return 1;
            default:
                fprintf(
                    stderr, "An unhandled error occurred: %lu",
                    (size_t) new_wstring_res.err.code
                );
                if (new_wstring_res.err.msg) {
                    fprintf(stderr, " - %s", new_wstring_res.err.msg);
                }
                fprintf(stderr, "\n");
                return 1;
        }
    }
    wstring_t new_wstring = new_wstring_res.ok;
    printf("Japanese: %ls\n", new_wstring.str);
    wstring_free(&new_wstring);
}

