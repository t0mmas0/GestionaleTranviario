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

	// TODO: Copia e Move dovrebbero essere virtuali per gestire i tipi specifici?
	// TODO: Move

	void muta();												//Muta automaticamento lo stato del treno
	void avanza();												//Fa avanzare il treno
	void aggiorna_fermata();									//Conta il tempo di fermata
	void cambia_stato(Stato s);									//Cambia lo stato del treno e imposta la velocità

	int get_id() const;				//Resituisce l'identificativo
	int get_velocità() const;		//Resituisce la velocità
	int get_posizione() const;		//Resituisce la posizione
	int get_ritardo() const;		//Resituisce il ritardo

	virtual void set_velocità(int v);	//Imposta la velocità

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
	void set_velocità(int v = MAX_SPEED) override;
private:
	static const int MAX_SPEED = 160;
};

class AltaVelocità : public Treno {
public:
	AltaVelocità(int id, std::list<Stazione>& Stazioni, std::vector<int>& Orari);
	void set_velocità(int v = MAX_SPEED) override;
private:
	static const int MAX_SPEED = 240;
};

class SuperVelocità : public Treno {
public:
	SuperVelocità(int id, std::list<Stazione>& Stazioni, std::vector<int>& Orari);
	void set_velocità(int v = MAX_SPEED);
private:
	static const int MAX_SPEED = 300;
};


#endif //GESTIONETRANVIARIA_TRENO_H
