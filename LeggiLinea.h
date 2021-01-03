//autore Marco Nardi 1218629

#ifndef GESTIONETRANVIARIA_LEGGILINEA_H
#define GESTIONETRANVIARIA_LEGGILINEA_H

#include "StazioneSecondaria.h"
#include <list>
#include <string>
#include <fstream>
#include <stdexcept>
#include <memory>
class LeggiLinea
{
private:
	std::list<std::shared_ptr<Stazione>> Stazioni;
	std::string NomeFile;
	std::ifstream Stream;
public:
	LeggiLinea(const std::string& file);
	std::list<std::shared_ptr<Stazione>> LeggiStazioni();

};

#endif // !GESTIONETRANVIARIA_LEGGILINEA_H

