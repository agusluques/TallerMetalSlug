#include "TextureBalas.h"

TextureBalas::TextureBalas() {
	id = 0;
	tipoArma = 0;
	xcord = 0;
	ycord = 0;
	spX = 0;
	spY = 0;

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
	//cout << "ycord: " << ycord << endl;
	drawingRect.y = ycord;
	if (drawingRect.x < 0) drawingRect.x = 0;
	//cout << "POS X CAMARA: " << camera->x << endl;
	//cout << "POS X CORD: " << xcord << endl;
	//cout << "drawingRect.x: " << drawingRect.x << endl;
	drawingRect.y = ycord;// - camera->y;

	//((anchoVentana*270)/altoVentana);

    //pasar escala x xml
	drawingRect.w = arma[tipoArma].w + escala;
	drawingRect.h = arma[tipoArma].h + escala;

	SDL_RenderCopyEx( window, mTexture, &arma[tipoArma], &drawingRect, 0, NULL, flipType );
}

void TextureBalas::actualizar(int x, int y, int spx, int spy, char flip){
	this->xcord = x;
	this->ycord = y;
	this-> spX = spx;
	this-> spY = spy;

	if (flip == 'D'){
		flipType = SDL_FLIP_NONE;
	}else flipType = SDL_FLIP_HORIZONTAL;

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
	//CORRIENDO
	arma[0].x=94; arma[0].y=72;
	arma[0].w=46; arma[0].h=6;

	arma[1].x=69; arma[1].y=67;
	arma[1].w=16; arma[1].h=16;

	arma[2].x=313; arma[2].y=271;
	arma[2].w=21; arma[2].h=13;

	
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
