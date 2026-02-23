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
	//Destructor virtual base
	virtual ~Objeto() {} 
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
	int getX() { return posX; }
	int getY() { return posY; }
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
	int color; //color asignado de libreria conio2
	
public:
	Enemigo(int x, int y, char a, int r, int c)
		: Objeto(x, y, a) {
		resistencia = r;
		color = c;
	}
	
	virtual void mostrar() {
		if (!estaVivo()) return; // No dibuja si el enemigo fue destruido
		textcolor(color);        // Cambia el color del texto en consola
		putchxy(posX, posY, apariencia);
		textcolor(WHITE);        // Restaura el color a blanco para el resto del juego
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
		if (!estaVivo()) return;
		
		borrar();
		posX += direccion;
		mostrar();
	}
	
	void bajar() {
		if (!estaVivo()) return;
		
		borrar();
		posY++;
		mostrar();
	}
};

//clases derivadas de enemigo
// enemigo débil (1 vida, verde, al frente)
class EnemigoDebil : public Enemigo {
public:
	EnemigoDebil(int x, int y)
		: Enemigo(x, y, 'W', 1, GREEN) {
	}
};

// Enemigo Medio (2 vidas, azul, en medio)
class EnemigoMedio : public Enemigo {
public:
	EnemigoMedio(int x, int y)
		: Enemigo(x, y, 'M', 2, LIGHTBLUE) {
	}
};

// Enemigo Fuerte (3 vidas, rojo, por detras)
class EnemigoFuerte : public Enemigo {
public:
	EnemigoFuerte(int x, int y)
		: Enemigo(x, y, 'X', 3, RED) {
	}
};
//clase para las balas
class Bala : public Objeto {
private:
	bool activa;
	int direccion; // -1 = sube (jugador), +1 = baja (enemigo)
	
public:
	Bala() : Objeto(0,0,'|') {
		activa = false;
		direccion = -1;
	}
	
	void disparar(int x, int y, int dir) {
		if (!activa) {
			posX = x;
			posY = y;
			direccion = dir;
			activa = true;
		}
	}
	
	void mover() {
		if (activa) {
			borrar();
			posY += direccion;
			
			if (posY <= 1 || posY >= 24) {
				activa = false;
			} else {
				mostrar();
			}
		}
	}
	
	bool estaActiva() {
		return activa;
	}
	
	int getX() { return posX; }
	int getY() { return posY; }
	
	void desactivar() {
		borrar();
		activa = false;
	}
};

int main(int argc, char *argv[]) {
	pantallaInicio();
	const int FILAS = 3;
	const int COLUMNAS = 5;
	NaveJugador jugador ('A', 3, 3, 40,22);
	jugador.mostrar();
	Bala bala;
	//matriz de punteros a la clase base enemigo
	Enemigo* enemigos[FILAS][COLUMNAS];
	// Fila superior (fuertes)
	for (int j = 0; j < COLUMNAS; j++) {
		enemigos[0][j] = new EnemigoFuerte(20 + j * 5, 2);
	}
	
	// Fila del medio
	for (int j = 0; j < COLUMNAS; j++) {
		enemigos[1][j] = new EnemigoMedio(20 + j * 5, 4);
	}
	
	// Fila inferior (débiles)
	for (int j = 0; j < COLUMNAS; j++) {
		enemigos[2][j] = new EnemigoDebil(20 + j * 5, 6);
	}
	// Mostrar enemigos al inicio
	for (int i = 0; i < FILAS; i++) {
		for (int j = 0; j < COLUMNAS; j++) {
			enemigos[i][j]->mostrar();
		}
	}
	
	int direccion = 1;
	while (true){
		
		bool tocarBorde = false;
		// detectar borde
		for (int i = 0; i < FILAS; i++) {
			for (int j = 0; j < COLUMNAS; j++) {
				if (enemigos[i][j]->estaVivo()) {
					if (enemigos[i][j]->getX() >= 79 || enemigos[i][j]->getX() <= 1)
						tocarBorde = true;
				}
			}
		}
		
		if (tocarBorde) {
			direccion *= -1; //cambia de direccion
			
			for (int i = 0; i < FILAS; i++) {
				for (int j = 0; j < COLUMNAS; j++) {
					enemigos[i][j]->bajar(); //bajan una fila
					enemigos[i][j]->mover(direccion); //salen del borde
				}
			}
		}
		else {
			for (int i = 0; i < FILAS; i++) {
				for (int j = 0; j < COLUMNAS; j++) {
					enemigos[i][j]->mover(direccion);
				}
			}
		}
		
		if (_kbhit()){
			char tecla = getch ();
			if (tecla == 27) // ESC para salir
				break;
			if (tecla == ' ') {
				// Dispara desde la posición actual de la nave hacia arriba (-1)
				// Se resta 1 a la posición Y para que la bala aparezca justo arriba de la nave.
				bala.disparar(jugador.getX(), jugador.getY() - 1, -1);
			} else {
				// Si no es espacio, se pasa la tecla al método mover para manejar A o D
				jugador.mover(tecla);
			}
		}
		
		// la bala debe moverse sola en cada ciclo
		bala.mover();
		
		// logica de colisión (Jugador -> Enemigo)
		if (bala.estaActiva()) { // Solo verificamos colisiones si hay una bala en pantalla
			for (int i = 0; i < FILAS; i++) {
				for (int j = 0; j < COLUMNAS; j++) {
					// Verificamos si el enemigo está vivo y si sus coordenadas coinciden con la bala
					if (enemigos[i][j]->estaVivo() && 
						bala.getX() == enemigos[i][j]->getX() && 
							bala.getY() == enemigos[i][j]->getY()) {
						
						// Si hay impacto: el enemigo reduce su resistencia y la bala se desactiva
						enemigos[i][j]->recibirDisparo();
						bala.desactivar();
					}
				}
			}
		}
		Sleep(90); //velocidad del enemigo
	}
	//liberar memoria
	for (int i = 0; i < FILAS; i++) {
		for (int j = 0; j < COLUMNAS; j++) {
			delete enemigos[i][j];
		}
	}
	return 0;
}

