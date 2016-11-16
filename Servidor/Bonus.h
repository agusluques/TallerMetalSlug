#ifndef BONUS_H_
#define BONUS_H_
#include <cstring>
#include <string>
using namespace std;
class Bonus{

private:
	int id;
	int xPos;
	int yPos;
	

public:
	string sprite;
	Bonus(int xPos, int yPos, int id, string sprite);
	int getPosX();
	int getPosY();
	int getId();
	void setPosX(int x);
	void setPosY(int y);
	~Bonus();
	
};

#endif /* BONUS_H_ */