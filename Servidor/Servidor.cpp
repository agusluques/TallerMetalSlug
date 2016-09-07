#include "socket.hpp"
#include <fstream>

#define TAM_MAX 8192
using namespace std;

void *escuchar_clientes(void *arg){
    mySocketSrv* servidor;

    servidor = (mySocketSrv*) arg;
    servidor->aceptarClientes();

	return NULL;
}


int main(int argc, char *argv[])
{
    ifstream infile(argv[1]);

    if (argc < 3 || infile.good() == false) {
    	cout << "Falta proveer datos al programa" << endl;
    	cout << "Modo de uso: ./Servidor ArchivoUsuarios.csv puerto" << endl;
       exit(0);
    }

	mySocketSrv servidor(argv[1], argv[2]);
	servidor.bindear();
	servidor.escuchar();

	pthread_t threadControl;
	pthread_create(&threadControl, NULL, escuchar_clientes, (void*)&servidor);

	bool abierto = true;
	while(abierto){
		sleep(10);
		char opc;
		system("clear");
		cout << "Si desea cerrar el servidor ingrese 'S'" << endl;
		cin >> opc;
		if (opc == 'S'){
			abierto = false;
		}

	}

	servidor.cerrar();

	return 0; /* we never get here */
}
