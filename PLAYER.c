//
// Created by foura on 05/03/2025.
//
#include <stdio.h>
#include <stdlib.h>
#include "player.h"
#include "map.h"

Player initPlayer() {
    Player player;
    player.level = 1;
    player.xp = 0;
    player.hp = 100;
    player.max_hp = 100;
    player.x = 0;  // Position initiale X
    player.y = 0;  // Position initiale Y
    for (int i = 0; i < 10; i++) {
        player.inventory[i] = 0;  // Inventaire vide
    }
    return player;
}

// Pour se deplacerS

//Changer de zone avec mise à jour de la position
void changeZone(Player *player, MapZone **currentZone, MapZone *zone1, MapZone *zone2, MapZone *zone3, int direction) {
    // Supprimer l'ancien joueur de la carte
    (*currentZone)->grid[player->y][player->x] = EMPTY;

    if ((*currentZone)->grid[player->y][player->x] == PORTAL_1_2 && direction == 3) { // Avancer vers zone suivante
        *currentZone = (player->currentZone == ZONE1) ? zone2 : zone1;
        player->currentZone = (player->currentZone == ZONE1) ? ZONE2 : ZONE1;
        player->y = (player->currentZone == ZONE2) ? 0 : (*currentZone)->height - 1;
    } else if ((*currentZone)->grid[player->y][player->x] == PORTAL_2_3 && direction == 3) { // Avancer vers zone suivante
        *currentZone = (player->currentZone == ZONE2) ? zone3 : zone2;
        player->currentZone = (player->currentZone == ZONE2) ? ZONE3 : ZONE2;
        player->y = (player->currentZone == ZONE3) ? 0 : (*currentZone)->height - 1;
    } else if ((*currentZone)->grid[player->y][player->x] == PORTAL_2_3 && direction == 2) { // Reculer vers zone précédente
        *currentZone = (player->currentZone == ZONE3) ? zone2 : zone1;
        player->currentZone = (player->currentZone == ZONE3) ? ZONE2 : ZONE1;
        player->y = (*currentZone)->height - 1;
    } else if ((*currentZone)->grid[player->y][player->x] == PORTAL_1_2 && direction == 2) { // Reculer vers zone précédente
        *currentZone = (player->currentZone == ZONE2) ? zone1 : zone2;
        player->currentZone = (player->currentZone == ZONE2) ? ZONE1 : ZONE2;
        player->y = (*currentZone)->height - 1;
    }

    player->x = (*currentZone)->width / 2;
    (*currentZone)->grid[player->y][player->x] = PLAYER;
    printf("Vous êtes maintenant dans la zone %d à la position (%d, %d).\n", player->currentZone, player->x, player->y);
}

// Fonction pour gérer les déplacements du joueur
void movePlayer(Player *player, MapZone **currentZone, MapZone *zone1, MapZone *zone2, MapZone *zone3, int direction) {
    int newX = player->x, newY = player->y;

    if (direction == 0) newY--; // Haut
    else if (direction == 1) newY++; // Bas
    else if (direction == 2) newX--; // Gauche
    else if (direction == 3) newX++; // Droite

    if (newX < 0 || newX >= (*currentZone)->width || newY < 0 || newY >= (*currentZone)->height) {
        printf("Déplacement impossible !\n");
        return;
    }

    int cell = (*currentZone)->grid[newY][newX];

    // Vérifier si le joueur marche sur un portail
    if (cell == PORTAL_1_2 || cell == PORTAL_2_3) {
        printf("Vous traversez un portail !\n");
        changeZone(player, currentZone, zone1, zone2, zone3, direction); // Ajout de la direction
        return;
    }

    // Déplacement normal
    (*currentZone)->grid[player->y][player->x] = EMPTY;
    player->x = newX;
    player->y = newY;
    (*currentZone)->grid[newY][newX] = PLAYER;
}

void interactWithRock(Player *player) {
    // Vérifier si le joueur a une pioche dans son inventaire
    int hasPickaxe = 0;
    for (int i = 0; i < 10; i++) {
        if (player->inventory[i] == 2) {  // 2 = Pioche en bois
            hasPickaxe = 1;
            break;
        }
    }

    if (hasPickaxe) {
        printf("Vous minez le rocher et obtenez une pierre.\n");
        // Ajouter une pierre à l'inventaire
        for (int i = 0; i < 10; i++) {
            if (player->inventory[i] == 0) {  // Trouver un emplacement vide
                player->inventory[i] = 6;  // 6 = Pierre
                break;
            }
        }
    } else {
        printf("Vous avez besoin d'une pioche pour miner ce rocher.\n");
    }
}

void interactWithPlant(Player *player) {
    int hasSickle = 0;
    for (int i = 0; i < 10; i++) {
        if (player->inventory[i] == 3) {  // 3 = Serpe en bois
            hasSickle = 1;
            break;
        }
    }

    if (hasSickle) {
        printf("Vous récoltez une plante et obtenez de l'herbe.\n");
        for (int i = 0; i < 10; i++) {
            if (player->inventory[i] == 0) {
                player->inventory[i] = 7;  // 7 = Herbe
                break;
            }
        }
    } else {
        printf("Vous avez besoin d'une serpe pour récolter cette plante.\n");
    }
}

void interactWithTree(Player *player) {
    int hasAxe = 0;
    for (int i = 0; i < 10; i++) {
        if (player->inventory[i] == 4) {  // 4 = Hache en bois
            hasAxe = 1;
            break;
        }
    }

    if (hasAxe) {
        printf("Vous coupez un arbre et obtenez du bois.\n");
        for (int i = 0; i < 10; i++) {
            if (player->inventory[i] == 0) {
                player->inventory[i] = 5;  // 5 = Sapin
                break;
            }
        }
    } else {
        printf("Vous avez besoin d'une hache pour couper cet arbre.\n");
    }
}

void gainXP(Player *player, int xp) {
    player->xp += xp;
    printf("Vous avez gagné %d XP.\n", xp);

    // Vérifier si le joueur monte de niveau
    int xpNeeded = player->level * 100;  // Exemple : 100 XP par niveau
    if (player->xp >= xpNeeded) {
        player->level++;
        player->xp = 0;
        player->max_hp += 10;  // Augmenter les points de vie maximum
        player->hp = player->max_hp;  // Restaurer les points de vie
        printf("Félicitations ! Vous êtes maintenant niveau %d.\n", player->level);
    }
}

void fightMonster(Player *player, int monsterHP, int monsterDamage) {
    printf("Un monstre apparaît !\n");
    int weaponDamage = 1;  // Dégâts de base si le joueur n'a pas d'arme
    int weaponIndex = -1;

    // Vérifier si le joueur a une arme dans son inventaire
    for (int i = 0; i < 10; i++) {
        if (player->inventory[i] == 1 || player->inventory[i] == 8 || player->inventory[i] == 19 || player->inventory[i] == 30) {
            weaponIndex = i;
            break;
        }
    }

    if (weaponIndex != -1) {
        // Définir les dégâts en fonction de l'arme
        switch (player->inventory[weaponIndex]) {
            case 1: weaponDamage = 1; break;  // Épée en bois
            case 8: weaponDamage = 2; break;  // Épée en pierre
            case 19: weaponDamage = 5; break; // Épée en fer
            case 30: weaponDamage = 10; break; // Épée en diamant
        }
    }

    while (player->hp > 0 && monsterHP > 0) {
        printf("Joueur HP: %d | Monstre HP: %d\n", player->hp, monsterHP);
        printf("Que voulez-vous faire ? (a: attaquer, f: fuir) : ");
        char action;
        scanf(" %c", &action);

        if (action == 'a') {
            // Joueur attaque
            monsterHP -= weaponDamage;
            printf("Vous infligez %d dégâts au monstre.\n", weaponDamage);

            // Réduire la durabilité de l'arme
            if (weaponIndex != -1) {
                player->inventory[weaponIndex]--;  // Réduire la durabilité
                if (player->inventory[weaponIndex] <= 0) {
                    printf("Votre arme est cassée !\n");
                    player->inventory[weaponIndex] = 0;  // L'arme est cassée
                }
            }
        } else if (action == 'f') {
            // Tentative de fuite
            if (rand() % 100 < 30) {  // 30% de chance de fuite
                printf("Vous avez réussi à fuir.\n");
                return;
            } else {
                printf("Vous n'avez pas réussi à fuir.\n");
            }
        } else {
            printf("Action invalide.\n");
        }

        // Monstre attaque
        if (monsterHP > 0) {
            player->hp -= monsterDamage;
            printf("Le monstre vous inflige %d degâts.\n", monsterDamage);
        }
    }

    if (player->hp > 0) {
        printf("Vous avez vaincu le monstre !\n");
        gainXP(player, 50);  // Gain d'XP après la victoire
    } else {
        printf("Vous avez été vaincu...\n");
    }
}

void craftItem(Player *player, int itemID) {
    int requiredResources[3] = {0};  // Ressources nécessaires pour le craft
    switch (itemID) {
        case 8:  // Épée en pierre
            requiredResources[0] = 2;  // Sapin
        requiredResources[1] = 3;  // Pierre
        break;
        case 9:  // Lance en pierre
            requiredResources[0] = 3;  // Sapin
        requiredResources[1] = 4;  // Pierre
        break;
        // Ajouter d'autres cas pour d'autres objets
    }

    // Vérifier si le joueur a les ressources nécessaires
    int hasResources = 1;
    for (int i = 0; i < 3; i++) {
        if (requiredResources[i] > 0) {
            int resourceCount = 0;
            for (int j = 0; j < 10; j++) {
                if (player->inventory[j] == 5 + i) {  // 5 = Sapin, 6 = Pierre, etc.
                    resourceCount++;
                }
            }
            if (resourceCount < requiredResources[i]) {
                hasResources = 0;
                break;
            }
        }
    }

    if (hasResources) {
        // Retirer les ressources de l'inventaire
        for (int i = 0; i < 3; i++) {
            if (requiredResources[i] > 0) {
                int resourcesToRemove = requiredResources[i];
                for (int j = 0; j < 10; j++) {
                    if (player->inventory[j] == 5 + i) {
                        player->inventory[j] = 0;
                        resourcesToRemove--;
                        if (resourcesToRemove == 0) break;
                    }
                }
            }
        }

        // Ajouter l'objet crafté à l'inventaire
        for (int i = 0; i < 10; i++) {
            if (player->inventory[i] == 0) {
                player->inventory[i] = itemID;
                printf("Vous avez crafté un nouvel objet !\n");
                break;
            }
        }
    } else {
        printf("Vous n'avez pas les ressources nécessaires pour ce craft.\n");
    }
}
void saveGame(Player player, MapZone *currentZone) {
    FILE *file = fopen("save.txt", "w");
    if (file == NULL) {
        printf("Erreur lors de la sauvegarde.\n");
        return;
    }

    // Sauvegarder les données du joueur
    fprintf(file, "=== PLAYER ===\n");
    fprintf(file, "%d\n", player.level);
    fprintf(file, "%d/%d\n", player.xp, player.level * 100);
    fprintf(file, "%d/%d\n", player.hp, player.max_hp);
    fprintf(file, "-- INVENTORY --\n");
    for (int i = 0; i < 10; i++) {
        fprintf(file, "%d ", player.inventory[i]);
    }
    fprintf(file, "\n");

    // Sauvegarder la carte
    fprintf(file, "=== MAP ===\n");
    fprintf(file, "%d %d\n", currentZone->width, currentZone->height);
    for (int i = 0; i < currentZone->height; i++) {
        for (int j = 0; j < currentZone->width; j++) {
            fprintf(file, "%d ", currentZone->grid[i][j]);
        }
        fprintf(file, "\n");
    }

    fclose(file);
    printf("Partie sauvegardée avec succès.\n");
}

void loadGame(Player *player, MapZone *currentZone) {
    FILE *file = fopen("save.txt", "r");
    if (file == NULL) {
        printf("Aucune sauvegarde trouvée.\n");
        return;
    }

    // Charger les données du joueur
    fscanf(file, "=== PLAYER ===\n");
    fscanf(file, "%d\n", &player->level);
    fscanf(file, "%d/%d\n", &player->xp, &player->max_hp);
    fscanf(file, "%d/%d\n", &player->hp, &player->max_hp);
    fscanf(file, "-- INVENTORY --\n");
    for (int i = 0; i < 10; i++) {
        fscanf(file, "%d ", &player->inventory[i]);
    }

    // Charger la carte
    fscanf(file, "=== MAP ===\n");
    fscanf(file, "%d %d\n", &currentZone->width, &currentZone->height);
    currentZone->grid = malloc(currentZone->height * sizeof(int *));
    for (int i = 0; i < currentZone->height; i++) {
        currentZone->grid[i] = malloc(currentZone->width * sizeof(int));
        for (int j = 0; j < currentZone->width; j++) {
            fscanf(file, "%d ", &currentZone->grid[i][j]);
        }
    }

    fclose(file);
    printf("Partie chargée avec succès.\n");
}
