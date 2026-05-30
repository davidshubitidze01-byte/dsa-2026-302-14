#include "map_utils.h"
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>

int main() {
  char map_name[50];
  int h_cnt, p_cnt, s_cnt;
  Position origin_pos;
  int valid_origin = 0;
  int hash_size;
  int opt;

  House *houses = NULL;
  Place *places = NULL;
  Street *streets = NULL;
  IntersectionMap *map = NULL;
  Street *closest = NULL;

  char street[100];
  int num;
  int total_nums;
  int *valid_nums = NULL;
  House *h = NULL;
  House *curr = NULL;
  char *best_match = NULL;
  int min_dist;
  char p_name[100];
  char p_name_guions[120];
  Place *p = NULL;

  struct timespec start_time,
      end_time; // Cronometro para el calculo del tiempo de la busqueda
                // secuencial del lab4 y de la busqueda con hasp map del lab5
  double time_lab4;
  double time_lab5;

  printf("Enter map name: ");
  scanf("%s", map_name);
  houses = load_houses(map_name, &h_cnt);
  places = load_places(map_name, &p_cnt);
  streets = load_streets(map_name, &s_cnt);
  printf("%d houses loaded. %d places loaded. %d streets loaded.\n", h_cnt,
         p_cnt, s_cnt);

  hash_size = (int)(s_cnt * 1.5);
  map = build_intersection_graph(streets, hash_size);

  printf(
      "\n ORIGIN \n Where are you? Address (1), Place (2), Coordinate (3)? ");
  scanf("%d", &opt);
  while (getchar() != '\n')
    ;

  if (opt == 1) { // Busqueda por dirección
    printf("Enter street name: ");
    fgets(street, 100, stdin);
    street[strcspn(street, "\n")] = 0;
    printf("Enter street number: ");
    scanf("%d", &num);
    h = find_house(houses, street, num);
    if (h) {
      printf("\nFound at (%f, %f)\n", h->pos.lat, h->pos.lon);
      origin_pos = h->pos;
      valid_origin = 1;
    } else {
      // Si no existe la dirección, buscamos si la calle es válida pero el
      // número no
      total_nums = 0;
      valid_nums = get_valid_numbers(houses, street, &total_nums);
      if (total_nums > 0) {
        printf("Street found, but number %d is invalid. Valid numbers:\n", num);
        for (int i = 0; i < total_nums; i++)
          printf("%d ", valid_nums[i]);
        printf("\nEnter a valid number: ");
        scanf("%d", &num);
        h = find_house(houses, street, num);
        if (h) {
          printf("Found at (%f, %f)\n", h->pos.lat, h->pos.lon);
          origin_pos = h->pos;
          valid_origin = 1;
        }
      } else {
        // Si la calle no existe, sugerimos la más parecida usando levenshtein
        printf("Street not found. Did you mean?: ");
        curr = houses;
        best_match = NULL;
        min_dist = 999;
        while (curr) {
          int d = levenshtein(street, curr->street);
          if (d < min_dist) {
            min_dist = d;
            best_match = curr->street;
          }
          curr = curr->next;
        }
        if (best_match)
          printf("%s?\n", best_match);
      }
      free(valid_nums);
    }
  } else if (opt == 2) { // Búsqueda por lugar
    printf("Enter place name: ");
    fgets(p_name, 100, stdin);
    p_name[strcspn(p_name, "\n")] = 0;
    p = find_place(places, p_name);

    if (!p) {
      sprintf(p_name_guions, "\"%s\"", p_name);
      p = find_place(places, p_name_guions);
    }

    if (p) {
      printf("\nFound at (%f, %f)\n", p->pos.lat, p->pos.lon);
      origin_pos = p->pos;
      valid_origin = 1;
    } else {
      printf("Place not found.\n");
    }

  } else if (opt == 3) { // Coordenadas
    printf("Enter latitude and longitude (e.g. 41.4037 2.1934): ");
    scanf("%lf %lf", &origin_pos.lat, &origin_pos.lon);
    valid_origin = 1;
  }
  if (valid_origin && streets) {
    closest = find_closest_street(streets, origin_pos);
    if (closest) { // Comenzamos la medicion de tiempo de la busqueda en el Lab4
                   // y en el Lab5
      printf("Busqueda Secuencial del Lab 4 \n");
      clock_gettime(CLOCK_MONOTONIC, &start_time);
      print_connected_streets(streets, closest);
      clock_gettime(CLOCK_MONOTONIC, &end_time);
      time_lab4 = (end_time.tv_sec - start_time.tv_sec) * 1000000 +
                  (end_time.tv_nsec - start_time.tv_nsec) / 1000;

      printf("Busqueda Secuencial del Lab 5 \n");
      clock_gettime(CLOCK_MONOTONIC, &start_time);
      print_connected_streets_hash(map, closest);
      clock_gettime(CLOCK_MONOTONIC, &end_time);
      time_lab5 = (end_time.tv_sec - start_time.tv_sec) * 1000000 +
                  (end_time.tv_nsec - start_time.tv_nsec) / 1000;

      printf("Map size (Streets):  %d\n", s_cnt);
      printf("Latencia Lab 4: %.2f microsegons\n", time_lab4);
      printf("Latencia Lab 5:   %.2f microsegons\n", time_lab5);

      // Lab 6
      int opt_d;
      Position dest_pos;
      int valid_dest = 0;

      printf("\n--- DESTINATION ---\nWhere do you want to go? Address (1), "
             "Place (2) or Coordinate (3)? ");
      scanf("%d", &opt_d);
      while (getchar() != '\n')
        ;

      if (opt_d == 1) {
        printf("Enter street name: ");
        fgets(street, 100, stdin);
        street[strcspn(street, "\n")] = 0;
        printf("Enter street number: ");
        scanf("%d", &num);
        House *h_d = find_house(houses, street, num);
        if (h_d) {
          printf("Found at (%f, %f)\n", h_d->pos.lat, h_d->pos.lon);
          dest_pos = h_d->pos;
          valid_dest = 1;
        } else {
          total_nums = 0;//Si la calle existe pero el número es inválido
          valid_nums = get_valid_numbers(houses, street, &total_nums);
          if (total_nums > 0) {
            printf("Street found, but number %d is invalid. Valid numbers:\n", num);
            for (int i = 0; i < total_nums; i++)
              printf("%d ", valid_nums[i]);
            printf("\nEnter a valid number: ");
            scanf("%d", &num);
            h_d = find_house(houses, street, num);
            if (h_d) {
              printf("Found at (%f, %f)\n", h_d->pos.lat, h_d->pos.lon);
              dest_pos = h_d->pos;
              valid_dest = 1;
            }
          } else {
            //Si la calle no existe, sugerimos la más parecida usando levenshtein:
            printf("Street not found. Did you mean?: ");
            curr = houses;
            best_match = NULL;
            min_dist = 999;
            while (curr) {
              int d = levenshtein(street, curr->street);
              if (d < min_dist) {
                min_dist = d;
                best_match = curr->street;
              }
              curr = curr->next;
            }
            if (best_match)
              printf("%s?\n", best_match);
          }
          free(valid_nums);
        }
      } else if (opt_d == 2) {
        printf("Enter place name: ");
        fgets(p_name, 100, stdin);
        p_name[strcspn(p_name, "\n")] = 0;
        Place *p_d = find_place(places, p_name);
        if (!p_d) {
          sprintf(p_name_guions, "\"%s\"", p_name);
          p_d = find_place(places, p_name_guions);
        }
        if (p_d) {
          printf("Found at (%f, %f)\n", p_d->pos.lat, p_d->pos.lon);
          dest_pos = p_d->pos;
          valid_dest = 1;
        }
      } else if (opt_d == 3) {
        printf("Enter latitude and longitude: ");
        scanf("%lf %lf", &dest_pos.lat, &dest_pos.lon);
        valid_dest = 1;
      }
      if (valid_dest) {
        Street *dest_closest = find_closest_street(streets, dest_pos);
        if (dest_closest) {
          printf("Closest street: %s\n", dest_closest->name);
          printf("Between %llu (%f, %f) and %llu (%f, %f)\n", dest_closest->from_id, dest_closest->from_pos.lat, dest_closest->from_pos.lon, dest_closest->to_id, dest_closest->to_pos.lat, dest_closest->to_pos.lon);

          // BFS
          PathNode *route = bfs_pathfinding(map, closest, dest_closest);
          print_route_directions(route, dest_closest);
          // Liberamos la memoria del camino encontrado
          void free_path_list(PathNode * head);
          free_path_list(route);
        }
      }
    }
  }

  while (houses) {
    House *t = houses;
    houses = houses->next;
    free(t);
  }
  while (places) {
    Place *t = places;
    places = places->next;
    free(t);
  }
  while (streets) {
    Street *t = streets;
    streets = streets->next;
    free(t);
  }

  if (map) {
    free_hash_map(map);
  }
  return 0;
}