// Implementation of utilities and default functions for pklstr

#include <stdlib.h>
#include <pklstr.h>

// -------- Default allocators --------

char *default_char_malloc(const size_t count) {
    return malloc(count);
}

char *default_char_realloc(char *str, const size_t count) {
    return realloc(str, count);
}

void default_char_free(char *str) {
    free(str);
}

wchar_t *default_wchar_malloc(const size_t count) {
    return malloc(count * sizeof(wchar_t));
}

wchar_t *default_wchar_realloc(wchar_t *str, const size_t count) {
    return realloc(str, count * sizeof(wchar_t));
}

void default_wchar_free(wchar_t *str) {
    free(str);
}

