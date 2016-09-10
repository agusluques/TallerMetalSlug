#include "usuarioClass.hpp" 
#include <iostream>
#include <cstring>

using namespace std;

usuarioClass::usuarioClass(char *usuario, char *contrasenia){
	strcpy(this->nombre,usuario);
	strcpy(this->pass,contrasenia);
	this->conectado = false;
}

bool usuarioClass::validarUsuario(char *contrasenia, char *mensaje) {
	if (this->conectado){
		strcpy(mensaje,"Usuario esta conectado");
		return false;
	}

	if (strcmp(this->pass,contrasenia) == 0){
		strcpy(mensaje,"Inicio de sesion exitoso");
		this->conectado = true;
		return true;
	}else{
		strcpy(mensaje,"Contraseña incorrecta");
		return false;
	}
}

string usuarioClass::nombreUsuario(){
	return this->nombre;
}
