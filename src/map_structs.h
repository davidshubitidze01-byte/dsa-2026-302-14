#ifndef MAP_STRUCTS_H
#define MAP_STRUCTS_H

typedef struct {
    double lat;
    double lon;
} Position;

typedef struct House {
    char street[100];
    int number;
    Position pos;
    struct House *next;
} House;

typedef struct Place {
    char name[100];
    Position pos;
    struct Place *next;
} Place;
#endif