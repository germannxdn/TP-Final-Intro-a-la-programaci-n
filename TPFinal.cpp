#include <iostream>
#include <conio2.h>
using namespace std;

//Clase para base para todo dibujo que se mueve//
class Objeto {
protected:
	int posX;
	int posY;
	char apariencia;
	
public:
	Objeto(int x, int y, char a) {
		posX = x;
		posY = y;
		apariencia = a;
	}
	
	void mostrar() {
		putchxy(posX, posY, apariencia);
	}
	
	void borrar() {
		putchxy(posX, posY, ' ');
	}
};

//clase para la nave del jugador//
class NaveJugador : public Objeto {
private:
	int vida;
	int defensa;
	
public:
	NaveJugador(char a, int v, int d, int x, int y)
		: Objeto(x, y, a) {
		vida = v;
		defensa = d;
	}
	
	void mover(char tecla) {
		borrar();
		
		if (tecla == 'a' && posX > 1)
			posX--;
		
		if (tecla == 'd' && posX < 79)
			posX++;
		
		mostrar();
	}
};

int main(int argc, char *argv[]) {
	NaveJugador jugador ('A', 3, 3, 10,10);
	jugador.mostrar();
	while (true){
		if (_kbhit()){
			char tecla = getch ();
			jugador.mover (tecla);
			}
	}
	return 0;
}

