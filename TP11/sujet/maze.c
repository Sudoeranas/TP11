// IG2I / SDD
// 2021-11-17
// Vincent Liard - P Kubiak

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "stack.h"

// Labyrinthe en anglais : maze.

// Représente un chemin sous la forme d'une chaîne de caractères
// composée des lettres u (up), d (down), l (left), r (right).
typedef char *path_t;

typedef struct
{
  int valeur;
  bool visited;
  char direction;   // u d r ou l
  int nbDirections; // ou nbrVoisins : c'est à dire le nb de cases utiles dans directionsPossibles et QuatreVoisins
  char directionsPossibles[5];
  int QuatreVoisins[5]; // valeur des 4(ou-) voisins
} T_cellule;
typedef struct
{
  //  v________
  // |   ______| ^
  // | |_____  | |
  // |  _|   | | |
  // |_|  _| | | row(6)
  // | __|___  | |es politiques, syndicaux et étudiants a appelé à une large manifestation à Ajaccio, dimanche 3 avril. Une décision qui n'a pas remporté les faveurs de l'ensemble des membres du collecti
  // |_______|v| v
  // <--col(5)->

  // Le labyrinthe comporte row lignes-couloirs horizontaux
  // (est-ouest) et col colonnes-couloirs verticaux (nord-sud).

  // Il y a implicitement des murs tout autour du labyrinthe.

  // L'entrée se trouve dans la cellule supérieure gauche
  // (nord-ouest), la sortie se trouve dans la cellule inférieure
  // droite (sud-est).

  // wallh[i][j] et wallv[i][j] encodent pour la cellule (i,j) le fait
  // qu'elle comporte un mur horizontal au dessous ou vertical à
  // droite (respectivement).

  // Nombre de lignes
  int row;
  // Nombre de colonnes
  int col;
  int num;
  // table de booléens murs horizontaux '_'
  bool *wallh;
  // table de booléens murs verticaux '|'
  bool *wallv;
  T_cellule *cells;

} maze_t;

bool *wall_under(maze_t *m, int row, int col);
bool *wall_right(maze_t *m, int row, int col);
maze_t *init_maze(int row, int col);
void free_maze(maze_t *m);
void print_maze_raw(maze_t const *m);
void print_maze(maze_t const *m, char *udlr, int t[200][2], int n);
void print_mazePLEIN(maze_t const *m);
void print_mazePLEINchemin(maze_t const *m, T_cellule *chemin, int t);
maze_t *compartmentalized_maze(int row, int col);
maze_t *random_maze(int lig, int col);
bool check_path(maze_t *m, path_t *p);
void printValeurs(maze_t *);
maze_t *init(maze_t *m);

void printValeurs(maze_t *m)

{

  int i, j;
  for (i = 0; i < m->row; i++)
  {
    printf("\n");
    for (j = 0; j < m->col; j++)
      printf("%3d|", m->cells[i * m->col + j].valeur);
  }
  printf("\n");
  printf("\n");
}

int main(void)
{ /////////////////////////////////////\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\MAIN
  /*  int wh[5 * 5] = {0,0,1,1,1,
                     0,1,1,1,0,
                     0,1,0,0,0,
                     1,0,1,0,0,
                     0,1,1,1,0};
    int wv[6 * 4] = {0,0,0,0,
                     1,0,0,0,
                     0,1,0,1,
                     1,0,1,1,
                     0,1,0,0,
                     0,0,0,1};
   */
  /* maze_t *m = init_maze(6, 5);
   if (!m) {
     printf("Echec d'allocation mémoire pour le labyrinthe.\n");
     return 0;
   }
   memcpy(m->wallh, wh, 25 * sizeof(int));
   memcpy(m->wallv, wv, 24 * sizeof(int));
*/
  int NL, NC;

  printf("\n quel sera le nombre de lignes de votre labyrinthe ? ");
  scanf("%d", &NL);
  printf("\n quel sera le nombre de colonnes de votre labyrinthe ? ");
  scanf("%d", &NC);
  maze_t *m = compartmentalized_maze(NL, NC);
  print_mazePLEIN(m);
  init(m);
  maze_t *a = random_maze(NL, NC);
  init(a);
  print_mazePLEIN(a);

  free_maze(m);
  return 0;
}

maze_t *init(maze_t *m)
{
  int k = 0; // compteur valeur case
  int tampon = 0;
  int compteur = 0;

  for (int i = 0; i < m->col; i++)
  {
    for (int j = 0; j < m->row; j++)
    {
      m->cells[k].valeur = k;
      m->cells[k].visited = 0;
      m->cells[k].direction = '-';
      if (k - 1 >= 0 && k % m->col != 0 && k != (m->col * ((m->row) - 1)))
      {
        m->cells[k].QuatreVoisins[tampon] = k - 1;
        tampon++;
      }
      if ((k + 1) % m->col != 0 && k != (m->col * m->row) - 1)
      {
        m->cells[k].QuatreVoisins[tampon] = k + 1;
        tampon++;
      }
      if (k - (m->col) >= 0)
      {
        m->cells[k].QuatreVoisins[tampon] = k - (m->col);
        tampon++;
      }
      if (k + (m->col) < m->col * m->row)
      {
        m->cells[k].QuatreVoisins[tampon] = k + (m->col);
        tampon++;
      }
      m->cells[k].QuatreVoisins[tampon] = -1;
      for (int i = 0; m->cells[k].QuatreVoisins[i] != -1; i++)
      {
        compteur++;
      }
      m->cells[k].nbDirections = compteur;

      printf("\n\n[%d]\n", m->cells[k].valeur);
      printf("[%d]\n\n", m->cells[k].nbDirections);
      for (int dv = 0; dv < 4; dv++)
      {

        printf("[%d]", m->cells[k].QuatreVoisins[dv]);
      }

      tampon = 0;
      k++;
      compteur = 0;
    }
  }
}

// Accès (en lecture et écriture) au mur vertical à droite de la
// cellule row, col du labyrinthe m
bool *wall_right(maze_t *m, int row, int col)
{
  if (row > m->row || col > m->col - 1)
    return NULL;
  return &(m->wallv[row * (m->col - 1) + col]);
}

// Accès (en lecture et écriture) au mur horizontal au dessous de la
// cellule row, col du labyrinthe m
bool *wall_under(maze_t *m, int row, int col)
{
  if (row > m->row - 1 || col > m->col)
    return NULL;
  return &(m->wallh[row * m->col + col]);
}

// Retourne un pointeur sur un maze_t complètement initialisé ou NULL
// si l'allocation a échoué.
maze_t *init_maze(int row, int col)
{
  maze_t *m;
  m = malloc(sizeof(maze_t));
  if (!m)
    return NULL;
  m->row = row;
  m->col = col;

  m->wallh = malloc((row - 1) * (col) * sizeof(int));
  m->wallv = malloc((row) * (col - 1) * sizeof(int));
  m->cells = malloc((row) * (col) * sizeof(T_cellule));
  if (!m->wallh || !m->wallv || !m->cells)
    return NULL;
  return m;
}

void free_maze(maze_t *m)
{
  free(m->wallv);
  free(m->wallh);
  free(m);
}

// « Labyrinthe dans lequel toutes les cellules sont fermées.
// Exemple : Le « labyrinthe » compartimenté 6 lignes, 5 colonnes
// ._________.
// |_|_|_|_|_|
// |_|_|_|_|_|
// |_|_|_|_|_|
// |_|_|_|_|_|
// |_|_|_|_|_|
// |_|_|_|_|_|
maze_t *compartmentalized_maze(int row, int col)
{
  maze_t *m;
  if (!(m = init_maze(row, col)))
    return NULL;
  // print_maze_raw(m);
  //  Lignes et colonnes internes
  for (int i = 0; i < m->row - 1; ++i)
    for (int j = 0; j < m->col - 1; ++j)
    {
      *wall_under(m, i, j) = 1;
      *wall_right(m, i, j) = 1;
    }
  // Dernière colonne (murs horizontaux) :
  for (int i = 0; i < m->row - 1; ++i)
    *wall_under(m, i, m->col - 1) = 1;
  // Dernière ligne (murs verticaux) :
  for (int j = 0; j < m->col - 1; ++j)
    *wall_right(m, m->row - 1, j) = 1;

  for (int i = 0; i < m->row; ++i)

    for (int j = 0; j < m->col; ++j)
      m->cells[i * col + j].valeur = i * col + j;
  return m;
}

void print_maze_raw(maze_t const *m)
{
  printf("row : %d, col : %d\n", m->row, m->col);
  printf("wallh %d lignes x %d colonnes :\n", m->row - 1, m->col);
  for (int i = 0; i < m->row - 1; ++i)
  {
    for (int j = 0; j < m->col; ++j)
      printf("%d ", m->wallh[i * m->col + j]);
    printf("\n");
  }

  printf("wallv %d lignes x %d colonnes :\n", m->row, m->col - 1);
  for (int i = 0; i < m->row; ++i)
  {
    for (int j = 0; j < m->col - 1; ++j)
      printf("%d ", m->wallv[i * (m->col - 1) + j]);
    printf("\n");
  }
}

// Affiche sur stdout le labyrinthe fourni en argument.

// Les caractères utilisés pour l'affichage sont
// '_' pour un mur en dessous de la cellule considérée,
// '|' pour un mur à droite de la cellule considérée.
// ----------------------------------------------------------------------------------------------------------------
void print_maze(maze_t const *m, char *udlr, int t[200][2], int n)
{
  print_maze_raw(m);
  int pos = 0;
  int trouve = -1;
  char car;
  // Mur supérieur
  printf(" v");
  for (int i = 0; i < m->col - 1; ++i)
    printf("__");
  printf(" \n");

  for (int i = 0; i < m->row - 1; i++)
  {
    printf("|");
    for (int j = 0; j <= m->col - 1; j++)
    {
      printf(m->wallh[i * m->col + j] ? "_" : " ");

      printf(m->wallv[i * (m->col - 1) + j] ? "|" : " ");
      printf("|");
    }
    printf("\n");
    printf("%s|\n", m->wallh[(i + 1) * m->col - 1] ? "_" : " ");
    // "|" : m->wallh[(i + 1) * m->col + j] ? "_" : " "
  }

  // Mur inférieur
  printf("|");
  for (int i = 0; i < m->col - 1; ++i)
  {
    printf("_|");
  }

  // printf(m->wallv[(m->row - 1) * (m->col - 1) + i] ? "_|" : "__");
  printf("v|\n");
}

void print_mazePLEIN(maze_t const *m)
{
  print_maze_raw(m);

  // Mur supérieur
  printf(" v");
  for (int i = 0; i < m->col - 1; ++i)
    printf("__");
  printf(" \n");

  for (int i = 0; i < m->row - 1; ++i)
  {
    printf("|");
    for (int j = 0; j < m->col - 1; ++j)
    {
      printf(m->wallh[i * m->col + j] ? "_" : " ");

      printf(m->wallv[i * (m->col - 1) + j] ? "|" : " ");
    }
    printf("%s|\n", m->wallh[(i + 1) * m->col - 1] ? "_" : " ");
    // "|" : m->wallh[(i + 1) * m->col + j] ? "_" : " "
  }

  // Mur inférieur
  printf("|");
  for (int i = 0; i < m->col - 1; ++i)
    printf(m->wallv[(m->row - 1) * (m->col - 1) + i] ? "_|" : "__");
  printf("v|\n");
}

maze_t *random_maze(int lig, int col)
{
  maze_t *m = compartmentalized_maze(lig, col);
  bool visited[lig][col];
  int deci, nbmurs = 0, i, j;
  srand(time(NULL));
  while (nbmurs != (lig * col) - 1)
  {
    for (i = 0; i < lig; ++i)
    {
      for (j = 0; j < col; ++j)
      {
        visited[i][j] = 0;
        deci = rand() % (2);

        int valCellActuelle, valCelleAdjacente;
        if (deci == 0 && (j + 1) != col)
        {

          valCellActuelle = m->cells[i * col + j].valeur;
          valCelleAdjacente = m->cells[i * col + j + 1].valeur;

          if (valCellActuelle != valCelleAdjacente)
          {
            for (int k = 0; k < lig * col; k++)
            {
              if (m->cells[k].valeur == valCelleAdjacente)
                m->cells[k].valeur = valCellActuelle;
            }
            *wall_right(m, i, j) = 0;

            nbmurs++;
          }
        }
        else if ((i + 1) != lig)
        {
          valCellActuelle = m->cells[i * col + j].valeur;
          valCelleAdjacente = m->cells[(i + 1) * col + j].valeur;

          if (valCellActuelle != valCelleAdjacente)
          {
            for (int k = 0; k < lig * col; k++)
            {
              if (m->cells[k].valeur == valCelleAdjacente)
                m->cells[k].valeur = valCellActuelle;
            }
            *wall_under(m, i, j) = 0;

            nbmurs++;
          }
        }
      }
    }
  }
  return m;
}

// Return the path from 0,0 to (row - 1, col - 1).
path_t solve_maze(maze_t *m)
{
  // Le plus long chemin sans recouvrement sur une grille row x col
  // mesure row x col.
  path_t p = malloc(m->row * m->col);
  // dstack_t s =

  /*while () {

  }
  */
  return p;
}

bool check_path(maze_t *m, path_t *p)
{
  return false;
}