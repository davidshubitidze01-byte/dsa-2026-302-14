#include "map_structs.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

//Encuentra carreteras aun si el casing no es igual
int strcasecompare(const char *s1, const char *s2) {
  while (*s1 && *s2) {
    if (tolower((unsigned char)*s1) != tolower((unsigned char)*s2))
    return 0;
    s1++; s2++;
  }
  return tolower((unsigned char)*s1) == tolower((unsigned char)*s2);
}
//Carga de houses.txt
House* load_houses(const char* map_name, int *count) {
  char path [100];
  sprintf(path, "maps/%s/houses.txt", map_name);
  FILE *f = fopen(path, "r");
  if (!f) return NULL;
  House *head = NULL, *curr = NULL;
  char line[256];
  *count = 0;
  while (fgets(line, sizeof(line), f)){
    House *new_h = malloc(sizeof(House));
    char *token = strtok(line, "|");
    strcpy(new_h->street, token);
    new_h->number = atoi(strtok(NULL, "|"));
    new_h->pos.lat = atof(strtok(NULL, "|"));
    new_h->pos.lon = atof(strtok(NULL, "|"));
    new_h->next = NULL;

    if (!head) head = new_h;
    else curr->next = new_h;
    curr = new_h;
    (*count)++;
  }
  fclose(f);
  return head;
}