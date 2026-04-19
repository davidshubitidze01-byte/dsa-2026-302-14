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
  int opt; scanf("%d", &opt); getchar();
  if (opt == 1) {
  } else if (opt == 2) {

  }

  return 0;
}