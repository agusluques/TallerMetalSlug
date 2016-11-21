#ifndef BALA_H_
#define BALA_H_

class bala{
	public:
		int id;
		int x;
		int y;
		float xVel;
		float yVel;
		int usr;
		int direccionDisparo;
		int tipoBala;

		bala(int xPos, int yPos, int usr, int id, int direccionDeDisparo, int tipoDeBala);
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

		void mover();
		bool estaVisible(int camaraX);
	
		~bala();
};

#endif /* BALA_H_ */
