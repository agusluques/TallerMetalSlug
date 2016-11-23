#ifndef BONUS_H_
#define BONUS_H_
#include <cstring>
#include <string>
using namespace std;
class Bonus{

private:
	int Id;
	int xPos;
	int yPos;
	int tipoBonus;

public:
	string sprite;
	Bonus(int xPos, int yPos, int tipoBonus, int Id);
	int getPosX();
	int getPosY();
	int getTipoBonus();
	int getId();
	void setId(int Id);
	void setPosX(int x);
	void setPosY(int y);
	void setTipoBonus(int tipo);
	bool estaVisible(int camaraX);
	~Bonus();
	
};

#endif /* BONUS_H_ */