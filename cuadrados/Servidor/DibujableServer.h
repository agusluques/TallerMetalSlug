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
	int spX;
	int spY;

	DibujableServer();
	void setX(int pos);
	void setY(int pos);
	void setSpX(int pos);
	void setSpY(int pos);
	void setSpriteId(string idSprite);
	void setId(int id);

	int getX(){
		return this->x;
	}
	int getY(){
		return this->y;
	}
	int getSpX(){
		return this->spX;
	}
	int getSpY(){
		return this->spY;
	}
	int getId(){
		return this->id;
	}

	virtual ~DibujableServer();
};

#endif /* DIBUJABLESERVER_H_ */
