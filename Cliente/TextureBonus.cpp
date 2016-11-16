#include "TextureBonus.h"

TextureBonus::TextureBonus() {
	tipo = 1;
	xcord = 0;
	ycord = 0;

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

void TextureBonus::render(SDL_Renderer *window, SDL_Texture *mTexture, int cameraX, int escala){
	SDL_Rect drawingRect;
	drawingRect.x = xcord - cameraX;
	if (drawingRect.x < 0) drawingRect.x = 0;
	//cout << "POS X CAMARA: " << camera->x << endl;
	//cout << "POS X CORD: " << xcord << endl;
	//cout << "drawingRect.x: " << drawingRect.x << endl;
	drawingRect.y = ycord;// - camera->y;

	//((anchoVentana*270)/altoVentana);

    //pasar escala x xml
	drawingRect.w = arma[tipo].w + escala;
	drawingRect.h = arma[tipo].h + escala;

	SDL_RenderCopyEx( window, texture, &arma[tipo], &drawingRect, 0, NULL,flipType);
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
	//CORRIENDO
	arma[0].x=0; arma[0].y=0;
	arma[0].w=22; arma[0].h=22;

	
}

void TextureBonus::free()
{
	//Free texture if it exists
	if( mTexture != NULL )
	{
		SDL_DestroyTexture( mTexture );
		mTexture = NULL;
		mWidth = 0;
		mHeight = 0;
	}
}
