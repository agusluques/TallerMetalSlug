#include "TextureBalas.h"

TextureBalas::TextureBalas() {
	id = 0;
	tipoArma = 0;
	xcord = 0;
	ycord = 0;
	spX = 0;
	spY = 0;
	tipoDisp = 0;

	mTexture = NULL;
	mWidth = 0;
	mHeight = 0;

	texture = NULL;
	flipType = SDL_FLIP_NONE;
}

TextureBalas::~TextureBalas() {
	//Deallocate
	//free();
}

void TextureBalas::render(SDL_Renderer *window, SDL_Texture *mTexture, int cameraX, int escala){
	SDL_Rect drawingRect;
	drawingRect.x = xcord - cameraX;
	drawingRect.y = ycord;

	if (drawingRect.x < 0) drawingRect.x = 0;

	drawingRect.w = bala[spX][spY].w + escala;
	drawingRect.h = bala[spX][spY].h + escala;

	SDL_RenderCopy(window, mTexture, &bala[spX][spY], &drawingRect);
}

void TextureBalas::actualizar(int x, int y, int spx, int spy){
	this->xcord = x;
	this->ycord = y;
	this-> spX = spx;
	this-> spY = spy;
}

void TextureBalas::setX(int movimiento){
	this->xcord = movimiento;
}

void TextureBalas::setY(int movimiento){
	this->ycord = movimiento;
}

void TextureBalas::setSpX(int movimiento){
	this->spX = movimiento;
}

void TextureBalas::setSpY(int movimiento){
	this->spY = movimiento;
}

void TextureBalas::setFlip(char flip){
	if (flip == 'D'){
		flipType = SDL_FLIP_NONE;
	}else flipType = SDL_FLIP_HORIZONTAL;
}

void TextureBalas::setTipoArma(int tipoArma){
	this->tipoArma = tipoArma;
}

void TextureBalas::setId(int id){
	this->id = id;
}

int TextureBalas::getTipoArma(){
	return this->tipoArma;
}

void TextureBalas::inicializarTexture(SDL_Renderer* window, char* text){
	loadFromFile(window, text);
	//BALAS 0 PISTOLITA
	bala[0][0].x = 0; bala[0][0].y = 0;
	bala[0][0].w = 6; bala[0][0].h = 6;

	bala[1][0].x = 0; bala[1][0].y = 0;
	bala[1][0].w = 6; bala[1][0].h = 6;

	bala[2][0].x = 0; bala[2][0].y = 0;
	bala[2][0].w = 6; bala[2][0].h = 6;

	bala[3][0].x = 0; bala[3][0].y = 0;
	bala[3][0].w = 6; bala[3][0].h = 6;

	bala[4][0].x = 0; bala[4][0].y = 0;
	bala[4][0].w = 6; bala[4][0].h = 6;

	bala[5][0].x = 0; bala[5][0].y = 0;
	bala[5][0].w = 6; bala[5][0].h = 6;

	bala[6][0].x = 0; bala[6][0].y = 0;
	bala[6][0].w = 6; bala[6][0].h = 6;
	
	bala[7][0].x = 0; bala[7][0].y = 0;
	bala[7][0].w = 6; bala[7][0].h = 6;

	//BALAS 1 MACHINE GUN
	//BALAS 2..
}

void TextureBalas::free()
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

int TextureBalas::getTipoDisparo(){
	return this->tipoDisp;
}

void TextureBalas::setTipoDisparo(int tipo){
	this->tipoDisp = tipo;
}
