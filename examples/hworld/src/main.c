// Display Hello, world! using string_t and wstring_t

#include <locale.h>
#include <stdio.h>
#include <pklstr.h>

int main(int argc, char **argv) {
    setlocale(LC_ALL, "en_US.UTF_8");

    string_result_t new_string_res = string_from("Hello, world!", CHAR_DEF_ALLOC);
    if (new_string_res.is_err) {
        fprintf(stderr, "Error occurred: %lu\n", (size_t) new_string_res.err);
        return 1;
    }
    string_t new_string = new_string_res.ok;
    printf("English: %s\n", new_string.str);
    string_free(&new_string);

    wstring_result_t new_wstring_res = wstring_from(L"こんにちは世界！", WCHAR_DEF_ALLOC);
    if (new_wstring_res.is_err) {
        fprintf(stderr, "Error occurred: %lu\n", (size_t) new_string_res.err);
        return 1;
    }
    wstring_t new_wstring = new_wstring_res.ok;
    printf("Japanese: %ls\n", new_wstring.str);
    wstring_free(&new_wstring);
}

