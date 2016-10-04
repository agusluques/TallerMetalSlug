#include <iostream>
#include <cstring>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#ifndef LTEXTURE_H_
#define LTEXTURE_H_

using namespace std;

class LTexture {

private:
	int id;

	SDL_Rect cuerpo[6][1];
	int xcord;
	int ycord;
	int spX;
	int spY;

	SDL_RendererFlip flipType;

public:
	LTexture();
	~LTexture();
	void free();
	void render(SDL_Renderer *window, SDL_Texture *mTexture);

	void moverX(int movimiento);
	void moverDerecha(int movimiento);
	void moverIzquierda(int movimiento);

	void moverY(int movimiento);
	void moverArriba(int movimiento);
	void moverAbajo(int movimiento);

	void setX(int movimiento);
	void setY(int movimiento);
	void setId(int id);
	int getId();
	int posicionX();
	int posicionY();

	void inicializarTexture();

};

#endif /* LTEXTURE_H_ */
