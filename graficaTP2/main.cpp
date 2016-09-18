/*
 * main.cpp
 *
 *  Created on: 15/9/2016
 *      Author: matu
 */

#include <iostream>
#include <SDL/SDL.h>

#define WIDTH 800
#define HEIGHT 600
#define BPP 24
#define MASK 255, 255, 255

int main(int argc, char *argv[]) {

	SDL_Surface *jugador, *screen, *fondo;
	SDL_Event event;
	int terminar = 0;
	SDL_Rect posicion;
	int posicionJugadorX=320, posicionJugadorY=40;
	Uint8* teclas;

	atexit(SDL_Quit);

	//INICIALIZO SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0){ // EXISTEN MAS (AUDIO CD..)
		printf("No se puede iniciar SDL: %s\n",SDL_GetError());
		exit(1);
	}

	//CARGO CUADRADO
	jugador = SDL_LoadBMP("img/cuadrado.bmp");
	if ( jugador == NULL ) {
		printf("No pude cargar gráfico: %s\n", SDL_GetError());
		exit(1);
	}

	//CARGO FONOD
	fondo = SDL_LoadBMP("img/fondo.bmp");
	if ( fondo == NULL ) {
		printf("No pude cargar gráfico: %s\n", SDL_GetError());
		exit(1);
	}

	SDL_SetColorKey (jugador, SDL_SRCCOLORKEY, SDL_MapRGB (jugador->format, MASK));

	//CREO UNA SCREEN
	screen = SDL_SetVideoMode(WIDTH, HEIGHT, BPP, SDL_HWSURFACE);
	if(screen == NULL){
		printf("No se ha podido establecer el modo de vídeo: %s\n", SDL_GetError());
		exit(1);
	}

	//TITULO VENTANA
	SDL_WM_SetCaption( "Moviendo un cuadrado", "Moviendo un cuadrado" );

	while(terminar == 0) {

		//CARGO FONDO EN LA SCREEN
		SDL_BlitSurface(fondo, NULL, screen, NULL);

		posicion.x=posicionJugadorX;
		posicion.y=posicionJugadorY;
		SDL_BlitSurface(jugador, NULL, screen, &posicion);

		SDL_Flip(screen);

		//SALIR CON ESC O CERRAR VENTANA*/
		while ( SDL_PollEvent(&event) ) {
			if (event.type == SDL_QUIT || event.key.keysym.sym == SDLK_ESCAPE)
				terminar = 1;
		}

		//LEO MOVIMIENTO POR TECLADO
		teclas = SDL_GetKeyState(NULL);
		//FLECHAS
		if (teclas[SDLK_UP]) { posicionJugadorY -= 2; }
		if (teclas[SDLK_DOWN]) { posicionJugadorY += 2;}
		if (teclas[SDLK_LEFT]) { posicionJugadorX -= 2; }
		if (teclas[SDLK_RIGHT]) { posicionJugadorX += 2; }
		//WSAD
		if (teclas[SDLK_w]) { posicionJugadorY -= 2; }
		if (teclas[SDLK_s]) { posicionJugadorY += 2;}
		if (teclas[SDLK_a]) { posicionJugadorX -= 2; }
		if (teclas[SDLK_d]) { posicionJugadorX += 2; }
	}

	SDL_Quit();

	return 0;
}
