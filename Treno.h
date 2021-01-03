//Simone Peraro 1216334
//Interfaccia Treno e sottoclassi derivate

#include <list>
#include <vector>
#include "Stazione.h"

#ifndef GESTIONETRANVIARIA_TRENO_H
#define GESTIONETRANVIARIA_TRENO_H

class Stazione;

class Treno {
public:
	//Costruttore
	Treno(int id, const std::list<Stazione>& Stazioni, const std::vector<int>& Orari);
	
	//TO-DO: Abilitare copia e move

	//TO-Do: Override ==
	bool Treno::operator ==(const Treno& treno) const;

	//Metodo chiamato per aggiornare lo stato interno al treno
	void Muta();
protected:
	int identificativo;
	int posizione;
	int ritardo;
	int minuti_fermata;

	const std::list<Stazione>& Stazioni;
	const std::vector<int>& Orari;

};

class Regionale : public Treno {
public:
	Regionale(int id, const std::list<Stazione>& Stazioni,const std::vector<int>& Orari);
	void Muta();
private:
	const int MAX_SPEED;
};

class AltaVelocità : public Treno {
public:
	AltaVelocità(int id, const std::list<Stazione>& Stazioni, const std::vector<int>& Orari);
	void Muta();
private:
	const int MAX_SPEED;
};

class SuperVelocità : public Treno {
public:
	SuperVelocità(int id, const std::list<Stazione>& Stazioni, const std::vector<int>& Orari);
	void Muta();
private:
	const int MAX_SPEED;
};


#endif //GESTIONETRANVIARIA_TRENO_H
