#include "map_structs.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Compara dos cadenas ignorando mayusculas y minúsculas
int strcasecompare(const char *s1, const char *s2) {
  while (*s1 && *s2) {
    if (tolower((unsigned char)*s1) != tolower((unsigned char)*s2))
      return 0;
    s1++;
    s2++;
  }
  return tolower((unsigned char)*s1) == tolower((unsigned char)*s2);
}

// Carga de houses.txt
House *load_houses(const char *map_name, int *count) {
    char path[100];
    sprintf(path, "maps/%s/houses.txt", map_name);
    FILE *f = fopen(path, "r");
    if (!f) return NULL;

    House *head = NULL, *curr = NULL;
    char line[256];
    *count = 0;
    while (fgets(line, sizeof(line), f)) {
        House *new_h = malloc(sizeof(House)); //Pedimos memoria para un nuevo nodo
        char *token = strtok(line, ",");
        if (!token) { free(new_h); continue; }
        strcpy(new_h->street, token);
        new_h->number = atoi(strtok(NULL, ","));
        new_h->pos.lat = atof(strtok(NULL, ","));
        new_h->pos.lon = atof(strtok(NULL, ","));
        new_h->next = NULL;

        if (!head) head = new_h;
        else curr->next = new_h;
        curr = new_h;
        (*count)++;
    }
    fclose(f);
    return head;
}

// places.txt
Place *load_places(const char *map_name, int *count) {
    char path[100];
    sprintf(path, "maps/%s/places.txt", map_name);
    FILE *f = fopen(path, "r");
    if (!f) return NULL;

    Place *head = NULL, *curr = NULL;
    char line[256];
    *count = 0;
    while (fgets(line, sizeof(line), f)) {
        Place *new_p = malloc(sizeof(Place));
        char *token = strtok(line, "|");
        strcpy(new_p->name, token);
        new_p->pos.lat = atof(strtok(NULL, "|"));
        new_p->pos.lon = atof(strtok(NULL, "|"));
        new_p->next = NULL;

        if (!head) head = new_p;
        else curr->next = new_p;
        curr = new_p;
        (*count)++;
    }
    fclose(f);
    return head;
}

//Abreviaturas (C. = Carrer)
int match_street(const char *input, const char *database) {
    char temp_in[100], temp_db[100];
    strcpy(temp_in, input);
    strcpy(temp_db, database);
    if (strncasecmp(temp_in, "C. ", 3) == 0 && strncasecmp(temp_db, "Carrer ", 7) == 0) {
        return strcasecompare(temp_in + 3, temp_db + 7);
    }
    return strcasecompare(temp_in, temp_db);
}

//Búsqueda secuencial
House* find_house(House *head, char *street, int number) {
    House *curr = head;
    while (curr) {
        if (match_street(street, curr->street) && curr->number == number) {
            return curr;
        }
        curr = curr->next;
    }
    return NULL;
}

//Búsqueda de lugares
Place* find_place(Place *head, char *name) {
    Place *curr = head;
    while (curr) {
        if (strcasecompare(name, curr->name)) return curr;
        curr = curr->next;
    }
    return NULL;
}

//Obtiene todos los números que existen en X calle
int* get_valid_numbers(House *head, const char *street, int *total) {
    int *numbers = malloc(sizeof(int) * 100); 
    *total = 0;
    House *curr = head;
    while (curr && *total < 100) {
        if (match_street(street, curr->street)) {
            numbers[*total] = curr->number;
            (*total)++;
        }
        curr = curr->next;
    }
    return numbers;
}

int levenshtein(const char *a, const char *b) {
    int m = strlen(a);
    int n = strlen(b);
    int D[m + 1][n + 1];
    for (int i = 0; i <= m; i++) D[i][0] = i;
    for (int j = 0; j <= n; j++) D[0][j] = j;
    for (int i = 1; i <= m; i++) {
        for (int j = 1; j <= n; j++) {
            int cost = (a[i - 1] == b[j - 1]) ? 0 : 1;
            int del = D[i - 1][j] + 1;
            int ins = D[i][j - 1] + 1;
            int sub = D[i - 1][j - 1] + cost;
            
            D[i][j] = (del < ins) ? (del < sub ? del : sub) : (ins < sub ? ins : sub);
        }
    }
    return D[m][n];
}