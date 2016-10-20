#include "usuarioClass.hpp" 
#include <iostream>
#include <cstring>

using namespace std;

usuarioClass::usuarioClass(char *usuario, int numCliente){
	strcpy(this->nombre,usuario);
	this->conectado = true;
	this->numeroCliente = numCliente;
}

bool usuarioClass::estaConectado(){
	return this->conectado;
}

void usuarioClass::conectar(){
	this->conectado = true;
	loggear(" se ha desconectado");
}

void usuarioClass::desconectar(){
	this->conectado=false;
	loggear(" se ha desconectado");
}

string usuarioClass::nombreUsuario(){
	return this->nombre;
}

void usuarioClass::loggear(string mensaje){
	ofstream archLog; //esta va en .hpp
	archLog.open("log.txt", std::fstream::app);
	char buffTime[20];
	struct tm *sTm;

	time_t now = time (0);
	sTm = gmtime (&now);
	strftime (buffTime, sizeof(buffTime), "%Y-%m-%d %H:%M:%S", sTm);

	archLog << buffTime << " " << nombreUsuario() << mensaje << endl;

	archLog.close();

}

usuarioClass::~usuarioClass(){

}
