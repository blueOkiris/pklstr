// Implement functionality for string_t

#include <stdio.h>
#include <string.h>
#include <pklstr.h>

// Allocation/Deallocation

string_result_t string_new(char_malloc_t malloc, char_realloc_t realloc, char_free_t free) {
    char *empty = malloc(1);
    if (!empty) {
        return (string_result_t) {
            .is_err = true,
            .err = (pklstr_err_t) {
                .code = PKLSTR_ERR_ALLOC_FAILED,
                .msg = NULL
            }
        };
    }
    return (string_result_t) {
        .is_err = false,
        .ok = (string_t) {
            .len = 0,
            .cap = 1,
            .str = empty,

            .malloc = malloc,
            .realloc = realloc,
            .free = free
        }
    };
}

string_result_t string_from(
        char *str, char_malloc_t malloc, char_realloc_t realloc, char_free_t free) {
    size_t len = strlen(str);
    char *str_mem = malloc(len + 1);
    if (!str_mem) {
        return (string_result_t) {
            .is_err = true,
            .err = (pklstr_err_t) {
                .code = PKLSTR_ERR_ALLOC_FAILED,
                .msg = NULL
            }
        };
    }
    memcpy(str_mem, str, len);
    str_mem[len] = '\0';
    return (string_result_t) {
        .is_err = false,
        .ok = (string_t) {
            .len = len,
            .cap = len + 1,
            .str = str_mem,

            .malloc = malloc,
            .realloc = realloc,
            .free = free
        }
    };
}

void string_free(string_t *ref_string) {
    ref_string->free(ref_string->str);
    ref_string->str = 0;
}

// Insertion/Removal

result_t string_append_char(string_t *ref_string, const char c) {
    if (ref_string->len + 2 > ref_string->cap) {
        // Resize (double)
        size_t new_cap = ref_string->len * 2 + 1;
        ref_string->str = ref_string->realloc(ref_string->str, new_cap);
        if (!ref_string->str) {
            return (result_t) {
                .is_err = true,
                .err = (pklstr_err_t) {
                    .code = PKLSTR_ERR_ALLOC_FAILED,
                    .msg = NULL
                }
            };
        }
        ref_string->cap = new_cap;
    }
    ref_string->str[ref_string->len] = c;
    ref_string->len += 1;
    ref_string->str[ref_string->len] = '\0';
    return (result_t) { .is_err = false, .ok = {} };
}

result_t string_append_str(string_t *ref_string, const char *str) {
    size_t str_to_add_len = strlen(str);
    size_t raw_cap = ref_string->len + str_to_add_len + 1;
    size_t double_cap = ref_string->len * 2 + 1;
    if (ref_string->len + str_to_add_len + 1 > ref_string->cap) {
        if (raw_cap > double_cap) {
            // Do one big allocation instead of multiple doubles
            ref_string->str = ref_string->realloc(ref_string->str, raw_cap);
            if (!ref_string->str) {
                return (result_t) {
                    .is_err = true,
                    .err = (pklstr_err_t) {
                        .code = PKLSTR_ERR_ALLOC_FAILED,
                        .msg = NULL
                    }
                };
            }
            ref_string->cap = raw_cap;
        } else {
            // Double
            ref_string->str = ref_string->realloc(ref_string->str, double_cap);
            if (!ref_string->str) {
                return (result_t) {
                    .is_err = true,
                    .err = (pklstr_err_t) {
                        .code = PKLSTR_ERR_ALLOC_FAILED,
                        .msg = NULL
                    }
                };
            }
            ref_string->cap = double_cap;
        }
    }
    memcpy(ref_string->str + ref_string->len, str, str_to_add_len);
    ref_string->len += str_to_add_len;
    ref_string->str[ref_string->len] = '\0';
    return (result_t) { .is_err = false, .ok = {} };
}

result_t string_append_string(string_t *ref_string, const string_t *other) {
    // Could be more efficient bc could use ->len instead of call to strlen, but not huge deal
    return string_append_str(ref_string, other->str);
}

result_t string_insert_char_at(string_t *ref_string, const char c, const size_t index) {
    if (index >= ref_string->len) {
        return (result_t) {
            .is_err = true,
            .err = (pklstr_err_t) {
                .code = PKLSTR_ERR_OUT_OF_BOUNDS,
                .msg = NULL
            }
        };
    }
    result_t append = string_append_char(ref_string, c); // Make room for the new character
    if (append.is_err) {
        return append;
    }
    // Shift the string
    for (size_t i = ref_string->len - 1; i > index; i--) {
        ref_string->str[i] = ref_string->str[i - 1];
    }
    ref_string->str[index] = c;
    ref_string->str[ref_string->len] = '\0';
    return (result_t) { .is_err = false, .ok = {} };
}

result_t string_insert_str_at(string_t *ref_string, const char *str, const size_t index) {
    if (index >= ref_string->len) {
        return (result_t) {
            .is_err = true,
            .err = (pklstr_err_t) {
                .code = PKLSTR_ERR_OUT_OF_BOUNDS,
                .msg = NULL
            }
        };
    }
    size_t len = strlen(str);
    result_t append = string_append_str(ref_string, str);
    if (append.is_err) {
        return append;
    }
    for (size_t i = ref_string->len - 1; i > index + len - 1; i--) {
        ref_string->str[i] = ref_string->str[i - len];
    }
    for (size_t i = 0; i < len; i++) {
        ref_string->str[i + index] = str[i];
    }
    ref_string->str[ref_string->len] = '\0';
    return (result_t) { .is_err = false, .ok = {} };
}

result_t string_insert_string_at(string_t *ref_string, const string_t *other, const size_t index) {
    return string_insert_str_at(ref_string, other->str, index);
}

result_t string_remove_at(string_t *ref_string, const size_t index, const size_t len) {
    if (index + len >= ref_string->len) {
        return (result_t) {
            .is_err = true,
            .err = (pklstr_err_t) {
                .code = PKLSTR_ERR_OUT_OF_BOUNDS,
                .msg = NULL
            }
        };
    }
    for (int i = 0; i < len; i++) {
        ref_string->str[i + index] = ref_string->str[i + index + len];
    }
    ref_string->len -= len;
    ref_string->str[ref_string->len] = '\0';
    return (result_t) { .is_err = false, .ok = {} };
}

string_result_t string_substring(string_t *ref_string, const size_t start, const size_t len) {
    if (start + len >= ref_string->len) {
        return (string_result_t) {
            .is_err = true,
            .err = (pklstr_err_t) {
                .code = PKLSTR_ERR_OUT_OF_BOUNDS,
                .msg = NULL
            }
        };
    }
    char *str_mem = ref_string->malloc(len + 1);
    if (!str_mem) {
        return (string_result_t) {
            .is_err = true,
            .err = (pklstr_err_t) {
                .code = PKLSTR_ERR_ALLOC_FAILED,
                .msg = NULL
            }
        };
    }
    memcpy(str_mem, ref_string->str + start, len);
    str_mem[len] = '\0';
    return (string_result_t) {
        .is_err = false,
        .ok = (string_t) {
            .len = len,
            .cap = len + 1,
            .str = str_mem,

            .malloc = ref_string->malloc,
            .realloc = ref_string->realloc,
            .free = ref_string->free
        }
    };
}

size_option_t string_index_of_char(string_t *ref_string, const char c) {
    for (size_t i = 0; i < ref_string->len; i++) {
        if (ref_string->str[i] == c) {
            return (size_option_t) { .is_some = true, .some = i };
        }
    }
    return (size_option_t) { .is_some = false, .none = {} };
}

size_option_t string_index_of_str(string_t *ref_string, const char *str) {
    size_t len = strlen(str);
    for (size_t i = 0; i < ref_string->len - len; i++) {
        if (strncmp(ref_string->str + i, str, len) == 0) {
            return (size_option_t) { .is_some = true, .some = i };
        }
    }
    return (size_option_t) { .is_some = false, .none = {} };
}

size_option_t string_index_of_string(string_t *ref_string, const string_t *other) {
    return string_index_of_str(ref_string, other->str);
}

