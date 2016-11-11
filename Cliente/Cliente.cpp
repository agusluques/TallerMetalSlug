#include "socket.hpp"
#include "Dibujable.h"
#include "Grafica.h"

void loggearCliente(string mensaje){
	ofstream archLog; //esta va en .hpp
	archLog.open("log.txt", std::fstream::app);
	char buffTime[20];
	struct tm *sTm;

	time_t now = time (0);
	sTm = gmtime (&now);
	strftime (buffTime, sizeof(buffTime), "%Y-%m-%d %H:%M:%S", sTm);

	archLog << buffTime << mensaje << endl;

	archLog.close();
}

void* controlarConexion(void *arg){

	mySocket* cliente;
	cliente = (mySocket*) arg;

	while(cliente->conexion() == true){
		/*sleep(1);
		char codigo;
		codigo = '0';
		//cliente->enviarMensaje(&codigo, sizeof(char));
		if((cliente->enviarMensaje(&codigo, sizeof(char)) == true) && (cliente->conexion() == true)){
			//cout << "entro a antes de desconectar " << endl;
			cliente->desconectar();
			cout << "Se perdio la conexion con el servidor" << endl;
			loggearCliente(" Se perdio la conexion con el servidor");
		}*/

		/*struct timeval tv;
		tv.tv_sec = 10;  // 10 Secs Timeout
		tv.tv_usec = 0;  // Not init'ing this can cause strange errors
		setsockopt(cliente->getFD(), SOL_SOCKET, SO_RCVTIMEO, (char *)&tv,sizeof(struct timeval));

		char cod = 'q';
		int data = recv(cliente->getFD(), &cod, sizeof(char), 0);
		if(data == -1 && (cliente->conexion() == true)){
			cout << "Se cayo la conexion!" << endl;
			cliente->desconectar();
			loggearCliente(" Se cayo la conexion!");
		}*/
		//cliente->recibirMensaje(&cod, sizeof(char));

	}
	return NULL;
}

int main(int argc, char *argv[])
{

	if (argc < 3) {
		cout << "Falta proveer datos al programa" << endl;
		cout << "Modo de uso: ./Cliente hostname/IP puerto" << endl;
		exit(0);
	}

	mySocket Cliente(argv[2], argv[1]);
	//bool conectado = false;
	char opc = '0';



	do{
		cout << "##### Menu #####" << endl;
		if (!Cliente.conexion()){
			cout << "1) Conectar" << endl;
			cout << "0) Salir" << endl;
		}
		else{
			cout << "1) Desconectar" << endl;
			cout << "5) Grafica" << endl;
		}
		cin >> opc;
		cin.get();
		switch(opc){
			case '1':
				if(!Cliente.conexion()){
					cout << "Conectando al servidor..." << endl;
					Cliente.conectar();
					//cout << "Se ha conectado correctamente con el servidor" << endl;
					//pthread_t threadControl;
					//pthread_create(&threadControl, NULL , controlarConexion, (void*)&Cliente);
				}else{
					cout << "Desconectando del servidor..." << endl;
					Cliente.cerrar();
					Cliente.desconectar();
				}
				break;

			case '0':
				cout << "Cerrando el programa..." << endl;
				return 0;
				break;

			case '5': {
				//repetir si sale mal ?..
				if(Cliente.iniciarGrafica()){
					Cliente.cerrarGrafica();
					Cliente.cerrar();
					Cliente.desconectar();
				}

				break;
			}
		}
	} while (opc != '0');
}
