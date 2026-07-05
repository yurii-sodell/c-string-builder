#include "string_builder_t.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define true_malloc (int)1
#define false_malloc (int)0

#define sb_layer_1 1
#define sb_layer_2 2

typedef struct string_builder_t {
    char* value;
    int count;
} string_builder_t;

sb_status check_standard_errors(const string_builder_t* str, int afterMalloc, int deep) {
    sb_status status = afterMalloc == 1 ? SB_MEMORY_FAULT : SB_IS_NULL;
    if (deep >= sb_layer_1 && str == NULL) return status;
    if (deep >= sb_layer_2 && str->value == NULL) return status;
    if (deep >= sb_layer_2 && str->count < 0) return SB_COUNT_IS_CORRUPTED;
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
        case SB_OUT_OF_BOUNDS:
            printf("\nSB out of bounds");
            break;
        default:
            printf("\nUndefined status");
            break;
    }
}

int sb_legth(const string_builder_t* sb) { return sb->count; };

char* sb_copy_of_string_value(const string_builder_t* sb) {
    int count = sb->count;
    char* to_return = malloc(count + 1);
    char* value = sb->value;
    for (int i = 0; i < count; i++) {
        to_return[i] = value[i];
    }
    to_return[count] = '\0';
    return to_return;
}

void sb_free(string_builder_t* sb) {
    free(sb->value);
    free(sb);
}

string_builder_t* sb_allocate_new(const int len) {
    string_builder_t* new_sb = malloc(sizeof(string_builder_t));
    sb_status status1 = check_standard_errors(new_sb, true_malloc, sb_layer_1);
    if (status1 != SB_OK) return NULL;

    new_sb = malloc(sizeof(string_builder_t));
    sb_status status2 = check_standard_errors(new_sb, true_malloc, sb_layer_2);
    if (status2 != SB_OK) return NULL;

    new_sb->value = malloc(len);
    sb_status status3 = check_standard_errors(new_sb, true_malloc, sb_layer_2);
    if (status3 != SB_OK) return NULL;

    new_sb->count = len;

    return new_sb;
}

string_builder_t* sb_from_string(const char* initialStr) {
    int len = strlen(initialStr);
    string_builder_t* to_return = sb_allocate_new(len);
    if (to_return == NULL) return NULL;

    to_return->count = len;
    for (int i = 0; i < len; i++) {
        to_return->value[i] = initialStr[i];
    }
    return to_return;
}

string_builder_t* sb_substring(const string_builder_t* sb, const int start, const int end) {
    sb_status status1 = check_standard_errors(sb, false_malloc, sb_layer_2);
    if (status1 != SB_OK) return NULL;

    if (start < 0 || end > sb->count || end <= start) {
        return sb;
    }

    int len = end - start;
    string_builder_t* toReturn = sb_allocate_new(len);
    if (toReturn == NULL) return NULL;

    for (int i = 0; i < len; i++) {
        toReturn->value[i] = sb->value[i + start];
    }
    return toReturn;
}

string_builder_t* sb_new() { return sb_allocate_new(0); }

sb_status sb_print(const string_builder_t* sb) {
    sb_status status = check_standard_errors(sb, false_malloc, sb_layer_2);
    if (status != SB_OK) return status;
    printf("\n%.*s", sb->count, sb->value);
    return SB_OK;
}

sb_status sb_invert(string_builder_t* sb) {
    sb_status status = check_standard_errors(sb, false_malloc, sb_layer_2);
    if (status != SB_OK) return status;

    int count = sb->count;

    char* newVal = malloc(count);
    if (newVal == NULL) {
        return SB_MEMORY_FAULT;
    }

    int b = count;
    int i = 0;
    while (i < count) {
        newVal[i++] = sb->value[--b];
    }
    while (b < count) {
        sb->value[b] = newVal[b];
        b++;
    }

    free(newVal);
    return SB_OK;
};

sb_status sb_trim(string_builder_t* sb) {
    sb_status status = check_standard_errors(sb, false_malloc, sb_layer_2);
    if (status != SB_OK) return status;

    int count = sb->count;
    char* value = sb->value;

    for (int i = 0; i < count; i++) {
        if (value[i] == ' ') {
            sb->count--;
            for (int b = i; b < count - 1; b++) {
                value[b] = value[b + 1];
            }
        }
    }
    int memory_to_realese = count - sb->count;
    if (memory_to_realese > 0) {
        char* temp = realloc(value, sb->count);
        if (temp == NULL) return SB_MEMORY_FAULT;
        value = temp;
    }
    return SB_OK;
}

sb_status sb_replace_all_chars_with(string_builder_t* sb, const char toReplaceWith) {
    sb_status status = check_standard_errors(sb, false_malloc, sb_layer_2);
    if (status != SB_OK) return status;

    for (int i = 0; i < sb->count; i++) {
        sb->value[i] = toReplaceWith;
    }
    return SB_OK;
}

sb_status sb_replace_value_with(string_builder_t* sb, const char* toReplaceWith) {
    sb_status status = check_standard_errors(sb, false_malloc, sb_layer_2);
    if (status != SB_OK) return status;

    int len = strlen(toReplaceWith);
    sb->value = malloc(len);
    if (sb->value == NULL) {
        return SB_MEMORY_FAULT;
    }
    sb->count = len;
    for (int i = 0; i < len; i++) {
        sb->value[i] = toReplaceWith[i];
    }
    return SB_OK;
}

sb_status sb_insert(string_builder_t* sb, const char* to_insert, const int start_index) {
    sb_status status = check_standard_errors(sb, false_malloc, sb_layer_2);
    if (status != SB_OK) return status;
    if (start_index < 0 || start_index > sb->count) return SB_OUT_OF_BOUNDS;

    int length_initial = sb->count;
    int lenght_to_insert = strlen(to_insert);
    int finalLenght = length_initial + lenght_to_insert;

    char* savedValue = sb->value;
    sb->value = malloc(finalLenght);
    char* value = sb->value;
    if (sb->value == NULL) return SB_MEMORY_FAULT;

    int end_insertion_index = start_index + lenght_to_insert;
    int i = 0;

    while (i < start_index) {
        value[i] = savedValue[i];
        i++;
    }
    while (i < end_insertion_index) {
        value[i] = to_insert[i - start_index];
        i++;
    }
    while (i < finalLenght) {
        value[i] = savedValue[i - lenght_to_insert];
        i++;
    }

    sb->count = finalLenght;
    free(savedValue);
    return SB_OK;
}

sb_status sb_replace(string_builder_t* sb, const int start, const int end,
                     const char* toReplaceWith) {
    if (start > end || start < 0 || end > sb->count) return SB_OUT_OF_BOUNDS;

    sb_status status = check_standard_errors(sb, false_malloc, sb_layer_2);
    if (status != SB_OK) return 0;

    int length_initial = sb->count;
    int lenght_to_insert = strlen(toReplaceWith);
    int length_to_remove = end - start;
    int final_lenght = length_initial + lenght_to_insert - length_to_remove;

    char* savedValue = sb->value;
    sb->value = malloc(final_lenght);
    char* value = sb->value;
    if (value == NULL) return SB_MEMORY_FAULT;

    int i = 0;
    while (i < start) {
        value[i] = savedValue[i];
        i++;
    }
    while ((i - start) < lenght_to_insert) {
        value[i] = toReplaceWith[i - start];
        i++;
    }

    int shifted = lenght_to_insert - length_to_remove;
    while (i < final_lenght) {
        value[i] = savedValue[i - shifted];
        i++;
    }

    sb->count = final_lenght;

    free(savedValue);
    return SB_OK;
}

sb_status sb_append(string_builder_t* sb, const char* to_append) {
    sb_status status = check_standard_errors(sb, false_malloc, sb_layer_2);
    if (status != SB_OK) return status;

    int len_initial = sb->count;

    int len_final = strlen(to_append) + len_initial;
    char* savedValue = sb->value;
    sb->value = malloc(len_final);
    if (sb->value == NULL) return SB_MEMORY_FAULT;

    char* value = sb->value;

    for (int i = 0; i < len_final; i++) {
        if (i < len_initial) {
            value[i] = savedValue[i];
        } else {
            value[i] = to_append[i - len_initial];
        }
    }

    sb->count = len_final;
    free(savedValue);
    return SB_OK;
}

int sb_contains(const string_builder_t* sb, const char* toFind) {
    sb_status status = check_standard_errors(sb, false_malloc, sb_layer_2);
    if (status != SB_OK) return 0;

    int sb_len = sb->count;
    int found = 0;
    int len = strlen(toFind);
    if (len > sb_len) return 0;

    for (int i = 0; i <= sb_len - len && found != 1; i++) {
        int innerHaveToBreak = 0;

        for (int b = 0; b < len && found != 1 && innerHaveToBreak != 1; b++) {
            if (sb->value[i + b] != toFind[b]) innerHaveToBreak = 1;
            if (b == (len - 1) && innerHaveToBreak == 0) {
                found = 1;
            }
        }
    }
    return found;
}

sb_status sb_set_char_at(string_builder_t* sb, const char to_set, const int index) {
    sb_status status = check_standard_errors(sb, false_malloc, sb_layer_2);
    if (status != SB_OK) return status;
    if (index < 0 || index > sb->count) return SB_OUT_OF_BOUNDS;
    sb->value[index] = to_set;
    return SB_OK;
}

sb_status sb_to_upper_case(string_builder_t* sb) {
    sb_status status = check_standard_errors(sb, false_malloc, sb_layer_2);
    if (status != SB_OK) return status;

    int shift = 'a' - 'A';
    int len = sb->count;
    char* value = sb->value;

    for (int i; i < len; i++) {
        char* current_char_p = &value[i];
        if (*current_char_p >= 'a' && *current_char_p <= 'z')
            *current_char_p = *current_char_p - shift;
    }

    return SB_OK;
}

sb_status sb_to_lower_case(string_builder_t* sb) {
    sb_status status = check_standard_errors(sb, false_malloc, sb_layer_2);
    if (status != SB_OK) return status;

    int shift = 'a' - 'A';
    int len = sb->count;
    char* value = sb->value;

    for (int i = 0; i < len; i++) {
        char* current_char_p = &value[i];
        if (*current_char_p >= 'A' && *current_char_p <= 'Z')
            *current_char_p = *current_char_p + shift;
    }

    return SB_OK;
};

sb_status sb_delete_char_at(string_builder_t* sb, const int index) {
    sb_status status = check_standard_errors(sb, false_malloc, sb_layer_2);
    if (status != SB_OK) return status;

    int count = sb->count;

    char* copy = sb->value;
    sb->value = malloc(count - 1);
    if (sb->value == NULL) return SB_MEMORY_FAULT;

    char* value = sb->value;

    int shift = 0;
    for (int i = 0; i < count; i++) {
        if (i == index) shift = 1;
        value[i] = copy[i + shift];
    }
    sb->count--;
    free(copy);
    return SB_OK;
}

sb_status sb_delete(string_builder_t* sb, const int start, const int end) {
    sb_status status = check_standard_errors(sb, false_malloc, sb_layer_2);
    if (status != SB_OK) return status;

    if (start > end || start < 0 || end > sb->count) return SB_OUT_OF_BOUNDS;

    int length_to_delete = end - start;
    int count = sb->count;
    int final_length = count - length_to_delete;

    char* saved_value = sb->value;
    sb->value = malloc(final_length);
    char* value = sb->value;

    int i = 0;
    while (i < start) {
        value[i] = saved_value[i];
        i++;
    }

    while (i < end) {
        i++;
    }

    while (i < count) {
        value[i - length_to_delete] = saved_value[i];
        i++;
    }

    sb->count = final_length;
    free(saved_value);
    return SB_OK;
}

int sb_legth(const string_builder_t* sb);
char* sb_copy_of_string_value(const string_builder_t* sb);