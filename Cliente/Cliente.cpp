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
		//arch.open("lorem.txt");
		if(arch.is_open()){
			error = false;
		}
	}
	cout << "El archivo " << archivo << " fue abierto correctamente" << endl;
	int cantEnvios = 0;
	while (cantEnvios < cantMax){
		//Enviar
		int i = 0;
		int tamanioRnd;
		tamanioRnd = 1 + rand() % (256);
		char acumulador[tamanioRnd];
		char c;
		cout<<tamanioRnd;
		for(i=0; i<tamanioRnd; i++){
			c = arch.get();
			if(c == EOF){
				arch.clear();
				arch.seekg(0, arch.beg);
			} else {
				acumulador[i] = c;
			}
		}

		for(int j = 0; j < tamanioRnd; j++){
			cout << acumulador[j];
		}
		cout << endl;
		cout << "Acumulador: " << acumulador<< endl;

		int usuarioRnd;
		//variable = limite_inferior + rand() % (limite_superior +1 - limite_inferior) ;
		//usuarioRnd = 0 + rand() % (5);
		usuarioRnd = 0 + rand() % (cliente->cantidadUsuariosDisponibles()); // +1?
		cout << "Usuario: " << usuarioRnd << endl;
		cout << "Tamanio del mensaje: " << tamanioRnd << endl;
		cliente->enviarMensaje(usuarioRnd, acumulador, tamanioRnd);
		cantEnvios++;
		sleep(frecEnvio);
	}
	arch.close();
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
	int opc = 0;
	do{
		cout << "##### Menu #####" << endl;
		if (!conectado){
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
		case 1:
			if (!conectado){
				cout << "Conectando al servidor..." << endl;
				conectado = Cliente.conectar();
			} else {
				cout << "Desconectando del servidor..." << endl;
				//Cliente.enviarMensaje("/salir");
				Cliente.desconectar();
				Cliente.cerrar();
				conectado = false;
			}
			break;

			case 0:
				cout << "Cerrando el programa..." << endl;
				return 0;
				break;

			case 2:
				cout << "Enviando mensajes..." << endl;
				Cliente.enviarMensaje();
				break;

			case 3:
				cout << "Recibiendo Datos..." << endl;
				//Falta implementar
				break;

			case 4:
				cout<< "Lorem Ipsum..." << endl;
				int frecEnvio, cantMax;
				cout << "Ingrese la frecuencua de envío de mensajes (En segundos)" << endl;
				cin >> frecEnvio;
				cout << "Ingrese la cantMax de mensajes a enviar" << endl;
				cin >> cantMax;
				cout << "Frec. envio: " << frecEnvio << endl;
				cout << "Cant. Max " << cantMax << endl;
				loremIpsum(frecEnvio, cantMax, &Cliente);
				break;

			default:
				cout << "Opcion incorrecta. Intente de nuevo" << endl;
				break;
		}
	} while (opc != 0);
}
