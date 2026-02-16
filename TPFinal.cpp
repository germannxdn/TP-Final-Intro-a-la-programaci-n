#include <iostream>
#include <conio2.h>
#include <windows.h>
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
	
	virtual void mostrar() {
		putchxy(posX, posY, apariencia);
	}
	
	virtual void borrar() {
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

// clase para los enemigos
class Enemigo : public Objeto {
private:
	int direccion; // 1 = derecha, -1 = izquierda
	
public:
	Enemigo(char a, int x, int y)
		: Objeto(x, y, a) {
		direccion = 1; // empieza moviéndose a la derecha
	}
	
	void mover() {
		borrar();
		
		posX += direccion;
		
		// Si toca el borde derecho
		if (posX >= 79) {
			direccion = -1;
			posY++;
		}
		
		// Si toca el borde izquierdo
		if (posX <= 1) {
			direccion = 1;
			posY++;
		}
		
		mostrar();
	}
};

int main(int argc, char *argv[]) {
	NaveJugador jugador ('A', 3, 3, 10,10);
	jugador.mostrar();
	Enemigo enemigo('X', 30, 2);
	enemigo.mostrar();
	while (true){
		if (_kbhit()){
			char tecla = getch ();
			if (tecla == 27) // ESC para salir
				break;
			jugador.mover (tecla);
			}
		enemigo.mover();
		Sleep(50); //velocidad del enemigo
	}
	return 0;
}

