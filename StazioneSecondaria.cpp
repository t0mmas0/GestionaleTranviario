//
// Created by Tommaso on 30/12/2020.
//

#include <iostream>
#include "StazioneSecondaria.h"
#include "algorithm"

StazioneSecondaria::StazioneSecondaria(int km, std::string nome) : Stazione(km,nome) {

}

bool StazioneSecondaria::isFreePass() {
    return semBinariTransito.getStatus();
}

void StazioneSecondaria::PrenotaTransito(Treno t) {
    std::cout<<"Accesso al Binario di Stazionamento del treno" << t.get_id() << "Alla Stazione" << this->nome;
    binariTransito.push_back(t);
    j++;
    if (j==2) {
        semBinariTransito.setRosso();
    }

}

void StazioneSecondaria::liberaBinarioTransito(Treno t) {
    binariTransito.erase(std::remove(binariTransito.begin(), binariTransito.end(), t), binariTransito.end());
    std::cout<<"il Treno N." <<t.get_id()<<"ha liberato il binario di transito";
    i--;
    semBinariTransito.setVerde();
}
@override
bool StazioneSecondaria::isPrincipale() {
 return false;
}





