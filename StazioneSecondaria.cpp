
/**
 * @author Tommaso Bertoldi 1218661
 * @brief Stazione della linea ferroviaria
 */
#include <iostream>
#include "StazioneSecondaria.h"
#include "algorithm"

StazioneSecondaria::StazioneSecondaria(int km, std::string nome) : Stazione(km,nome) {

}

bool StazioneSecondaria::isFreePass(Treno *t) {
    if(t->isReverse()){
        return semBinariTransitoInverso.getStatus();
    }
    return semBinariTransito.getStatus();
}

void StazioneSecondaria::PrenotaTransito(Treno *t) {
    if(t->isReverse()){
        std::cout<<"Accesso al Binario di Stazionamento del treno" << t->get_id() << "Alla Stazione" << this->nome;
        binariTransitoInverso.push_back(*t);
        js++;
        semBinariTransitoInverso.setRosso();

    }else {
        std::cout << "Accesso al Binario di Stazionamento del treno" << t->get_id() << "Alla Stazione" << this->nome;
        binariTransito.push_back(*t);
        j++;
        semBinariTransito.setRosso();
    }
}

void StazioneSecondaria::liberaBinarioTransito(Treno *t) {
    if(t->isReverse()) {
        binariTransitoInverso.pop_back();
        std::cout << "il Treno N." << t->get_id() << "ha liberato il binario di transito";
        is--;
        semBinariTransitoInverso.setVerde();
    }else{
        binariTransito.pop_back();
        std::cout << "il Treno N." << t->get_id() << "ha liberato il binario di transito";
        i--;
        semBinariTransito.setVerde();
    }
}

bool StazioneSecondaria::isPrincipale() {
 return false;
}





