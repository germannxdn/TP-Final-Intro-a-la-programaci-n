#include <iostream>
#include <conio2.h>
#include <windows.h>
#include <ctime>
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
	int getVidas() { return vida; } // para mostrar en el HUD
	
	void perderVida() {
		if (vida > 0) vida--;
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
//funcion para mostrar el HUD
void dibujarHUD(int vidas, int puntos) {
	textcolor(WHITE);
	// Vidas a la izquierda
	gotoxy(2, 1); 
	cout << "VIDAS: " << vidas << "   "; 
	
	// Puntos al centro/derecha
	gotoxy(30, 1); 
	cout << "PUNTOS: " << puntos << "      ";
}
int main(int argc, char *argv[]) {
	srand(time(NULL)); // generador de números aleatorios
	Bala balaEnemigo;  // instancia de bala para los enemigos
	pantallaInicio();
	const int FILAS = 3;
	const int COLUMNAS = 5;
	NaveJugador jugador ('A', 3, 3, 40,22);
	jugador.mostrar();
	Bala bala;
	int puntos = 0;
	int enemigosVivos = FILAS * COLUMNAS; // Para detectar la victoria
	//matriz de punteros a la clase base enemigo
	Enemigo* enemigos[FILAS][COLUMNAS];
	// Fila superior (fuertes)
	for (int j = 0; j < COLUMNAS; j++) {
		enemigos[0][j] = new EnemigoFuerte(20 + j * 5, 4);
	}
	
	// Fila del medio
	for (int j = 0; j < COLUMNAS; j++) {
		enemigos[1][j] = new EnemigoMedio(20 + j * 5, 6);
	}
	
	// Fila inferior (débiles)
	for (int j = 0; j < COLUMNAS; j++) {
		enemigos[2][j] = new EnemigoDebil(20 + j * 5, 8);
	}
	// Mostrar enemigos al inicio
	for (int i = 0; i < FILAS; i++) {
		for (int j = 0; j < COLUMNAS; j++) {
			enemigos[i][j]->mostrar();
		}
	}
	
	int direccion = 1;
	int contadorFrames = 0;
	while (true){
		//Dibujar interfaz
		dibujarHUD(jugador.getVidas(), puntos);
		// colision bala enemiga contra jugador
		if (balaEnemigo.estaActiva()) {
			if (balaEnemigo.getX() == jugador.getX() && 
				balaEnemigo.getY() == jugador.getY()) {
				
				jugador.perderVida();   // Restamos 1 a la variable vida
				balaEnemigo.desactivar(); // Quitamos la bala de la pantalla
				//efecto visual cuando el jugador sufre daño
				textcolor(LIGHTRED);
				jugador.mostrar(); 
				Sleep(250); // Pausa para ver el efecto
				textcolor(WHITE);
				jugador.mostrar();
				
				// Termina el juego si las vidas llegan a cero
				if (jugador.getVidas() <= 0) {
					clrscr();
					cout << "GAME OVER - Te has quedado sin vidas." << endl;
					Sleep(2000);
					break; 
				}
			}
		}
		if (contadorFrames % 5 == 0) { //enemigos mas lentos
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
						puntos +=100; //sumar puntos con cada impacto
						if (!enemigos[i][j]->estaVivo()) {
							enemigosVivos--; // Descontar si el enemigo murió
						}
					}
				}
			}
		}
		// logica del disparo aleatorio enemigos
		if (!balaEnemigo.estaActiva()) { // solo disparan si no hay una bala enemiga en pantalla
			int filaAzar = rand() % FILAS;
			int colAzar = rand() % COLUMNAS;
			
			// probabilidad de disparo 3% en cada ciclo
			if (enemigos[filaAzar][colAzar]->estaVivo() && (rand() % 100 < 3)) {
				// dispara desde la posición del enemigo hacia abajo (dirección +1)
				balaEnemigo.disparar(enemigos[filaAzar][colAzar]->getX(), 
					enemigos[filaAzar][colAzar]->getY() + 1, 1);
			}
		}
		
		// Mover la bala enemiga de forma autónoma
		balaEnemigo.mover();
		//condicion de victoria
		if (enemigosVivos <= 0) {
			clrscr();
			textcolor(LIGHTGREEN);
			gotoxy(30, 12); cout << "¡VICTORIA! PUNTAJE FINAL: " << puntos;
			Sleep(3000);
			break;
		}
		contadorFrames++;
		Sleep(30); //velocidad
	}
	//liberar memoria
	for (int i = 0; i < FILAS; i++) {
		for (int j = 0; j < COLUMNAS; j++) {
			delete enemigos[i][j];
		}
	}
	return 0;
}

