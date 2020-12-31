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
