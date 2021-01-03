//Simone Peraro 1216334
#include "Treno.h"
#include "Stazione.h"

Treno::Treno(const std::list<Stazione>& Stazioni, const std::vector<int>& Orari)
	:posizione{ 0 }, ritardo{ 0 }, minuti_fermata{ 0 }, Stazioni{ Stazioni }, Orari{Orari} {
}
