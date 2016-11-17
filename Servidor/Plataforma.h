/*
 * Plataforma.h
 *
 *  Created on: 2/11/2016
 *      Author: pablo
 */


#ifndef SERVIDOR_PLATAFORMA_H_
#define SERVIDOR_PLATAFORMA_H_



class Plataforma {


public:

	int id;
	int x;
	int y;
	int longitud;
	int suelo;


	Plataforma();
	int getId();
	int getX();
	int getY();
	int getLong();
	int getSuelo();
	void setSuelo(int cordY);
	void setId(int id);
	void setX(int x);
	void setY(int y);
	void setLong(int longitud);

	virtual ~Plataforma();
};

#endif /* SERVIDOR_PLATAFORMA_H_ */
