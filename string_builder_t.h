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


typedef struct string_builder_union
{
    sb_status status;
    string_builder_t* string_builder_t;
    
} string_builder_union;



string_builder_union* sb_from_string(const char* initialStr);
string_builder_union* sb_substring(const string_builder_union* str, int start, int end);

int sb_contains(const string_builder_union* str, const char* toFind);
sb_status sb_print(const string_builder_union* str);
sb_status sb_invert(string_builder_union* str);
sb_status sb_trim(string_builder_union* str);
sb_status sb_replace_all_chars_with(string_builder_union* str, char toReplaceWith);
sb_status sb_replace_value_with(string_builder_union* str, char* toReplaceWith);
sb_status sb_append(string_builder_union* str, char* to_append);
sb_status sb_set_char_at(string_builder_union* str, char to_set);

void sb_handle_status(sb_status status);