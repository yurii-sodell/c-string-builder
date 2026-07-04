typedef struct string_builder_t {
    char* value;
    int count;
} string_builder_t;

typedef enum {
    SB_OK,
    SB_NOK,
    SB_OUT_OF_BOUNDS,
    SB_MEMORY_FAULT,
    SB_IS_NULL,
} sb_status;

typedef struct string_builder_union {
    sb_status status;
    string_builder_t* string_builder_t;

} string_builder_union;

string_builder_union* sb_new();
string_builder_union* sb_from_string(const char* initialStr);
string_builder_union* sb_substring(const string_builder_union* str, int start, int end);

int sb_contains(const string_builder_union* str, const char* toFind);
sb_status sb_print(const string_builder_union* str);
sb_status sb_invert(string_builder_union* str);
sb_status sb_trim(string_builder_union* str);
sb_status sb_append(string_builder_union* str, const char* to_append);
sb_status sb_set_char_at(string_builder_union* str, const char to_set, const int index);
sb_status sb_insert(string_builder_union* str, const char* to_insert, const int start_index);
sb_status sb_to_upper_case(string_builder_union* sb);
sb_status sb_to_lower_case(string_builder_union* sb);
sb_status sb_replace_all_chars_with(string_builder_union* str, const char toReplaceWith);
sb_status sb_replace_value_with(string_builder_union* str, const char* toReplaceWith);
sb_status sb_replace(string_builder_union* sb, const int start, const int end,
                     const char* toReplaceWith);
sb_status sb_delete_char_at(string_builder_union* sb, const int index);
sb_status sb_delete(string_builder_union* sb, const int start, const int end);

void sb_handle_status(sb_status status);
void sb_free(string_builder_union* sb);