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

	drawingRect.w = bala[tipoDisp][spY].w;
	drawingRect.h = bala[tipoDisp][spY].h;

	if (drawingRect.x < 0) drawingRect.x = 0;

	int angle;

	switch (spX){
	case 0:
		angle = 0;
		break;
	case 1:
		angle = 315;
		break;
	case 2:
		angle = 270;
		break;
	case 3:
		angle = 225;
		break;
	case 4:
		angle = 180;
		break;
	case 5:
		angle = 135;
		break;
	case 6:
		angle = 90;
		break;
	case 7:
		angle = 45;
		break;

	}

	if(drawingRect.h == 5) { //H
		drawingRect.w += 10;
		//drawingRect.h += ;
	}else if(drawingRect.h == 8) { //R
		drawingRect.w += 50;
		//drawingRect.h += ;
	}else if(drawingRect.h == 64){ //S
		drawingRect.w += 80;
		drawingRect.h += 30;
	}else if(drawingRect.h == 119){ //rayo tanque
		angle = 0;
		drawingRect.w += 80;
		drawingRect.h += 30;
	}
	else if(drawingRect.h == 31){ //bomba helicoptero
		angle = 0;
		drawingRect.w += 30;
		drawingRect.h += 10;
	}

	drawingRect.w += escala;
	drawingRect.h += escala;

	SDL_RenderCopyEx( window, mTexture, &bala[tipoDisp][spY], &drawingRect, angle, NULL, flipType );
}

void TextureBalas::actualizar(int x, int y, int spx, int tipoDisp, int spy){
	this->xcord = x;
	this->ycord = y;
	this->spX = spx;
	this->tipoDisp = tipoDisp;
	this->spY = spy;
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

	//BALAS 1 MACHINE GUN
	bala[1][0].x = 0; bala[1][0].y = 25;
	bala[1][0].w = 25; bala[1][0].h = 5;

	//BALAS 2 R
	bala[2][0].x = 0; bala[2][0].y = 86;
	bala[2][0].w = 50; bala[2][0].h = 8;

	//BALAS 3 S
	bala[3][0].x = 0; bala[3][0].y = 108;
	bala[3][0].w = 50; bala[3][0].h = 64;

	bala[3][1].x = 50; bala[3][1].y = 108;
	bala[3][1].w = 60; bala[3][1].h = 64;

	bala[3][2].x = 110; bala[3][2].y = 108;
	bala[3][2].w = 80; bala[3][2].h = 64;

	bala[3][3].x = 190; bala[3][3].y = 108;
	bala[3][3].w = 81; bala[3][3].h = 64;

	bala[3][4].x = 271; bala[3][4].y = 108;
	bala[3][4].w = 87; bala[3][4].h = 64;

	bala[3][5].x = 357; bala[3][5].y = 108;
	bala[3][5].w = 88; bala[3][5].h = 64;

	bala[3][6].x = 180; bala[3][6].y = 374;
	bala[3][6].w = 88; bala[3][6].h = 64;

	//BALAS JEFE 2 TANQUE
	bala[4][0].x = 0; bala[4][0].y = 200;
	bala[4][0].w = 47; bala[4][0].h = 119;
	//tiritos
	bala[5][0].x = 0; bala[5][0].y = 25;
	bala[5][0].w = 25; bala[5][0].h = 5;

	//BALAS JEFE 3 HELICOPTERO
	bala[6][0].x = 0; bala[6][0].y = 320;
	bala[6][0].w = 17; bala[6][0].h = 31;
	//tiritos
	bala[7][0].x = 0; bala[7][0].y = 25;
	bala[7][0].w = 25; bala[7][0].h = 5;

}

void TextureBalas::free()
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

int TextureBalas::getTipoDisparo(){
	return this->tipoDisp;
}

void TextureBalas::setTipoDisparo(int tipo){
	this->tipoDisp = tipo;
}
