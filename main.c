#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "MAP.h"
#include "PLAYER.h"SS


int main() {
    srand(time(NULL));

    MapZone zone1 = initZone(10, 10, ZONE1);
    MapZone zone2 = initZone(10, 10, ZONE2);
    MapZone zone3 = initZone(10, 10, ZONE3);
    MapZone *currentZone = &zone1;

    placePortals(&zone1);
    placePortals(&zone2);
    placePortals(&zone3);

    Player player = initPlayer();
    currentZone->grid[player.y][player.x] = PLAYER;

    printMap(*currentZone);

    char action;
    while (1) {
        printf("Déplacez-vous (Z: Haut, S: Bas, Q: Gauche, D: Droite, X: Quitter): ");
        scanf(" %c", &action);
        if (action == 'X') break;
        else if (action == 'Z') movePlayer(&player, &currentZone, &zone1, &zone2, &zone3, 0);
        else if (action == 'S') movePlayer(&player, &currentZone, &zone1, &zone2, &zone3, 1);
        else if (action == 'Q') movePlayer(&player, &currentZone, &zone1, &zone2, &zone3, 2);
        else if (action == 'D') movePlayer(&player, &currentZone, &zone1, &zone2, &zone3, 3);
        printMap(*currentZone);
    }
    return 0;
}