#include <assert.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

/******************************************
 *                PLATEAU                 *
 ******************************************/

/* taille du plateau par default */
#define SIZE 7

/* type des valeurs du plateau */
typedef enum {
    EMPTY,
    BLACK_P,
    WHITE_P,
    RED_P
} content;

//emplacement des pions 2.3.4
//emplacement des pions noir
void where_b (content (*b)[SIZE]){
  int i,j;
  for (i=0;i<SIZE;i++){
    for(j=0;j<SIZE; j++){
      if((i<2 && j>=SIZE-2)||(i>=SIZE-2 && j<2)){
        b[i][j]=BLACK_P;
      }
    }
  }
}

//emplacement des pions blanc
void where_w(content (*b)[SIZE]){
  int i, j;
  for(i = 0; i < SIZE; i++){
    for(j = 0; j < SIZE; j++){
      if((i<2 && j<2)||(i>SIZE-3 && j>SIZE-3)){
        b[i][j]=WHITE_P;
      }
    }
  }
}

//emplacement des pions rouge
void where_r(content (*b)[SIZE]){
  int i, j;
  for(i = 2; i <= 4; i++){
    for(j = 2; j <= 4; j++){
      b[i][j]= RED_P;

    }
  }
  b[3][1]= RED_P;
  b[1][3]= RED_P;
  b[3][5]= RED_P;
  b[5][3]= RED_P;
}

//fonction d'initialisation du plateau 5:
void init_board(content (*b)[SIZE]){
  int i, j;
  for(i = 0; i < SIZE; i++){
    for(j = 0; j < SIZE; j++){
      b[i][j]= EMPTY;
    }
  }
  where_w(b);
  where_b(b);
  where_r(b);
}

/******************************************
 *       DEPLACEMENTS ET DIRECTIONS       *
 ******************************************/

/* structure representant une position */
typedef struct{
  int i;
  int j;
}position;

/* type de direction de deplacement des pions sur le plateau */
typedef enum {
  NORD,
  SUD,
  EST,
  OUEST
} direction;

/* structure representant le coup interdit au prochain tour */
typedef struct{
  int a;
  int b;
  direction d;
}ban_coup;

//fonction d'echange de pions 6
void swap (content *i, content *j) {
  int n = *i;
  *i = *j;
  *j = n;
}

//fonction de déplacement vers le nord 7
content dpl_N(content (*b)[SIZE], position pos, ban_coup *ban){
  int i, n;
  content a = EMPTY;
  for(i = pos.i; b[i][pos.j] != EMPTY; i--){
    n = i;
  }
  n--;
  ban -> a = n;
  ban -> b = pos.j;
  ban -> d = SUD;
  if(!(n >= 0 && n < SIZE)){
    n = 0;
    a = b[n][pos.j];
    b[n][pos.j] = EMPTY;
    ban -> a = -1;
  }
  while(n != pos.i){
    swap(&b[n][pos.j], &b[n+1][pos.j]);
    n++;
  }
  return a;
}

//fonction de déplacement vers le sud 8
content dpl_S(content (*b)[SIZE], position pos, ban_coup *ban){
  int i, n;
  content a = EMPTY;
  for(i = pos.i; b[i][pos.j] != EMPTY; i++){
    n = i;
  }
  n++;
  ban -> a = n;
  ban -> b = pos.j;
  ban -> d = NORD;
  if(!(n>=0 && n<SIZE)){
    n = SIZE-1;
    a = b[n][pos.j];
    b[n][pos.j] = EMPTY;
    ban -> a = -1;
  }
  while(n != pos.i){
    swap(&b[n][pos.j], &b[n-1][pos.j]);
    n--;
  }
  return a;
}
//fonction de déplacement vers l'est 9
content dpl_E(content (*b)[SIZE], position pos, ban_coup *ban){
  int j, n;
  content a = EMPTY;
  for(j = pos.j; b[pos.i][j] != EMPTY; j++){
    n = j;
  }
  n++;
  ban -> a = pos.i;
  ban -> b = n;
  ban -> d = OUEST;
  if(!(n>=0 && n<SIZE)){
    n = SIZE-1;
    a = b[pos.i][n];
    b[pos.i][n] = EMPTY;
    ban -> b = -1;
  }
  while(n != pos.j){
    swap(&b[pos.i][n], &b[pos.i][n-1]);
    n--;
  }
  return a;
}

//fonction de déplacement vers l'ouest 10
content dpl_O(content (*b)[SIZE],position pos, ban_coup *ban){
  int j, n;
  content a = EMPTY;
  for(j= pos.j; b[pos.i][j]!=EMPTY; j--){
    n=j;
  }
  n--;
  ban -> a = pos.i;
  ban -> b = n;
  ban -> d = EST;
  if(!(n>=0 && n<SIZE)){
    n=0;
    a = b[pos.i][n];
    b[pos.i][n]=EMPTY;
    ban -> b = -1;
  }
  while(n!=pos.j){
    swap(&b[pos.i][n], &b[pos.i][n+1]);
    n++;
  }
  return a;
}

//fonction déplacement des pions 11
content deplacement (content (*b)[SIZE] ,position pos, direction dir, ban_coup *ban){
  switch (dir){
    case NORD : return dpl_N(b, pos, ban);

    case SUD : return dpl_S(b, pos, ban);

    case EST : return dpl_E(b, pos, ban);

    case OUEST : return dpl_O(b, pos, ban);
  }
}

/******************************************
 *         COMPTEURS ET JOUEURS           *
 ******************************************/

/* type des joueurs */
typedef enum {
  JOUW,
  JOUB
}joueur;

/* structure  permettant de representer les pions pris par les joueurs */
typedef struct{
  int nb_b;
  int nb_w;
  int nb_rb;
  int nb_rw;
}compteur;

//fonction changement de joueur 12
joueur tour (joueur jou){
  if (jou==JOUB){
    return JOUW;
  }
  return JOUB;
}

//fonction compteur 13
void compte (content a, joueur j, compteur *c){
  if(j == JOUB){
    switch(a){
      case WHITE_P : c -> nb_w ++; break;
      case RED_P : c -> nb_rb ++; break;
      default : break;
    }
  }
  else{
    switch(a){
      case BLACK_P : c -> nb_b ++; break;
      case RED_P : c -> nb_rw ++; break;
      default : break;
    }
  }
}

//fonction de vérification d'incrémentation du compteur
int verif_compt(compteur c1, compteur c2){
  return(c1.nb_w != c2.nb_w || c1.nb_rw != c2.nb_rw ||
        c1.nb_b != c2.nb_b || c1.nb_rb != c2.nb_rb);
}

/******************************************
 *           REPONSEs JOUEURS             *
 ******************************************/

/* structure representant un coup */
typedef struct{
  position pos;
  joueur pl;
  direction dir;
}coup;

/* fonction demandant un coup a jouer */
void choose_move(coup c){
  int flag, i, j;
  char dir;
  do{
        flag = 1;
        printf("choose a pawn and a direction to move (i, j, dir): ");
        scanf("%d, %d, %c", &i, &j, &dir);
        switch (dir) {
          case 'N': c.dir = NORD; break;
          case 'S': c.dir = SUD; break;
          case 'E': c.dir = EST; break;
          case 'O': c.dir = OUEST; break;
          default :
            flag = 0;
            printf("Invalid direction, direction must be N, S, E, or W.\n");
        }
        if(i < 0 || i >= SIZE || j < 0 || j >= SIZE){
          flag = 0;
          printf("Positions invalides, les positions i et j doivent être comprise entre 0 et %d.\n", SIZE-1);
        }
        else{
          c.pos.i = i;
          c.pos.j = j;
        }
      }while(!flag);
}
/******************************************
 *        DETECTION D'UN GAGNANT          *
 ******************************************/

/* type representant un gagnant */
typedef enum{
  PLAYING,
  WHITE_WIN,
  BLACK_WIN,
} result;

/* fonction renvoyant un gagnant s'il y a un gagnant */
result winner(compteur compt){
  if(compt.nb_b == 8 || compt.nb_rw == 7){
    return WHITE_WIN;
  }
  if(compt.nb_w == 8 || compt.nb_rb == 7){
    return BLACK_WIN;
  }
  return PLAYING;
}

/******************************************
 *           FONCTION PRINCIPALE          *
 ******************************************/

/* type reponses de la fonction principale */
typedef enum {
  EMPTY_POS,
  PION_ADVERSE,
  PION_RED,
  SORTIE_MONPION,
  PAS_ESPACE,
  RETOUR_EN_ARRIERE,
  SUCCES
}erreur;

//fonction pr savoir s'il y a un espace derriere 14
//TRUE=pas d'espace
//FALSE=il y a un espace ou vide
int sansespace (content b[SIZE][SIZE], coup c){
 switch(c.dir){
    case NORD:
    if(c.pos.i == SIZE-1){
      return 0;
    }
    return (b[c.pos.i+1][c.pos.j]!=EMPTY);

    case SUD:
    if(c.pos.i == 0){
      return 0;
    }
    return (b[c.pos.i-1][c.pos.j]!=EMPTY);

    case EST:
    if(c.pos.j == 0){
      return 0;
    }
    return (b[c.pos.i][c.pos.j-1]!=EMPTY);

    case OUEST:
    if(c.pos.j == SIZE-1){
      return 0;
    }
    return (b[c.pos.i][c.pos.j+1]!=EMPTY);
  }
  return 0;
}

content sortie_pion(content (*b)[SIZE], coup c){
  int i, j, n;
  switch(c.dir) {
    case NORD :
    for(i = c.pos.i; b[i][c.pos.j] != EMPTY; i--){
      n = i;
    }
    n--;
    if(!(n>=0 && n<SIZE)){
      n=0;
    }
    return b[n][c.pos.j];

    case SUD :
    for(i = c.pos.i; b[i][c.pos.j] != EMPTY; i++){
      n = i;
    }
    n++;
    if(!(n>=0 && n<SIZE)){
      n = SIZE-1;
    }
    return b[n][c.pos.j];

    case EST :
    for(j = c.pos.j; b[c.pos.i][j] != EMPTY; j++){
      n = j;
    }
    n++;
    if(!(n>=0 && n<SIZE)){
      n = SIZE-1;
    }
    return b[c.pos.i][n];

    case OUEST :
    for(j= c.pos.j; b[c.pos.i][j]!=EMPTY; j--){
      n=j;
    }
    n--;
    if(!(n>=0 && n<SIZE)){
      n=0;
    }
    return b[c.pos.i][n];
  }
}

//fonction reponse 16
erreur reply (content b[SIZE][SIZE], coup c, ban_coup *ban){
  if (b[c.pos.i][c.pos.j]==EMPTY){
    return EMPTY_POS;
  }
  else if (b[c.pos.i][c.pos.j]==RED_P){
    return PION_RED;
  }
  else if((c.pl==JOUW && b[c.pos.i][c.pos.j]==BLACK_P) || (c.pl==JOUB && b[c.pos.i][c.pos.j]==WHITE_P)){
    return PION_ADVERSE;
  }
  //pion directement après celui qu'on veut bouger
  if ((sansespace(b, c))==1){
    return PAS_ESPACE;
  }
  //sortie de son propre pion
  if((c.pl==JOUW && sortie_pion(b, c)==WHITE_P)|| (c.pl==JOUB && sortie_pion(b, c)==BLACK_P)){
    return SORTIE_MONPION;
  }
  //retour en arriere
  else if(c.pos.i == ban -> a && c.pos.j == ban -> b && c.dir == ban -> d){
    return RETOUR_EN_ARRIERE;
  }
  return SUCCES;
}

/******************************************
 *               AFFICHAGE                *
 ******************************************/

// fonction d'affichage des numéros de colonne
void affichage_num_col(){
  int i;
  if (SIZE > 10){
    printf(" ");
  }
  else{
    printf("");
  }
  printf("i|j ");
  for (i = 0; i < SIZE; i ++){
    if (i < 10){
      printf("%d  ", i);
    }
    else{
      printf("%d ", i);
    }
  }
  printf("\n");
}

// fonction d'affichage de la délimitation du haut et bas plateau
void affichage_bord(){
  int i;
  if (SIZE > 10){
    printf("   +");
  }
  else{
    printf("  +");
  }
  for (i = 0; i < (3 * SIZE); i ++){
    printf("-");
  }
  printf("+\n");
}

// fonction d'affichage de la délimitation droite et des numéros de ligne du plateau
void affichage_bord_droite(int i){
    if (SIZE > 10){
      if (i < 10){
        printf("%d  |", i);
      }
      else{
        printf("%d |", i);
      }
    }
    else{
      printf("%d |", i);
    }
}

//fonction d'affichage du plateau :
void print_board(content (*b)[SIZE]) {
  int i, j;

  affichage_num_col();
  affichage_bord();

  for(i = 0; i < SIZE; i++){
    affichage_bord_droite(i);
    for(j = 0; j < SIZE; j++){
      switch(b[i][j]){
        case BLACK_P : printf(" B "); break;
        case WHITE_P : printf(" W "); break;
        case RED_P : printf(" R "); break;
        default : printf(" . ");
      }
    }
    printf("|\n");
  }
  affichage_bord();
  printf("\n");
}

void print_tour(coup c){
  if(c.pl == JOUW){
    printf("White's turn to play\n");
  }
  else{
    printf("Black's turn to play\n");
  }
}


//condition pr gagner : 7 billes r ou toutes les billes adverses
/******************************************
 *                  MAIN                  *
 ******************************************/

int main(){
  srand((time(NULL)));

  coup c;
  c.pl=JOUW;
  content plateau[SIZE][SIZE], a;
  compteur compt, compt2;
  ban_coup ban;
  ban.a = ban.b = -1;
  ban.d = OUEST;
  erreur err;
  char replay, t;
  int pl, k, l;
  compt.nb_b = compt.nb_w = compt2.nb_b = compt2.nb_w = 0;
  compt.nb_rb = compt.nb_rw = compt2.nb_rb = compt2.nb_rw = 0;

  printf("\nKuba ready to play !!!\n\n");
  printf("How many players (1 or 2)? : ");
  scanf("%d", &pl);
  printf("\n\n");

  int flag;

  init_board(plateau);
  print_board(plateau);

  printf("\nWhite begin\n");
  //if (pl==2){
    while (winner(compt)==PLAYING){
      choose_move(c);
      err = reply(plateau, c, &ban);
      switch (err){
        case EMPTY_POS:
        printf ("\nThe position is empty\n");
        continue;

        case PION_RED:
        printf ("\nRed pawns can't be moved\n");
        continue;

        case PION_ADVERSE:
        printf ("\nIt is not your pawn\n");
        continue;

        case PAS_ESPACE:
        printf ("\nThere is a pawn right behind\n");
        continue;

        case RETOUR_EN_ARRIERE:
        printf ("\nPawns positions are back to your last turn ones\n");
        continue;

        case SORTIE_MONPION:
        printf ("\nYour own pawn got out of the board\n"); continue;

        case SUCCES:
        a = deplacement(plateau, c.pos, c.dir, &ban);
        compte(a, c.pl, &compt);
        printf("\n\n");
        print_board(plateau);

        if (verif_compt(compt, compt2) == 1){
          compt2.nb_rw = compt.nb_rw;
          compt2.nb_w = compt.nb_w;
          compt2.nb_rb = compt.nb_rb;
          compt2.nb_b = compt.nb_b;

          if (winner(compt)!=PLAYING){
            break;
          }

          printf ("would you like to replay ? (y or n) : ");
          scanf (" %c", &replay);

          if(replay == 'n'){
            c.pl = tour(c.pl);
          }
        }
        else {
          c.pl=tour(c.pl);
        }

      }

      print_tour(c);
    }
    result win;
    win = winner(compt);
    switch(win){
      case WHITE_WIN: printf("White Win's !!"); break;
      case BLACK_WIN: printf("Black win's !!"); break;
      default: break;
    }
    //}
  //}
  return 0;
}
/*
  else if(pl==1){
    while (winner(compt)==PLAYING){

    }
  }


  return 0;
}*/
gcc -o sdl_exemple sdl_exemple.c `sdl2-config --cflags --libs` -lSDL2_gfx -lSDL2_ttf
/*
 * Le code ci-dessous donne quelques exemples d'usage de SDL. Sous ubuntu, les
 * packages a installer avec leurs  dependances sont :
 *
 * libsdl-2-2.0-0 (ou version superieure)
 * libsdl2-dev
 * libsdl2-gfx-1.0-0 (ou version superieure)
 * libsdl2-gfx-dev
 * libsdl2-ttf-2.0-0 (ou version superieure)
 * libsdl2-ttf-dev
 *
 * La compilation se fait via la commande suivante :
 *
 * gcc -o sdl_exemple sdl_exemple.c `sdl2-config --cflags --libs` -lSDL2_gfx -lSDL2_ttf
 *
 * ou par : sh compile.sh dans le terminal - le fichier contient cette commande.
 *
 * La documentation de SDL se trouve sur ces sites :
 * SDL2 (librairire de base) :
 * http://wiki.libsdl.org/FrontPage
 * SDL2 gfx (des primitives graphiques supplementaires)
 * https://www.ferzkopp.net/Software/SDL2_gfx/Docs/html/_s_d_l2__gfx_primitives_8c.html
 * SDL2 ttf (pour l'affichage de texte)
 *
 * Ce que cette librairie permet de faire :
 * - afficher une fenetre a l'ecran
 * - capturer les mouvements de souris dans les fenetres
 * - capturer les evenements clavier lorsque la fenetre a le focus
 * - dessiner dans la fenetre.
 * - afficher des portions d'images dans la fenetre
 * - afficher du texte dans la fenetre.
 * La librairie n'est pas parfaite, et contient clairement des bugs : il
 * arrive que certains elements ne soient pas affiches d'une execution a
 * l'autre.
 *
 * Remarque : toutes les fonctions de cette librairie renvoient une  valeur
 * singuliere en cas d'erreur. Pour ne pas demesurement allonger ce code, la
 * detection des erreurs est limitee a quelques etapes-clefs (initialsations,
 * creation de la fenetre et de son renderer, creation de surfaces mais on peut
 * completer ce code en verifiant les valeurs des retours (ou par des
 * assertions sur leurs valeurs).
 *
 * Le code est par ailleurs tres (trop) lourdement commente - toutes ces
 * precisions sont inutiles apres s'etre suffisamment longtemps plonge
 * dans la doc.
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "SDL.h"
#include "SDL2_gfxPrimitives.h"
#include "SDL_ttf.h"

enum status {QUIT, CONTINUE};
/* etat actuel de la boucle d'evenements, c.f. plus bas. */

/* fonction utilitaire calculant une valeur de couleur sous la forme d'un
   entier signe sur 4 octets partir de ses composantes (entre 0 et 255) :
 * r : rouge
 * g : vert
 * b : bleu
 * a : opacite
 */
Uint32 color (Uint8 r, Uint8 g, Uint8 b, Uint8 a) {
    return r | g << 8 | b << 16 | a << 24;
}

/* fonction utilitaire transferant une "surface" vers la fenetre via son
 * "renderer" (c.f. plus bas) aux coordonnes (x, y)
 */
void draw_surface (SDL_Renderer *renderer, SDL_Surface *surface, int x, int y) {
    /* traduction de la surface en texture */
    SDL_Texture *texture =
	SDL_CreateTextureFromSurface (renderer, surface);
    assert (texture != NULL);
    /* construction du rectangle specifiant la zone d'affichage */
    SDL_Rect dest_rect =
	(SDL_Rect) {x, y, surface -> w, surface -> h};
    /* demande de transfert de la texture vers la fenetre */
    SDL_RenderCopy (renderer, texture, NULL, &dest_rect);
    /* mis a jour de la fenetre */
    SDL_RenderPresent(renderer);
    /* liberation des ressources allouees */
    SDL_DestroyTexture(texture);
}

/* fonction utilitaire affichant un texte a l'ecran. */
void draw_text (const char *text,       // pointeur vers la chaine
		SDL_Renderer *renderer, // renderer de la fenetre
		TTF_Font *font,         // font, c.f. plus bas
		int x,                  // coordonnes du coin superieur
		int y,                  // gauche du texte (l'axe y est inverse)
		Uint32 text_color) {
    Uint8 r = text_color & 0xff;
    Uint8 g = (text_color >> 8) & 0xff;
    Uint8 b = (text_color >> 16) & 0xff;
    SDL_Color fgc = {r, g, b};
    /* construction de la surface contenant le texte */
    SDL_Surface *text_surface =
	TTF_RenderUTF8_Blended (font, text, fgc);
    assert (text_surface != NULL);
    /* transfert de la surface vers l'ecran */
    draw_surface (renderer, text_surface, x, y);
    /* liberation des ressources allouees */
    SDL_FreeSurface (text_surface);
}

int main (int argc, char *argv[]) {
    /* La toute premiere etape est l'initialisation de SDL et de SDL_ttf */
    assert (!SDL_Init(SDL_INIT_VIDEO));
    atexit (SDL_Quit);
    assert (TTF_Init() != -1);
    atexit (TTF_Quit);

    /* La creation d'une fenetre graphique se fait en deux temps. Dans un
     * premier temps, on cree la fenetre  elle-meme. L'appel ci-dessous
     * renvoie un point d'acces a la fenetre, ou NULL sila creation a echouee.
     */
    SDL_Window *window;
    window = SDL_CreateWindow
	("A window",                 /* titre */
	 SDL_WINDOWPOS_CENTERED,     /* placement en x */
	 SDL_WINDOWPOS_CENTERED,     /* placement en y */
	 600,                         /* largeur */
	 400,                         /* hauteur */
	 0                            /* drapeaux, cf, la doc.  */
	 );
    assert (window != NULL);


    /* Dans un second temps, on cree pour la fenetre un "renderer" (un point
     * d'acces aux pixels de la fenetre a l'ecran) via ce renderer que l'on
     * pourra dessiner dans la fenetre, y transferer une image, etc.
     */
    SDL_Renderer *renderer = NULL;
    renderer = SDL_CreateRenderer (window, -1, 0);
    assert (renderer != NULL);

    /*  Pour effacer le contenu de la fenetre : */
    //SDL_RenderClear(renderer);

    /* quelques couleurs : */
    Uint32
	RED = color (255, 0, 0, 255),
	GREEN = color (0, 255, 0, 255),
	BLUE = color (0, 0, 255, 255),
	YELLOW = color (255, 255, 0, 255),
	CYAN = color (0, 255, 255, 255),
	MAGENTA = color (255, 0, 255, 255);
    /* quelques graphismes : cercle, cercle rempli, ligne, rectangle, rectangle
     * rempli... pour la liste complete de actions, c.f. le lien ci-dessus
     * (gfx). Les instructions n'auront d'effet sur la fenetre qu'au moment de
     * son rafraichissement.
     */
    circleColor (renderer, 50, 50, 48, RED);
    filledCircleColor (renderer, 150, 50, 48, GREEN);
    lineColor (renderer, 200, 0, 300, 100, BLUE);
    rectangleColor (renderer, 300, 0, 400, 100, YELLOW);
    boxColor (renderer, 401, 0, 500, 100, CYAN);
    /* Pour rafraichir la fenetre : */
    SDL_RenderPresent(renderer);
    /* remarque importante : si aucune operation graphique n'a ete faite entre
     * deux rafraichissements, le second efface le contenu de la fenetre - ce
     * point est a prendre en compte si une operation est par exemple soumise a
     * un if : il vaut mieux dans ce cas rafraichir apres l'operation effectuee,
     * ou apres chaque suite d'operations (mais pas apres  *chaque* operation,
     * le rafraichissement n'etant pas gratuit).
     */


    /* chargement d'un fichier de fonts : nom + taille des caracteres en pt. */
    TTF_Font *font;
    font = TTF_OpenFont("TurretRoad-Bold.ttf", 32);
    assert (font != NULL);

    /* exemple de trace de texte (avec la fonction ci-dessus) */
    draw_text ("Hello World!",
    	       renderer, font, 100, 200, MAGENTA);

    draw_text ("Hello World!",
    	       renderer, font, 100, 300, BLUE);

    /* Afficher une image bitmap dans la fenetre. L'image est chargee en
     * memoire dans une "surface" (une matrice de pixels hors-ecran). On ne
     * peut malheureusement pas charger autre chose que des bmp - il faudrait
     * (encore) plus de librairies externes (elles existent).
     */
    SDL_Surface *img_surface = NULL;
    img_surface = SDL_LoadBMP("test.bmp");
    assert (img_surface != NULL);
    draw_surface (renderer, img_surface, 300, 200);

    /* On peut rendre le fond de l'image chargee transparent. 255, 255, 255 :
     * sont le composantes RGB du blanc, le blanc  sera transparent - noter
     * que le resultat n'est pas parfait, mais cela ne vient pas de l'image,
     * seulement de SDL.
     */
    SDL_SetColorKey(img_surface, SDL_TRUE,
		    SDL_MapRGB(img_surface -> format, 255, 255, 255));
    draw_surface (renderer, img_surface, 400, 200);
    /* liberation des ressources allouees */
    SDL_FreeSurface(img_surface);


    /* boucle de capture d'evenements dans la fenetre. la boucle dure tant
     * que la variable status n'a pas pris la valeur QUIT, a la fermeture
     * de la fenetre.
     */
    enum status status = CONTINUE;
    do {
        SDL_Event e;
	/* si un nouvel evenement s'est produit.. */
        if (SDL_PollEvent(&e)) {
	    /* examen du type de l'evenement */
	    switch (e.type) {
		/* fermeture de la fenetre */
	    case SDL_QUIT :
		printf("fermeture de la fenetre.\n");
		status = QUIT;
		break;
		/* deplacement de la souris, pointeur dans la fenetre */
	    case SDL_MOUSEMOTION :
		printf("deplacement : %d %d\n",
		       e.motion.x,
		       e.motion.y);
		break;
		/* clic de souris, pointeur dans la fenetre */
	    case SDL_MOUSEBUTTONDOWN :
		printf("clic : %d %d, bouton ",
		       e.button.x,
		       e.button.y);
		/* avec l'un des boutons : */
		switch (e.button.button) {
		case SDL_BUTTON_LEFT   : printf ("gauche.\n");  break;
		case SDL_BUTTON_MIDDLE : printf ("central.\n"); break;
		case SDL_BUTTON_RIGHT  : printf ("droit.\n"); break;
		default : printf("autre.\n"); break;
		}
		break;
	    case SDL_MOUSEBUTTONUP :
		printf("de-clic : %d %d, bouton ",
		       e.button.x,
		       e.button.y);
		/* avec l'un des boutons : */
		switch (e.button.button) {
		case SDL_BUTTON_LEFT   : printf ("gauche.\n");  break;
		case SDL_BUTTON_MIDDLE : printf ("central.\n"); break;
		case SDL_BUTTON_RIGHT  : printf ("droit.\n"); break;
		default : printf("autre.\n"); break;
		}
		break;
	    case SDL_KEYDOWN :
		/* touche presse, focus sur la fenetre */
		printf("touche pressee : ");
		/* extraction de la valeur de la touche pressee :
		 * (1) extraction d'un description la touche
		 */
		SDL_Keysym ks = e.key.keysym;
		/* (2) affichage du code de la touche. cette valeur peut etre
		 * comparee a une constante de la forme SDLK_a, SDLK_b, ...,
		 * SLDK_z (touches a, b, ..., z)
		 * c.f. https://wiki.libsdl.org/SDL_Keycode
		 */
		printf("%d ", ks.sym);
		/* pour les touches 0 ... 9, a... z, le code de la touche
		 * coincide avec son code ascii - sans tenir compte d'un shift
		 * eventuel.
		 */
		if (ks.sym >= 32 && ks.sym <= 127)
		    printf(" soit '%c'\n", (char) ks.sym);
		else printf("\n");
		break;
	    case SDL_KEYUP :
		/* touche relachee, focus sur la fenetre */
		printf("touche relachee\n");
		break;

	    }
        }
    } while (status != QUIT);

    /* liberation des ressources allouees. */
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    return 0;
}
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>

/******************************************
 *                PLATEAU                 *
 ******************************************/

/* taille du plateau par default */
#define SIZE 7

/* type des valeurs du plateau */
typedef enum {
    EMPTY,
    BLACK_P,
    WHITE_P,
    RED_P
} content;

//emplacement des pions 2.3.4
//emplacement des pions noir
void where_b (content (*b)[SIZE]){
  int i,j;
  for (i=0;i<SIZE;i++){
    for(j=0;j<SIZE; j++){
      if((i<2 && j>=SIZE-2)||(i>=SIZE-2 && j<2)){
        b[i][j]=BLACK_P;
      }
    }
  }
}

//emplacement des pions blanc
void where_w(content (*b)[SIZE]){
  int i, j;
  for(i = 0; i < SIZE; i++){
    for(j = 0; j < SIZE; j++){
      if((i<2 && j<2)||(i>SIZE-3 && j>SIZE-3)){
        b[i][j]=WHITE_P;
      }
    }
  }
}

//emplacement des pions rouge
void where_r(content (*b)[SIZE]){
  int i, j;
  for(i = 2; i <= 4; i++){
    for(j = 2; j <= 4; j++){
      b[i][j]= RED_P;

    }
  }
  b[3][1]= RED_P;
  b[1][3]= RED_P;
  b[3][5]= RED_P;
  b[5][3]= RED_P;
}

//fonction d'initialisation du plateau 5:
void init_board(content (*b)[SIZE]){
  int i, j;
  for(i = 0; i < SIZE; i++){
    for(j = 0; j < SIZE; j++){
      b[i][j]= EMPTY;
    }
  }
  where_w(b);
  where_b(b);
  where_r(b);
}

/******************************************
 *       DEPLACEMENTS ET DIRECTIONS       *
 ******************************************/

/* structure representant une position */
typedef struct{
  int i;
  int j;
}position;

/* type de direction de deplacement des pions sur le plateau */
typedef enum {
  NORD,
  SUD,
  EST,
  OUEST
} direction;

/* structure representant le coup interdit au prochain tour */
typedef struct{
  int a;
  int b;
  direction d;
}ban_coup;

//fonction d'echange de pions 6
void swap (content *i, content *j) {
  int n = *i;
  *i = *j;
  *j = n;
}

//fonction de déplacement vers le nord 7
content dpl_N(content (*b)[SIZE], position pos, ban_coup *ban){
  int i, n;
  content a = EMPTY;
  for(i = pos.i; b[i][pos.j] != EMPTY; i--){
    n = i;
  }
  n--;
  ban -> a = n;
  ban -> b = pos.j;
  ban -> d = SUD;
  if(!(n >= 0 && n < SIZE)){
    n = 0;
    a = b[n][pos.j];
    b[n][pos.j] = EMPTY;
    ban -> a = -1;
  }
  while(n != pos.i){
    swap(&b[n][pos.j], &b[n+1][pos.j]);
    n++;
  }
  return a;
}

//fonction de déplacement vers le sud 8
content dpl_S(content (*b)[SIZE], position pos, ban_coup *ban){
  int i, n;
  content a = EMPTY;
  for(i = pos.i; b[i][pos.j] != EMPTY; i++){
    n = i;
  }
  n++;
  ban -> a = n;
  ban -> b = pos.j;
  ban -> d = NORD;
  if(!(n>=0 && n<SIZE)){
    n = SIZE-1;
    a = b[n][pos.j];
    b[n][pos.j] = EMPTY;
    ban -> a = -1;
  }
  while(n != pos.i){
    swap(&b[n][pos.j], &b[n-1][pos.j]);
    n--;
  }
  return a;
}
//fonction de déplacement vers l'est 9
content dpl_E(content (*b)[SIZE], position pos, ban_coup *ban){
  int j, n;
  content a = EMPTY;
  for(j = pos.j; b[pos.i][j] != EMPTY; j++){
    n = j;
  }
  n++;
  ban -> a = pos.i;
  ban -> b = n;
  ban -> d = OUEST;
  if(!(n>=0 && n<SIZE)){
    n = SIZE-1;
    a = b[pos.i][n];
    b[pos.i][n] = EMPTY;
    ban -> b = -1;
  }
  while(n != pos.j){
    swap(&b[pos.i][n], &b[pos.i][n-1]);
    n--;
  }
  return a;
}

//fonction de déplacement vers l'ouest 10
content dpl_O(content (*b)[SIZE],position pos, ban_coup *ban){
  int j, n;
  content a = EMPTY;
  for(j= pos.j; b[pos.i][j]!=EMPTY; j--){
    n=j;
  }
  n--;
  ban -> a = pos.i;
  ban -> b = n;
  ban -> d = EST;
  if(!(n>=0 && n<SIZE)){
    n=0;
    a = b[pos.i][n];
    b[pos.i][n]=EMPTY;
    ban -> b = -1;
  }
  while(n!=pos.j){
    swap(&b[pos.i][n], &b[pos.i][n+1]);
    n++;
  }
  return a;
}

//fonction déplacement des pions 11
content deplacement (content (*b)[SIZE] ,position pos, direction dir, ban_coup *ban){
  switch (dir){
    case NORD : return dpl_N(b, pos, ban);

    case SUD : return dpl_S(b, pos, ban);

    case EST : return dpl_E(b, pos, ban);

    case OUEST : return dpl_O(b, pos, ban);
  }
}

/******************************************
 *         COMPTEURS ET JOUEURS           *
 ******************************************/

/* type des joueurs */
typedef enum {
  JOUW,
  JOUB
}joueur;

/* structure  permettant de representer les pions pris par les joueurs */
typedef struct{
  int nb_b;
  int nb_w;
  int nb_rb;
  int nb_rw;
}compteur;

//fonction changement de joueur 12
joueur tour (joueur jou){
  if (jou==JOUB){
    return JOUW;
  }
  return JOUB;
}

//fonction compteur 13
void compte (content a, joueur j, compteur *c){
  if(j == JOUB){
    switch(a){
      case WHITE_P : c -> nb_w ++; break;
      case RED_P : c -> nb_rb ++; break;
      default : break;
    }
  }
  else{
    switch(a){
      case BLACK_P : c -> nb_b ++; break;
      case RED_P : c -> nb_rw ++; break;
      default : break;
    }
  }
}

//fonction de vérification d'incrémentation du compteur
int verif_compt(compteur c1, compteur c2){
  return(c1.nb_w != c2.nb_w || c1.nb_rw != c2.nb_rw ||
        c1.nb_b != c2.nb_b || c1.nb_rb != c2.nb_rb);
}

/******************************************
 *        DETECTION D'UN GAGNANT          *
 ******************************************/

/* type representant un gagnant */
typedef enum{
  PLAYING,
  WHITE_WIN,
  BLACK_WIN,
} result;

/* fonction renvoyant un gagnant s'il y a un gagnant */
result winner(compteur compt){
  if(compt.nb_b == 8 || compt.nb_rw == 7){
    return WHITE_WIN;
  }
  if(compt.nb_w == 8 || compt.nb_rb == 7){
    return BLACK_WIN;
  }
  return PLAYING;
}

/******************************************
 *           FONCTION PRINCIPALE          *
 ******************************************/

/* structure representant un coup */
typedef struct{
  position pos;
  joueur pl;
  direction dir;
}coup;

/* type reponses de la fonction principale */
typedef enum {
  EMPTY_POS,
  PION_ADVERSE,
  PION_RED,
  SORTIE_MONPION,
  PAS_ESPACE,
  RETOUR_EN_ARRIERE,
  SUCCES
}erreur;

//fonction pr savoir s'il y a un espace 14
int sansespace (content b[SIZE][SIZE], coup c){
 switch(c.dir){
    case NORD:
    return (b[(c.pos.i)+1][c.pos.j]!=EMPTY);

    case SUD:
    return (b[c.pos.i-1][c.pos.j]!=EMPTY);

    case EST:
    return (b[c.pos.i][c.pos.j-1]!=EMPTY);

    case OUEST:
    return (b[c.pos.i][c.pos.j+1]!=EMPTY);
  }
  return 0;
}

content sortie_pion(content (*b)[SIZE], coup c){
  int i, j, n;
  switch(c.dir) {
    case NORD :
    for(i = c.pos.i; b[i][c.pos.j] != EMPTY; i--){
      n = i;
    }
    n--;
    if(!(n>=0 && n<SIZE)){
      n=0;
    }
    return b[n][c.pos.j];

    case SUD :
    for(i = c.pos.i; b[i][c.pos.j] != EMPTY; i++){
      n = i;
    }
    n++;
    if(!(n>=0 && n<SIZE)){
      n = SIZE-1;
    }
    return b[n][c.pos.j];

    case EST :
    for(j = c.pos.j; b[c.pos.i][j] != EMPTY; j++){
      n = j;
    }
    n++;
    if(!(n>=0 && n<SIZE)){
      n = SIZE-1;
    }
    return b[c.pos.i][n];

    case OUEST :
    for(j= c.pos.j; b[c.pos.i][j]!=EMPTY; j--){
      n=j;
    }
    n--;
    if(!(n>=0 && n<SIZE)){
      n=0;
    }
    return b[c.pos.i][n];
  }
}
//fonction reponse 16
erreur reply (content b[SIZE][SIZE], coup c, ban_coup *ban){
  if (b[c.pos.i][c.pos.j]==EMPTY){
    return EMPTY_POS;
  }
  else if (b[c.pos.i][c.pos.j]==RED_P){
    return PION_RED;
  }
  else if((c.pl==JOUW && b[c.pos.i][c.pos.j]==BLACK_P) || (c.pl==JOUB && b[c.pos.i][c.pos.j]==WHITE_P)){
    return PION_ADVERSE;
  }
  //sortie de son propre pion
  if((c.pl==JOUW && sortie_pion(b, c)==WHITE_P)|| (c.pl==JOUB && sortie_pion(b, c)==BLACK_P)){
    return SORTIE_MONPION;
  }
  //retour en arriere
  else if(c.pos.i == ban -> a && c.pos.j == ban -> b && c.dir == ban -> d){
    return RETOUR_EN_ARRIERE;
  }

  //pion directement après celui qu'on veut bouger
  if ((sansespace(b, c))==1){
    return PAS_ESPACE;
  }
  return SUCCES;
}

/******************************************
 *               AFFICHAGE                *
 ******************************************/

// fonction d'affichage des numéros de colonne
void affichage_num_col(){
  int i;
  if (SIZE > 10){
    printf(" ");
  }
  else{
    printf("");
  }
  printf("i|j ");
  for (i = 0; i < SIZE; i ++){
    if (i < 10){
      printf("%d  ", i);
    }
    else{
      printf("%d ", i);
    }
  }
  printf("\n");
}

// fonction d'affichage de la délimitation du haut et bas plateau
void affichage_bord(){
  int i;
  if (SIZE > 10){
    printf("   +");
  }
  else{
    printf("  +");
  }
  for (i = 0; i < (3 * SIZE); i ++){
    printf("-");
  }
  printf("+\n");
}

// fonction d'affichage de la délimitation droite et des numéros de ligne du plateau
void affichage_bord_droite(int i){
    if (SIZE > 10){
      if (i < 10){
        printf("%d  |", i);
      }
      else{
        printf("%d |", i);
      }
    }
    else{
      printf("%d |", i);
    }
}

//fonction d'affichage du plateau :
void print_board(content (*b)[SIZE]) {
  int i, j;

  affichage_num_col();
  affichage_bord();

  for(i = 0; i < SIZE; i++){
    affichage_bord_droite(i);
    for(j = 0; j < SIZE; j++){
      switch(b[i][j]){
        case BLACK_P : printf(" B "); break;
        case WHITE_P : printf(" W "); break;
        case RED_P : printf(" R "); break;
        default : printf(" . ");
      }
    }
    printf("|\n");
  }
  affichage_bord();
  printf("\n");
}


//condition pr gagner : 7 billes r ou toutes les billes adverses
/******************************************
 *                  MAIN                  *
 ******************************************/

int main(){
  srand((time(NULL)));
  coup c;
  c.pl=JOUW;
  content plateau[SIZE][SIZE], a;
  compteur compt, compt2;
  ban_coup ban;
  ban.a = ban.b = -1;
  ban.d = OUEST;
  erreur err;
  //char replay, direct;
  init_board(plateau);
  compt.nb_b = compt.nb_w = compt2.nb_b = compt2.nb_w = 0;
  compt.nb_rb = compt.nb_rw = compt2.nb_rb = compt2.nb_rw = 0;
  print_board(plateau);
  int pl;
  printf("How many players (1 or 2)? : ");
  scanf("%d", &pl);
  printf("White begin\n");
/*
  //if (pl==2){
    while (winner(compt)==PLAYING){
      printf("choose a pawn and a direction to move (i, j, dir): ");
      scanf("%d, %d, %c", &c.pos.i, &c.pos.j, &direct);
      c.dir = NORD;
      printf("%d\n", c.dir);
      err = reply(plateau, c, &ban);
      printf("ok !!");
      if(err == SUCCES){
        a = deplacement(plateau, c.pos, c.dir, &ban);
        compte(a, c.pl, &compt);
        print_board(plateau);
        if (verif_compt(compt, compt2) == 1){
          compt2.nb_rw = compt.nb_rw;
          compt2.nb_w = compt.nb_w;
          compt2.nb_rb = compt.nb_rb;
          compt2.nb_b = compt.nb_b;
          printf("would you like to replay ? (y or n) : ");
          scanf("%c\n", &replay);
          if(replay == 'n'){
            tour(c.pl);
          }
        }
        else{
          tour(c.pl);
        }
      }
      else{
        switch (err){
          case EMPTY_POS:
          printf ("The position is empty");

          case PION_RED:
          printf ("Red pawns can't be moved");

          case PION_ADVERSE:
          printf ("It is not your pawn");

          case PAS_ESPACE:
          printf ("There is a pawn right behind");

          case RETOUR_EN_ARRIERE:
          printf ("Pawns positions are back to your last turn one");

          case SORTIE_MONPION:
          printf ("Your own pawn got out of the board");

          default : continue;
        }
      }
    }*/

/*
  else if(pl==1){
    while (winner(compt)==PLAYING){

    }
  }*/
  int k, l;
  char t;
  printf("choose a pawn and a direction to move (i, j, dir): ");
  scanf("%d, %d, %c", &k, &l, &t);
  c.pos.i = k;
  c.pos.j = l;
  switch (t) {
    case 'N': c.dir = NORD; break;
    case 'S': c.dir = SUD; break;
    case 'E': c.dir = EST; break;
    case 'O': c.dir = OUEST; break;
  }
  err = reply(plateau, c, &ban);
  printf("%d\n", err);
  a = deplacement(plateau, c.pos, c.dir, &ban);
  printf("%d\n", a);
  print_board(plateau);
  compte(a, JOUB, &compt);
  printf("%d , %d , %d , %d\n", compt.nb_rw, compt.nb_w, compt.nb_b, compt.nb_rb);
  printf("%d, %d, %d\n", ban.a, ban.b, ban.d);

/*
  c.pos.i = 5;
  c.pos.j = 1;
  c.dir = NORD;
  a = deplacement(plateau, c.pos, c.dir, &ban);
  printf("%d\n", a);
  print_board(plateau);
  compte(a, JOUB, &compt);
  printf("%d , %d , %d , %d\n", compt.nb_rw, compt.nb_w, compt.nb_b, compt.nb_rb);
  printf("%d, %d, %d\n", ban.a, ban.b, ban.d);


  c.pos.i = 4;
  c.pos.j = 1;
  c.dir = NORD;
  a = deplacement(plateau, c.pos, c.dir, &ban);
  printf("%d\n", a);
  print_board(plateau);
  compte(a, JOUB, &compt);
  printf("%d , %d , %d , %d\n", compt.nb_rw, compt.nb_w, compt.nb_b, compt.nb_rb);
  printf("%d, %d, %d\n", ban.a, ban.b, ban.d);*/

  return 0;
}
