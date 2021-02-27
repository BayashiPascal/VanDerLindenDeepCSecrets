#include "stdlib.h"
#include "stdio.h"
#include "string.h"

struct file_struct;
#define file struct file_struct*
struct file_struct {
  file flink;
  char* fname;
};

file alloc_file(char* s) {
  file f = malloc(sizeof(struct file_struct));
  f->fname = strdup(s);
  f->flink = NULL;
  return f;
}

#define FILE_HASH 2
file file_hash_table[FILE_HASH] = {NULL, NULL};

int hash_filename(char* s) {
  //return 0;
  int length = strlen(s);
  return (length + 4 * (s[0] + 4 * s[length / 2])) % FILE_HASH;
}

file find_filename(char* s) {
  int hash_value = hash_filename(s);
  file f = file_hash_table[hash_value];
  while (f != NULL) {

    if (strcmp(f->fname, s) == 0) {

      return f;

    }

    f = f->flink;

  }

  f = alloc_file(s);
  f->flink = file_hash_table[hash_value];
  file_hash_table[hash_value] = f;
  return f;

}

int main() {

#define NB_TEST 5
  char* test_filename[NB_TEST] = {"A", "B", "AB", "A", "C"};
  for (int i=0; i < NB_TEST; ++i) {

    file f = find_filename(test_filename[i]);
    printf("f: %p f->fname: %s\n", f, f->fname);
    for (int j=0; j<FILE_HASH; ++j) {
      file g = file_hash_table[j];
      printf("file_hash_table %d: ", j);
      while (g != NULL) {
        printf("(%p, %s), ", g, g->fname);
        g = g->flink;
      }
      printf("\n");
    }

  }

  return 0;

}
