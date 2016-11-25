
#include "TextureScore.h"

TextureScore::TextureScore() {
	texture = NULL;

}

void TextureScore::setId(int id){
	this->id= id;
}

void TextureScore::setX(int x){
	this->xcord =x;
}

void TextureScore::setY(int y){
	this->ycord =y;
}

void TextureScore::setColor(SDL_Color color){

	this->color = color;
}

void TextureScore::setAumentable(int cantidad){

	this->aumentable = cantidad;
}

void TextureScore::inicializarTexture(SDL_Renderer* window, string aMostrar){

	char score[4];
	strcpy(score, aMostrar.c_str());
	TTF_Font *font = TTF_OpenFont("OpenSans-ExtraBold.ttf",50);
	SDL_Surface* superficie = TTF_RenderText_Solid(font, score,color);
	texture = SDL_CreateTextureFromSurface( window, superficie);

	SDL_FreeSurface(superficie);
}

void TextureScore::renderScore(SDL_Renderer* window){

	puntajeRect.x = xcord;
    puntajeRect.y = ycord;
	SDL_QueryTexture(texture, NULL, NULL, &puntajeRect.w, &puntajeRect.h);
	SDL_RenderCopy(window, texture, NULL, &puntajeRect);
}

TextureScore::~TextureScore() {
	// TODO Auto-generated destructor stub
}

