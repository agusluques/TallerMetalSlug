CXX = g++
CXXFLAGS = -lpthread -pthread -lSDL2 -lSDL2_image -g

LIBRERIAS = socket.cpp
OTRAS = usuarioClass.cpp
OTRAS1 = mensajeClass.cpp
OTRAS2 = Dibujable.cpp
OTRAS3 = Grafica.cpp
OTRAS4 = DibujableServer.cpp
OTRAS5 = LTexture.cpp
OTRAS6 = FondoServer.cpp
OTRAS7 = Plataforma.cpp
OTRAS8 = Escenario.cpp
OTRAS9 = bala.cpp
OTRAS10 = TextureBalas.cpp
OTRAS11 = TextureBonus.cpp
OTRAS12 = Bonus.cpp

CLIENTE = Cliente
SERVIDOR = Servidor

.PHONY: all clean cliente servidor

all: clean cliente servidor

cliente:
	$(CXX) $(CLIENTE)/$(CLIENTE).cpp $(CLIENTE)/$(LIBRERIAS) $(CLIENTE)/$(OTRAS5) $(CLIENTE)/$(OTRAS3) $(CLIENTE)/$(OTRAS10) $(CLIENTE)/$(OTRAS11) $(CXXFLAGS) -o $(CLIENTE)/cliente

servidor:
	$(CXX) $(SERVIDOR)/$(SERVIDOR).cpp $(SERVIDOR)/$(LIBRERIAS) $(SERVIDOR)/$(OTRAS) \
	$(SERVIDOR)/$(OTRAS1) $(SERVIDOR)/$(OTRAS4) $(SERVIDOR)/$(OTRAS6) $(SERVIDOR)/$(OTRAS7) $(SERVIDOR)/$(OTRAS8) $(SERVIDOR)/$(OTRAS9) $(SERVIDOR)/$(OTRAS12) $(CXXFLAGS) -o $(SERVIDOR)/servidor

clean:
	rm -f $(CLIENTE)/cliente $(SERVIDOR)/servidor
