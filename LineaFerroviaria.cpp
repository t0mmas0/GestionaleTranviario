
#include <algorithm>
#include "LineaFerroviaria.h"
#include "LeggiLinea.h"
#include "LeggiOrari.h"
/**
 * @author Tommaso Bertoldi 1218661
 * @brief  linea ferroviaria
 */

LineaFerroviaria::LineaFerroviaria(std::string fileLinea,std::string fileOrari)
{
    LeggiLinea readerL {fileLinea};
    linea = readerL.leggiStazioni();
    LeggiOrari readerO {fileOrari};
    readerO.leggiFile(linea);
    treniAndata = readerO.getTreniDa();
    treniRitorno = readerO.getTreniPer();
    orario = 0;
}



void LineaFerroviaria::esegui() {
   while (true){
       for(std::list<std::shared_ptr<Treno>>::const_iterator i = --treniAttiviAndata.end(); (*i)->get_stato() == distrutto; i--){
           treniAttiviAndata.erase(i);
       }
       for(std::list<std::shared_ptr<Treno>>::const_iterator i = treniAttiviAndata.begin(); (*i)->get_stato() == distrutto; i++){
           treniAttiviRitorno.erase(i);
       }
       sort();
       controlloTamponamenti();
       gestioneMovimento();
       attivaTreni();

       ///incremento orario
       orario++;
       ///darimuovere sennò se spacca tutto
       break;
   }
}


void LineaFerroviaria::sort() {
    std::sort(treniAttiviAndata.begin(), treniAttiviAndata.end(), compara);
    std::sort(treniAttiviRitorno.begin(), treniAttiviRitorno.end(), comparaInversi);
}

bool  LineaFerroviaria::compara(std::_List_iterator<std::shared_ptr<Treno> > a, std::_List_iterator<std::shared_ptr<Treno> > b) {
    if ((*a)->get_posizione()==(*b)->get_posizione()){
        if((*a)->max_speed()>=(*b)->max_speed())
            return false;
        return true;
    }
    return (*a)->get_posizione()<(*b)->get_posizione();
}
bool  LineaFerroviaria::comparaInversi(std::_List_iterator<std::shared_ptr<Treno> >  a, std::_List_iterator<std::shared_ptr<Treno> >  b) {
    if ((*a)->get_posizione()==(*b)->get_posizione()){
        if((*a)->max_speed()<=(*b)->max_speed())
            return false;
        return true;
    }
    return (*a)->get_posizione()<(*b)->get_posizione();
}

void LineaFerroviaria::controlloTamponamenti() {
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
    for (std::list<std::shared_ptr<Treno>>::const_iterator i = treniAttiviAndata.begin();
         i != std::prev(--treniAttiviAndata.end()); i++) {
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

void LineaFerroviaria::attivaTreni() {
    for (std::list<std::shared_ptr<Treno>>::const_iterator i = treniAndata.begin(); i != treniAndata.end(); i++) {
        if ((*i)->get_orario() > orario)
            break;
        if ((*linea.begin())->isFreeStop((*i))) {
            (*i)->attiva(orario);
            treniAttiviAndata.emplace(treniAttiviAndata.begin(), (*i));
            treniAndata.erase(i);
        } else {
            break;
        }
    }
    for (std::list<std::shared_ptr<Treno>>::const_iterator i = treniRitorno.begin(); i != treniRitorno.end(); i++) {
        if ((*i)->get_orario() > orario)
            break;
        if ((*linea.end())->isPrincipale()) {
            if ((*linea.begin())->isFreeStop((*i))) {
                (*i)->attiva(orario);
                treniAttiviRitorno.push_back(*i);
                treniRitorno.erase(i);
            } else {
                break;
            }
        } else {
            if ((*i)->max_speed() > 160) {
                if ((*linea.begin())->isFreePass((*i))) {
                    (*i)->attiva(orario);
                    treniAttiviRitorno.push_back(*i);
                    treniRitorno.erase(i);
                } else {
                    break;
                }
            } else {
                if ((*linea.begin())->isFreeStop((*i))) {
                    (*i)->attiva(orario);
                    treniAttiviRitorno.push_back(*i);
                    treniRitorno.erase(i);
                } else {
                    break;
                }
            }
        }
    }
}

void LineaFerroviaria::gestioneMovimento() {
    for(std::list<std::shared_ptr<Treno>>::const_iterator i = --treniAttiviAndata.end(); --i !=treniAttiviAndata.begin(); i--){
        (*i)->esegui();
    }
    for(std::list<std::shared_ptr<Treno>>::const_iterator i = treniAttiviAndata.begin();i!=treniAttiviAndata.end(); i++){
        (*i)->esegui();
    }
}
