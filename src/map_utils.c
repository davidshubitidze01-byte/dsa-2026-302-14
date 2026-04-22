#include "map_structs.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Compara dos cadenas ignorando mayusculas y minúsculas
int strcasecompare(const char *s1, const char *s2) {
    if (!s1 || !s2) return 0;
    return strcasecmp(s1, s2) == 0; //Retorna 1 si són iguals
}

// Carga de houses.txt
House *load_houses(const char *map_name, int *count) {
    char path[100];
    sprintf(path, "maps/%s/houses.txt", map_name);
    FILE *f = fopen(path, "r");
    if (!f) {
        printf("Can't open: %s\n", path);
        return NULL;
    }

    House *head = NULL, *curr = NULL;
    char line[512];
    *count = 0;
    while (fgets(line, sizeof(line), f)) {
        House *new_h = malloc(sizeof(House)); //Pedimos memoria para un nuevo nodo
        new_h->next = NULL;
        //Formato Calle, Numero, Lat, Lon
        char *token = strtok(line, ",");
        if (!token) { free(new_h); continue; }
        strcpy(new_h->street, token);
        new_h->number = atoi(strtok(NULL, ","));
        new_h->pos.lat = atof(strtok(NULL, ","));
        new_h->pos.lon = atof(strtok(NULL, ","));
        new_h->next = NULL;
//Insertar en la lista
        if (!head) head = new_h;
        else curr->next = new_h;
        curr = new_h;
        (*count)++;
    }
    fclose(f);
    return head;
}

//Elimina espacios en blanco al inicio y final de una cadena para facilitart la búsqueda de sitios
void trim(char *s) {
    char *p = s;
    int l = strlen(p);
    while (isspace(p[l - 1])) p[--l] = 0;
    while (*p && isspace(*p)) ++p, --l;
    memmove(s, p, l + 1);
}

// places.txt
Place *load_places(const char *map_name, int *count) {
    char path[100];
    sprintf(path, "maps/%s/places.txt", map_name);
    FILE *f = fopen(path, "r");
    if (!f) {
        printf("Can't open: %s\n", path);
        return NULL;
    }
    Place *head = NULL, *curr = NULL;
    char line[512];
    *count = 0;
    while (fgets(line, sizeof(line), f)) {
        line[strcspn(line, "\r\n")] = 0;//Limpiar saltos de linea
        if (strlen(line) < 10) continue;
        Place *new_p = malloc(sizeof(Place));
        new_p->next = NULL;
        char *token = strtok(line, ","); 
        token = strtok(NULL, ",");
        if (!token) { free(new_p); continue; }
        strncpy(new_p->name, token, 99);
        new_p->name[99] = '\0';
        token = strtok(NULL, ",");
        token = strtok(NULL, ",");
        if (token) new_p->pos.lat = atof(token);
        token = strtok(NULL, ",");
        if (token) new_p->pos.lon = atof(token);
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

//Búsqueda secuencial de una casa específica
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

//Obtiene todos los números que existen en X calle si el numero indicado por user no existe
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

//Sugerencias utilizando levensthein (^^^)
int levenshtein(const char *a, const char *b) {
    if (!a || !b) return 999;
    int m = strlen(a);
    int n = strlen(b);
    int D[m + 1][n + 1];
    for (int i = 0; i <= m; i++) D[i][0] = i;
    for (int j = 0; j <= n; j++) D[0][j] = j;
    for (int i = 1; i <= m; i++) {
        for (int j = 1; j <= n; j++) {
            int cost = (tolower((unsigned char)a[i - 1]) == tolower((unsigned char)b[j - 1])) ? 0 : 1;
            int del = D[i - 1][j] + 1;
            int ins = D[i][j - 1] + 1;
            int sub = D[i - 1][j - 1] + cost;
            
            D[i][j] = (del < ins) ? (del < sub ? del : sub) : (ins < sub ? ins : sub);
        }
    }
    return D[m][n];
}