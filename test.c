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