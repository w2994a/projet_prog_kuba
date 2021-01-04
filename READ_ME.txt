Jeu Kuba

+----------------------------+
|        INFORMATIONS        |
+----------------------------+

ce programme permet de jouer au jeu Kuba original à 2 joueurs ou à 1 joueur
contre l'ordinateur. Les règle du jeu sont les mêmes que le jeu original, les
conditions de victoires sont : capturer toutes les billes (ou pions) de
l'adversaire (8 billes) ou 7 billes rouge.
Ce programme permet aussi de jouer à une variante du jeu, inventé par
les auteurs, où l'on peut jouer à Kuba à 4 joueurs ou 3 joueurs et 1
ordinateur. Les règles sont les mêmes que le jeu original, sauf pour les
conditions de victoires sont : capturer 4 billes (ou pions) adverse et 1
billes rouge ou 5 billes rouge.
Ce programme permet également de sauvegarder la partie en cours de jeu en
entrant : Q à la place de la direction du coup à jouer lors de la demande de
directions, puis d'accepter de quitter Kuba et d'accepter la sauvegarde de la
partie en cours. Les sauvegarde sont effectuées en fonction du nombre de
joueurs. il y a 4 type de sauvegardes différentes à 1, 2, 3, ou 4 joueurs.
Pour reprendre la dernière partie jouer en fonction du nombre de joueurs, il
suffit d'accepter le chargement de la dernière partie joué après la demande du
nombre de joueur.

! attention : Lors de la sauvegarde si une sauvegarde existais déjà celle-ci
sera écraser.

+----------------------------+
|  COMPILATION ET EXECUTION  |
+----------------------------+

pour compiler et exécuter Kuba.c, les librairies stdio.h , stdio_ext.h ,
assert.h , stdlib.h , time.h doivent être installés.

pour le compiler placer vous dans le repertoire contenant le jeu et entrer dans
le terminal ou le shell :

gcc Kuba.c -o Kuba


Pour lancer l'exécutable créé et joue à Kuba rester dans le repertoire
contenant le jeu entrer dans le terminal ou le shell :

./Kuba

+----------------------------+
|          AUTEURS           |
+----------------------------+

LEGRAS mia
AMORY william
