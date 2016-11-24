#include <iostream>
#include <cstring>

#ifndef SERVIDOR_DIBUJABLESERVERADICIONAL_H_
#define SERVIDOR_DIBUJABLESERVERADICIONAL_H_

using namespace std;

class DibujableServerAdicional {
public:

	int id;
	int aumentable;


	DibujableServerAdicional();
	void setId(int id);
	void setAumentable(int cantidad);
	void aumentar (int cantidad);

	int getId();
	int getAumentable();


	~DibujableServerAdicional();
};

#endif /* SERVIDOR_DIBUJABLESERVERADICIONAL_H_ */
