#ifndef MAP_STRUCTS_H
#define MAP_STRUCTS_H
//Estructura para coordenadas
typedef struct {
  double lat;
  double lon;
} Position;

//Nodo de lista enlazada para casas
typedef struct House {
  char street[100];
  int number;
  Position pos;
  struct House *next;
} House;

//nodo de lista enlazada para lugares de interés
typedef struct Place {
  char name[100];
  Position pos;
  struct Place *next;
} Place;
#endif