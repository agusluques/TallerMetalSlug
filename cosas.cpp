// constructing maps
#include <iostream>
#include <map>
#include <vector>
#include <fstream>
#include <list>

using namespace std;

class Mensaje {
	private:
		string destinatarioM;
		string mensajeM;
		string autorM;
	public:
		void cargar(string destinatario, string mensaje, string autor) {
			this->destinatarioM = destinatario;
			this->mensajeM = mensaje;
			this->autorM = autor;
		}
		string destinatario(){
			return this->destinatarioM;
		}
		string mensaje(){
			return this->mensajeM;
		}
};

int main () {
	map<string,string> mapa;
	vector<string> usuarios;

	//INSERTAR EN EL MAPA LOS USUARIOS DESDE UN CSV
	ifstream file("usuarios.csv"); //aca va la ruta de argv

	while(!file.eof()) {
		string usr;
		string pass;

		getline(file,usr,';');
		getline(file,pass);

		if(file.eof())
			break;

		mapa.insert(std::pair<string,string>(usr,pass));
	}

	//VALIDAR USUARIO
	string passCliente = "contraseñaPepe"; //ESTO SE LO MANDA EL CLIENTE AL SERVIDOR
	string usrCliente = "pepe"; //ESTO SE LO MANDA EL CLIENTE AL SERVIDOR

	if ( mapa[usrCliente].compare(passCliente) == 0)
		cout << "Accede" << endl << endl;
	else cout << "No accede" << endl << endl;

	//IMPRIMIR MAPA
	for(map<string,string>::iterator it = mapa.begin(); it != mapa.end(); ++it) {
		cout << (*it).first << " : " << (*it).second << endl;
	}

	cout << endl;

	/*ESTO PUEDE SERVIR PARA QUE EL SERVIDOR LE DE LOS USUARIOS AL CLIENTE*/

	//VOLCAR USUARIOS DEL MAPA AL VECTOR
	for(map<string,string>::iterator it = mapa.begin(); it != mapa.end(); ++it) {
		usuarios.push_back(it->first);
	}

	//IMPRIMIR VECTOR
	for(vector<string>::iterator it = usuarios.begin(); it != usuarios.end(); ++it) {
	    cout << *it << endl;
	}

	cout << endl;

	///////////////////////////////////////////////////////////////////////////////////////////
	/*ESTA LISTA PUEDE SERVIR PARA ALAMACENAR LOS MAILS EN EL SERVIDOR*/
	list<Mensaje> lista;

	Mensaje msj;
	msj.cargar("pepe","que hace mono","juanito");
	lista.push_back(msj);
	msj.cargar("juanito","todo bien guachin!","pepe");
	lista.push_back(msj);

	//IMPRIMO MENSAJES DE LA LISTA
	for (list<Mensaje>::iterator it = lista.begin(); it != lista.end(); it++)
		cout << it->mensaje() << endl;

	return 0;
}
