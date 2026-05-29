#ifndef MAP_UTILS_H
#define MAP_UTILS_H
#include "map_structs.h"

int strcasecompare(const char *s1, const char *s2);
int match_street(const char *input, const char *database);
House *load_houses(const char *map_name, int *count);
Place *load_places(const char *map_name, int *count);
Street *load_streets(const char *map_name, int *count);
House *find_house(House *head, char *street, int number);
Place *find_place(Place *head, char *name);
int *get_valid_numbers(House *head, const char *street, int *total);
int levenshtein(const char *a, const char *b);
double haversine(Position posA, Position posB);
Position midpoint(Position a, Position b);
Street *find_closest_street(Street *head, Position user_pos);
void print_connected_streets(Street *head, Street *closest);
IntersectionMap *create_hash_map(int size);
int hash_function(unsigned long long id, int table_size);
void insert_hash_map(IntersectionMap *map, Street *street);
IntersectionMap *build_intersection_graph(Street *streets_head, int table_size);
void print_connected_streets_hash(IntersectionMap *map, Street *closest);
void free_hash_map(IntersectionMap *map);

#endif