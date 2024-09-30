// Implementation of wstring_t functionality

#include <string.h>
#include <pklstr.h>

inline size_t wstrlen(wchar_t *str) {
    size_t len = 0;
    while (str[len++]) {}
    return len;
}

// Allocation/Deallocation

wstring_result_t wstring_new(wchar_malloc_t malloc, wchar_realloc_t realloc, wchar_free_t free) {
    wchar_t *empty = malloc(sizeof(wchar_t));
    if (!empty) {
        return (wstring_result_t) {
            .is_err = true,
            .err = (pklstr_err_t) {
                .code = PKLSTR_ERR_ALLOC_FAILED,
                .msg = NULL
            }
        };
    }
    return (wstring_result_t) {
        .is_err = false,
        .ok = (wstring_t) {
            .len = 0,
            .cap = sizeof(wchar_t),
            .str = empty,

            .malloc = malloc,
            .realloc = realloc,
            .free = free
        }
    };
}

wstring_result_t wstring_from(
        wchar_t *str, wchar_malloc_t malloc, wchar_realloc_t realloc, wchar_free_t free) {
    size_t len = wstrlen(str);
    size_t capacity = sizeof(wchar_t) * (len + 1);
    wchar_t *str_mem = malloc(capacity);
    if (!str_mem) {
        return (wstring_result_t) {
            .is_err = true,
            .err = (pklstr_err_t) {
                .code = PKLSTR_ERR_ALLOC_FAILED,
                .msg = NULL
            }
        };
    }
    memcpy(str_mem, str, capacity);
    str_mem[len] = '\0';
    return (wstring_result_t) {
        .is_err = false,
        .ok = (wstring_t) {
            .len = len,
            .cap = capacity,
            .str = str_mem,

            .malloc = malloc,
            .realloc = realloc,
            .free = free
        }
    };
}

void wstring_free(wstring_t *ref_string) {
    ref_string->free(ref_string->str);
    ref_string->str = 0;
}

