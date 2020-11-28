#include <glib.h>
#include <stdio.h>
#include <string.h>
#define MAXLEN 1024

int add_word(GHashTable *hsh, char *word) {
  int free_word = TRUE;
  int *val = g_hash_table_lookup(hsh, word);
  if (val == NULL) {
    val = (int*)g_malloc(sizeof(int));
    *val = 0;
    g_hash_table_insert(hsh, word, val);
    free_word = FALSE;
  }
  *val = (*val) + 1;
  return free_word;
}

gint word_freq_comparator(gconstpointer left, gconstpointer right, gpointer data) {
  GHashTable *hsh = (GHashTable *)data;
  int *left_val = g_hash_table_lookup(hsh, *((char**)left));
  int *right_val = g_hash_table_lookup(hsh, *((char**)right));
  int l = *left_val;
  int r = *right_val;
  if (l < r)
    return 1;
  if (l == r)
    return 0;
  return -1;
}

gint word_lexicographic_comparator(gconstpointer _left, gconstpointer _right) {
  char *left = *((char**)_left);
  char *right = *((char**)_right);
  return strncmp(left, right, MAXLEN+1);
}

int main() {
  GHashTable *hsh = g_hash_table_new(g_str_hash, g_str_equal);
  char buf[MAXLEN+1];

  while (1) {
    char *s = fgets(buf, MAXLEN, stdin);
    if (s == NULL) {
      break;
    }
    char **words = g_strsplit_set(s, " \t\n\r\x0b\x0c!\"#$%&\\'()*+,-./:;<=>?@[\\]^_`{|}~", -1);
    int i = 0;
    while (words[i] != NULL) {
      char *word_copy = g_strndup(words[i], MAXLEN+1);
      if (words[i][0] == 0) {
        i++;
        continue;
      }
      int free_word = add_word(hsh, word_copy);
      if (free_word)
        g_free(word_copy);
      i++;
    }
    g_strfreev(words);
  }

  int table_size = g_hash_table_size(hsh);
  printf("There are %d keys in the hash table\n", table_size);

  GArray *ar = g_array_sized_new(FALSE, FALSE, sizeof(char *), table_size);

  GHashTableIter iter;
  gpointer key, value;

  g_hash_table_iter_init(&iter, hsh);

  while (g_hash_table_iter_next(&iter, &key, &value)) {
    g_array_append_val(ar, key);
  }

  g_array_sort(ar, word_lexicographic_comparator);
  g_array_sort_with_data(ar, word_freq_comparator, hsh);

  for (int i = 0; i < table_size; i++) {
    char *word = g_array_index(ar, char *, i);
    int *val = g_hash_table_lookup(hsh, word);
    printf("%s: %d\n", word, *val);
  }

  g_array_free(ar, TRUE);
  g_hash_table_destroy(hsh);
  return 0;
}