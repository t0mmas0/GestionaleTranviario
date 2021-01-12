
/**
 * @author Tommaso Bertoldi 1218661
 * @brief Stazione della linea ferroviaria
 */
#include <iostream>
#include "StazioneSecondaria.h"
#include "Grafica.h"

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
        Grafica::accessoAlBinario(true,id,this->nome);
        binariTransitoInverso=id;
        semBinariTransitoInverso.setRosso();

    }else {
        Grafica::accessoAlBinario(true,id,this->nome);
        binariTransito=id;
        semBinariTransito.setRosso();
    }
}

void StazioneSecondaria::liberaBinarioTransito(int id, bool reverse) {
    if(reverse) {
        binariTransitoInverso = -1;
        semBinariTransitoInverso.setVerde();
    }else{
        binariTransito = -1;
        semBinariTransito.setVerde();
    }
    Grafica::liberaBinario(false, id,this->nome);

}







