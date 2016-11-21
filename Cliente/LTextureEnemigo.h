#include <iostream>
#include <cstring>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#ifndef LTEXTUREENEMIGO_H_
#define LTEXTUREENEMIGO_H_

using namespace std;

class LTextureEnemigo {

public:
	int id;
	int tipo;

	SDL_Rect player[13][9];
	
	int xcord;
	int ycord;
	int spX;
	int spY;

	int mWidth;
	int mHeight;

	SDL_RendererFlip flipType;
	SDL_Texture* mTexture;

//public:
	SDL_Texture* texture;
	LTextureEnemigo();
	~LTextureEnemigo();
	void free();
	void render(SDL_Renderer *window, SDL_Texture *texture, int cameraX, int escala);

	void actualizar(int x, int y, int spx, int spy, char flip);
	
	bool loadFromFile( string path,  SDL_Renderer* window );

	void setX(int movimiento);
	void setY(int movimiento);
	void setSpX(int movimiento);
	void setSpY(int movimiento);
	void setFlip(char flip);
	void setId(int id);
	int getId();
	int posicionX();
	int posicionY();

	void inicializarTexture(SDL_Renderer* window, char* text);
	bool loadFromFile(SDL_Renderer* window, char* path );
};

#endif /* LTEXTUREENEMIGO_H_ */
