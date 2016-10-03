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

	SDL_Rect torzo[6][1];
	SDL_Rect pierna[6][1];
	int xcord;
	int ycord;
	int spX;
	int spY;

public:
	LTexture();
	~LTexture();
	void free();
	//void render( int x, int y, SDL_Rect* clip = NULL , int flag = 0);
	void render(SDL_Renderer *window, SDL_Texture *mTexture);

	void moverX(int movimiento);
	void moverY(int movimiento);
	void setX(int movimiento);
	void setY(int movimiento);
	void setId(int id);
	int getId();
	int posicionX();
	int posicionY();

	void inicializarTexture();

};

#endif /* LTEXTURE_H_ */
