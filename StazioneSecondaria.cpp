//
// Created by Tommaso on 30/12/2020.
//

#include <iostream>
#include "StazioneSecondaria.h"
#include "algorithm"

bool StazioneSecondaria::richiestaTransito(Treno t) {
    if (i<2) {
        binarioTransito.push_back(t);
        std::cout << "Richiesta di transito accettata";
        return true;
    }
    std::cout << "Richiesta di transito rifiutata, binari occupati";
    return false;
}

StazioneSecondaria::StazioneSecondaria(int km, std::string nome) : Stazione(km,nome) {

}

void StazioneSecondaria::uscitaBinarioTransitoStazione(Treno t) {
    binarioOrdinario.erase(std::remove(binarioOrdinario.begin(), binarioOrdinario.end(), t), binarioOrdinario.end());
    std::cout << "Binario liberato correttamente";

}
