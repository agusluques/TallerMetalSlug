#include "usuarioClass.hpp" 
#include <iostream>
using namespace std;


usuarioClass::usuarioClass(char * usuario, char * contraseña){
	this->nombre = usuario;
	this->pass = contraseña;
	this->conectado = false;

}