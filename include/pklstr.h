// API entry for the library. Include this in your project

#pragma once

// If you want to use your own size_t declaration, use -DPKL_NOSTDDEF or #define PKL_NOSTDDEF 1
#ifndef PKL_NOSTDDEF
#include <stddef.h>
#endif

// If you want to use your own uint8_t etc types, use -DPKL_NOSTDINT or #define PKL_NOSTDINT 1
#ifndef PKL_NOSTDINT
#include <stdint.h>
#endif

// If you want to use a custom wchar_t, use -DPKL_NOWCHAR or #define PKL_NOWCHAR 1
#ifndef PKL_NOWCHAR
#include <wchar.h>
#endif

// If you want to use a custom bool, use -DPKL_NOSTDBOOL or #define PKL_NOSTDBOOL 1
#ifndef PKL_NOSTDBOOL
#include <stdbool.h>
#endif

// -------- Types --------

// Allocator used by the string library. A default is provided (based on malloc)
// This should return NULL on error. Size_t is the number of characters in the string no '\0'
typedef char *(*char_malloc_t)(const size_t);

// Same as char_malloc_t above, but for reallocating memory to grow it (default based on realloc)
typedef char *(*char_realloc_t)(char *, const size_t);

// Free allocated memory in the str
typedef void (*char_free_t)(char *);

// Wchar allocator types
typedef wchar_t *(*wchar_malloc_t)(const size_t);
typedef wchar_t *(*wchar_realloc_t)(wchar_t *, const size_t);
typedef void (*wchar_free_t)(wchar_t *);

// Our core string types. These are what you're creating and manipulating
typedef struct {
    size_t len;     // How many elements in string (not including '\0') in chars
    size_t cap;     // How much memory the string takes up (including '\0') in bytes
    char *str;      // The raw data

    char_malloc_t malloc;
    char_realloc_t realloc;
    char_free_t free;
} string_t;
typedef struct {
    size_t len;     // How many elements in string in wchars
    size_t cap;     // How much memory in bytes
    wchar_t *str;   // Raw data

    wchar_malloc_t malloc;
    wchar_realloc_t realloc;
    wchar_free_t free;
} wstring_t;

// Error codes
typedef enum {
    PKLSTR_ERR_ALLOC_FAILED,        // Malloc/Realloc failed
    PKLSTR_ERR_OUT_OF_BOUNDS        // Index provided to a function was outside the string's len
} pklstr_err_code_t;

// Errors
typedef struct {
    pklstr_err_code_t code;
    char *msg;
} pklstr_err_t;

// "Result<String, PklstrErr>" - Either a valid string_t (ok) or an error (err)
typedef struct {
    bool is_err;
    union {
        string_t ok;
        pklstr_err_t err;
    };
} string_result_t;
typedef struct {
    bool is_err;
    union {
        wstring_t ok;
        pklstr_err_t err;
    };
} wstring_result_t;

// "Result<(), PklstrErr>" - Either an error, or nothing (success)
typedef struct {
    bool is_err;
    union {
        struct {} ok;
        pklstr_err_t err;
    };
} result_t;

// "Option<size_t>" - Either an index or nothing
typedef struct {
    bool is_some;
    union {
        size_t some;
        struct {} none;
    };
} size_option_t;

// -------- Functions --------

// Default allocators (malloc)

char *default_char_malloc(const size_t count);
char *default_char_realloc(char *str, const size_t count);
void default_char_free(char *str);
wchar_t *default_wchar_malloc(const size_t count);
wchar_t *default_wchar_realloc(wchar_t *str, const size_t count);
void default_wchar_free(wchar_t *str);

// Autofill for constructors to use defaults
#define CHAR_DEF_ALLOC      default_char_malloc, default_char_realloc, default_char_free
#define WCHAR_DEF_ALLOC     default_wchar_malloc, default_wchar_realloc, default_wchar_free

// Allocation/Deallocation

string_result_t string_new(                 // Allocate space for an empty string
    char_malloc_t malloc,
    char_realloc_t realloc,
    char_free_t free
);
string_result_t string_from(                // Allocate space for a string and fill it by default
    char *str,
    char_malloc_t malloc,
    char_realloc_t realloc,
    char_free_t free
);
void string_free(string_t *ref_string);     // Deallocate a string

wstring_result_t wstring_new(
    wchar_malloc_t malloc,
    wchar_realloc_t realloc,
    wchar_free_t free
);
wstring_result_t wstring_from(
    wchar_t *str,
    wchar_malloc_t malloc,
    wchar_realloc_t realloc,
    wchar_free_t free
);
void wstring_free(wstring_t *ref_string);

// Insertion/Removal

result_t string_append_char(string_t *ref_string, const char c);
result_t string_append_str(string_t *ref_string, const char *str);
result_t string_append_string(string_t *ref_string, const string_t *other);
result_t string_insert_char_at(string_t *ref_string, const char c, const size_t index);
result_t string_insert_str_at(string_t *ref_string, const char *str, const size_t index);
result_t string_insert_string_at(string_t *ref_string, const string_t *other, const size_t index);
result_t string_remove_at(string_t *ref_string, const size_t index, const size_t len);

result_t wstring_append_char(wstring_t *ref_string, const wchar_t c);
result_t wstring_append_str(wstring_t *ref_string, const wchar_t *str);
result_t wstring_append_string(wstring_t *ref_string, const wstring_t *other);
result_t wstring_insert_char_at(wstring_t *ref_string, const wchar_t c, const size_t index);
result_t wstring_insert_str_at(wstring_t *ref_string, const wchar_t *str, const size_t index);
result_t wstring_insert_string_at(wstring_t *ref_string, const wstring_t *other, const size_t index);
result_t wstring_remove_at(wstring_t *ref_string, const size_t index, const size_t len);

// Other

string_result_t string_substring(string_t *ref_string, const size_t start, const size_t len);
size_option_t string_index_of_char(string_t *ref_string, const char c);
size_option_t string_index_of_str(string_t *ref_string, const char *str);
size_option_t string_index_of_string(string_t *ref_string, const string_t *other);

wstring_result_t wstring_substring(wstring_t *ref_string, const size_t start, const size_t len);
size_option_t wstring_index_of_char(wstring_t *ref_string, const wchar_t c);
size_option_t wstring_index_of_str(wstring_t *ref_string, const wchar_t *str);
size_option_t wstring_index_of_string(wstring_t *ref_string, const wstring_t *other);

