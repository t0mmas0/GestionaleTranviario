//Simone Peraro 1216334
#include "Treno.h"
#include "Stazione.h"

Treno::Treno(int id, std::list<Stazione>& Stazioni, std::vector<int>& Orari)
	:identificativo{id}, posizione{ 0 }, ritardo{ 0 }, minuti_fermata{ 0 }, Stazioni{ Stazioni }, Orari{Orari} {
}

Treno::Treno(const Treno& treno)
:identificativo{ treno.identificativo }, posizione{ treno.posizione }, ritardo{ treno.ritardo }, minuti_fermata{ treno.minuti_fermata }, Stazioni{ treno.Stazioni }, Orari{ treno.Orari }{
}

Treno& Treno::operator=(const Treno& treno){
	identificativo = treno.identificativo;
	posizione = treno.posizione;
	ritardo = treno.ritardo;
	minuti_fermata = treno.minuti_fermata;
	Stazioni = treno.Stazioni;
	Orari = treno.Orari;
	return *this;
}

bool Treno::operator==(const Treno& treno) const{
	return identificativo == treno.identificativo;
}

Regionale::Regionale(int id, std::list<Stazione>& Stazioni, std::vector<int>& Orari)
	: Treno(id, Stazioni, Orari), MAX_SPEED{160} {
}

AltaVelocità::AltaVelocità(int id, std::list<Stazione>& Stazioni, std::vector<int>& Orari)
	: Treno(id, Stazioni, Orari), MAX_SPEED{240}{
}

SuperVelocità::SuperVelocità(int id, std::list<Stazione>& Stazioni, std::vector<int>& Orari)
	: Treno(id, Stazioni, Orari), MAX_SPEED{300}{
}
