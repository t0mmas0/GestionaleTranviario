#include <iostream>
#include <algorithm>
#include "LineaFerroviaria.h"
#include "LeggiLinea.h"
#include "LeggiOrari.h"
/**
 * @author Tommaso Bertoldi 1218661
 * @brief  linea ferroviaria
 */

LineaFerroviaria::LineaFerroviaria(std::string fileOrari, std::string fileLinea)
{
	LeggiLinea readerL{ fileLinea };
	linea = readerL.leggiStazioni();
	LeggiOrari readerO{ fileOrari };
	readerO.leggiFile(linea);
	treniAndata = readerO.getTreniDa();
	treniRitorno = readerO.getTreniPer();
	orario = 0;
}



void LineaFerroviaria::esegui() {

	while (true) {
		if (treniAttiviAndata.size() != 0) {
			if (treniAttiviAndata.size() == 1) {
				auto i = treniAttiviAndata.begin();
				if ((*i)->get_stato() == distrutto)
					treniAttiviAndata.erase(i);
			}
			else {
				auto i = --treniAttiviAndata.end();
				while ((*i)->get_stato() == distrutto) {
					i = --treniAttiviAndata.erase(i);		//Restituisce list.end(), dobbiamo decrementare
					if (treniAttiviAndata.size() == 1) {
						if ((*i)->get_stato() == distrutto)
							treniAttiviAndata.erase(i);
						break;
					}
				}
			}
			
		}
		if (treniAttiviRitorno.size() != 0) {
			if (treniAttiviRitorno.size() == 1) {
				auto i = treniAttiviRitorno.begin();
				if ((*i)->get_stato() == distrutto)
					treniAttiviRitorno.erase(i);
			}
			else {
				auto i = treniAttiviRitorno.begin();
				while ((*i)->get_stato() == distrutto) {
					i = treniAttiviRitorno.erase(i);
					if (treniAttiviRitorno.size() == 1) {
						if ((*i)->get_stato() == distrutto)
							treniAttiviRitorno.erase(i);
						break;
					}
				}
			}
			
		}
		
		
		sort();
		controlloTamponamenti();
		gestioneMovimento();
		attivaTreni();

		///incremento orario
		orario++;
		std::cout << "Simulazione al minuto " << orario << std::endl;
		if ((treniAndata.size() == 0 && treniRitorno.size() == 0) && (treniAttiviAndata.size() == 0 && treniAttiviRitorno.size() == 0))
			break;
	}
}


bool compara(std::shared_ptr<Treno> a, std::shared_ptr<Treno> b) {
	if ((*a).get_posizione() == (*b).get_posizione()) {
		if ((*a).max_speed() >= (*b).max_speed())
			return false;
		return true;
	}
	return (*a).get_posizione() < (*b).get_posizione();
}

bool comparaInversi(std::shared_ptr<Treno> a, std::shared_ptr<Treno> b) {
	if ((*a).get_posizione() == (*b).get_posizione()) {
		if ((*a).max_speed() <= (*b).max_speed())
			return false;
		return true;
	}
	return (*a).get_posizione() < (*b).get_posizione();
}

void LineaFerroviaria::sort() {
	if (treniAttiviAndata.size() != 0) {
		treniAttiviAndata.sort(compara);
	}
	if (treniAttiviRitorno.size() != 0) {
		treniAttiviRitorno.sort(comparaInversi);
	}
	
}

void LineaFerroviaria::controlloTamponamenti() {
	if (treniAttiviAndata.size() > 1) {
		for (std::list<std::shared_ptr<Treno>>::const_iterator i = --treniAttiviAndata.end();
			i != std::next(treniAttiviAndata.begin()); i--) {
			std::list<std::shared_ptr<Treno>>::const_iterator j = std::prev(i);
			if ( ((*i)->get_stato() == movimento || (*i)->get_stato() == attesa) && ((*j)->get_stato() == movimento || (*j)->get_stato() == attesa)) {
				double front = (*i)->get_posizione() + (*i)->get_velocita() / 60;
				double back = (*j)->get_posizione() + (*j)->get_velocita() / 60;
				if (front - back < 10) {
					(*j)->set_velocita((*i)->get_velocita());
				}
			}
		}
	}
	if (treniAttiviRitorno.size() > 1) {
		for (std::list<std::shared_ptr<Treno>>::const_iterator i = treniAttiviRitorno.begin(); i != std::prev(--treniAttiviRitorno.end()); i++) {
			std::list<std::shared_ptr<Treno>>::const_iterator j = std::next(i);
			if (((*i)->get_stato() == movimento || (*i)->get_stato() == attesa) && ((*j)->get_stato() == movimento || (*j)->get_stato() == attesa)) {
				double back = (*i)->get_posizione() + (*i)->get_velocita() / 60;
				double front = (*j)->get_posizione() + (*j)->get_velocita() / 60;
				if (front - back < 10) {
					(*j)->set_velocita((*i)->get_velocita());
				}
			}
		}
	}
}

void LineaFerroviaria::attivaTreni() {
	std::list<std::shared_ptr<Treno>>::const_iterator i = treniAndata.begin();
	while (i != treniAndata.end()) {
		if ((*i)->get_orario() > orario)
			break;
		if ((*linea.begin())->isFreeStop((*i)->get_id(), (*i)->isReverse())) {
			(*i)->attiva(orario);
			//treniAttiviAndata.emplace(treniAttiviAndata.begin(), (*i));
			treniAttiviAndata.push_front((*i));
			//mi faccio restituire il puntatore al prossimo elemento, non serve incrementare il puntatore 
			i = treniAndata.erase(i);
		}
		else {
			break;
		}
	}


	i = treniRitorno.begin();
	while (i != treniRitorno.end()) {
		if ((*i)->get_orario() > orario)
			break;
		if ((*linea.rbegin())->isPrincipale()) {
			if ((*linea.rbegin())->isFreeStop((*i)->get_id(), (*i)->isReverse())) {
				(*i)->attiva(orario);
				treniAttiviRitorno.push_back(*i);
				i = treniRitorno.erase(i);
			}
			else {
				break;
			}
		}
		else {
			if ((*i)->max_speed() > 160) {
				if ((*linea.rbegin())->isFreePass((*i)->get_id(), (*i)->isReverse())) {
					(*i)->attiva(orario);
					treniAttiviRitorno.push_back(*i);
					i = treniRitorno.erase(i);
				}
				else {
					break;
				}
			}
			else {
				if ((*linea.rbegin())->isFreeStop((*i)->get_id(), (*i)->isReverse())) {
					(*i)->attiva(orario);
					treniAttiviRitorno.push_back(*i);
					i = treniRitorno.erase(i);
				}
				else {
					break;
				}
			}
		}
	}
}

void LineaFerroviaria::gestioneMovimento() {
	if (treniAttiviAndata.size() != 0) {
		for (std::list<std::shared_ptr<Treno>>::const_reverse_iterator i = treniAttiviAndata.rbegin(); i != treniAttiviAndata.rend(); i++) {
			(*i)->esegui();
		}
	}
	if (treniAttiviRitorno.size() != 0) {
		for (std::list<std::shared_ptr<Treno>>::const_iterator i = treniAttiviRitorno.begin(); i != treniAttiviRitorno.end(); i++) {
			(*i)->esegui();
		}
	}
}
