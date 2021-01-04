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

	Treno();																	//Costruttore di default
	Treno(int id, std::list<Stazione>& Stazioni, std::vector<int>& Orari);		//Costruttore
	Treno(const Treno& treno);													//Costruttore di copia

	void muta();												//Muta automaticamento lo stato del treno
	void avanza();												//Fa avanzare il treno
	void aggiorna_fermata();									//Conta il tempo di fermata

	int get_id() const;			//Getters
	int get_velocità() const;
	int get_posizione() const;
	int get_ritardo() const;

	virtual void set_velocità(int v);

	Treno& operator=(const Treno& treno);						//Assegnamento di copia
	bool Treno::operator ==(const Treno& treno) const;			//Operatore di confronto

protected:
	int identificativo;
	int velocità;
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

class AltaVelocità : public Treno {
public:
	AltaVelocità(int id, std::list<Stazione>& Stazioni, std::vector<int>& Orari);
private:
	const int MAX_SPEED;
};

class SuperVelocità : public Treno {
public:
	SuperVelocità(int id, std::list<Stazione>& Stazioni, std::vector<int>& Orari);
private:
	const int MAX_SPEED;
};


#endif //GESTIONETRANVIARIA_TRENO_H
