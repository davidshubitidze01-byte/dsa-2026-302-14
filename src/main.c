#include "map_structs.h"
#include "map_utils.h"
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

int main() {
  char map_name[50];
  printf("Enter map name: ");
  scanf("%s", map_name);
  int h_cnt, p_cnt;
  House *houses = load_houses(map_name, &h_cnt);
  Place *places = load_places(map_name, &p_cnt);
  printf("%d houses loades. %d places loaded. \n", h_cnt, p_cnt);

  printf("\n ORIGIN \n Where are you? Adress (1), Place (2), Coordinate (3)?");
  int opt;
  scanf("%d", &opt);
  getchar();
  if (opt == 1) {
    char street[100];
      int num;
      printf("Enter street name (e.g. \"Carrer de Roc Boronat\"): ");
      fgets(street, 100, stdin);
      street[strcspn(street, "\n")] = 0;

      printf("Enter street number (e.g. \"138\"): ");
      scanf("%d", &num);

      House *h = find_house(houses, street, num);
      if (h) {
          printf("\nFound at (%f, %f)\n", h->pos.lat, h->pos.lon);
      } else {
          printf("Address not found.\n");
      }
  } else if (opt == 2) {
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
  } else if (opt == 3) {
      printf("Not implemented yet.\n");
  }

  return 0;
}