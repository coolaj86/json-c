int get_file_contents(const char* filename, char** outbuffer);
inline char* get_json_string_val(json_object* json_obj, const char* key);
inline char* memstrcpy(const char* in);

typedef struct {
  char* id;
  char* sortAs;
  char* glossTerm;
  char* acronym;
  char* abbrev;
  char* glossDef;
  char** glossSeeAlso;
} glossList_s;

typedef struct {
  char* title;
  glossList_s* glossList;
} glossDiv_s;

typedef struct {
  char* title;
  int pageCount;
  glossDiv_s glossDiv;
} glossary_s;
