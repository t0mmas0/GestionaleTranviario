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
    std::cout<<"            ___________   _______________________________________^__\n";
    std::cout<<"             ___   ___ |||  ___   ___   ___    ___ ___  |   __  ,----\\ \n";
    std::cout<<"            |   | |   |||| |   | |   | |   |  |   |   | |  |  | |_____\\ \n";
    std::cout<<"            |___| |___|||| |___| |___| |___|  | O | O | |  |  |        \\ \n";
    std::cout<<"                       |||                    |___|___| |  |__|         ) \n";
    std::cout<<"            ___________|||______________________________|______________/ \n";
    std::cout<<"                       |||                                        /-------- \n";
    std::cout<<"            -----------'''---------------------------------------' \n";
    std::cout<<std::endl;
    std::cout<<"                                  _             _        \n";
    std::cout<<"                                 | |_ _ __ __ _(_)_ __  ___ \n";
    std::cout<<"                                 | __| '__/ _` | | '_ \\/ __|\n";
    std::cout<<"                                 | |_| | | (_| | | | | \\__ \\\n";
    std::cout<<"                                  \\__|_|  \\__,_|_|_| |_|___/\n";
    std::cout<<"                                       ____                                                     \n";
    std::cout<<"                                      |  _ \\                                                    \n";
    std::cout<<"                                      | |_) |_   _                                              \n";
    std::cout<<"                                      |  _ <| | | |                                             \n";
    std::cout<<"                                      | |_) | |_| |                                             \n";
    std::cout<<"            _______                   |____/ \\__, |          ____            _        _     _ _ \n";
    std::cout<<"           |__   __|                          __/ |         |  _ \\          | |      | |   | (_)\n";
    std::cout<<"              | | ___  _ __ ___  _ __ ___   _|___/__  ___   | |_) | ___ _ __| |_ ___ | | __| |_ \n";
    std::cout<<"              | |/ _ \\| '_ ` _ \\| '_ ` _ \\ / _` / __|/ _ \\  |  _ < / _ \\ '__| __/ _ \\| |/ _` | |\n";
    std::cout<<"              | | (_) | | | | | | | | | | | (_| \\__ \\ (_) | | |_) |  __/ |  | || (_) | | (_| | |\n";
    std::cout<<"            __|_|\\___/|_| |_| |_|_| |_| |_|\\__,_|___/\\___/  |____/ \\___|_|   \\__\\___/|_|\\__,_|_|\n";
    std::cout<<"            |  \\/  |                     | \\ | |             | (_)                               \n";
    std::cout<<"            | \\  / | __ _ _ __ ___ ___   |  \\| | __ _ _ __ __| |_                                \n";
    std::cout<<"            | |\\/| |/ _` | '__/ __/ _ \\  | . ` |/ _` | '__/ _` | |                               \n";
    std::cout<<"            | |  | | (_| | | | (_| (_) | | |\\  | (_| | | | (_| | |                               \n";
    std::cout<<"            |_|  |_|\\__,_|_|  \\___\\___/  |_| \\_|\\__,_|_|  \\__,_|_|                               \n";
    std::cout<<"              _____ _                              _____                                         \n";
    std::cout<<"             / ____(_)                            |  __ \\                                        \n";
    std::cout<<"            | (___  _ _ __ ___   ___  _ __   ___  | |__) |__ _ __ __ _ _ __ ___                  \n";
    std::cout<<"             \\___ \\| | '_ ` _ \\ / _ \\| '_ \\ / _ \\ |  ___/ _ \\ '__/ _` | '__/ _ \\                 \n";
    std::cout<<"             ____) | | | | | | | (_) | | | |  __/ | |  |  __/ | | (_| | | | (_) |                \n";
    std::cout<<"            |_____/|_|_| |_| |_|\\___/|_| |_|\\___| |_|   \\___|_|  \\__,_|_|  \\___/                 " << std::endl;
    std::cout<<"\n";
    std::cout<<"                                                 (@@@)     (@@@@@)\n";
    std::cout<<"                                           (@@)     (@@@@@@@)        (@@@@@@@)\n";
    std::cout<<"                                     (@@@@@@@)   (@@@@@)       (@@@@@@@@@@@)\n";
    std::cout<<"                                (@@@)     (@@@@@@@)   (@@@@@@)             (@@@)\n";
    std::cout<<"                           (@@@@@@)    (@@@@@@)                (@)\n";
    std::cout<<"                       (@@@)  (@@@@)           (@@)\n";
    std::cout<<"                    (@@)              (@@@)\n";
    std::cout<<"                   .-.               \n";
    std::cout<<"                   ] [    .-.      _    .-----.\n";
    std::cout<<"                 .\"   \"\"\"\"   \"\"\"\"\"\" \"\"\"\"| .--`\n";
    std::cout<<"                (:--:--:--:--:--:--:--:-| [___    .------------------------.\n";
    std::cout<<"                 |     :  :  :  :  :  : [_9_] |'='|.----------------------.|\n";
    std::cout<<"                /|.___________________________|___|'--.___.--.___.--.___.-'| \n";
    std::cout<<"               / ||_.--.______.--.______.--._ |---\\'--\\-.-/==\\-.-/==\\-.-/-'/--\n";
    std::cout<<"              /__;^=(==)======(==)======(==)=^~^^^ ^^^^(-)^^^^(-)^^^^(-)^^^    \n";
    std::cout<<"            ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"<<std::endl;


	while (true) {
		if (treniAttiviAndata.size() != 0) {
			if (treniAttiviAndata.size() == 1) {
				auto i = treniAttiviAndata.begin();
				if ((*i)->get_stato() == distrutto)
					treniAttiviAndata.erase(i);
			}
			else {
				for (std::list<std::shared_ptr<Treno>>::const_iterator i = --treniAttiviAndata.end(); (*i)->get_stato() == distrutto; i--) {
					treniAttiviAndata.erase(i);
					if (treniAttiviAndata.size() == 1)
						break;
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
				for (std::list<std::shared_ptr<Treno>>::const_iterator i = treniAttiviRitorno.begin(); (*i)->get_stato() == distrutto; i++) {
					treniAttiviRitorno.erase(i);
					if (treniAttiviRitorno.size() == 1)
						break;
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
		///darimuovere sennò se spacca tutto
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
			if ((*i)->get_stato() == movimento ||
				((*i)->get_stato() == attesa && (*j)->get_stato() == movimento || ((*j)->get_stato() == attesa))) {
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
			if ((*i)->get_stato() == movimento ||
				((*i)->get_stato() == attesa && (*j)->get_stato() == movimento || ((*j)->get_stato() == attesa))) {
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
			if ((*linea.begin())->isFreeStop((*i)->get_id(), (*i)->isReverse())) {
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
				if ((*linea.begin())->isFreePass((*i)->get_id(), (*i)->isReverse())) {
					(*i)->attiva(orario);
					treniAttiviRitorno.push_back(*i);
					i = treniRitorno.erase(i);
				}
				else {
					break;
				}
			}
			else {
				if ((*linea.begin())->isFreeStop((*i)->get_id(), (*i)->isReverse())) {
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
