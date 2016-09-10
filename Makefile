CXX = g++
CXXFLAGS = -lpthread -pthread

LIBRERIAS = socket.cpp
OTRAS = usuarioClass.cpp
OTRAS1 = mensajeClass.cpp

CLIENTE = Version\ Franco/Cliente
SERVIDOR = Version\ Franco/Servidor

.PHONY: all clean cliente servidor

all: clean cliente servidor

cliente:
	$(CXX) $(CLIENTE)/Cliente.cpp $(CLIENTE)/$(LIBRERIAS) $(CXXFLAGS) -o $(CLIENTE)/cliente

servidor:
	$(CXX) $(SERVIDOR)/Servidor.cpp $(SERVIDOR)/$(LIBRERIAS) $(SERVIDOR)/$(OTRAS) \
	$(SERVIDOR)/$(OTRAS1) $(CXXFLAGS) -o $(SERVIDOR)/servidor

clean:
	rm -f $(CLIENTE)/cliente $(SERVIDOR)/servidor