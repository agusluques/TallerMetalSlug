#include "usuarioClass.hpp" 
#include <iostream>
#include <cstring>

using namespace std;

usuarioClass::usuarioClass(){

}

usuarioClass::usuarioClass(char *usuario, char *contrasenia){
	strcpy(this->nombre,usuario);
	strcpy(this->pass,contrasenia);
	this->conectado = false;
}

bool usuarioClass::estaConectado(){
	return this->conectado;
}

void usuarioClass::desconectar(){
	this->conectado=false;
	loggear(" Se ha desconectado");

}

bool usuarioClass::validarUsuario(char *contrasenia, char *mensaje) {
	if (this->conectado){
		strcpy(mensaje,"Usuario esta conectado");
		// usuario intento de doble conexion, aqui iria otro posible logeo
		return false;
	}

	if (strcmp(this->pass,contrasenia) == 0){
		strcpy(mensaje,"Inicio de sesion exitoso");
		loggear(" Inicio sesion exitosamente");
		this->conectado = true;
		return true;
	}else{
		strcpy(mensaje,"Contraseña incorrecta");
		// si se quisiera controlar cantidad de veces para ingresar la contraseña aca iria otro log.
		return false;
	}
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
