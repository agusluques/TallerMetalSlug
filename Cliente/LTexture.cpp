#include "LTexture.h"

LTexture::LTexture() {
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

LTexture::~LTexture() {
	//Deallocate
	//free();
}

void LTexture::render(SDL_Renderer *window, SDL_Texture *mTexture, int cameraX, int escala){
	SDL_Rect drawingRect;
	drawingRect.x = xcord - cameraX;
	if (drawingRect.x < 0) drawingRect.x = 0;
	//cout << "POS X CAMARA: " << camera->x << endl;
	//cout << "POS X CORD: " << xcord << endl;
	//cout << "drawingRect.x: " << drawingRect.x << endl;
	drawingRect.y = ycord - 65;// - camera->y;

	//((anchoVentana*270)/altoVentana);

	escala += 35;

    //pasar escala x xml
	drawingRect.w = player[spX][spY].w + escala;
	drawingRect.h = player[spX][spY].h + escala;

	SDL_RenderCopyEx( window, mTexture, &player[spX][spY], &drawingRect, 0, NULL, flipType );
}

void LTexture::actualizar(int x, int y, int spx, int spy, char flip){
	this->xcord = x;
	this->ycord = y;
	this-> spX = spx;
	this-> spY = spy;

	if (flip == 'D'){
		flipType = SDL_FLIP_NONE;
	}else flipType = SDL_FLIP_HORIZONTAL;

}

void LTexture::setX(int movimiento){
	this->xcord = movimiento;
}

void LTexture::setY(int movimiento){
	this->ycord = movimiento;
}

void LTexture::setSpX(int movimiento){
	this->spX = movimiento;
}

void LTexture::setSpY(int movimiento){
	this->spY = movimiento;
}

void LTexture::setFlip(char flip){
	if (flip == 'D'){
		flipType = SDL_FLIP_NONE;
	}else flipType = SDL_FLIP_HORIZONTAL;
}

void LTexture::setId(int id){
	this->id = id;
}

int LTexture::getId(){
	return this->id;
}

void LTexture::inicializarTexture(SDL_Renderer* window, char* text){
	loadFromFile(window, text);
	//CAMINAR
	player[0][0].x=0; player[0][0].y=65;
	player[0][0].w=40; player[0][0].h=65;

	player[1][0].x=40; player[1][0].y=65;
	player[1][0].w=40; player[1][0].h=65;

	player[2][0].x=80; player[2][0].y=65;
	player[2][0].w=40; player[2][0].h=65;

	player[3][0].x=120; player[3][0].y=65;
	player[3][0].w=40; player[3][0].h=65;

	player[4][0].x=160; player[4][0].y=65;
	player[4][0].w=40; player[4][0].h=65;

	player[5][0].x=200; player[5][0].y=65;
	player[5][0].w=40; player[5][0].h=65;

	//PARADO
	player[0][1].x=0; player[0][1].y=0;
	player[0][1].w=40; player[0][1].h=65;

	// sprite gris cuando se cierra la ventana.. ver cuando se mata el cliente q onda..
	//momia
	player[1][1].x = 0; player[1][1].y = 130;
	player[1][1].w = 40; player[1][1].h = 65;
	//zombi
	player[2][1].x = 40; player[2][1].y = 130;
	player[2][1].w = 40; player[2][1].h = 65;

	//SALTANDO
	player[0][2].x=0; player[0][2].y=195;
	player[0][2].w=40; player[0][2].h=65;

	//Disparando
	player[0][3].x=0; player[0][3].y=260;
	player[0][3].w=40; player[0][3].h=65;

	player[1][3].x=40; player[1][3].y=260;
	player[1][3].w=40; player[1][3].h=65;

	player[2][3].x=80; player[2][3].y=260;
	player[2][3].w=40; player[2][3].h=65;

	player[3][3].x=120; player[3][3].y=260;
	player[3][3].w=40; player[3][3].h=65;

	//Apunta arriba
	player[0][4].x=0; player[0][4].y=325;
	player[0][4].w=40; player[0][4].h=65;

	//Dispara arriba
	player[1][4].x=40; player[1][4].y=325;
	player[1][4].w=40; player[1][4].h=65;

	player[2][4].x=80; player[2][4].y=325;
	player[2][4].w=40; player[2][4].h=65;

	player[3][4].x=120; player[3][4].y=325;
	player[3][4].w=40; player[3][4].h=65;

	//Apunta abajo
	player[0][5].x=0; player[0][5].y=455;
	player[0][5].w=40; player[0][5].h=65;
	//Dispara abajo
	player[1][5].x=40; player[1][5].y=455;
	player[1][5].w=40; player[1][5].h=65;

	player[2][5].x=80; player[2][5].y=455;
	player[2][5].w=40; player[2][5].h=65;

	player[3][5].x=120; player[3][5].y=455;
	player[3][5].w=40; player[3][5].h=65;

	//Apunta diagArriba
	player[0][7].x=160; player[0][7].y=325;
	player[0][7].w=40; player[0][7].h=65;
	//Dispara diagArriba
	player[1][7].x=200; player[1][7].y=325;
	player[1][7].w=40; player[1][7].h=65;

	player[2][7].x=240; player[2][7].y=325;
	player[2][7].w=40; player[2][7].h=65;

	player[3][7].x=280; player[3][7].y=325;
	player[3][7].w=40; player[3][7].h=65;

	//Apunta diagAbajo
	player[0][8].x=160; player[0][8].y=455;
	player[0][8].w=40; player[0][8].h=65;
	//Dispara diagAbajo
	player[1][8].x=200; player[1][8].y=455;
	player[1][8].w=40; player[1][8].h=65;

	player[2][8].x=240; player[2][8].y=455;
	player[2][8].w=40; player[2][8].h=65;

	player[3][8].x=280; player[3][8].y=455;
	player[3][8].w=40; player[3][8].h=65;

	//muriendo
	player[0][6].x=0; player[0][6].y=390;
	player[0][6].w=40; player[0][6].h=65;

	player[1][6].x=40; player[1][6].y=390;
	player[1][6].w=40; player[1][6].h=65;

	player[2][6].x=80; player[2][6].y=390;
	player[2][6].w=40; player[2][6].h=65;

	player[3][6].x=120; player[3][6].y=390;
	player[3][6].w=40; player[3][6].h=65;

	player[4][6].x=160; player[4][6].y=390;
	player[4][6].w=40; player[4][6].h=65;

	player[5][6].x=200; player[5][6].y=390;
	player[5][6].w=40; player[5][6].h=65;

	player[6][6].x=240; player[6][6].y=390;
	player[6][6].w=40; player[6][6].h=65;

	player[7][6].x=410; player[7][6].y=200;
	player[7][6].w=40; player[7][6].h=65;

	//cancheriando
	player[0][9].x=0; player[0][9].y=520;
	player[0][9].w=40; player[0][9].h=65;

}

void LTexture::free()
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
