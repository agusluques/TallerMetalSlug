#include "TextureBonus.h"

TextureBonus::TextureBonus() {
	tipo = 1;
	xcord = 0;
	ycord = 0;
	id = 0;

	mTexture = NULL;
	mWidth = 0;
	mHeight = 0;
	flipType = SDL_FLIP_NONE;
	texture = NULL;
}

TextureBonus::~TextureBonus() {
	//Deallocate
	//free();
}

void TextureBonus::actualizar(int x, int y){
	this->xcord = x;
	this->ycord = y;
}

void TextureBonus::setId(int id){
	this->id = id;
}

void TextureBonus::render(SDL_Renderer *window, SDL_Texture *mTexture, int cameraX, int escala){
	SDL_Rect drawingRect;
	drawingRect.x = xcord - cameraX;
	drawingRect.y = ycord;

	if (drawingRect.x < 0) drawingRect.x = 0;

	drawingRect.w = bonus[tipo].w + escala;
	drawingRect.h = bonus[tipo].h + escala;

	SDL_RenderCopy(window, mTexture, &bonus[tipo], &drawingRect);
}


void TextureBonus::setX(int movimiento){
	this->xcord = movimiento;
}

void TextureBonus::setY(int movimiento){
	this->ycord = movimiento;
}


void TextureBonus::setTipo(int tipo){
	this->tipo = tipo;
}


int TextureBonus::getTipo(){
	return this->tipo;
}

void TextureBonus::inicializarTexture(SDL_Renderer* window, char* text){
	loadFromFile(window, text);
	
	bonus[0].x=0; bonus[0].y=0;
	bonus[0].w=33; bonus[0].h=29;

	bonus[1].x=36; bonus[1].y=8;
	bonus[1].w=23; bonus[1].h=20;

	bonus[2].x=64; bonus[2].y=0;
	bonus[2].w=33; bonus[2].h=29;

	bonus[3].x=98; bonus[3].y=8;
	bonus[3].w=23; bonus[3].h=20;

	bonus[4].x=127; bonus[4].y=0;
	bonus[4].w=33; bonus[4].h=29;

	bonus[5].x=0; bonus[5].y=34;
	bonus[5].w=26; bonus[5].h=20;

	bonus[6].x=35; bonus[6].y=33;
	bonus[6].w=25; bonus[6].h=25;
	
}

void TextureBonus::free()
{
	//Free texture if it exists
	if( texture != NULL )
	{
		SDL_DestroyTexture( texture );
		mTexture = NULL;
		mWidth = 0;
		mHeight = 0;
	}
}
