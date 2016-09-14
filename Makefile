CXX = g++
CXXFLAGS = -lpthread -pthread

LIBRERIAS = socket.cpp
OTRAS = usuarioClass.cpp
OTRAS1 = mensajeClass.cpp

CLIENTE = Cliente
SERVIDOR = Servidor

.PHONY: all clean cliente servidor

all: clean cliente servidor

cliente:
	$(CXX) $(CLIENTE)/$(CLIENTE).cpp $(CLIENTE)/$(LIBRERIAS) $(CXXFLAGS) -o $(CLIENTE)/cliente

servidor:
	$(CXX) $(SERVIDOR)/$(SERVIDOR).cpp $(SERVIDOR)/$(LIBRERIAS) $(SERVIDOR)/$(OTRAS) \
	$(SERVIDOR)/$(OTRAS1) $(CXXFLAGS) -o $(SERVIDOR)/servidor

clean:
	rm -f $(CLIENTE)/cliente $(SERVIDOR)/servidor