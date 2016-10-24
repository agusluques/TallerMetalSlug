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

void LTexture::render(SDL_Renderer *window, SDL_Texture *mTexture, SDL_Rect *camera, int escala){
	SDL_Rect drawingRect;

	drawingRect.x = xcord - camera->x;
	if (drawingRect.x < 0) drawingRect.x = 0;
//	cout << "POS CAMARA: " << camera->x << endl;
//	cout << "POS EN VENTANA: " << drawingRect.x << endl;
    drawingRect.y = ycord;// - camera->y;

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
	/*cuerpo[0][0].x=8; cuerpo[0][0].y=443;
	cuerpo[0][0].w=35; cuerpo[0][0].h=50;

	cuerpo[1][0].x=57; cuerpo[1][0].y=441;
	cuerpo[1][0].w=35; cuerpo[1][0].h=50;

	cuerpo[2][0].x=98; cuerpo[2][0].y=441;
	cuerpo[2][0].w=35; cuerpo[2][0].h=50;

	cuerpo[3][0].x=131; cuerpo[3][0].y=441;
	cuerpo[3][0].w=35; cuerpo[3][0].h=50;

	cuerpo[4][0].x=163; cuerpo[4][0].y=441;
	cuerpo[4][0].w=35; cuerpo[4][0].h=50;

	cuerpo[5][0].x=200; cuerpo[5][0].y=441;
	cuerpo[5][0].w=35; cuerpo[5][0].h=50;*/

	//CORRIENDO
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

	// sprite gris cuando se cierra la ventana.. ver cuando se mata el cliente q onda..
	//momia
	player[1][1].x = 0; player[1][1].y = 100;
	player[1][1].w = 30; player[1][1].h = 40;
	//zombi
	player[2][1].x = 40; player[2][1].y = 100;
	player[2][1].w = 30; player[2][1].h = 40;

	//SALTANDO
	player[0][2].x=0; player[0][2].y=150;
	player[0][2].w=30; player[0][2].h=50;

	player[1][2].x=40; player[1][2].y=150;
	player[1][2].w=30; player[1][2].h=50;

	player[2][2].x=80; player[2][2].y=150;
	player[2][2].w=30; player[2][2].h=50;

	player[3][2].x=120; player[3][2].y=150;
	player[3][2].w=30; player[3][2].h=50;

	player[4][2].x=160; player[4][2].y=150;
	player[4][2].w=30; player[4][2].h=50;

	player[5][2].x=200; player[5][2].y=150;
	player[5][2].w=30; player[5][2].h=50;
}

void LTexture::free()
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


//void LTexture::render(SDL_Renderer* window, int x, int y, SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip flip )
//{
//	//Set rendering space and render to screen
//	SDL_Rect renderQuad = { x, y, mWidth, mHeight };
//
//	//Set clip rendering dimensions
//	if( clip != NULL )
//	{
//		renderQuad.w = clip->w;
//		renderQuad.h = clip->h;
//	}
//
//	//Render to screen
//	SDL_RenderCopyEx( window, mTexture, clip, &renderQuad, angle, center, flip );
//}
