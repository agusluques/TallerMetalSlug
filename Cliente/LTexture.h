#include <iostream>
#include <cstring>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#ifndef LTEXTURE_H_
#define LTEXTURE_H_

using namespace std;

class LTexture {

public:
	int id;

	//SDL_Rect cuerpo[6][1];
	SDL_Rect player[6][3];
	
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
	LTexture();
	~LTexture();
	void free();
	void render(SDL_Renderer *window, SDL_Texture *mTexture, int cameraX, int escala);

	void actualizar(int x, int y, int spx, int spy, char flip);
	
	bool loadFromFile( string path,  SDL_Renderer* window );

	void render(SDL_Renderer* window, int x, int y, SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip flip );

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
			//SDL_FreeSurface( loadedSurface );
		}

		return (texture != NULL);
	}

};

#endif /* LTEXTURE_H_ */
