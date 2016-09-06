#include "socket.hpp"
#include <fstream>

#define TAM_MAX 8192
using namespace std;

/*void error(const char *msg)
{
    perror(msg);
    exit(1);
}*/

/*
void *atender_cliente(void *arg) //FUNCION PROTOCOLO
{
	char buffer[TAM_MAX];
    int n;
    int newsockfd = (long)arg;
    bool abierto = true;

    while (abierto){
        //READ
        bzero(buffer,TAM_MAX);
        n = read(newsockfd,buffer,TAM_MAX-1);
        if (n < 0){
        	cout << "ERROR, no se pudo leer el mensaje" << endl;
        } else {
        	if(buffer[0] == '/'){
        		abierto = false;
        		cout << "ENTRO A LO QUE ESTOY PROBANDO!!!!" << endl;
        	}
            cout << "Mensaje:" << buffer << endl;
        }

        //WRITE
        char msg[256]="Mensaje recibido por el servidor\n";
        n = write(newsockfd,msg,strlen(msg));
        if (n < 0){
        	cout << "ERROR writing to socket" << endl;
			close(newsockfd);
			abierto = false;
        }
     }   
    //close(newsockfd);
    return NULL;
}*/

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

	servidor.aceptarClientes();

	pthread_t threadControl;
	pthread_create(&threadControl, NULL, escuchar_clientes, (void*)&servidor);

	/*bool abierto = true;
	while(abierto){
		sleep(10);
		char opc;
		system("clear");
		cout << "Si desea cerrar el servidor ingrese 'S'" << endl;
		cin >> opc;
		if (opc == 'S'){
			abierto = false;
		}

	}*/

	servidor.cerrar();

	return 0; /* we never get here */
}
