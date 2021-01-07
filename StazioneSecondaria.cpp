
/**
 * @author Tommaso Bertoldi 1218661
 * @brief Stazione della linea ferroviaria
 */
#include <iostream>
#include "StazioneSecondaria.h"
#include "algorithm"

StazioneSecondaria::StazioneSecondaria(int km, std::string nome) : Stazione(km,nome) {

}

bool StazioneSecondaria::isFreePass(const std::shared_ptr<Treno> t) {
    if(t->isReverse()){
        return semBinariTransitoInverso.getStatus();
    }
    return semBinariTransito.getStatus();
}

void StazioneSecondaria::PrenotaTransito(const std::shared_ptr<Treno> t) {
    if(t->isReverse()){
        std::cout<<"Accesso al Binario di Stazionamento del treno" << t->get_id() << "Alla Stazione" << this->nome;
        binariTransitoInverso=t;
        semBinariTransitoInverso.setRosso();

    }else {
        std::cout << "Accesso al Binario di Stazionamento del treno" << t->get_id() << "Alla Stazione" << this->nome;
        binariTransito=t;
        semBinariTransito.setRosso();
    }
}

void StazioneSecondaria::liberaBinarioTransito(const std::shared_ptr<Treno> t) {
    if(t->isReverse()) {
        binariTransitoInverso = nullptr;
        std::cout << "il Treno N." << t->get_id() << "ha liberato il binario di transito";
        semBinariTransitoInverso.setVerde();
    }else{
        binariTransito = nullptr;
        std::cout << "il Treno N." << t->get_id() << "ha liberato il binario di transito";
        semBinariTransito.setVerde();
    }
}

bool StazioneSecondaria::isPrincipale() {
 return false;
}





