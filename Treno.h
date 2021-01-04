//Simone Peraro 1216334
//Interfaccia Treno e sottoclassi derivate

#ifndef GESTIONETRANVIARIA_TRENO_H
#define GESTIONETRANVIARIA_TRENO_H

#include <list>
#include <vector>
#include <memory>

#include "Stazione.h"

//Forward declaration
class Stazione;

//Enumeratore stato treno
enum Stato:int { attesa = 0, movimento = 1, parcheggio = 2, fermata = 3, attivato = 4, distrutto = 5, stazione = 6};

class Treno {
public:

	Treno();																					//Costruttore di default
	Treno(int id, std::list<std::shared_ptr<Stazione>>& Stazioni, std::vector<int>& Orari, bool reverse = false);		//Costruttore
	Treno(const Treno& treno);																	//Costruttore di copia

	// TODO: Copia e Move dovrebbero essere virtuali per gestire i tipi specifici?
	// TODO: Move

	void attiva(int ora);										//Attiva per la prima volta il treno sulla linea
	void muta();												//Muta automaticamento lo stato del treno
	void avanza();												//Fa avanzare il treno
	void aggiorna_fermata();									//Conta il tempo di fermata
	void cambia_stato(Stato s);									//Cambia lo stato del treno e imposta la velocità
	void calcola_ritardo();										//Calcola il ritardo del treno in stazione
	void chiama_stazione();										//Chiama stazione

	int get_id() const;				//Resituisce l'identificativo
	int get_velocità() const;		//Resituisce la velocità
	int get_posizione() const;		//Resituisce la posizione
	int get_ritardo() const;		//Resituisce il ritardo

	virtual void set_velocità(int v = 0);	//Imposta la velocità. Poichè questa interfaccia non ha una velocità limite predefinita, essa viene posta a 0 di default

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
	bool reverse;

};

class Regionale : public Treno {
public:
	Regionale(int id, std::list<std::shared_ptr<Stazione>>& Stazioni, std::vector<int>& Orari, bool reverse = false);
	void set_velocità(int v = MAX_SPEED) override;
private:
	static const int MAX_SPEED = 160;
};

class AltaVelocità : public Treno {
public:
	AltaVelocità(int id, std::list<std::shared_ptr<Stazione>>& Stazioni, std::vector<int>& Orari, bool reverse = false);
	void set_velocità(int v = MAX_SPEED) override;
private:
	static const int MAX_SPEED = 240;
};

class SuperVelocità : public Treno {
public:
	SuperVelocità(int id, std::list<std::shared_ptr<Stazione>>& Stazioni, std::vector<int>& Orari, bool reverse = false);
	void set_velocità(int v = MAX_SPEED);
private:
	static const int MAX_SPEED = 300;
};


#endif //GESTIONETRANVIARIA_TRENO_H
