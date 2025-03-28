//
// Created by foura on 05/03/2025.
//

#ifndef MAP_H
#define MAP_H
#define ZONE 1
#define ZONE 2
#define ZONE 3

// Définition des cases
enum TileType {
    EMPTY = 0, PLAYER = 1, PNJ = 2, PLANT1 = 3, ROCK1 = 4, WOOD1 = 5,
    PLANT2 = 6, ROCK2 = 7, WOOD2 = 8, PLANT3 = 9, ROCK3 = 10, WOOD3 = 11,
    MONSTER = 12, BOSS = 99, PORTAL_1_2 = -2, PORTAL_2_3 = -3, WALL = -1
};
typedef struct {
    int **grid;
    int width;
    int height;
    int zoneID;
}MapZone;

MapZone initZone(int width, int height, int zoneID);
void placeRocks(MapZone *zone, int numRocks);
void freeZone(MapZone *zone);
void printMap(MapZone zone);
void placePNJ(MapZone *zone);
void placePortals(MapZone *zone);

#endif //MAP_H
