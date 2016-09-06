CXX = g++
CXXFLAGS = -lpthread -pthread

LIBRERIAS = socket.cpp

CLIENTE = Cliente
SERVIDOR = Servidor

.PHONY: all clean cliente servidor

all: clean cliente servidor

cliente:
	$(CXX) $(CLIENTE)/$(CLIENTE).cpp $(CLIENTE)/$(LIBRERIAS) $(CXXFLAGS) -o $(CLIENTE)/$(CLIENTE)

servidor:
	$(CXX) $(SERVIDOR)/$(SERVIDOR).cpp $(SERVIDOR)/$(LIBRERIAS) $(CXXFLAGS) -o $(SERVIDOR)/$(SERVIDOR)

clean:
	rm -f $(CLIENTE)/$(CLIENTE) $(SERVIDOR)/$(SERVIDOR)

