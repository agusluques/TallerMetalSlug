
#include "TextureEnergia.h"

TextureEnergia::TextureEnergia() {
	id = 0;
	xcord = 0;
	ycord = 0;
	spY = 0;

	texture = NULL;
	mWidth = 0;
	mHeight = 0;

}

TextureEnergia::~TextureEnergia() {
	// TODO Auto-generated destructor stub
}


void TextureEnergia::renderEnergia(SDL_Renderer* window, SDL_Texture *mTexture, int escala){

	SDL_Rect rect1;
	SDL_Rect rect2;

	rect1.h = 215;
	rect1.w = 800;
	rect1.x = 0;
	rect1.y = 0;

	rect2.h = 50;
	rect2.w = 140;
	rect2.x = getPos();
	rect2.y = 0;

	SDL_RenderCopy( window, texture, &energia[spY], &rect2 );
}

void TextureEnergia::actualizar(int x, int y, int spy){

	this-> spY += spy;
	if (spY > 10)
	  spy = 10;
	  //muerto eliminar

}

void TextureEnergia::setX(int movimiento){
	this->xcord = movimiento;
}

void TextureEnergia::setY(int movimiento){
	this->ycord = movimiento;
}


void TextureEnergia::setSpY(int movimiento){
	this->spY = movimiento;
}

void TextureEnergia::setId(int id){
	this->id = id;
}

void TextureEnergia::setPos(int pos){
	this->posicion = pos;
}

int TextureEnergia::getPos(){
	return posicion;
}

int TextureEnergia::getId(){
	return this->id;
}

int TextureEnergia::getSpY(){

	return spY;
}

void TextureEnergia::inicializarTexture(SDL_Renderer* window, char* text){
	loadFromFile(window, text);
	//perdiendo energia


	energia[0].x=0; energia[0].y=0;
	energia[0].w=800; energia[0].h=215;

	energia[1].x=0; energia[1].y=215;
	energia[1].w=800; energia[1].h=215;

	energia[2].x=0; energia[2].y=430;
	energia[2].w=800; energia[2].h=215;

	energia[3].x=0; energia[3].y=645;
	energia[3].w=800; energia[3].h=215;

	energia[4].x=0; energia[4].y=860;
	energia[4].w=800; energia[4].h=215;

	energia[5].x=0; energia[5].y=1075;
	energia[5].w=800; energia[5].h=215;

	energia[6].x=0; energia[6].y=1300;
	energia[6].w=800; energia[6].h=215;

	energia[7].x=0; energia[7].y=1515;
	energia[7].w=800; energia[7].h=215;

	energia[8].x=0; energia[8].y=1730;
	energia[8].w=800; energia[8].h=215;

	energia[9].x=0; energia[9].y=1945;
	energia[9].w=800; energia[9].h=215;

	energia[10].x=0; energia[10].y=2160;
	energia[10].w=800; energia[10].h=215;
}

void TextureEnergia::free()
{
	//Free texture if it exists
	if( texture != NULL )
	{
		SDL_DestroyTexture( texture );
		texture = NULL;
		mWidth = 0;
		mHeight = 0;
	}
}

