#include <iostream>
#include <conio2.h>
#include <windows.h>
using namespace std;
//pantalla de inicio
void pantallaInicio() {
	clrscr();
	cout << "============================\n";
	cout << "SPACE INVADERS LITE\n";
	cout << "German Cerda\n";
	cout << "============================\n";
	cout << "Controles:\n";
	cout << "A - Izquierda\n";
	cout << "D - Derecha\n";
	cout << "ESC - Salir\n\n";
	cout << "Presione una tecla para comenzar...";
	getch();
	clrscr();
}
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
protected:
	int resistencia;
public:
	Enemigo(char a, int x, int y, int r)
		: Objeto(x, y, a) {
		resistencia = r;
	}
	void recibirDisparo() {
		resistencia--;
		if (resistencia <= 0) {
			borrar();
		}
	}
	
	bool estaVivo() {
		return resistencia > 0;
	}
	
	int getX() { return posX; }
	int getY() { return posY; }
	
	void mover(int direccion) {
		borrar();
		posX += direccion;
		mostrar();
	}
	
	void bajar() {
		borrar();
		posY++;
		mostrar();
	}
};

int main(int argc, char *argv[]) {
	pantallaInicio();
	const int FILAS = 3;
	const int COLUMNAS = 5;
	NaveJugador jugador ('A', 3, 3, 40,22);
	jugador.mostrar();
	Enemigo enemigos[FILAS][COLUMNAS] = {
		{
		Enemigo('X', 20, 2, 1),
			Enemigo('X', 25, 2, 1),
			Enemigo('X', 30, 2, 1),
			Enemigo('X', 35, 2, 1),
			Enemigo('X', 40, 2, 1)
	},
	{
		Enemigo('M', 20, 4, 2),
			Enemigo('M', 25, 4, 2),
			Enemigo('M', 30, 4, 2),
			Enemigo('M', 35, 4, 2),
			Enemigo('M', 40, 4, 2)
	},
		{
			Enemigo('W', 20, 6, 3),
				Enemigo('W', 25, 6, 3),
				Enemigo('W', 30, 6, 3),
				Enemigo('W', 35, 6, 3),
				Enemigo('W', 40, 6, 3)
		}
	};
	// Mostrar enemigos al inicio
	for (int i = 0; i < FILAS; i++) {
		for (int j = 0; j < COLUMNAS; j++) {
			enemigos[i][j].mostrar();
		}
	}
	int direccion = 1;
	while (true){
		
		bool tocarBorde = false;
		// detectar borde
		for (int i = 0; i < FILAS; i++) {
			for (int j = 0; j < COLUMNAS; j++) {
				if (enemigos[i][j].estaVivo()) {
					if (enemigos[i][j].getX() >= 79 || enemigos[i][j].getX() <= 1)
						tocarBorde = true;
				}
			}
		}
		
		if (tocarBorde) {
			direccion *= -1; //cambia de direccion
			
			for (int i = 0; i < FILAS; i++) {
				for (int j = 0; j < COLUMNAS; j++) {
					enemigos[i][j].bajar(); //bajan una fila
					enemigos[i][j].mover(direccion); //salen del borde
				}
			}
		}
		else {
			for (int i = 0; i < FILAS; i++) {
				for (int j = 0; j < COLUMNAS; j++) {
					enemigos[i][j].mover(direccion);
				}
			}
		}
		if (_kbhit()){
			char tecla = getch ();
			if (tecla == 27) // ESC para salir
				break;
			jugador.mover (tecla);
			}
		
		Sleep(150); //velocidad del enemigo
	}
	return 0;
}

