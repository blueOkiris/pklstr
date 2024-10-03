// Implementation of wstring_t functionality

#include <stdio.h>
#include <string.h>
#include <pklstr.h>

inline size_t wstrlen(const wchar_t *str) {
    size_t len = 0;
    while (str[len++]) {}
    return len - 1;
}

inline bool wstreqn(const wchar_t *a, const wchar_t *b, const size_t len) {
    for (size_t i = 0; i < len; i++) {
        if (a[i] != b[i]) {
            return false;
        }
    }
    return true;
}

// Allocation/Deallocation

wstring_result_t wstring_new(wchar_malloc_t malloc, wchar_realloc_t realloc, wchar_free_t free) {
    wchar_t *empty = malloc(sizeof(wchar_t));
    if (!empty) {
        return (wstring_result_t) {
            .is_err = true,
            .err = PKLSTR_ERR_ALLOC_FAILED
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
            .err = PKLSTR_ERR_ALLOC_FAILED
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

// Insertion/Removal

pklstr_err_option_t wstring_append_char(wstring_t *ref_string, const wchar_t c) {
    if ((ref_string->len + 2) * sizeof(wchar_t) > ref_string->cap) {
        // Resize (double)
        size_t new_cap = (ref_string->len * 2 + 1) * sizeof(wchar_t);
        ref_string->str = ref_string->realloc(ref_string->str, new_cap);
        if (!ref_string->str) {
            return (pklstr_err_option_t) {
                .is_some = true,
                .some = PKLSTR_ERR_ALLOC_FAILED
            };
        }
        ref_string->cap = new_cap;
    }
    ref_string->str[ref_string->len] = c;
    ref_string->len += 1;
    ref_string->str[ref_string->len] = L'\0';
    return (pklstr_err_option_t) { .is_some = false, .none = {} };
}

pklstr_err_option_t wstring_append_str(wstring_t *ref_string, const wchar_t *str) {
    size_t str_to_add_len = wstrlen(str);
    size_t raw_cap = (ref_string->len + str_to_add_len + 1) * sizeof(wchar_t);
    size_t double_cap = (ref_string->len * 2 + 1) * sizeof(wchar_t);
    if ((ref_string->len + str_to_add_len + 1) * sizeof(wchar_t) > ref_string->cap) {
        if (raw_cap > double_cap) {
            // Do one big allocation instead of multiple doubles
            ref_string->str = ref_string->realloc(ref_string->str, raw_cap);
            if (!ref_string->str) {
                return (pklstr_err_option_t) {
                    .is_some = true,
                    .some = PKLSTR_ERR_ALLOC_FAILED
                };
            }
            ref_string->cap = raw_cap;
        } else {
            // Double
            ref_string->str = ref_string->realloc(ref_string->str, double_cap);
            if (!ref_string->str) {
                return (pklstr_err_option_t) {
                    .is_some = true,
                    .some = PKLSTR_ERR_ALLOC_FAILED
                };
            }
            ref_string->cap = double_cap;
        }
    }
    for (size_t i = 0; i < str_to_add_len; i++) {
        ref_string->str[ref_string->len + i] = str[i];
    }
    ref_string->len += str_to_add_len;
    ref_string->str[ref_string->len] = L'\0';
    return (pklstr_err_option_t) { .is_some = false, .none = {} };
}

pklstr_err_option_t wstring_append_string(wstring_t *ref_string, const wstring_t *other) {
    // Could be more efficient bc could use ->len instead of call to strlen, but not huge deal
    return wstring_append_str(ref_string, other->str);
}

pklstr_err_option_t wstring_insert_char_at(wstring_t *ref_string, const wchar_t c, const size_t index) {
    if (index >= ref_string->len) {
        return (pklstr_err_option_t) {
            .is_some = true,
            .some = PKLSTR_ERR_OUT_OF_BOUNDS
        };
    }
    pklstr_err_option_t append = wstring_append_char(ref_string, c); // Make room for the new character
    if (append.is_some) {
        return append;
    }
    // Shift the string
    for (size_t i = ref_string->len - 1; i > index; i--) {
        ref_string->str[i] = ref_string->str[i - 1];
    }
    ref_string->str[index] = c;
    ref_string->str[ref_string->len] = '\0';
    return (pklstr_err_option_t) { .is_some = false, .none = {} };
}

pklstr_err_option_t wstring_insert_str_at(wstring_t *ref_string, const wchar_t *str, const size_t index) {
    if (index >= ref_string->len) {
        return (pklstr_err_option_t) {
            .is_some = true,
            .some = PKLSTR_ERR_OUT_OF_BOUNDS
        };
    }
    size_t len = wstrlen(str);
    pklstr_err_option_t append = wstring_append_str(ref_string, str);
    if (append.is_some) {
        return append;
    }
    for (size_t i = ref_string->len - 1; i > index + len - 1; i--) {
        ref_string->str[i] = ref_string->str[i - len];
    }
    for (size_t i = 0; i < len; i++) {
        ref_string->str[i + index] = str[i];
    }
    ref_string->str[ref_string->len] = '\0';
    return (pklstr_err_option_t) { .is_some = false, .none = {} };
}

pklstr_err_option_t wstring_insert_string_at(
        wstring_t *ref_string, const wstring_t *other, const size_t index) {
    return wstring_insert_str_at(ref_string, other->str, index);
}

pklstr_err_option_t wstring_remove_at(
        wstring_t *ref_string, const size_t index, const size_t len) {
    if (index + len >= ref_string->len) {
        return (pklstr_err_option_t) {
            .is_some = true,
            .some = PKLSTR_ERR_OUT_OF_BOUNDS
        };
    }
    for (int i = 0; i < len; i++) {
        ref_string->str[i + index] = ref_string->str[i + index + len];
    }
    ref_string->len -= len;
    ref_string->str[ref_string->len] = '\0';
    return (pklstr_err_option_t) { .is_some = false, .none = {} };
}

// Other

wstring_result_t wstring_substring(wstring_t *ref_string, const size_t start, const size_t len) {
    if (start + len >= ref_string->len) {
        return (wstring_result_t) {
            .is_err = true,
            .err = PKLSTR_ERR_OUT_OF_BOUNDS
        };
    }
    wchar_t *str_mem = ref_string->malloc(sizeof(wchar_t) * (len + 1));
    if (!str_mem) {
        return (wstring_result_t) {
            .is_err = true,
            .err =  PKLSTR_ERR_ALLOC_FAILED
        };
    }
    memcpy(str_mem, ref_string->str + start, len * sizeof(wchar_t));
    str_mem[len] = '\0';
    return (wstring_result_t) {
        .is_err = false,
        .ok = (wstring_t) {
            .len = len,
            .cap = sizeof(wchar_t) * (len + 1),
            .str = str_mem,

            .malloc = ref_string->malloc,
            .realloc = ref_string->realloc,
            .free = ref_string->free
        }
    };
}

size_option_t wstring_index_of_char(wstring_t *ref_string, const wchar_t c) {
    for (size_t i = 0; i < ref_string->len; i++) {
        if (ref_string->str[i] == c) {
            return (size_option_t) { .is_some = true, .some = i };
        }
    }
    return (size_option_t) { .is_some = false, .none = {} };
}

size_option_t wstring_index_of_str(wstring_t *ref_string, const wchar_t *str) {
    size_t len = wstrlen(str);
    for (size_t i = 0; i < ref_string->len - len; i++) {
        if (wstreqn(ref_string->str + i, str, len)) {
            return (size_option_t) { .is_some = true, .some = i };
        }
    }
    return (size_option_t) { .is_some = false, .none = {} };
}

size_option_t wstring_index_of_string(wstring_t *ref_string, const wstring_t *other) {
    return wstring_index_of_str(ref_string, other->str);
}

