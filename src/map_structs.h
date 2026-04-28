#ifndef MAP_STRUCTS_H
#define MAP_STRUCTS_H

//Estructura para las coordenadas
typedef struct {
  double lat;
  double lon;
} Position;

//Nodo de lista enlazada para las casas
typedef struct House {
  char street[100]; //Nombre de la calle
  int number; //Numero de la casa
  Position pos; //Coordenada
  struct House *next; //Puntero al siguiente nodo
} House;

//nodo de lista enlazada para lugares de interés
typedef struct Place {
  char name[100]; //nombre
  Position pos; //Coordenadas
  struct Place *next; //Puntero al siguiente nodo
} Place;
#endif