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
	int destinatario;
	int tipoDisparo;

public:
	bala(int xPos, int yPos, int usr, bool dir, int id, int destinatario, int tipoDisparo);
	int getPosX();
	int getPosY();
	float getVelX();
	float getVelY();
	int getUsr();
	int getId();
	int getDestinatario();
	int getTipoDisparo();
	void setTipoDisparo(int tipo);
	void setDestinatario(int usrDestino);
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