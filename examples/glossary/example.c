#include <stdio.h> // for fopen, fclose
#include <stdlib.h> // for exit
#include <string.h> // for strlen, strcpy
#include <json/json.h> // for json_object

#include "example.h"

int main (int argc, char** argv) {
  char* filename = "example.json";
  char* buffer = NULL;
  struct json_object *root_obj;
  struct json_object *glossary_obj;
  struct json_object *glossList_obj;
  struct json_object *glossListElement_obj;
  struct json_object *glossSeeAlso_obj;
  struct json_object *glossSeeAlsoElement_obj;
  struct json_object *glossDiv_obj;
  glossary_s glossary = {};
  glossDiv_s glossDiv = {};
  glossList_s* glossList;
  int glossList_len = 0;
  int i = 0;
  int glossSeeAlso_len = 0;
  int j = 0;
  char** glossSeeAlso;

  get_file_contents(filename, &buffer);
  printf("%s",buffer);

  root_obj = json_tokener_parse(buffer);
  printf("root_obj.to_string():\n%s\n\n", json_object_to_json_string(root_obj));

  glossary_obj = json_object_object_get(root_obj, "glossary");
    printf("glossary_obj.to_string():\n%s\n\n", json_object_to_json_string(glossary_obj));
    glossary.title = get_json_string_val(glossary_obj, "title");
    glossary.pageCount = json_object_get_int(json_object_object_get(glossary_obj, "pageCount"));
    glossDiv_obj = json_object_object_get(glossary_obj, "GlossDiv");
      printf("glossDiv_obj.to_string():\n%s\n\n", json_object_to_json_string(glossDiv_obj));
      glossDiv.title = get_json_string_val(glossDiv_obj, "title");
        glossList_obj = json_object_object_get(glossDiv_obj, "GlossList");
        glossList_len = json_object_array_length(glossList_obj);
        glossList = malloc(sizeof(glossList) * glossList_len);
        for (i = 0; i < glossList_len; i += 1) {
          glossListElement_obj = json_object_array_get_idx(glossList_obj, i);
          glossList[i].id = get_json_string_val(glossListElement_obj, "ID");
          glossList[i].sortAs = get_json_string_val(glossListElement_obj, "SortAs");
          glossList[i].glossTerm = get_json_string_val(glossListElement_obj, "GlossTerm");
          glossList[i].acronym = get_json_string_val(glossListElement_obj, "Acronym");
          glossList[i].abbrev = get_json_string_val(glossListElement_obj, "Abbrev");
          glossList[i].glossDef = get_json_string_val(glossListElement_obj, "GlossDef");
          // GlossSeeAlso is a string Array
          glossSeeAlso_obj = json_object_object_get(glossListElement_obj,"GlossSeeAlso");
          glossSeeAlso_len = json_object_array_length(glossSeeAlso_obj);
          glossSeeAlso = malloc(sizeof(char*) * glossSeeAlso_len);
          for (j = 0; j < glossSeeAlso_len; j += 1) {
            glossSeeAlsoElement_obj = json_object_array_get_idx(glossSeeAlso_obj, j);
            glossSeeAlso[j] = memstrcpy(json_object_get_string(glossSeeAlsoElement_obj));
          }
          glossList[i].glossSeeAlso = glossSeeAlso;
        }
      glossDiv.glossList = glossList;
    glossary.glossDiv = glossDiv;

  // returns null
  root_obj = json_object_object_get(root_obj, "doesn't exist");
  printf("root_obj[\"doesn't exist\"].to_string():\n%s\n", json_object_to_json_string(root_obj));

  // causes segfault
  printf("root_obj[\"doesn't exist\"][\"key of null\"].to_string():\nNOTE: this will segfault\n\n");
  root_obj = json_object_object_get(root_obj, "key of null");
  printf("%s",json_object_to_json_string(root_obj));


  free(buffer);
  return EXIT_SUCCESS;
}

inline char* get_json_string_val(json_object* json_obj, const char* key) {
  return memstrcpy(json_object_get_string(json_object_object_get(json_obj, key)));
}

inline char* memstrcpy(const char* in) {
  char * out;

  out = malloc(strlen(in) + 1);
  strcpy(out, in);

  return out;
}

/**
 * Place the contents of the specified file into a memory buffer
 *
 * @param[in] filename The path and name of the file to read
 * @param[out] filebuffer A pointer to the contents in memory
 * @return status 0 success, 1 on failure
 */
int get_file_contents(const char* filename, char** outbuffer) {
  FILE* file = NULL;
  long filesize;
  const int blocksize = 1;
  size_t readsize;
  char* filebuffer;

  // Open the file
  file = fopen(filename, "r");
  if (NULL == file)
  {
    printf("'%s' not opened\n", filename);
    exit(EXIT_FAILURE);
  }

  // Determine the file size
  fseek(file, 0, SEEK_END);
  filesize = ftell(file);
  rewind (file);

  // Allocate memory for the file contents
  filebuffer = (char*) malloc(sizeof(char) * filesize);
  *outbuffer = filebuffer;
  if (filebuffer == NULL)
  {
    fputs ("malloc out-of-memory", stderr);
    exit(EXIT_FAILURE);
  }

  // Read in the file
  readsize = fread(filebuffer, blocksize, filesize, file);
  if (readsize != filesize)
  {
    fputs ("didn't read file completely",stderr);
    exit(EXIT_FAILURE);
  }

  // Clean exit
  fclose(file);
  return EXIT_SUCCESS;
}
