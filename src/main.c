#include "map_structs.h"
#include "map_utils.h"
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <string.h>

int main() {
    char map_name[50];
    printf("Enter map name: ");
    scanf("%s", map_name);
    int h_cnt, p_cnt;
    // Cargamos los datos de los ficheros a las listas enlazadas
    House *houses = load_houses(map_name, &h_cnt);
    Place *places = load_places(map_name, &p_cnt);
    printf("%d houses loaded. %d places loaded. \n", h_cnt, p_cnt);
    printf("\n ORIGIN \n Where are you? Address (1), Place (2), Coordinate (3)? ");
    int opt;
    scanf("%d", &opt);
    getchar();
    if (opt == 1) { //Busqueda por dirección
        char street[100];
        int num;
        printf("Enter street name: ");
        fgets(street, 100, stdin);
        street[strcspn(street, "\n")] = 0;
        printf("Enter street number: ");
        scanf("%d", &num);
        House *h = find_house(houses, street, num);
        if (h) {
            printf("\nFound at (%f, %f)\n", h->pos.lat, h->pos.lon);
        } else {
            //Si no existe la dirección, buscamos si la calle es válida pero el número no
            int total_nums = 0;
            int *valid_nums = get_valid_numbers(houses, street, &total_nums);
            if (total_nums > 0) {
                printf("Street found, but number %d is invalid. Valid numbers:\n", num);
                for (int i = 0; i < total_nums; i++) printf("%d ", valid_nums[i]);
                printf("\nEnter a valid number: ");
                scanf("%d", &num);
                h = find_house(houses, street, num);
                if (h) printf("Found at (%f, %f)\n", h->pos.lat, h->pos.lon);
            } else {
                //Si la calle no existe, sugerimos la más parecida usando levenshtein
                printf("Street not found. Did you mean: ");
                House *curr = houses;
                char *best_match = NULL;
                int min_dist = 999;
                while (curr) {
                    int d = levenshtein(street, curr->street);
                    if (d < min_dist) { min_dist = d; best_match = curr->street; }
                    curr = curr->next;
                }
                if (best_match) printf("%s?\n", best_match);
            }
            free(valid_nums);
        }
    } else if (opt == 2) { //Búsqueda por lugar
        char p_name[100];
        printf("Enter place name: ");
        fgets(p_name, 100, stdin);
        p_name[strcspn(p_name, "\n")] = 0;

        Place *p = find_place(places, p_name);
        if (p) {
            printf("\nFound at (%f, %f)\n", p->pos.lat, p->pos.lon);
        } else {
            printf("Place not found.\n");
        }

    } else if (opt == 3) { //Coordenadas
        printf("Not implemented yet.\n");
    }

    return 0;
}