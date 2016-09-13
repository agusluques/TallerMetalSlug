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
	int usuarioRnd;
	usuarioRnd = 0 + rand() % (cliente->cantidadUsuariosDisponibles());
	tamanioRnd = 1 + rand() % (200);
	cout << "Tamanio del mensaje: " << tamanioRnd << endl;
	while ((cantEnvios < cantMax) && (cliente->conexion() == true)){
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
    	sleep(1);
    	char codigo;
    	codigo = '0';
    	//cliente->enviarMensaje(&codigo, sizeof(char));
    	if((cliente->enviarMensaje(&codigo, sizeof(char)) == true) && (cliente->conexion() == true)){
		//cout << "entro a antes de desconectar " << endl;    		
		cliente->desconectar();
    		cout << "Se perdio la conexion con el servidor" << endl;
    	} else {
    		//cout << "Sigo conectado" << endl;
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
			cout << "2) Enviar" << endl;
			cout << "3) Recibir" << endl;
			cout << "4) Lorem Ipsum" << endl;
		}
		cin >> opc;
		cin.get();
		switch(opc){
				case '1':
					if(!Cliente.conexion()){
						cout << "Conectando al servidor..." << endl;
					    Cliente.conectar();
					    //cout << "Se ha conectado correctamente con el servidor" << endl;
    					    pthread_t threadControl;
					    pthread_create(&threadControl, NULL , controlarConexion, (void*)&Cliente);
					}else{
						cout << "Desconectando del serrvidor..." << endl;
						Cliente.cerrar();
						Cliente.desconectar();
							
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
					int tam = 1;
					
					int tam2;
					
					Cliente.recibirMensaje();
					while(tam != 0){
						Cliente.recibirMensaje(&tam, sizeof(int));
						char mensaje[tam];
						if (tam != 0){
							cout << "Tamanio del mensaje: " << tam << endl;
							Cliente.recibirMensaje(&mensaje, sizeof(char)*tam);
							cout << "Mensaje: " << mensaje << endl;
							Cliente.recibirMensaje(&tam2, sizeof(int));
							char usuario[tam2];
							Cliente.recibirMensaje(&usuario, sizeof(char)*tam2);
							cout << "De: "<<usuario<<endl;
						}
					}
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
