#include "socket.hpp"

void loremIpsum(int frecEnvio, int cantMax, mySocket* cliente){
	ifstream arch;
	string archivo;
	bool error = true;
	while (error){
		cout<< "Ingrese en nombre del archivo de texto a utilizar" << endl;
		cin >> archivo;
		cout << "NOMBRE DEL ARCHIVO: " << archivo << endl;
		arch.open(archivo.c_str());
		if(arch.is_open()){
			error = false;
		}
	}
	cout << "El archivo " << archivo << " fue abierto correctamente" << endl;
	int cantEnvios = 0;
	int tamanioRnd;
	tamanioRnd = 1 + rand() % (200);
	cout << "Tamanio del mensaje: " << tamanioRnd << endl;
	while (cantEnvios < cantMax){
		int i;
		char acumulador[tamanioRnd + 1];
		char c;
		for(i=0; i<tamanioRnd; i++){
			c = arch.get();
			if(c == EOF){
				arch.clear();
				arch.seekg(0, arch.beg);
				acumulador[i] = ' ';
			} else {
				acumulador[i] = c;
			}
		}
		acumulador[tamanioRnd] = '\0';

		cout << "Acumulador: " << acumulador << endl;

		int usuarioRnd;
		//variable = limite_inferior + rand() % (limite_superior +1 - limite_inferior) ;
		usuarioRnd = 0 + rand() % (5);
		cout << "Usuario: " << usuarioRnd << endl;
		cliente->enviarMensaje(usuarioRnd, acumulador, (tamanioRnd+1)*(sizeof(char)));
		cantEnvios++;
		usleep(1000000/frecEnvio);
	}
	int tiempo = (1000000/frecEnvio);
	cout << "Se enviaron " << cantEnvios << " mensajes" << endl;
	cout << "Fecuencia: " << tiempo << endl;
	arch.close();
}

void* controlarConexion(void *arg){

    mySocket* cliente;
    cliente = (mySocket*) arg;

    while(cliente->conexion() == true){
    	sleep(10);
    	char codigo;
    	codigo = '0';
    	//cliente->enviarMensaje(&codigo, sizeof(char));
    	if(cliente->enviarMensaje(&codigo, sizeof(char))){
    		cliente->desconectar();
    		cout << "Se perdio la conexion con el servidor" << endl;
    	}
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
	bool conectado = false;
	char opc = '0';



	do{
		cout << "##### Menu #####" << endl;
		if (!Cliente.conexion()){
			cout << "1) Conectar" << endl;
			cout << "0) Salir" << endl;
		}
		else{
			cout << "1) Desconectar" << endl;
			cout << "2) Enviar" << endl;
			cout << "3) Recibir" << endl;
			cout << "4) Lorem Ipsum" << endl;
		}
		cin >> opc;
		cin.get();
		switch(opc){
				case '1':
					switch(Cliente.conexion()){
						case false:
							cout << "Conectando al servidor..." << endl;
						    Cliente.conectar();
						    cout << "Se ha conectado correctamente con el servidor" << endl;
						    conectado = true;
							pthread_t threadControl;
							pthread_create(&threadControl, NULL , controlarConexion, (void*)&Cliente);
						    break;

						case true:
							if(!Cliente.conexion()){
								break;
							}
							cout << "Desconectando del serrvidor..." << endl;
							Cliente.cerrar();
							Cliente.desconectar();
							break;	
					}	
					break;					

				case '0':
					cout << "Cerrando el programa..." << endl;
					return 0;
					break;

				case '2':
					if(!Cliente.conexion()){
						break;
					}
					cout << "Enviando mensajes..." << endl;
				    Cliente.enviarMensaje();
				    break;

				case '3':
				{
					if(!Cliente.conexion()){
						break;
					}
					cout << "Recibiendo Datos..." << endl;
					//Falta implementar
					int tam;
					Cliente.recibirMensaje();
					Cliente.recibirMensaje(&tam, sizeof(int));
					cout << "Tamanio del mensaje: " << tam << endl;
					//cha mensaje[(tam+1)];
					char mensaje[tam];
					Cliente.recibirMensaje(&mensaje, sizeof(char)*tam);
					//mensaje[tam] = '\n';
					cout << "Mensaje: " << mensaje << endl;
					break;
				}
				case '4':
				{
					if(!Cliente.conexion()){
						break;
					}
					cout<< "Lorem Ipsum..." << endl;
					int frecEnvio, cantMax;
					cout << "Ingrese la frecuencua de envío de mensajes (Mensajes/Segundo)" << endl;
					cin >> frecEnvio;
					cout << "Ingrese la cantMax de mensajes a enviar" << endl;
					cin >> cantMax;
					cout << "Frec. envio: " << frecEnvio << endl;
					cout << "Cant. Max " << cantMax << endl;
					loremIpsum(frecEnvio, cantMax, &Cliente);
					break;
				}
			}
	} while (opc != '0');
}
