
/**
 * @author Tommaso Bertoldi 1218661
 * @brief Stazione della linea ferroviaria
 */
#include <iostream>
#include "StazioneSecondaria.h"

StazioneSecondaria::StazioneSecondaria(int km, std::string nome) : Stazione(km,nome) {
principale = false;
}

bool StazioneSecondaria::isFreePass(int id, bool reverse) {
    if(reverse){
        return semBinariTransitoInverso.getStatus();
    }
    return semBinariTransito.getStatus();
}

void StazioneSecondaria::PrenotaTransito(int id, bool reverse) {
    if(reverse){
        std::cout << "Accesso al Binario di transito del treno" << id << " alla Stazione" << this->nome << std::endl;
        binariTransitoInverso=id;
        semBinariTransitoInverso.setRosso();

    }else {
        std::cout << "Accesso al Binario di transito del treno" << id << " alla Stazione" << this->nome << std::endl;
        binariTransito=id;
        semBinariTransito.setRosso();
    }
}

void StazioneSecondaria::liberaBinarioTransito(int id, bool reverse) {
    if(reverse) {
        binariTransitoInverso = -1;
        std::cout << "il Treno N." << id << "ha liberato il binario di transito";
        semBinariTransitoInverso.setVerde();
    }else{
        binariTransito = -1;
        std::cout << "il Treno N." << id << "ha liberato il binario di transito";
        semBinariTransito.setVerde();
    }
}







