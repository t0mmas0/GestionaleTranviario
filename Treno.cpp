//Simone Peraro 1216334
#include "Treno.h"
#include "Stazione.h"

Treno::Treno(int id, const std::list<Stazione>& Stazioni, const std::vector<int>& Orari)
	:identificativo{id}, posizione{ 0 }, ritardo{ 0 }, minuti_fermata{ 0 }, Stazioni{ Stazioni }, Orari{Orari} {
}

bool Treno::operator==(const Treno& treno) const{
	return identificativo == treno.identificativo;
}

Regionale::Regionale(int id, const std::list<Stazione>& Stazioni, const std::vector<int>& Orari)
	: Treno(id, Stazioni, Orari), MAX_SPEED{160} {
}

AltaVelocità::AltaVelocità(int id, const std::list<Stazione>& Stazioni, const std::vector<int>& Orari)
	: Treno(id, Stazioni, Orari), MAX_SPEED{240}{
}

SuperVelocità::SuperVelocità(int id,  const std::list<Stazione>& Stazioni, const std::vector<int>& Orari)
	: Treno(id, Stazioni, Orari), MAX_SPEED{300}{
}
