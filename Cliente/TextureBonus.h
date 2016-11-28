#include <iostream>
#include <cstring>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#ifndef TEXTUREBONUS_H_
#define TEXTUREBONUS_H_

using namespace std;

class TextureBonus
{
public:
	int tipo;
	int id;

	SDL_Rect bonus[7];
	
	int xcord;
	int ycord;

	int mWidth;
	int mHeight;
	SDL_RendererFlip flipType;
	SDL_Texture* mTexture;

	SDL_Texture* texture;
	TextureBonus();
	~TextureBonus();
	void free();
	void render(SDL_Renderer *window, SDL_Texture *mTexture, int cameraX, int escala);
	
	bool loadFromFile( string path,  SDL_Renderer* window );

	void render(SDL_Renderer* window, int x, int y, SDL_Rect* clip, double angle, SDL_Point* center);

	void setX(int movimiento);
	void setY(int movimiento);
	void setTipo(int tipo);
	void setId(int id);
	int getTipo();
	int posicionX();
	int posicionY();
	void inicializarTexture(SDL_Renderer* window, char* text);
	void actualizar(int x, int y);
	
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

#endif /* TEXTUREBONUS_H_ */