//
// Created by foura on 05/03/2025.
//
#include "MAP.h"

#ifndef PLAYER_H
#define PLAYER_H

typedef struct {
    int level;
    int xp;
    int hp;
    int max_hp;
    int x,y;  // Position X et Y sur la carte
    int inventory[10];  // Inventaire de 10 emplacements
    int currentZone;
} Player;
Player initPlayer();
void handleInput(MapZone *zone, Player *player);
void interactWithRock(Player *player);
void gainXP(Player *player, int xp);
void fightMonster(Player *player, int monsterHP, int monsterDamage);
void changeZone(Player *player, MapZone **currentZone, MapZone *zone1, MapZone *zone2, MapZone *zone3, int direction);
void movePlayer(Player *player, MapZone **zone, MapZone *zone1, MapZone *zone2, MapZone *zone3, int direction);
void gainXP(Player *player, int xp);
void interactWithTree(Player *player);
void interactWithPlant(Player *player);
void craftItem(Player *player, int itemID);
void loadGame(Player *player, MapZone *currentZone);
void saveGame(Player player, MapZone *currentZone);
#endif //PLAYER_H
