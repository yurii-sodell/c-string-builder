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
    if (deep >= sb_layer_4 && str->string_builder_t->count < 0) return status;
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

void sb_free(string_builder_union* sb) {
    free(sb->string_builder_t->value);
    free(sb->string_builder_t);
    free(sb);
}

string_builder_union* sb_allocate_new(const int len) {
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

string_builder_union* sb_substring(const string_builder_union* sb, const int start, const int end) {
    sb_status status1 = check_standard_errors(sb, false_malloc, sb_layer_4);
    if (status1 != SB_OK) return NULL;

    if (start < 0 || end > sb->string_builder_t->count || end <= start) {
        return sb;
    }

    int len = end - start;
    string_builder_union* toReturn = sb_allocate_new(len);
    if (toReturn == NULL) return NULL;

    for (int i = 0; i < len; i++) {
        toReturn->string_builder_t->value[i] = sb->string_builder_t->value[i + start];
    }
    return toReturn;
}

string_builder_union* sb_new() { return sb_allocate_new(0); }

sb_status sb_print(const string_builder_union* sb) {
    sb_status status = check_standard_errors(sb, false_malloc, sb_layer_4);
    if (status != SB_OK) return status;
    printf("\n%.*s", sb->string_builder_t->count, sb->string_builder_t->value);
    return SB_OK;
}

sb_status sb_invert(string_builder_union* sb) {
    sb_status status = check_standard_errors(sb, false_malloc, sb_layer_4);
    if (status != SB_OK) return status;

    int count = sb->string_builder_t->count;

    char* newVal = malloc(count);
    if (newVal == NULL) {
        return SB_MEMORY_FAULT;
    }

    int b = count;
    int i = 0;
    while (i < count) {
        newVal[i++] = sb->string_builder_t->value[--b];
    }
    while (b < count) {
        sb->string_builder_t->value[b] = newVal[b];
        b++;
    }

    free(newVal);
    return SB_OK;
};

sb_status sb_trim(string_builder_union* sb) {
    sb_status status = check_standard_errors(sb, false_malloc, sb_layer_4);
    if (status != SB_OK) return status;

    int count = sb->string_builder_t->count;
    char* value = sb->string_builder_t->value;

    for (int i = 0; i < count; i++) {
        if (value[i] == ' ') {
            sb->string_builder_t->count--;
            for (int b = i; b < count - 1; b++) {
                value[b] = value[b + 1];
            }
        }
    }
    int memory_to_realese = count - sb->string_builder_t->count;
    if (memory_to_realese > 0) {
        char* temp = realloc(value, sb->string_builder_t->count);
        if (temp == NULL) return SB_MEMORY_FAULT;
        value = temp;
    }
    return SB_OK;
}

sb_status sb_replace_all_chars_with(string_builder_union* sb, const char toReplaceWith) {
    sb_status status = check_standard_errors(sb, false_malloc, sb_layer_4);
    if (status != SB_OK) return status;

    for (int i = 0; i < sb->string_builder_t->count; i++) {
        sb->string_builder_t->value[i] = toReplaceWith;
    }
    return SB_OK;
}

sb_status sb_replace_value_with(string_builder_union* sb, const char* toReplaceWith) {
    sb_status status = check_standard_errors(sb, false_malloc, sb_layer_4);
    if (status != SB_OK) return status;

    int len = strlen(toReplaceWith);
    sb->string_builder_t->value = malloc(len);
    if (sb->string_builder_t->value == NULL) {
        return SB_MEMORY_FAULT;
    }
    sb->string_builder_t->count = len;
    for (int i = 0; i < len; i++) {
        sb->string_builder_t->value[i] = toReplaceWith[i];
    }
    return SB_OK;
}

sb_status sb_insert(string_builder_union* sb, const char* to_insert, const int start_index) {
    sb_status status = check_standard_errors(sb, false_malloc, sb_layer_4);
    if (status != SB_OK) return status;
    if (start_index < 0 || start_index > sb->string_builder_t->count) return SB_OUT_OF_BOUNDS;

    int length_initial = sb->string_builder_t->count;
    int lenght_to_insert = strlen(to_insert);
    int finalLenght = length_initial + lenght_to_insert;

    char* savedValue = sb->string_builder_t->value;
    sb->string_builder_t->value = malloc(finalLenght);
    char* value = sb->string_builder_t->value;
    if (sb->string_builder_t->value == NULL) return SB_MEMORY_FAULT;

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

    sb->string_builder_t->count = finalLenght;
    free(savedValue);
    return SB_OK;
}

sb_status sb_replace(string_builder_union* sb, const int start, const int end,
                     const char* toReplaceWith) {
    if (start > end || start < 0 || end > sb->string_builder_t->count) return SB_OUT_OF_BOUNDS;

    sb_status status = check_standard_errors(sb, false_malloc, sb_layer_4);
    if (status != SB_OK) return 0;

    int length_initial = sb->string_builder_t->count;                         // 12
    int lenght_to_insert = strlen(toReplaceWith);                             // 5
    int length_to_remove = end - start;                                       // 12 - 8 => remove 4
    int final_lenght = length_initial + lenght_to_insert - length_to_remove;  // 12 + 5 - 4 => 13

    char* savedValue = sb->string_builder_t->value;
    sb->string_builder_t->value = malloc(final_lenght);
    char* value = sb->string_builder_t->value;
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

    sb->string_builder_t->count = final_lenght;

    free(savedValue);
    return SB_OK;
}

sb_status sb_append(string_builder_union* sb, const char* to_append) {
    sb_status status = check_standard_errors(sb, false_malloc, sb_layer_4);
    if (status != SB_OK) return status;

    int len_initial = sb->string_builder_t->count;

    int len_final = strlen(to_append) + len_initial;
    char* savedValue = sb->string_builder_t->value;
    sb->string_builder_t->value = malloc(len_final);
    if (sb->string_builder_t->value == NULL) return SB_MEMORY_FAULT;

    char* value = sb->string_builder_t->value;

    for (int i = 0; i < len_final; i++) {
        if (i < len_initial) {
            value[i] = savedValue[i];
        } else {
            value[i] = to_append[i - len_initial];
        }
    }

    sb->string_builder_t->count = len_final;
    free(savedValue);
    return SB_OK;
}

int sb_contains(const string_builder_union* sb, const char* toFind) {
    sb_status status = check_standard_errors(sb, false_malloc, sb_layer_4);
    if (status != SB_OK) return 0;

    int sb_len = sb->string_builder_t->count;
    int found = 0;
    int len = strlen(toFind);
    if (len > sb_len) return 0;

    for (int i = 0; i <= sb_len - len && found != 1; i++) {
        int innerHaveToBreak = 0;

        for (int b = 0; b < len && found != 1 && innerHaveToBreak != 1; b++) {
            if (sb->string_builder_t->value[i + b] != toFind[b]) innerHaveToBreak = 1;
            if (b == (len - 1) && innerHaveToBreak == 0) {
                found = 1;
            }
        }
    }
    return found;
}

sb_status sb_set_char_at(string_builder_union* sb, const char to_set, const int index) {
    sb_status status = check_standard_errors(sb, false_malloc, sb_layer_4);
    if (status != SB_OK) return status;
    if (index < 0 || index > sb->string_builder_t->count) return SB_OUT_OF_BOUNDS;
    sb->string_builder_t->value[index] = to_set;
    return SB_OK;
}

sb_status sb_to_upper_case(string_builder_union* sb) {
    sb_status status = check_standard_errors(sb, false_malloc, sb_layer_4);
    if (status != SB_OK) return status;

    int shift = 'a' - 'A';
    int len = sb->string_builder_t->count;
    char* value = sb->string_builder_t->value;

    for (int i; i < len; i++) {
        char* current_char_p = &value[i];
        if (*current_char_p >= 'a' && *current_char_p <= 'z')
            *current_char_p = *current_char_p - shift;
    }

    return SB_OK;
}

sb_status sb_to_lower_case(string_builder_union* sb) {
    sb_status status = check_standard_errors(sb, false_malloc, sb_layer_4);
    if (status != SB_OK) return status;

    int shift = 'a' - 'A';
    int len = sb->string_builder_t->count;
    char* value = sb->string_builder_t->value;

    for (int i = 0; i < len; i++) {
        char* current_char_p = &value[i];
        if (*current_char_p >= 'A' && *current_char_p <= 'Z')
            *current_char_p = *current_char_p + shift;
    }

    return SB_OK;
};

sb_status sb_delete_char_at(string_builder_union* sb, const int index) {
    sb_status status = check_standard_errors(sb, false_malloc, sb_layer_4);
    if (status != SB_OK) return status;

    int count = sb->string_builder_t->count;

    char* copy = sb->string_builder_t->value;
    sb->string_builder_t->value = malloc(count - 1);
    if (sb->string_builder_t->value == NULL) return SB_MEMORY_FAULT;

    char* value = sb->string_builder_t->value;

    int shift = 0;
    for (int i = 0; i < count; i++) {
        if (i == index) shift = 1;
        value[i] = copy[i + shift];
    }
    sb->string_builder_t->count--;
    free(copy);
    return SB_OK;
}

sb_status sb_delete(string_builder_union* sb, const int start, const int end) {
    sb_status status = check_standard_errors(sb, false_malloc, sb_layer_4);
    if (status != SB_OK) return status;

    if (start > end || start < 0 || end > sb->string_builder_t->count) return SB_OUT_OF_BOUNDS;

    int length_to_delete = end - start;
    int count = sb->string_builder_t->count;
    int final_length = count - length_to_delete;

    printf("\n1 %u %u %u", count, length_to_delete, final_length);

    char* saved_value = sb->string_builder_t->value;
    sb->string_builder_t->value = malloc(final_length);
    char* value = sb->string_builder_t->value;

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

    sb->string_builder_t->count = final_length;
    free(saved_value);
    return SB_OK;
}
