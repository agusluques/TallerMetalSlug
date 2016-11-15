#ifndef BALA_H_
#define BALA_H_

class bala{

private:
	int id;
	int xPos;
	int yPos;
	float xVel;
	float yVel;
	int usr;
	bool direccion; //TRUE = derecha y FALSE = izquierda

public:
	bala(int xPos, int yPos, int usr, bool dir, int id);
	int getPosX();
	int getPosY();
	float getVelX();
	float getVelY();
	int getUsr();
	int getId();
	void setPosX(int x);
	void setPosY(int y);
	void setVelX(int vel);
	void setVelY(int vel);
	void setUsr(int usr);
	bool getDireccion();
	void setDireccion(bool dir);
	~bala();
	
};

#endif /* BALA_H_ */