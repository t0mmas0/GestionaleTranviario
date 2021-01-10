//autore Marco Nardi 1218629

#ifndef GESTIONETRANVIARIA_LEGGIORARI_H
#define GESTIONETRANVIARIA_LEGGIORARI_H

#include "Treno.h"
#include <list>
#include <string>
#include <fstream>
class LeggiOrari
{
	/*
	fix correggiorari reverse e altro
	ORDINARE TRENI
	COMMENTARE
	*/
private:
	std::list<std::shared_ptr<Treno>> TreniDa; //treni da 0 a fine
	std::list<std::shared_ptr<Treno>> TreniPer;//treni da fine a 0
	std::string NomeFile;
	std::ifstream Stream;
	void correggiOrari(const std::list<std::shared_ptr<Stazione>>& Stazioni, std::vector<int>& orari, int velocita, bool reverse, int tipoTreno);
	void sortTreni();
	int getVelocita(int tipoTreno);
	
public:
	std::list<std::shared_ptr<Treno>> getTreniDa();
	std::list<std::shared_ptr<Treno>> getTreniPer();
	LeggiOrari(const std::string& file);
	void leggiFile(const std::list<std::shared_ptr<Stazione>>& Stazioni);
};

#endif // !GESTIONETRANVIARIA_LEGGILINEA_H

