#include <iostream>
#include <cstring>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>


#ifndef CLIENTE_TEXTUREENERGIA_H_
#define CLIENTE_TEXTUREENERGIA_H_

using namespace std;

class TextureEnergia
{
public:

	SDL_Rect energia[11];
	int id;
	int xcord;
	int ycord;
	int spY;
	int mWidth;
	int mHeight;

	int posicion;

	SDL_Texture* texture;

	TextureEnergia();

	void free();
	~TextureEnergia();

	void actualizar(int x, int y, int spy);
	void renderEnergia(SDL_Renderer* window, int escala);
	void setX(int movimiento);
	void setY(int movimiento);
	void setSpY(int movimiento);
	void setId(int id);
	void setPos (int pos);
	int getPos();
	int getId();
	int getSpY ();

	void inicializarTexture(SDL_Renderer* window, char* text);

	bool loadFromFile(SDL_Renderer* window, char* path ) {
		if( texture != NULL ) {
			SDL_DestroyTexture( texture );
			texture = NULL;

		}
		SDL_Surface* loadedSurface = IMG_Load( path );

		if( loadedSurface == NULL ) {
			printf( "Unable to load image %s! SDL_image Error: %s\n", path, IMG_GetError() );
		}
		else {
			this->texture = SDL_CreateTextureFromSurface( window, loadedSurface );
			//this->mTexture = IMG_LoadTexture(window, "Clarkcopia.png");
			if( this->texture == NULL ) {
				printf( "Unable to create texture from %s! SDL Error: %s\n", path, SDL_GetError() );
			}
			else {
				//mWidth = loadedSurface->w;
				//mHeight = loadedSurface->h;
			}
			SDL_FreeSurface( loadedSurface );
		}

		return (texture != NULL);
	}

};

#endif /* CLIENTE_TEXTUREENERGIA_H_ */
