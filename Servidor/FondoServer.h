#ifndef FONDOSERVER_H_
#define FONDOSERVER_H_

#include <string>

using namespace std;

class FondoServer {
public:
	int zindex;
	string spriteId;
	int ancho;

	FondoServer();
	void setAncho(int ancho);
	void setSpriteId(string idSprite);
	void setZindex(int z);
	virtual ~FondoServer();
};

#endif /* FONDOSERVER_H_ */
