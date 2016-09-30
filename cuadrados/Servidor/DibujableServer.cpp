#include "DibujableServer.h"

#include <cstring>

DibujableServer::DibujableServer() {

}

void DibujableServer::setX(int pos) {
	x = pos;
}

void DibujableServer::setY(int pos) {
	y = pos;
}

void DibujableServer::setSpriteId(string idSprite) {
	//strcpy(spriteId,idSprite.c_str());
	spriteId = idSprite;
}

void DibujableServer::setId(int id) {
	this->id = id;
}

DibujableServer::~DibujableServer() {
	// TODO Auto-generated destructor stub
}

