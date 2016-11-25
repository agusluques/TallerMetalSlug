#include <iostream>
#include <cstring>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>


#ifndef CLIENTE_TEXTURESCORE_H_
#define CLIENTE_TEXTURESCORE_H_

using namespace std;

class TextureScore {
public:

	int id;
	int xcord;
	int ycord;

	int aumentable;

	SDL_Texture* texture;
	SDL_Color color;
	SDL_Rect puntajeRect;

	TextureScore();
	void setId(int id);
	void setX(int x);
	void setY(int y);
	void setAumentable(int cantidad);
	void setColor (SDL_Color color);
	void inicializarTexture (SDL_Renderer* window, string aMostrar);
	void renderScore(SDL_Renderer* window);

    ~TextureScore();
};

#endif /* CLIENTE_TEXTURESCORE_H_ */
