#include "FondoServer.h"

#include <cstring>

FondoServer::FondoServer() {

}

void FondoServer::setAncho(int ancho) {
	this->ancho = ancho;
}


void FondoServer::setSpriteId(string idSprite) {
	//strcpy(spriteId,idSprite.c_str());
	this->spriteId = idSprite;
}

void FondoServer::setZindex(int z) {
	this->zindex = z;
}

FondoServer::~FondoServer() {
	// TODO Auto-generated destructor stub
}

