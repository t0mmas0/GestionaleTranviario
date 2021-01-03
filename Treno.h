//Simone Peraro 1216334
//Interfaccia Treno e sottoclassi derivate

#include <list>
#include <vector>
#include "Stazione.h"

#ifndef GESTIONETRANVIARIA_TRENO_H
#define GESTIONETRANVIARIA_TRENO_H

class Treno {
public:
	//Costruttore
	Treno(const std::list<Stazione>& Stazioni, const std::vector<int>& Orari);
	
	//TO-DO: Disabilitare copia e move

	//Metodo chiamato per aggiornare lo stato interno al treno
	void Muta();
protected:
	int posizione;
	int ritardo;
	int minuti_fermata;

	std::list<Stazione>& Stazioni;

};

class Regionale : public Treno {
public:
	Regionale(std::list<Stazione> Stazioni, std::vector<int> Orari);
	void Muta();
};

class AltaVelocità : public Treno {
public:
	AltaVelocità(std::list<Stazione> Stazioni, std::vector<int> Orari);
	void Muta();
};

class SuperVelocità : public Treno {
public:
	SuperVelocità(std::list<Stazione> Stazioni, std::vector<int> Orari);
	void Muta();
};


#endif //GESTIONETRANVIARIA_TRENO_H
