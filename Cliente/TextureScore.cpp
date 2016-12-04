
#include "TextureScore.h"

TextureScore::TextureScore() {
	texture = NULL;
	superficie = NULL;
	font = NULL;

	id = 0;
	xcord = 0;
	ycord = 0;

	aumentable = 0;
	//font = TTF_OpenFont("OpenSans-ExtraBold.ttf",50);

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

	if(font != NULL) TTF_CloseFont(font);
	font = TTF_OpenFont("OpenSans-ExtraBold.ttf",50);

	if(superficie != NULL) SDL_FreeSurface(superficie);
	superficie = TTF_RenderText_Solid(font, score, color);
	texture = SDL_CreateTextureFromSurface( window, superficie);

	TTF_CloseFont(font);
	font = NULL;
	SDL_FreeSurface(superficie);
	superficie = NULL;
}

void TextureScore::renderScore(SDL_Renderer* window){
	puntajeRect.x = xcord;
    puntajeRect.y = ycord;
	SDL_QueryTexture(texture, NULL, NULL, &puntajeRect.w, &puntajeRect.h);
	SDL_RenderCopy(window, texture, NULL, &puntajeRect);
	if(texture != NULL) SDL_DestroyTexture(texture);
	texture = NULL;
}

TextureScore::~TextureScore() {
	// TODO Auto-generated destructor stub
}

void TextureScore::free()
{
	//Free texture if it exists
	if( texture != NULL )
	{
		SDL_DestroyTexture( texture );
		texture = NULL;
	
	}
}

