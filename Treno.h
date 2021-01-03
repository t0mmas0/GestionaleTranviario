//Simone Peraro 1216334
//Interfaccia Treno e sottoclassi derivate

#include <list>
#include <vector>
#include "Stazione.h"

#ifndef GESTIONETRANVIARIA_TRENO_H
#define GESTIONETRANVIARIA_TRENO_H

//Forward declaration
class Stazione;

//Enumeratore stato treno
enum Stato:int { attesa = 0, movimento = 1, parcheggio = 2, fermata = 3};

class Treno {
public:

	//Costruttore di default
	Treno();

	//Costruttore
	Treno(int id, std::list<Stazione>& Stazioni, std::vector<int>& Orari);

	//Costruttore di copia
	Treno(const Treno& treno);

	//Funzione che muta automaticamente gli attributi del treno
	void muta();

	//Funzione che fa avanzare il treno
	void avanza();

	//Funzione che aggiorna il tempo di fermata
	void aggiorna_fermata();
	
	//TO-DO: Move?

	//Operatore di copia
	Treno& operator=(const Treno& treno);

	//Override operator ==
	bool Treno::operator ==(const Treno& treno) const;

protected:
	int identificativo;
	int velocit�;
	int posizione;
	int ritardo;
	int minuti_fermata;

	Stato stato;

	std::list<Stazione>& Stazioni;
	std::vector<int>& Orari;

};

class Regionale : public Treno {
public:
	Regionale(int id, std::list<Stazione>& Stazioni, std::vector<int>& Orari);
private:
	const int MAX_SPEED;
};

class AltaVelocit� : public Treno {
public:
	AltaVelocit�(int id, std::list<Stazione>& Stazioni, std::vector<int>& Orari);
private:
	const int MAX_SPEED;
};

class SuperVelocit� : public Treno {
public:
	SuperVelocit�(int id, std::list<Stazione>& Stazioni, std::vector<int>& Orari);
private:
	const int MAX_SPEED;
};


#endif //GESTIONETRANVIARIA_TRENO_H
