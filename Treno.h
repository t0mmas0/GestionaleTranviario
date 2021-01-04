//Simone Peraro 1216334
//Interfaccia Treno e sottoclassi derivate

#include <list>
#include <vector>
#include <memory>
#include "Stazione.h"

#ifndef GESTIONETRANVIARIA_TRENO_H
#define GESTIONETRANVIARIA_TRENO_H

//Forward declaration
class Stazione;

//Enumeratore stato treno
enum Stato:int { attesa = 0, movimento = 1, parcheggio = 2, fermata = 3, attivato = 4, distrutto = 5};

class Treno {
public:

	Treno();																	//Costruttore di default
	Treno(int id, std::list<std::shared_ptr<Stazione>>& Stazioni, std::vector<int>& Orari);		//Costruttore
	Treno(const Treno& treno);													//Costruttore di copia

	// TODO: Copia e Move dovrebbero essere virtuali per gestire i tipi specifici?
	// TODO: Move

	void attiva(int ora);										//Attiva per la prima volta il treno sulla linea
	void muta();												//Muta automaticamento lo stato del treno
	void avanza();												//Fa avanzare il treno
	void aggiorna_fermata();									//Conta il tempo di fermata
	void cambia_stato(Stato s);									//Cambia lo stato del treno e imposta la velocità
	void calcola_ritardo();

	int get_id() const;				//Resituisce l'identificativo
	int get_velocità() const;		//Resituisce la velocità
	int get_posizione() const;		//Resituisce la posizione
	int get_ritardo() const;		//Resituisce il ritardo

	virtual void set_velocità(int v);	//Imposta la velocità

	Treno& operator=(const Treno& treno);						//Assegnamento di copia
	bool Treno::operator ==(const Treno& treno) const;			//Operatore di confronto

protected:
	int orario;
	int identificativo;
	int velocità;
	int ritardo;
	int minuti_fermata;

	double posizione;

	Stato stato;

	std::list<std::shared_ptr<Stazione>>& Stazioni;
	std::list<std::shared_ptr<Stazione>>::iterator iteratore_stazioni;

	std::vector<int>& Orari;
	int indice_orario;

	bool attivato;
};

class Regionale : public Treno {
public:
	Regionale(int id, std::list<std::shared_ptr<Stazione>>& Stazioni, std::vector<int>& Orari);
	void set_velocità(int v = MAX_SPEED) override;
private:
	static const int MAX_SPEED = 160;
};

class AltaVelocità : public Treno {
public:
	AltaVelocità(int id, std::list<std::shared_ptr<Stazione>>& Stazioni, std::vector<int>& Orari);
	void set_velocità(int v = MAX_SPEED) override;
private:
	static const int MAX_SPEED = 240;
};

class SuperVelocità : public Treno {
public:
	SuperVelocità(int id, std::list<std::shared_ptr<Stazione>>& Stazioni, std::vector<int>& Orari);
	void set_velocità(int v = MAX_SPEED);
private:
	static const int MAX_SPEED = 300;
};


#endif //GESTIONETRANVIARIA_TRENO_H
