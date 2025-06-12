// mots_melees.c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <conio.h>
#include <windows.h>
#include "header.c"

void Color(int couleurTexte, int couleurFond)
{
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(h, couleurFond * 16 + couleurTexte);
}

void initialiserGrille(Grille *grille, int taille)
{
    grille->taille = taille;
    for (int i = 0; i < taille; ++i)
        for (int j = 0; j < taille; ++j) {
            grille->lettres[i][j] = '.';
            grille->couleur[i][j] = 0;
        }
    srand((unsigned)time(NULL));
}

void chargerListeMots(Mot listeMots[], int *nbMots, int theme)
{
    *nbMots = 5;
    if (theme == 1) {
        strcpy(listeMots[0].mot, "chat");
        strcpy(listeMots[1].mot, "chien");
        strcpy(listeMots[2].mot, "lion");
        strcpy(listeMots[3].mot, "zebre");
        strcpy(listeMots[4].mot, "tigre");
    } else if (theme == 2) {
        strcpy(listeMots[0].mot, "france");
        strcpy(listeMots[1].mot, "chine");
        strcpy(listeMots[2].mot, "japon");
        strcpy(listeMots[3].mot, "brazil");
        strcpy(listeMots[4].mot, "egypt");
    } else {
        strcpy(listeMots[0].mot, "code");
        strcpy(listeMots[1].mot, "bug");
        strcpy(listeMots[2].mot, "test");
        strcpy(listeMots[3].mot, "java");
        strcpy(listeMots[4].mot, "cafe");
    }
    for (int i = 0; i < *nbMots; ++i) listeMots[i].trouve = 0;
}

void placerMotsDansGrille(Grille *grille, Mot listeMots[], int nbMots)
{
    for (int i = 0; i < nbMots; ++i) {
        int len = (int)strlen(listeMots[i].mot);
        int direction, x, y, j, ok;

        do {
            ok        = 1;
            direction = rand() % 4;
            x = rand() % grille->taille;
            y = rand() % grille->taille;

            for (j = 0; j < len; ++j) {
                int xi = x, yi = y;
                if (direction == 0) yi += j;
                else if (direction == 1) yi -= j;
                else if (direction == 2) xi += j;
                else                     xi -= j;

                if (xi < 0 || yi < 0 ||
                    xi >= grille->taille || yi >= grille->taille ||
                    (grille->lettres[xi][yi] != '.' &&
                     grille->lettres[xi][yi] != listeMots[i].mot[j])) {
                    ok = 0; break;
                }
            }
        } while (!ok);

        for (j = 0; j < len; ++j) {
            int xi = x, yi = y;
            if (direction == 0) yi += j;
            else if (direction == 1) yi -= j;
            else if (direction == 2) xi += j;
            else                     xi -= j;
            grille->lettres[xi][yi] = listeMots[i].mot[j];
            grille->couleur[xi][yi] = 0;
        }
    }
}

void remplirCasesVides(Grille *grille)
{
    for (int i = 0; i < grille->taille; ++i)
        for (int j = 0; j < grille->taille; ++j)
            if (grille->lettres[i][j] == '.')
                grille->lettres[i][j] = 'a' + rand() % 26;
}

void afficherGrille(Grille *grille, Position curseur)
{
    for (int i = 0; i < grille->taille; ++i) {
        for (int j = 0; j < grille->taille; ++j) {
            if (i == curseur.x && j == curseur.y) {
                Color(0, 13); printf("*%c ", grille->lettres[i][j]); Color(7, 0);
            } else if (grille->couleur[i][j] == 1) {
                Color(0, 4);  printf(" %c ", grille->lettres[i][j]); Color(7, 0);
            } else if ((i + j) % 2 == 0) {
                Color(0, 14); printf(" %c ", grille->lettres[i][j]); Color(7, 0);
            } else {
                Color(0, 7);  printf(" %c ", grille->lettres[i][j]); Color(7, 0);
            }
        }
        putchar('\n');
    }
}

void afficherInterface(Grille *grille, Mot listeMots[], int nbMots, Position curseur)
{
    char menuLignes[MAX_GRID_SIZE * 2 + 10][30];
    int  nMenu = 0;

    sprintf(menuLignes[nMenu++], "2468 : deplacer");
    sprintf(menuLignes[nMenu++], "ESPACE: selection");
    sprintf(menuLignes[nMenu++], "q     : quitter");
    sprintf(menuLignes[nMenu++], "");
    sprintf(menuLignes[nMenu++], "Mots restants :");
    for (int i = 0; i < nbMots; ++i)
        if (!listeMots[i].trouve)
            sprintf(menuLignes[nMenu++], "- %s", listeMots[i].mot);
    sprintf(menuLignes[nMenu++], "");
    sprintf(menuLignes[nMenu++], "Mots trouves :");
    for (int i = 0; i < nbMots; ++i)
        if (listeMots[i].trouve)
            sprintf(menuLignes[nMenu++], "- %s", listeMots[i].mot);

    int total = (grille->taille > nMenu) ? grille->taille : nMenu;

    for (int lig = 0; lig < total; ++lig) {
        if (lig < nMenu)
            printf("%-25s", menuLignes[lig]);
        else
            printf("%-25s", "");

        if (lig < grille->taille) {
            for (int col = 0; col < grille->taille; ++col) {
                if (lig == curseur.x && col == curseur.y) {
                    Color(0, 13); printf("*%c ", grille->lettres[lig][col]); Color(7, 0);
                } else if (grille->couleur[lig][col] == 1) {
                    Color(0, 4);  printf(" %c ", grille->lettres[lig][col]); Color(7, 0);
                } else if ((lig + col) % 2 == 0) {
                    Color(0, 14); printf(" %c ", grille->lettres[lig][col]); Color(7, 0);
                } else {
                    Color(0, 7);  printf(" %c ", grille->lettres[lig][col]); Color(7, 0);
                }
            }
        }
        putchar('\n');
    }
}

int validerMot(Grille *grille, Mot listeMots[], int nbMots,
               Position debut, Position fin)
{
    char motTrouve[MAX_WORD_LEN];
    int dx = (fin.x - debut.x == 0) ? 0 : (fin.x - debut.x) / abs(fin.x - debut.x);
    int dy = (fin.y - debut.y == 0) ? 0 : (fin.y - debut.y) / abs(fin.y - debut.y);

    int x = debut.x, y = debut.y, k = 0;
    while (x != fin.x + dx || y != fin.y + dy) {
        motTrouve[k++] = grille->lettres[x][y];
        x += dx; y += dy;
    }
    motTrouve[k] = '\0';

    for (int i = 0; i < nbMots; ++i) {
        if (!listeMots[i].trouve && strcmp(motTrouve, listeMots[i].mot) == 0) {
            x = debut.x; y = debut.y;
            while (x != fin.x + dx || y != fin.y + dy) {
                grille->couleur[x][y] = 1;
                x += dx; y += dy;
            }
            listeMots[i].trouve = 1;
            return 1;
        }
    }
    return 0;
}

void boucleJeu(Grille *grille, Mot listeMots[], int nbMots)
{
    int trouve = 0;
    Position curseur = {0, 0}, debut = {-1, -1};

    while (trouve < nbMots) {
        system("cls");
        afficherInterface(grille, listeMots, nbMots, curseur);

        char c = _getch();
        if (c == '8' && curseur.x > 0)                 curseur.x--;
        if (c == '2' && curseur.x < grille->taille-1)  curseur.x++;
        if (c == '4' && curseur.y > 0)                 curseur.y--;
        if (c == '6' && curseur.y < grille->taille-1)  curseur.y++;
        if (c == 'q') return;

        if (c == ' ') {
            if (debut.x == -1) {
                debut = curseur;
            } else {
                if (validerMot(grille, listeMots, nbMots, debut, curseur)) {
                    printf("\nMot correct !\n");        ++trouve;
                } else {
                    printf("\nMot incorrect.\n");
                }
                debut.x = debut.y = -1;
                system("pause");
            }
        }
    }
    printf("\nBravo, vous avez trouve tous les mots !\n");
    system("pause");
}

void afficherMenu()
{
    int choix, theme = 1, taille = 12;

    do {
        printf("\e[1;1H\e[2J");
        printf("\n***********************=== MENU ===**************************\n");
        printf("*     1. Jouer                                              *\n"
               "*     2. Choisir le theme                                   *\n"
               "*     3. Choisir la taille de la grille                     *\n"
               "*     4. Quitter                                            *\n"
               "*                                                           *\n" );
        printf("*************************************************************\n");
        printf("\nVeuillez entrer votre choix: \n");

        scanf("%d", &choix);

        if (choix == 2) {
            printf("\n1. Animaux\n2. Pays\n3. Informatique\nChoix : ");
            scanf("%d", &theme);
        } else if (choix == 3) {
            printf("\nEntrez une taille de grille (entre 6 et 20) : ");
            scanf("%d", &taille);
            if (taille < 6)             taille = 6;
            if (taille > MAX_GRID_SIZE) taille = MAX_GRID_SIZE;
        } else if (choix == 1) {
            Grille grille;
            Mot   listeMots[MAX_WORDS];
            int   nbMots;

            initialiserGrille(&grille, taille);
            chargerListeMots(listeMots, &nbMots, theme);
            placerMotsDansGrille(&grille, listeMots, nbMots);
            remplirCasesVides(&grille);

            boucleJeu(&grille, listeMots, nbMots);
        }
    } while (choix != 4);
}

