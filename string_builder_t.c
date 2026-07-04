#include "string_builder_t.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define true_malloc (int)1
#define false_malloc (int)0

#define sb_layer_1 1
#define sb_layer_2 2
#define sb_layer_3 3
#define sb_layer_4 4


sb_status check_standard_errors(const string_builder_union* str, int afterMalloc, int deep) {
    sb_status status = afterMalloc == 1 ? SB_MEMORY_FAULT : SB_IS_NULL;
    if (deep >= sb_layer_1 && str == NULL) return status;
    if (deep >= sb_layer_2 && str->string_builder_t == NULL) return status;
    if (deep >= sb_layer_3 && str->string_builder_t->value == NULL) return status;
    if (deep >= sb_layer_4 && str->string_builder_t->count <= 0) return status;
    return SB_OK;
}

void sb_handle_status(sb_status status) {
    switch (status) {
        case SB_MEMORY_FAULT:
            printf("\nMemory fault");
            break;
        case SB_IS_NULL:
            printf("\nSB is NULL"); 
            break;
        case SB_OK:
            printf("\nSB is OK");
            break;
        default:
            printf("\nUndefined status");
            break;
    }
}

string_builder_union* sb_allocate_new(int len) {
    string_builder_union* new_sb = malloc(sizeof(string_builder_union));
    sb_status status1 = check_standard_errors(new_sb, true_malloc, sb_layer_1); 
    if (status1 != SB_OK) return NULL;

    new_sb->string_builder_t = malloc(sizeof(string_builder_t));
    sb_status status2 = check_standard_errors(new_sb, true_malloc, sb_layer_2);
    if (status2 != SB_OK) return NULL;

    new_sb->string_builder_t->value = malloc(len);
    sb_status status3 = check_standard_errors(new_sb, true_malloc, sb_layer_3);
    if (status3 != SB_OK) return NULL;

    new_sb->string_builder_t->count = len;

    return new_sb;
}

string_builder_union* sb_from_string(const char* initialStr) {
    int len = strlen(initialStr);
    string_builder_union* newString = sb_allocate_new(len);
    if (newString == NULL) return NULL;

    newString->string_builder_t->count = len;
    for (int i = 0; i < len; i++) {
        newString->string_builder_t->value[i] = initialStr[i];
    }
    return newString;
}

string_builder_union* sb_substring(const string_builder_union* str, int start, int end) {
    sb_status status1 = check_standard_errors(str, false_malloc, sb_layer_4);
    if (status1 != SB_OK) return NULL;

    if (str->string_builder_t->value == 0 || start < -0 || end > str->string_builder_t->count ||
        end <= start) {
        return (string_builder_union*)str;
    }

    int len = end - start;
    string_builder_union* toReturn = sb_allocate_new(len - 1);
    if (toReturn == NULL) return NULL;

    for (int i = 0; i < len; i++) {
        toReturn->string_builder_t->value[i] = str->string_builder_t->value[i + start];
    }
    return toReturn;
}

sb_status sb_print(const string_builder_union* str) {
    sb_status status = check_standard_errors(str, false_malloc, sb_layer_4);
    if (status != SB_OK) return status;
    printf("\n%.*s", str->string_builder_t->count, str->string_builder_t->value);
    return SB_OK;
}

sb_status sb_invert(string_builder_union* str) {
    sb_status status = check_standard_errors(str, false_malloc, sb_layer_4);
    if (status != SB_OK) return status;

    sb_status orig_sb_count = str->string_builder_t->count;
    
    char* newVal = malloc(orig_sb_count);
    if (newVal == NULL) {
        return SB_MEMORY_FAULT;
    }

    int b = orig_sb_count; //3
    int i = 0;
    while (i < orig_sb_count) {
        newVal[i++] = str->string_builder_t->value[--b];
    }
    while (b < orig_sb_count) {
        str->string_builder_t->value[b] = newVal[b];
        b++;
    }
    
    free(newVal);
    return SB_OK;
};

sb_status sb_trim(struct string_builder_union* str) {
    sb_status status = check_standard_errors(str, false_malloc, sb_layer_4);
    if (status != SB_OK) return status;

    int count = str->string_builder_t->count;
    for (int i = 0; i < count; i++) {
        if (str->string_builder_t->value[i] == ' ') {
            str->string_builder_t->count--;
            for (int b = i; b < count; b++) {
                str->string_builder_t->value[b] = str->string_builder_t->value[b + 1];
            }
        }
    }

    return SB_OK;
}

sb_status sb_replace_all_chars_with(string_builder_union* str, char toReplaceWith) {
    sb_status status = check_standard_errors(str, false_malloc, sb_layer_4);
    if (status != SB_OK) return status;

    for (int i = 0; i < str->string_builder_t->count; i++) {
        str->string_builder_t->value[i] = toReplaceWith;
    }
    return SB_OK;
}

sb_status sb_replace_value_with(string_builder_union* str, char* toReplaceWith) {
    sb_status status = check_standard_errors(str, false_malloc, sb_layer_4);
    if (status != SB_OK) return status;

    int len = strlen(toReplaceWith);
    str->string_builder_t->value = malloc(len);
    if (str->string_builder_t->value == NULL) {
        return SB_MEMORY_FAULT;
    }

    str->string_builder_t->count = len;

    for (int i = 0; i < len; i++) {
        str->string_builder_t->value[i] = toReplaceWith[i];
    }

    return SB_OK;
}

sb_status sb_append(string_builder_union* str, char* to_append) {
    int len_initial = str->string_builder_t->count;
    int len_final = strlen(to_append) + len_initial;
    str->string_builder_t->count = len_final;

    char* savedValue = str->string_builder_t->value;

    str->string_builder_t->value = malloc(len_final);
    int status = check_standard_errors(str, true_malloc, sb_layer_4);
    if (status != SB_OK) return status;

    for (int i = 0; i < str->string_builder_t->count; i++) {
        if (i < len_initial) {
            str->string_builder_t->value[i] = savedValue[i];
        } else {
            str->string_builder_t->value[i] = to_append[i - len_initial];
        }
    }

    free(savedValue);
    return SB_OK;
}

int sb_contains(const string_builder_union* str, const char* toFind) {
    sb_status status = check_standard_errors(str, false_malloc, sb_layer_4);
    if (status != SB_OK) return 0;

    int found = 0;
    int len = strlen(toFind);
    if (len > str->string_builder_t->count) return 0;

    for (int i = 0; i <= str->string_builder_t->count - len && found != 1; i++) {
        int innerHaveToBreak = 0;

        for (int b = 0; b < len && found != 1 && innerHaveToBreak != 1; b++) {
            if (str->string_builder_t->value[i + b] != toFind[b]) innerHaveToBreak = 1;
            if (b == (len - 1) && innerHaveToBreak == 0) {
                found = 1;
            }
        }
    }
    return found;
}
