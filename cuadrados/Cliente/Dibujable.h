#include <SDL2/SDL.h>
#include <string>

#ifndef DIBUJABLE_H_
#define DIBUJABLE_H_

using namespace std;

class Dibujable {
private:
	int id;
	SDL_Surface *sprite; //esta se la puede variar segun el movimiento
	SDL_Rect posicion;
public:
	Dibujable();
	virtual ~Dibujable();
	void moverX(int movimiento);
	void moverY(int movimiento);
	void setX(int movimiento);
	void setY(int movimiento);
	void setId(int id);
	int getId();
	void setSprite(string rutaImg);
	int posicionX();
	int posicionY();
	SDL_Rect* getPosicion();
	SDL_Surface* getSprite();
};

#endif /* DIBUJABLE_H_ */
