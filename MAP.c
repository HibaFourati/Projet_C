//
// Created by foura on 05/03/2025.
//
#include <stdio.h>
#include <stdlib.h>
#include "map.h"

MapZone initZone(int width, int height, int zoneID) {
    MapZone zone;
    zone.width = width;
    zone.height = height;
    zone.zoneID = zoneID;
    zone.grid = malloc(height * sizeof(int *));
    for (int i = 0; i < height; i++) {
        zone.grid[i] = malloc(width * sizeof(int));
        for (int j = 0; j < width; j++) {
            zone.grid[i][j] = 0;  // Initialise toutes les cases à "vide"
        }
    }
    return zone;
}

// Placement des portails entre zones
void placePortals(MapZone *zone) {
    if (zone->zoneID == ZONE1) zone->grid[zone->height - 1][zone->width / 2] = PORTAL_1_2;
    else if (zone->zoneID == ZONE2) {
        zone->grid[0][zone->width / 2] = PORTAL_1_2;
        zone->grid[zone->height - 1][zone->width / 2] = PORTAL_2_3;
    } else if (zone->zoneID == ZONE3) {
        zone->grid[0][zone->width / 2] = PORTAL_2_3;
    }
}

void freeZone(MapZone *zone) {
    for (int i = 0; i < zone->height; i++) {
        free(zone->grid[i]);
    }
    free(zone->grid);
}

// Pour placer des élmnts sur la carte , par exple pour un rocher
void placeRocks(MapZone *zone, int numRocks) {
    for (int i = 0; i < numRocks; i++) {
        int x = rand() % zone->width;
        int y = rand() % zone->height;
        if (zone->grid[y][x] == 0) {  // Vérifie que la case est vide
            zone->grid[y][x] = 4;     // 4 = Rocher (voir le tableau des valeurs)
        }
    }
}

// Placement des PNJ
void placePNJ(MapZone *zone) {
    int x = rand() % zone->width;
    int y = rand() % zone->height;
    zone->grid[y][x] = PNJ;
}

// Placement des ressources (plantes, bois, rochers)
void placeResources(MapZone *zone) {
    int numResources = 5;
    for (int i = 0; i < numResources; i++) {
        int x = rand() % zone->width;
        int y = rand() % zone->height;
        if (zone->grid[y][x] == EMPTY) {
            if (zone->zoneID == ZONE1) zone->grid[y][x] = PLANT1;
            else if (zone->zoneID == ZONE2) zone->grid[y][x] = PLANT2;
            else if (zone->zoneID == ZONE3) zone->grid[y][x] = PLANT3;
        }
    }
}

// Pur afficher la carte dans le terminal
// Fonction pour afficher la carte
void printMap(MapZone zone) {
    for (int i = 0; i < zone.height; i++) {
        for (int j = 0; j < zone.width; j++) {
            printf("%2d ", zone.grid[i][j]);
        }
        printf("\n");
    }
}
