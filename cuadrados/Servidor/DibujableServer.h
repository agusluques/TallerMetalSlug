#ifndef DIBUJABLESERVER_H_
#define DIBUJABLESERVER_H_

#include <string>

using namespace std;

class DibujableServer {
public:
	int id;
	string spriteId;
	int x;
	int y;

	DibujableServer();
	void setX(int pos);
	void setY(int pos);
	void setSpriteId(string idSprite);
	void setId(int id);
	virtual ~DibujableServer();
};

#endif /* DIBUJABLESERVER_H_ */
