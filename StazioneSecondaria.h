//
// Created by Tommaso on 30/12/2020.
//

#ifndef GESTIONALETRANVIARIO_STAZIONESECONDARIA_H
#define GESTIONALETRANVIARIO_STAZIONESECONDARIA_H

#include "Stazione.h"

class StazioneSecondaria:public Stazione {
private:
    Semaforo semBinariTransito;
    std::vector <Treno> binariTransito{2};
    int j=0;


public:
    StazioneSecondaria(int km, std::string nome);
    bool isFreePass();
    void PrenotaTransito(Treno t);
    void liberaBinarioTransito(Treno t);
};


#endif //GESTIONALETRANVIARIO_STAZIONESECONDARIA_H
