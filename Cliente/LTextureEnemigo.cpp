#include "LTextureEnemigo.h"

LTextureEnemigo::LTextureEnemigo() {
	id = 0;
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

LTextureEnemigo::~LTextureEnemigo() {
	//Deallocate
	//free();
}

void LTextureEnemigo::render(SDL_Renderer *window, SDL_Texture *texture, int cameraX, int escala){
	SDL_Rect drawingRect;
	if (xcord != cameraX) drawingRect.x = xcord - cameraX;
	else drawingRect.x = xcord; //BUSCAR MANERA MAS LINDA...

	drawingRect.y = ycord; //- cameraY;

    //pasar escala x xml
	drawingRect.w = player[spX][spY].w + escala;// + 30;
	drawingRect.h = player[spX][spY].h + escala;// + 30;

	if(tipo == 6) { //jefe1
		drawingRect.w += 120;
		drawingRect.h += 100;
	}else if(tipo == 2) { //rifle
		drawingRect.w += 20;
		drawingRect.h += 20;
	}

	//si ycord + drawingRect.h se pasa de la pantalla, hay q subirlo x el tamaño del sprite..
	if((drawingRect.y + drawingRect.h > 600)) drawingRect.y = 600 - drawingRect.h;

	SDL_RenderCopyEx( window, texture, &player[spX][spY], &drawingRect, 0, NULL, flipType );
}

void LTextureEnemigo::actualizar(int x, int y, int spx, int spy, char flip){
	this->xcord = x;
	this->ycord = y;
	this-> spX = spx;
	this-> spY = spy;

	if (flip == 'D'){
		flipType = SDL_FLIP_NONE;
	}else flipType = SDL_FLIP_HORIZONTAL;

}

void LTextureEnemigo::setX(int movimiento){
	this->xcord = movimiento;
}

void LTextureEnemigo::setY(int movimiento){
	this->ycord = movimiento;
}

void LTextureEnemigo::setSpX(int movimiento){
	this->spX = movimiento;
}

void LTextureEnemigo::setSpY(int movimiento){
	this->spY = movimiento;
}

void LTextureEnemigo::setFlip(char flip){
	if (flip == 'D'){
		flipType = SDL_FLIP_NONE;
	}else flipType = SDL_FLIP_HORIZONTAL;
}

void LTextureEnemigo::setId(int id){
	this->id = id;
}

int LTextureEnemigo::getId(){
	return this->id;
}

bool LTextureEnemigo::loadFromFile(SDL_Renderer* window, char* path ) {
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

void LTextureEnemigo::inicializarTexture(SDL_Renderer* window, char* text){
	loadFromFile(window, text);
	//HUYENDO
	player[0][0].x=0; player[0][0].y=50;
	player[0][0].w=30; player[0][0].h=40;

	player[1][0].x=40; player[1][0].y=50;
	player[1][0].w=30; player[1][0].h=40;

	player[2][0].x=80; player[2][0].y=50;
	player[2][0].w=30; player[2][0].h=40;

	player[3][0].x=120; player[3][0].y=50;
	player[3][0].w=30; player[3][0].h=40;

	player[4][0].x=160; player[4][0].y=50;
	player[4][0].w=30; player[4][0].h=40;

	player[5][0].x=200; player[5][0].y=50;
	player[5][0].w=30; player[5][0].h=40;

	//PARADO
	player[0][1].x=0; player[0][1].y=0;
	player[0][1].w=30; player[0][1].h=40;

	//ASUSTADO
	player[1][1].x = 0; player[1][1].y = 100;
	player[1][1].w = 30; player[1][1].h = 40;

	player[2][1].x = 40; player[2][1].y = 100;
	player[2][1].w = 30; player[2][1].h = 40;

	//ATAQUE DE SALTO
	player[0][2].x=0; player[0][2].y=160;
	player[0][2].w=30; player[0][2].h=40;

	player[1][2].x=40; player[1][2].y=150;
	player[1][2].w=30; player[1][2].h=50;

	player[2][2].x=80; player[2][2].y=160;
	player[2][2].w=40; player[2][2].h=40;

	player[3][2].x=130; player[3][2].y=160;
	player[3][2].w=40; player[3][2].h=40;

	player[4][2].x=180; player[4][2].y=160;
	player[4][2].w=40; player[4][2].h=40;

	player[5][2].x=230; player[5][2].y=160;
	player[5][2].w=40; player[5][2].h=40;

	player[6][2].x=280; player[6][2].y=160;
	player[6][2].w=40; player[6][2].h=40;

	player[7][2].x=330; player[7][2].y=160;
	player[7][2].w=50; player[7][2].h=40;

	player[8][2].x=390; player[8][2].y=150;
	player[8][2].w=50; player[8][2].h=50;

	player[9][2].x=450; player[9][2].y=150;
	player[9][2].w=50; player[9][2].h=50;

	player[10][2].x=510; player[10][2].y=150;
	player[10][2].w=50; player[10][2].h=50;

	player[11][2].x=570; player[11][2].y=160;
	player[11][2].w=50; player[11][2].h=40;

	player[12][2].x=630; player[12][2].y=160;
	player[12][2].w=50; player[12][2].h=40;

	//DISPARO
	player[0][3].x=0; player[0][3].y=210;
	player[0][3].w=50; player[0][3].h=50;

	player[1][3].x=50; player[1][3].y=210;
	player[1][3].w=50; player[1][3].h=50;

	player[2][3].x=100; player[2][3].y=210;
	player[2][3].w=50; player[2][3].h=50;

	player[3][3].x=150; player[3][3].y=210;
	player[3][3].w=50; player[3][3].h=50;

	player[4][3].x=200; player[4][3].y=210;
	player[4][3].w=50; player[4][3].h=50;

	player[5][3].x=250; player[5][3].y=210;
	player[5][3].w=50; player[5][3].h=50;

	player[6][3].x=300; player[6][3].y=210;
	player[6][3].w=50; player[6][3].h=50;

	player[7][3].x=350; player[7][3].y=210;
	player[7][3].w=50; player[7][3].h=50;

	player[8][3].x=400; player[8][3].y=210;
	player[8][3].w=50; player[8][3].h=50;

	player[9][3].x=450; player[9][3].y=210;
	player[9][3].w=50; player[9][3].h=50;

	//usar otra clase x ser otra texture? o unificar todo en solados.png
	//SOLDADOS JEFE 1
	//cae
	player[0][4].x=0; player[0][4].y=541;
	player[0][4].w=40; player[0][4].h=54;

	player[1][4].x=40; player[1][4].y=541;
	player[1][4].w=40; player[1][4].h=54;

	player[2][4].x=80; player[2][4].y=541;
	player[2][4].w=40; player[2][4].h=54;

	player[3][4].x=120; player[3][4].y=541;
	player[3][4].w=40; player[3][4].h=54;

	//dispara
	player[0][5].x=0; player[0][5].y=595;
	player[0][5].w=40; player[0][5].h=54;

	player[1][5].x=40; player[1][5].y=595;
	player[1][5].w=40; player[1][5].h=54;

	//helicoptero
	player[0][6].x=0; player[0][6].y=319;
	player[0][6].w=180; player[0][6].h=111;

	player[1][6].x=180; player[1][6].y=319;
	player[1][6].w=180; player[1][6].h=111;
	/* FIN SOLDADOS JEFE 1*/

	//caminando
	player[0][7].x=0; player[0][7].y=270;
	player[0][7].w=30; player[0][7].h=40;

	player[1][7].x=30; player[1][7].y=270;
	player[1][7].w=30; player[1][7].h=40;

	player[2][7].x=60; player[2][7].y=270;
	player[2][7].w=30; player[2][7].h=40;

	player[3][7].x=90; player[3][7].y=270;
	player[3][7].w=30; player[3][7].h=40;

	player[4][7].x=120; player[4][7].y=270;
	player[4][7].w=30; player[4][7].h=40;

	player[5][7].x=150; player[5][7].y=270;
	player[5][7].w=30; player[5][7].h=40;

	player[6][7].x=180; player[6][7].y=270;
	player[6][7].w=30; player[6][7].h=40;

	player[7][7].x=210; player[7][7].y=270;
	player[7][7].w=30; player[7][7].h=40;

	player[8][7].x=240; player[8][7].y=270;
	player[8][7].w=30; player[8][7].h=40;

	player[9][7].x=270; player[9][7].y=270;
	player[9][7].w=30; player[9][7].h=40;

	player[10][7].x=300; player[10][7].y=270;
	player[10][7].w=30; player[10][7].h=40;

	player[11][7].x=330; player[11][7].y=270;
	player[11][7].w=30; player[11][7].h=40;

	//muriendo
	player[0][8].x=0; player[0][8].y=660;
	player[0][8].w=40; player[0][8].h=40;

	player[1][8].x=40; player[1][8].y=660;
	player[1][8].w=40; player[1][8].h=40;

	player[2][8].x=80; player[2][8].y=660;
	player[2][8].w=40; player[2][8].h=40;

	player[3][8].x=120; player[3][8].y=660;
	player[3][8].w=40; player[3][8].h=40;

	player[4][8].x=160; player[4][8].y=660;
	player[4][8].w=40; player[4][8].h=40;

	player[5][8].x=200; player[5][8].y=660;
	player[5][8].w=40; player[5][8].h=40;

	player[6][8].x=240; player[6][8].y=660;
	player[6][8].w=40; player[6][8].h=40;

}

void LTextureEnemigo::free()
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
