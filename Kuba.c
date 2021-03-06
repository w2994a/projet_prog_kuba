#include <stdio.h>
#include <stdio_ext.h>
#include <assert.h>
#include <stdlib.h>
#include <time.h>

/******************************************
 *                PLATEAU                 *
 ******************************************/

// taille du plateau par default
#define SIZE 7

// type des valeurs du plateau
typedef enum {
    EMPTY,
    RED_P,
    BLACK_P,
    WHITE_P,
    GREEN_P,
    ORANGE_P
} content;

// fonction d'initialisation du plateau à 1 ou 2 joueurs
void init_2_player (content (*b)[SIZE]){
  int i,j;
  for (i=0; i<SIZE; i++){
    for(j=0; j<SIZE; j++){
      if((i<2 && j<2) || (i>SIZE-3 && j>SIZE-3)){
        b[i][j] = WHITE_P;
      }
      else if((i<2 && j>SIZE-3) || (i>SIZE-3 && j<2)){
        b[i][j] = BLACK_P;
      }
      else if((i>1 && i<5) && (j>1 && j<5)){
        b[i][j] = RED_P;
      }
      else{
        b[i][j] = EMPTY;
      }
    }
  }
  b[3][1]= RED_P;
  b[1][3]= RED_P;
  b[3][5]= RED_P;
  b[5][3]= RED_P;
}

// fonction d'initialisation du plateau à 3 ou 4 joueurs
void init_4_player (content (*b)[SIZE]){
  int i,j;
  for (i=0; i<SIZE; i++){
    for(j=0; j<SIZE; j++){
      if(i<2 && j<2){
        b[i][j] = WHITE_P;
      }
      else if(i<2 && j>SIZE-3){
        b[i][j] = BLACK_P;
      }
      else if(i>SIZE-3 && j<2){
        b[i][j] = GREEN_P;
      }
      else if(i>SIZE-3 && j>SIZE-3){
        b[i][j] = ORANGE_P;
      }
      else if((i>1 && i<5) && (j>1 && j<5)){
        b[i][j] = RED_P;
      }
      else{
        b[i][j] = EMPTY;
      }
    }
  }
  b[3][1]= RED_P;
  b[1][3]= RED_P;
  b[3][5]= RED_P;
  b[5][3]= RED_P;
}

// fonction d'initialisation du plateau
void init_board(content (*b)[SIZE], int player){
  if(player == 1 || player == 2){
    init_2_player(b);
  }
  else if(player == 3 || player == 4){
    init_4_player(b);
  }
}

/******************************************
 *       DEPLACEMENTS ET DIRECTIONS       *
 ******************************************/

// structure representant une position
typedef struct{
  int i;
  int j;
}position;

// type de direction de deplacement des pions sur le plateau
typedef enum {
  NORD,
  SUD,
  EST,
  OUEST
} direction;

// structure representant le coup interdit au prochain tour
typedef struct{
  int a;
  int b;
  direction d;
}ban_coup;

// fonction d'initialisation du coup banni
void init_ban_coup(ban_coup *ban){
  ban -> a = ban -> b = -1;
  ban -> d = NORD;
}

// fonction d'echange de pions
void swap (content *i, content *j) {
  int n = *i;
  *i = *j;
  *j = n;
}

// fonction de déplacement vers le nord
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

// fonction de déplacement vers le sud
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
// fonction de déplacement vers l'est
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

// fonction de déplacement vers l'ouest
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

// fonction déplacement des pions
content deplacement (content (*b)[SIZE] ,position pos, direction dir,
ban_coup *ban){
  switch (dir){
    case NORD : return dpl_N(b, pos, ban);

    case SUD : return dpl_S(b, pos, ban);

    case EST : return dpl_E(b, pos, ban);

    case OUEST : return dpl_O(b, pos, ban);
  }
  return EMPTY;
}

/******************************************
 *         COMPTEURS ET JOUEURS           *
 ******************************************/

// type des joueurs
typedef enum {
  JOUW,
  JOUB,
  JOUG,
  JOUO
}joueur;

// structure  permettant de representer les pions pris ou perdu par les joueurs
typedef struct{
  int nb_b;
  int nb_w;
  int nb_g;
  int nb_o;
  int nb_rb;
  int nb_rw;
  int nb_rg;
  int nb_ro;
}compteur;

// fonction changement de joueur
joueur tour (joueur jou, compteur rem_pawn, int player){
  if(player == 1 || player == 2){
    if (jou==JOUW){
      return JOUB;
    }
  }
  else if(player == 3 || player == 4){
    if(jou == JOUW){
      if(rem_pawn.nb_b == 0){
        tour(JOUB, rem_pawn, player);
      }
      else{
        return JOUB;
      }
    }
    else if(jou == JOUB){
      if(rem_pawn.nb_g == 0){
        tour(JOUG, rem_pawn, player);
      }
      else{
        return JOUG;
      }
    }
    else if(jou == JOUG){
      if(rem_pawn.nb_o == 0){
        tour(JOUO, rem_pawn, player);
      }
      else{
        return JOUO;
      }
    }
    else if(jou == JOUO){
      if(rem_pawn.nb_w == 0){
        tour(JOUW, rem_pawn, player);
      }
      else{
        return JOUW;
      }
    }
  }
  return JOUW;
}

// fonction d'initialisation des compteurs
void init_compt(compteur *compt, compteur *compt2, compteur *rem_pawn,
int player){
  compt -> nb_b = compt -> nb_w = compt2 -> nb_b = compt2 -> nb_w = 0;
  compt -> nb_g = compt -> nb_o = compt2 -> nb_g = compt2 -> nb_o = 0;
  compt -> nb_rb = compt -> nb_rw = compt2 -> nb_rb = compt2 -> nb_rw = 0;
  compt -> nb_rg = compt -> nb_ro = compt2 -> nb_rg = compt2 -> nb_ro = 0;

  if(player == 1 || player == 2){
    rem_pawn -> nb_b = rem_pawn -> nb_w = 8;
    rem_pawn -> nb_g = rem_pawn -> nb_o = 0;
    }
  else if(player == 3 || player == 4){
    rem_pawn -> nb_b = rem_pawn -> nb_w =
    rem_pawn -> nb_g = rem_pawn -> nb_o = 4;
  }
}

// fonction compteur
void compte (content a, joueur j, compteur *c, compteur *c2){
  if(j == JOUB){
    switch(a){
      case WHITE_P : c -> nb_b ++; c2 -> nb_w --; break;
      case GREEN_P : c -> nb_b ++; c2 -> nb_g --; break;
      case ORANGE_P : c -> nb_b ++; c2 -> nb_o --; break;
      case RED_P : c -> nb_rb ++; break;
      default : break;
    }
  }
  else if(j == JOUW){
    switch(a){
      case BLACK_P : c -> nb_w ++; c2 -> nb_b --; break;
      case GREEN_P : c -> nb_w ++; c2 -> nb_g --; break;
      case ORANGE_P : c -> nb_w ++; c2 -> nb_o --; break;
      case RED_P : c -> nb_rw ++; break;
      default : break;
    }
  }
  else if(j == JOUG){
    switch(a){
      case WHITE_P : c -> nb_g ++; c2 -> nb_w --; break;
      case BLACK_P : c -> nb_g ++; c2 -> nb_b --; break;
      case ORANGE_P : c -> nb_g ++; c2 -> nb_o --; break;
      case RED_P : c -> nb_rg ++; break;
      default : break;
    }
  }
  else{
    switch(a){
      case WHITE_P : c -> nb_o ++; c2 -> nb_w --; break;
      case BLACK_P : c -> nb_o ++; c2 -> nb_b --; break;
      case GREEN_P : c -> nb_o ++; c2 -> nb_g --; break;
      case RED_P : c -> nb_ro ++; break;
      default : break;
    }
  }
}

// fonction de mise à jour du compteur
void update_compt(compteur *c1, compteur *c2){
  c2 -> nb_w = c1 -> nb_w;
  c2 -> nb_b = c1 -> nb_b;
  c2 -> nb_g = c1 -> nb_g;
  c2 -> nb_o = c1 -> nb_o;
  c2 -> nb_rw = c1 -> nb_rw;
  c2 -> nb_rb = c1 -> nb_rb;
  c2 -> nb_rg = c1 -> nb_rg;
  c2 -> nb_ro = c1 -> nb_ro;
}

// fonction de vérification d'incrémentation du compteur
int verif_compt(compteur c1, compteur c2){
  return(c1.nb_w != c2.nb_w || c1.nb_rw != c2.nb_rw ||
  c1.nb_b != c2.nb_b || c1.nb_rb != c2.nb_rb ||
  c1.nb_g != c2.nb_g || c1.nb_rg != c2.nb_rg ||
  c1.nb_o != c2.nb_o || c1.nb_ro != c2.nb_ro );
}

/******************************************
 *           REPONSES JOUEURS             *
 ******************************************/

// structure representant un coup
typedef struct{
  position pos;
  joueur pl;
  direction dir;
}coup;

// fonction demandant le nombre de joueurs
int choose_nb_player(){
  int k, pl;
  do{
      printf("choose a number of players (1, 2, 3 or 4) : ");
      k = scanf ("%d", &pl);
      __fpurge (stdin);
      if (!k) {
        printf("Invalid entry, the number of players must be between 1 and \
4.\n");
      }
      if(pl < 0 || pl > 4){
        k = 0;
        printf("Invalid entry, the number of players must be between 1 and \
4.\n");
      }
  }while (!k);
  printf("\n\n");
  return pl;
}

// fonction demandant si l'on souhaite charger la derniere partie sauvegarde
char choose_load(int player){
  int flag; char ld;
  do{
      flag = 1;
      printf("do you want to continue the last game played with %d player(s) ?\
\n(y or n) : ", player);
      scanf(" %c", &ld);
      switch (ld) {
        case 'y': break;
        case 'n': break;
        default :
          flag = 0;
          printf("Invalid entry, the answer must be yes (y) or no (n).\n");
      }
  }while(!flag);
  return ld;
}

// fonction demandant un coup a jouer au joueur
int choose_move(coup *c){
  int k, i, j, flag;
  char dir;

  printf("\nchoose a pawn and a direction to move (i, j, dir) :\n");
  do{
      printf("choose a position i : ");
      k = scanf ("%d", &i);
      __fpurge (stdin);
      if (!k) {
        printf("Invalid entry, position i must be between 0 and %d.\n",
        SIZE-1);
      }
      if(i < 0 || i >= SIZE){
        k = 0;
        printf("Invalid position, i must be between 0 and %d.\n",
        SIZE-1);
      }
  }while (!k);

  do{
      printf("choose a position j : ");
      k = scanf ("%d", &j);
      __fpurge (stdin);
      if (!k) {
        printf("Invalid entry, position j must be between 0 and %d.\n",
        SIZE-1);
      }
      if(j < 0 || j >= SIZE){
        k = 0;
        printf("Invalid positions, j must be between 0 and %d.\n",
        SIZE-1);
      }
  }while (!k);

  do{
      flag = 1;
      printf("choose a direction (dir) : ");
      scanf(" %c", &dir);
      switch (dir) {
        case 'N': c -> dir = NORD; break;
        case 'S': c -> dir = SUD; break;
        case 'E': c -> dir = EST; break;
        case 'W': c -> dir = OUEST; break;
        case 'Q': return 1;
        default :
          flag = 0;
          printf("Invalid direction, must be N, S, E, W or Q for quit.\n");
      }
  }while(!flag);
  c -> pos.i = i;
  c -> pos.j = j;
  return 0;
}

// fonction demandant au joueur s'il souhaite passer son tour
char choose_pass_tour(){
  int flag; char pt;
  do{
      flag = 1;
      printf("do you want skip your turn ? (y or n) : ");
      scanf(" %c", &pt);
      switch (pt) {
        case 'y': break;
        case 'n': break;
        default :
          flag = 0;
          printf("Invalid entry, the answer must be yes (y) or no (n).\n");
      }
  }while(!flag);
  printf("\n");
  return pt;
}

// fonction demandant au joueur s'il souhaite rejouer
char choose_replay(){
  int flag; char rp;
  do{
      flag = 1;
      printf("\nwould you like to replay ? (y or n) : ");
      scanf(" %c", &rp);
      switch (rp) {
        case 'y': break;
        case 'n': break;
        default :
          flag = 0;
          printf("Invalid entry, the answer must be yes (y) or no (n).\n");
      }
  }while(!flag);
  return rp;
}

/******************************************
 *         SAUVEGARDE / CHARGEMENT        *
 *         INITIALISATION PLATEAU         *
 *              QUITTER KUBA              *
 ******************************************/

// fonction de sauvegarde du jeu
void save_game(const char *nom_fichier, content (*b)[SIZE], coup c,
compteur compt, compteur rem_pawn, ban_coup bc){
  FILE *fw;
  int i, j;
  fw = fopen(nom_fichier, "w");
  for(i = 0; i < SIZE; i++){
    for(j = 0; j < SIZE; j++){
      fprintf(fw, "%u ", b[i][j]);
    }
    fprintf(fw, "\n");
  }
  fprintf(fw, "%u %u %u %u %u %u %u %u", compt.nb_b, compt.nb_w, compt.nb_g,
  compt.nb_o, compt.nb_rb, compt.nb_rw, compt.nb_rg,compt.nb_ro);
  fprintf(fw, " %u %u %u %u", rem_pawn.nb_b, rem_pawn.nb_w, rem_pawn.nb_g,
  rem_pawn.nb_o);
  fprintf(fw, " %u %u %u", bc.a, bc.b, bc.d);
  fprintf(fw, " %u\n", c.pl);
  fclose(fw);
}

// fonction de chargement du jeu
void loading_game(const char *nom_fichier, content (*b)[SIZE], coup *c,
compteur *compt, compteur *rem_pawn, ban_coup *bc, int player){
  FILE *fr = NULL;
  int i, j;
  fr = fopen(nom_fichier, "r");
  if(fr != NULL){
    for(i = 0; i < SIZE; i++){
      for(j = 0; j < SIZE; j++){
        fscanf(fr, "%u", &b[i][j]);
      }
    }

    fscanf(fr, "%u", &compt -> nb_b);
    fscanf(fr, "%u", &compt -> nb_w);
    fscanf(fr, "%u", &compt -> nb_g);
    fscanf(fr, "%u", &compt -> nb_o);
    fscanf(fr, "%u", &compt -> nb_rb);
    fscanf(fr, "%u", &compt -> nb_rw);
    fscanf(fr, "%u", &compt -> nb_rg);
    fscanf(fr, "%u", &compt -> nb_ro);

    fscanf(fr, "%u", &rem_pawn -> nb_b);
    fscanf(fr, "%u", &rem_pawn -> nb_w);
    fscanf(fr, "%u", &rem_pawn -> nb_g);
    fscanf(fr, "%u", &rem_pawn -> nb_o);

    fscanf(fr, "%u", &bc -> a);
    fscanf(fr, "%u", &bc -> b);
    fscanf(fr, "%u", &bc -> d);

    fscanf(fr, "%u", &c -> pl);
  }
  else{
    printf("saving doesn't exist.\n");
    printf("\nWhite begin\n\n");
    init_board(b, player);
  }
}

// fonction d'initialisation ou de chargement du plateau
void load_board(content (*b)[SIZE], coup *c, compteur *compt, compteur compt2,
compteur *rem_pawn, ban_coup *bc, char load, int player){
  if(load == 'y'){
    if(player == 2){
      loading_game("save_2_player.txt", b, c, compt, rem_pawn, bc, player);
      update_compt(compt, &compt2);
    }
    else if(player == 1){
      loading_game("save_1_player.txt", b, c, compt, rem_pawn, bc, player);
      update_compt(compt, &compt2);
    }
    else if(player == 3){
      loading_game("save_3_player.txt", b, c, compt, rem_pawn, bc, player);
      update_compt(compt, &compt2);
    }
    else if(player == 4){
      loading_game("save_4_player.txt", b, c, compt, rem_pawn, bc, player);
      update_compt(compt, &compt2);
    }
    printf("\n");
  }
  else{
    init_board(b, player);
    printf("\nWhite begin\n\n");
  }
}

// fonction de cofirmation pour quitter le jeu et demande si on souhaite
// sauvegarder la partie en cours.
int choose_quit(content (*b)[SIZE], coup c, compteur compt, compteur rem_pawn,
ban_coup bc, int player){
  int flag;
  char k;

  do{
    flag = 1;
    printf("do you want quit the game ? (y or n) : ");
    scanf(" %c", &k);
    switch (k) {
      case 'y': break;
      case 'n': return 1;
      default :
        flag = 0;
        printf("Invalid entry, the answer must be yes (y) or no (n).\n");
    }
  }while(!flag);

  do{
    flag = 1;
    printf("do you want to save the game ? (overwrite the last %d player \
saved game)\n (y or n) : ", player);
    scanf(" %c", &k);
    switch (k) {
      case 'y':
        if(player == 1){
          save_game("save_1_player.txt", b, c, compt, rem_pawn, bc);
        }
        else if(player == 2){
          save_game("save_2_player.txt", b, c, compt, rem_pawn, bc);
        }
        else if(player == 3){
          save_game("save_3_player.txt", b, c, compt, rem_pawn, bc);
        }
        else if(player == 4){
          save_game("save_4_player.txt", b, c, compt, rem_pawn, bc);
        }
      case 'n': break;
      default :
        flag = 0;
        printf("Invalid entry, the answer must be yes (y) or no (n).\n");
    }
  }while(!flag);
  return 0;
}

/******************************************
 *        DETECTION D'UN GAGNANT          *
 ******************************************/

// type representant un gagnant
typedef enum{
  PLAYING,
  WHITE_WIN,
  BLACK_WIN,
  GREEN_WIN,
  ORANGE_WIN
} result;

// fonction renvoyant un gagnant s'il y a un gagnant
result winner(compteur compt, int player){
  if(player == 1 || player == 2){
    if(compt.nb_w == 8 || compt.nb_rw == 7){
      return WHITE_WIN;
    }
    if(compt.nb_b == 8 || compt.nb_rb == 7){
      return BLACK_WIN;
    }
  }

  else if(player == 3 || player == 4){
    if((compt.nb_w == 4 && compt.nb_rb >= 1 ) || compt.nb_rw == 5){
      return WHITE_WIN;
    }
    if((compt.nb_b >= 4 && compt.nb_rb >= 1 ) || compt.nb_rb == 5){
      return BLACK_WIN;
    }
    if((compt.nb_g >= 4 && compt.nb_rb >= 1 ) || compt.nb_rg == 5){
      return GREEN_WIN;
    }
    if((compt.nb_o >= 4 && compt.nb_rb >= 1 ) || compt.nb_ro == 5){
      return ORANGE_WIN;
    }
  }
  return PLAYING;
}

/******************************************
 *           FONCTION PRINCIPALE          *
 ******************************************/

// types de reponse de la fonction principale
typedef enum {
  EMPTY_POS,
  PION_ADVERSE,
  PION_RED,
  SORTIE_MONPION,
  BLOQUE,
  PAS_ESPACE,
  RETOUR_EN_ARRIERE,
  SUCCES
}erreur;

// fonction pour savoir s'il y a un espace derriere le pion qu'on veut déplacer
// (TRUE=pas d'espace; FALSE=il y a un espace ou vide)
int sansespace (content (*b)[SIZE], coup c){
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

// fonction verifiant si le pion du joueur dont c'est le tour sort du plateau
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
  return EMPTY;
}

// fonction reponse
erreur reply (content (*b)[SIZE], coup c, ban_coup *ban){
  if (b[c.pos.i][c.pos.j]==EMPTY){
    return EMPTY_POS;
  }
  else if (b[c.pos.i][c.pos.j]==RED_P){
    return PION_RED;
  }
  else if((c.pl==JOUW && b[c.pos.i][c.pos.j]!=WHITE_P) ||
  (c.pl==JOUB && b[c.pos.i][c.pos.j]!=BLACK_P) ||
  (c.pl==JOUG && b[c.pos.i][c.pos.j]!=GREEN_P) ||
  (c.pl==JOUO && b[c.pos.i][c.pos.j]!=ORANGE_P)){
    return PION_ADVERSE;
  }
  //pion bloqué
  if(b[c.pos.i-1][c.pos.j]&&b[c.pos.i][c.pos.j-1]&&b[c.pos.i][c.pos.j+1]&&b[c.pos.i+1][c.pos.j]){
    return BLOQUE;
  }
  //pion directement après celui qu'on veut bouger
  if ((sansespace(b, c))==1){
    return PAS_ESPACE;
  }
  //sortie de son propre pion
  if((c.pl==JOUW && sortie_pion(b, c)==WHITE_P)||
  (c.pl==JOUB && sortie_pion(b, c)==BLACK_P) ||
  (c.pl==JOUG && sortie_pion(b, c)==GREEN_P) ||
  (c.pl==JOUO && sortie_pion(b, c)==ORANGE_P)){
    return SORTIE_MONPION;
  }
  //retour en arriere
  else if(c.pos.i == ban -> a && c.pos.j == ban -> b && c.dir == ban -> d){
    return RETOUR_EN_ARRIERE;
  }
  return SUCCES;
}

/******************************************
 *             CHOIX DE L'ORDI            *
 ******************************************/

// fonction choissant un coup
void choose_move_computer(content (*b)[SIZE], coup *c, ban_coup *ban){
  erreur err;
  do{
    c -> pos.i = rand() % SIZE;
    c -> pos.j = rand() % SIZE;
    c -> dir = rand() % 4;
    err = reply(b, *c, ban);
  }while(err != SUCCES);
}

/******************************************
 *               AFFICHAGE                *
 ******************************************/

// fonction d'affichage des numéros de colonne du plateau
void affichage_num_col(){
  int i;
  if (SIZE > 10){
    printf(" ");
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

// fonction d'affichage de la délimitation droite et
// des numéros de ligne du plateau
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

// fonction d'affichage du joueur dont c'est le tour
void print_tour(coup c){
  if(c.pl == JOUW){
    printf("\nWhite's turn to play\n");
  }
  else if(c.pl == JOUB){
    printf("\nBlack's turn to play\n");
  }
  else if(c.pl == JOUG){
    printf("\nGreen's turn to play\n");
  }
  else{
    printf("\nOrange's turn to play\n");
  }
}

// fonction d'affichage des lignes du tableau des scores
void print_line(){
  int i;
  for(i=0; i<26; i++){
    printf("- ");
  }
}

// fonction d'affichage des points
void print_score(compteur *compt, compteur *rem_pawn, int player){
  print_line();
  printf("\n|                     SCORE                       |\n");
  print_line();
  printf("\n| Player | Opponent's pawn | Red pawn | remaining |\n");
  printf("|        |    captured     | captured |   pawn    |\n");
  print_line();
  printf("\n| WHITE  |        %d        |    %d     |     %d     |\n",
  compt -> nb_w, compt -> nb_rw, rem_pawn -> nb_w);
  print_line();
  printf("\n| BLACK  |        %d        |    %d     |     %d     |\n",
  compt -> nb_b, compt -> nb_rb, rem_pawn -> nb_b);
  print_line();
  if(player > 2){
    printf("\n| GREEN  |        %d        |    %d     |     %d     |\n",
    compt -> nb_g, compt -> nb_rg, rem_pawn -> nb_g);
    print_line();
    printf("\n| ORANGE |        %d        |    %d     |     %d     |\n",
    compt -> nb_o, compt -> nb_ro, rem_pawn -> nb_o);
    print_line();

  }
}

// fonction d'affichage du plateau
void print_board(content (*b)[SIZE], compteur *compt, compteur *rem_pawn,
int player) {
  int i, j;

  printf("            ");
  affichage_num_col();
  printf("            ");
  affichage_bord();

  for(i = 0; i < SIZE; i++){
    printf("            ");
    affichage_bord_droite(i);
    for(j = 0; j < SIZE; j++){
      switch(b[i][j]){
        case BLACK_P : printf(" B "); break;
        case WHITE_P : printf(" W "); break;
        case RED_P : printf(" R "); break;
        case GREEN_P : printf(" G "); break;
        case ORANGE_P : printf(" O "); break;
        default : printf(" . ");
      }
    }
    printf("|\n");
  }
  printf("            ");
  affichage_bord();
  print_score(compt, rem_pawn, player);
  printf("\n");
}

// fonction d'affichage du gagnant
void print_winner(result win){
  switch(win){
    case WHITE_WIN: printf("White Win's !!"); break;
    case BLACK_WIN: printf("Black win's !!"); break;
    case GREEN_WIN: printf("Green win's !!"); break;
    case ORANGE_WIN: printf("Orange win's !!"); break;
    default: break;
  }
}

/******************************************
 *                  MAIN                  *
 ******************************************/

int main(){
  srand((time(NULL)));

  coup c;
  content plateau[SIZE][SIZE], a;
  compteur compt, compt2, rem_pawn;
  ban_coup ban;
  erreur err;
  char replay, load, pass_tour;
  int player, quit, quit2;


  printf("\nKuba ready to play !!!\n\n");
  printf("To quit Kuba, entry Q instead of the direction when choosing a move \
to play.\n\n");
  // nb de joueurs
  player = choose_nb_player();
  // initialisation des compteurs de pions gagné, de pions perdu et
  //de vérification d'indrémentation du compteur de pion gagné
  init_compt(&compt, &compt2, &rem_pawn, player);
  // initialisation du coup banni au prochain tour
  init_ban_coup(&ban);
  // initialisation du premier joueur à jouer
  c.pl = JOUW;
  // continue derniere partie jouer ou non
  load = choose_load(player);
  // initialisation du plateau ou chargement de celui-ci
  load_board(plateau, &c, &compt, compt2, &rem_pawn, &ban, load, player);
  print_board(plateau, &compt, &rem_pawn, player);

  while (winner(compt, player)==PLAYING){
    print_tour(c);
    // partie joueurs réels
    if(player == 2 || player == 4 ||
    (player == 1 && c.pl == JOUW) || (player == 3 && c.pl != JOUO)){
      // choix deplacement ou quitter jeu
      quit = choose_move(&c);
      if(quit == 1){
        // verification quitter jeu et demande de sauvegarde
        quit2 = choose_quit(plateau, c, compt, rem_pawn, ban, player);
        // quitte le jeu ou le continue
        if(quit2 == 1){
          continue;
        }
        else if(quit2 == 0){
          break;
        }

      }
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

        case BLOQUE:
        printf ("\nYou can't move this pawn\n");
        // demande si on souhaite passer son tour
        pass_tour = choose_pass_tour();
        if(pass_tour == 'y'){
          c.pl = tour(c.pl, rem_pawn, player);
          print_board(plateau, &compt, &rem_pawn, player);
          continue;
        }
        else{
          continue;
        }

        case PAS_ESPACE:
        printf ("\nThere is a pawn right behind\n");
        continue;

        case RETOUR_EN_ARRIERE:
        printf ("\nPawns positions are back to your last turn ones\n");
        continue;

        case SORTIE_MONPION:
        printf ("\nYour own pawn got out of the board\n");

        case SUCCES:
        // réalise le déplacement et revoie le pion ejecté du plateau
        a = deplacement(plateau, c.pos, c.dir, &ban);
        // mise a jour des compeurs de pion gagné et de pion perdu
        compte(a, c.pl, &compt, &rem_pawn);
        printf("\n\n");
        print_board(plateau, &compt, &rem_pawn, player);

        // vérifie si le compteur à été incrémenté
        if (verif_compt(compt, compt2) == 1){
          // mise à jour des compteurs
          update_compt(&compt, &compt2);
          // si gagnant -> sortie de la boucle while
          if (winner(compt, player)!=PLAYING){
            break;
          }
          // demande si l'on souhaite rejouer
          replay = choose_replay();

          if(replay == 'n'){
            //changement de joueur si le joueur ne souhaite pas rejouer
            c.pl = tour(c.pl,rem_pawn, player);
          }
        }
        else {
          //changement de joueur
          c.pl = tour(c.pl, rem_pawn, player);
        }
      }
    }
    // partie joueur ordinateur
    else if((player == 1 && c.pl == JOUB) ||
    (player == 3 && c.pl == JOUO)){
      // choisi un coup à jouer
      choose_move_computer(plateau, &c, &ban);
      // réalise le déplacement et revoie le pion ejecté du plateau
      a = deplacement(plateau, c.pos, c.dir, &ban);
      // mise a jour des compeurs de pion gagné et de pion perdu
      compte(a, c.pl, &compt, &rem_pawn);
      printf("\n\n");
      print_board(plateau, &compt, &rem_pawn, player);

      // vérifie si le compteur à été incrémenté et rejoue s'il l'a été
      if (verif_compt(compt, compt2) == 1){
        // mise à jour des compteurs
        update_compt(&compt, &compt2);
        // si gagnant -> sortie de la boucle while
        if (winner(compt, player)!=PLAYING){
          break;
        }
      }
      else {
        //changement de joueur
        c.pl=tour(c.pl, rem_pawn, player);
      }
    }

  }
  result win;
  win = winner(compt, player);
  // afffichage du gagnant
  print_winner(win);
  printf("\n\nKuba finish, see you soon !!\n");
  return 0;
}
