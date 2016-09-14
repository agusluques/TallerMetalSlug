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
    	cout << "Modo de uso: "<< argv[0]<< " ArchivoUsuarios.csv puerto" << endl;
       exit(0);
    }
    infile.close();
	mySocketSrv servidor(argv[1], argv[2]);
	servidor.bindear();
	servidor.escuchar();

	pthread_t threadControl;
	pthread_create(&threadControl, NULL , escuchar_clientes, (void*)&servidor);

	bool abierto = true;
	while(abierto){
		sleep(10);
		char opc;
		system("clear");

		cout << "0000      0000   00000000000000   00000000000000   00000000000000   0000          " << endl;
		cout << "00000    00000   00000000000000   00000000000000   00000000000000   0000          " << endl;
		cout << "000000  000000   0000                  0000        0000      0000   0000          " << endl;
		cout << "0000 0000 0000   0000                  0000        0000      0000   0000          " << endl;
		cout << "0000  00  0000   00000000000000        0000        0000      0000   0000          " << endl;
		cout << "0000      0000   00000000000000        0000        00000000000000   0000          " << endl;
		cout << "0000      0000   0000                  0000        00000000000000   0000          " << endl;
		cout << "0000      0000   0000                  0000        0000      0000   00000000000000" << endl;
		cout << "0000      0000   00000000000000        0000        0000      0000   00000000000000" << endl;
		cout << "0000      0000   00000000000000        0000        0000      0000   00000000000000" << endl;
		cout << endl;
		cout << endl;
		cout << "00000000000000  0000            0000      0000  00000000000000 ....._|______________________" << endl;
		cout << "00000000000000  0000            0000      0000  00000000000000 ..../ `--|||||||||||---------|" << endl;
		cout << "0000            0000            0000      0000  0000           .../_==o ____________________|" << endl;
		cout << "0000            0000            0000      0000  0000           .....),---.(_(__) /" << endl;
		cout << "00000000000000  0000            0000      0000  00000000000000 ....// () ),------" << endl;
		cout << "00000000000000  0000            0000      0000  00000000000000 ...//___//" << endl;
		cout << "          0000  0000            0000      0000  0000      0000 ../-----/ " << endl;
		cout << "          0000  0000            0000      0000  0000      0000 ./____ /" << endl;
		cout << "00000000000000  00000000000000  00000000000000  00000000000000" << endl;
		cout << "00000000000000  00000000000000  00000000000000  00000000000000" << endl;
		
		cout << "Si desea cerrar el servidor ingrese 'S'" << endl;
		cin >> opc;
		if (opc == 'S'){
			servidor.loggearInterno( " FIN DE ACTIVIDAD DEL SERVIDOR");
			abierto = false;
		}

	}

	servidor.cerrar();

	return 0; /* we never get here */
}
