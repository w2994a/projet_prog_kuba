#include <stdio.h>
#include <stdio_ext.h>
#include <assert.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

/******************************************
 *                PLATEAU                 *
 ******************************************/

// taille du plateau par default
#define SIZE 7

// type des valeurs du plateau
typedef enum {
    EMPTY,
    BLACK_P,
    WHITE_P,
    RED_P
} content;

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

//fonction d'initialisation du plateau
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

// structure representant une position
typedef struct{
  int i;
  int j;
}position;

//type de direction de deplacement des pions sur le plateau
typedef enum {
  NORD,
  SUD,
  EST,
  OUEST
} direction;

//structure representant le coup interdit au prochain tour
typedef struct{
  int a;
  int b;
  direction d;
}ban_coup;

// fonction d'echange de pions
void swap (content *i, content *j) {
  int n = *i;
  *i = *j;
  *j = n;
}

// fonction de déplacement vers le nord 7
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
// fonction de déplacement vers l'est 9
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

// type des joueurs
typedef enum {
  JOUW,
  JOUB
}joueur;

//structure  permettant de representer les pions pris par les joueurs
typedef struct{
  int nb_b;
  int nb_w;
  int nb_rb;
  int nb_rw;
}compteur;

// fonction changement de joueur
joueur tour (joueur jou){
  if (jou==JOUB){
    return JOUW;
  }
  return JOUB;
}

// fonction compteur
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
  return(c1.nb_w != c2.nb_w || c1.nb_rw != c2.nb_rw ||c1.nb_b != c2.nb_b || c1.nb_rb != c2.nb_rb);
}

/******************************************
 *           REPONSES JOUEURS             *
 ******************************************/

//structure representant un coup
typedef struct{
  position pos;
  joueur pl;
  direction dir;
}coup;

// fonction demandant le nombre de joueurs
int choose_nb_player(){
  int k, pl;
  do{
      printf("choose a number of players (1 or 2) :");
      k = scanf ("%d", &pl);
      __fpurge (stdin);
      if (!k) {
        printf("invalid entry, the number of players must be 1 or 2.");
      }
  }while (!k);
  printf("\n\n");
  return pl;
}

// fonction demandant si l'on souhaite charger la derniere partie sauvegarde
char choose_load(){
  int flag; char ld;
  do{
      flag = 1;
      printf("do you want to continue the last game played ? (y or n) : ");
      scanf(" %c", &ld);
      switch (ld) {
        case 'y': break;
        case 'n': break;
        default : flag = 0; printf("Invalid entry, the answer must be yes (y) or no (n).\n");
      }
  }while(!flag);
  return ld;
}

/* fonction demandant un coup a jouer au joueur */
/*
void choose_move1(coup *c){
  int flag, k, l;
  char t;
  do{
    flag = 1;
    printf("choose a pawn and a direction to move (i, j, dir): ");
    scanf("%d, %d, %s", &k, &l, &t);
    switch (t) {
      case 'N': c -> dir = NORD; break;
      case 'S': c -> dir = SUD; break;
      case 'E': c -> dir = EST; break;
      case 'W': c -> dir = OUEST; break;
      default : flag = 0; printf("Invalid direction, direction must be N, S, E, or W.\n");
      }
      if(k < 0 || k >= SIZE || l < 0 || l >= SIZE){
        flag = 0;
        printf("Invalid positions, positions i and j must be between 0 and %d.\n", SIZE-1);
      }
      else{
        c -> pos.i = k;
        c -> pos.j = l;
      }
    }while(!flag);
}*/

// fonction demandant un coup a jouer au joueur
int choose_move(coup *c){
  int k, i, j, flag;
  char dir;

  printf("\nchoose a pawn and a direction to move (i, j, dir): \n");
  do{
      printf("choose a position i :");
      k = scanf ("%d", &i);
      __fpurge (stdin);
      if (!k) {
        printf("Invalid positions, positions i must be between 0 and %d.\n", SIZE-1);
      }
      if(i < 0 || i >= SIZE){
        k = 0;
        printf("Invalid positions, positions i and j must be between 0 and %d.\n", SIZE-1);
      }
  }while (!k);

  do{
      printf("choose a position j :");
      k = scanf ("%d", &j);
      __fpurge (stdin);
      if (!k) {
        printf("Invalid positions, positions j must be between 0 and %d.\n", SIZE-1);
      }
      if(j < 0 || j >= SIZE){
        k = 0;
        printf("Invalid positions, positions i and j must be between 0 and %d.\n", SIZE-1);
      }
  }while (!k);

  do{
      flag = 1;
      printf("choose a direction (dir): ");
      scanf(" %c", &dir);
      switch (dir) {
        case 'N': c -> dir = NORD; break;
        case 'S': c -> dir = SUD; break;
        case 'E': c -> dir = EST; break;
        case 'W': c -> dir = OUEST; break;
        case 'Q': return 1;
        default : flag = 0; printf("Invalid direction, direction must be N, S, E, or W.\n");
      }
  }while(!flag);
  c -> pos.i = i;
  c -> pos.j = j;
  return 0;
}

// fonction demandant au joueur s'il souhaite rejouer

char choose_replay(){
  int flag; char rp;
  do{
      flag = 1;
      printf("would you like to replay ? (y or n) : ");
      scanf(" %c", &rp);
      switch (rp) {
        case 'y': break;
        case 'n': break;
        default : flag = 0; printf("Invalid entry, the answer must be yes (y) or no (n).\n");
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
void save_game(const char *nom_fichier, content (*b)[SIZE], coup c, compteur compt){
  FILE *fw;
  int i, j;
  fw = fopen(nom_fichier, "w");
  for(i = 0; i < SIZE; i++){
    for(j = 0; j < SIZE; j++){
      fprintf(fw, "%d ", b[i][j]);
    }
    fprintf(fw, "\n");
  }
  fprintf(fw, "%d %d %d %d", compt.nb_b, compt.nb_w, compt.nb_rb, compt.nb_rw);
  fprintf(fw, " %d\n", c.pl);
  fclose(fw);
}
//exemple d'utilisation
//save_game("save_2_player.txt", tab, c, compt);

// fonction de chargement du jeu
void loading_game(const char *nom_fichier,content (*b)[SIZE],coup *c, compteur *compt){
  FILE *fr = NULL;
  int i, j;
  fr = fopen(nom_fichier, "r");
  if(fr != NULL){
    for(i = 0; i < SIZE; i++){
      for(j = 0; j < SIZE; j++){
        fscanf(fr, "%d", &b[i][j]);
      }
    }
    fscanf(fr, "%d", &compt -> nb_b);
    fscanf(fr, "%d", &compt -> nb_w);
    fscanf(fr, "%d", &compt -> nb_rb);
    fscanf(fr, "%d", &compt -> nb_rw);
    fscanf(fr, "%d", &c -> pl);
  }
  else{
    printf("saving doesn't exist.\n");
    init_board(b);
  }
}
//exemple d'utilisation
//loading_game("save_2_player.txt", tab, &c, &compt);

// fonction d'initialisation ou de chargement du plateau
void load_board(content (*b)[SIZE], coup c, compteur compt, compteur compt2, char load, int player){
  if(load == 'y'){
    if(player == 2){
      loading_game("save_2_player.txt", b, &c, &compt);
      compt2.nb_rw = compt.nb_rw;
      compt2.nb_w = compt.nb_w;
      compt2.nb_rb = compt.nb_rb;
      compt2.nb_b = compt.nb_b;
    }
    else if(player == 1){
      loading_game("save_1_player.txt", b, &c, &compt);
      compt2.nb_rw = compt.nb_rw;
      compt2.nb_w = compt.nb_w;
      compt2.nb_rb = compt.nb_rb;
      compt2.nb_b = compt.nb_b;
    }
  }
  else{
    init_board(b);
    printf("\nWhite begin\n");
  }
}

int choose_quit(content (*b)[SIZE], coup c, compteur compt, int player){
  int flag;
  char k;

  do{
    flag = 1;
    printf("do you want quit the game ? (y or n) : ");
    scanf(" %c", &k);
    switch (k) {
      case 'y': break;
      case 'n': return 1;
      default : flag = 0; printf("Invalid entry, the answer must be yes (y) or no (n).\n");
    }
  }while(!flag);

  do{
    flag = 1;
    printf("do you want save the game ? (y or n) : ");
    scanf(" %c", &k);
    switch (k) {
      case 'y':
        if(player == 1){
          save_game("save_1_player.txt", b, c, compt);
        }
        else if(player == 2){
          save_game("save_2_player.txt", b, c, compt);
        }
      case 'n': break;
      default : flag = 0; printf("Invalid entry, the answer must be yes (y) or no (n).\n");
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
} result;

// fonction renvoyant un gagnant s'il y a un gagnant
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

// types de reponse de la fonction principale
typedef enum {
  EMPTY_POS,
  PION_ADVERSE,
  PION_RED,
  SORTIE_MONPION,
  PAS_ESPACE,
  RETOUR_EN_ARRIERE,
  SUCCES
}erreur;

//fonction pour savoir s'il y a un espace derriere le pion qu'on veut déplacer (TRUE=pas d'espace; FALSE=il y a un espace ou vide)
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
}

// fonction reponse
erreur reply (content (*b)[SIZE], coup c, ban_coup *ban){
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
 *             CHOIX DE L'ORDI            *
 ******************************************/

// fonction choissant un coup
void choose_move_computer(content (*b)[SIZE], coup *c, ban_coup *ban){
  int i, j, k;
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

//fonction d'affichage de la délimitation droite et des numéros de ligne du plateau
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
  else{
    printf("\nBlack's turn to play\n");
  }
}

// fonction d'affichage des lignes du tableau des scores
void print_line(){
  int i;
  for(i=0; i<20; i++){
    printf("- ");
  }
}

// fonction d'affichage des points
void print_score(compteur *compt){
  print_line();
  printf("\n|               SCORE                 |\n");
  print_line();
  printf("\n| Player | Opponent's pawn | Red pawn |\n");
  printf("|        |    captured     | captured |\n");
  print_line();
  printf("\n| WHITE  |        %d        |    %d     |\n",compt -> nb_b, compt -> nb_rw);
  print_line();
  printf("\n| BLACK  |        %d        |    %d     |\n",compt -> nb_w, compt -> nb_rb);
  print_line();
}

// fonction d'affichage du plateau
void print_board(content (*b)[SIZE], compteur *compt) {
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
  print_score(compt);
  printf("\n");
}

// fonction d'affichage du gagnant
void print_winner(result win){
  switch(win){
    case WHITE_WIN: printf("White Win's !!"); break;
    case BLACK_WIN: printf("Black win's !!"); break;
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
  compteur compt, compt2;
  ban_coup ban;
  erreur err;
  char replay, load;
  int player, quit, quit2;

  c.pl=JOUW;
  ban.a = ban.b = -1;
  ban.d = OUEST;
  compt.nb_b = compt.nb_w = compt2.nb_b = compt2.nb_w = 0;
  compt.nb_rb = compt.nb_rw = compt2.nb_rb = compt2.nb_rw = 0;


  printf("\nKuba ready to play !!!\n\n");
  printf("To quit Kuba, entry Q instead of the direction when choosing a move to play.\n\n");
  //nb de joueurs
  player = choose_nb_player();
  //continue derniere partie jouer ou non
  load = choose_load();
  //initialisation du plateau ou chargement de celui-ci
  load_board(plateau, c, compt, compt2, load, player);
  print_board(plateau, &compt);

  while (winner(compt)==PLAYING){
    print_tour(c);
    if(player == 2 || (player == 1 && c.pl == JOUW)){
      quit = choose_move(&c);
      if(quit == 1){
        quit2 = choose_quit(plateau, c, compt, player);
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
        print_board(plateau, &compt);

        if (verif_compt(compt, compt2) == 1){
          compt2.nb_rw = compt.nb_rw;
          compt2.nb_w = compt.nb_w;
          compt2.nb_rb = compt.nb_rb;
          compt2.nb_b = compt.nb_b;

          if (winner(compt)!=PLAYING){
            break;
          }

          replay = choose_replay();

          if(replay == 'n'){
            c.pl = tour(c.pl);
          }
        }
        else {
            c.pl = tour(c.pl);
        }
      }
    }
    else if(player == 1 && c.pl == JOUB){
      choose_move_computer(plateau, &c, &ban);
      a = deplacement(plateau, c.pos, c.dir, &ban);
      compte(a, c.pl, &compt);
      printf("\n\n");
      print_board(plateau, &compt);

      if (verif_compt(compt, compt2) == 1){
        compt2.nb_rw = compt.nb_rw;
        compt2.nb_w = compt.nb_w;
        compt2.nb_rb = compt.nb_rb;
        compt2.nb_b = compt.nb_b;

        if (winner(compt)!=PLAYING){
          break;
        }
      }
      else {
        c.pl=tour(c.pl);
      }
    }

  }
  result win;
  win = winner(compt);
  print_winner(win);
  // afffichage gagnant
  printf("\n\nKuba finish, see you soon !!\n");
  return 0;
}
