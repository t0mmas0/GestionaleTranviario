//
// Created by Tommaso on 30/12/2020.
//

#ifndef GESTIONALETRANVIARIO_STAZIONESECONDARIA_H
#define GESTIONALETRANVIARIO_STAZIONESECONDARIA_H

#include "Stazione.h"

class StazioneSecondaria:public Stazione {
private:
    std::vector <Treno> binarioTransito{2};
    int j=0;
public:
    StazioneSecondaria(int km, std::string nome);
    bool richiestaTransito(Treno t);
    void uscitaBinarioTransitoStazione(Treno t);
};


#endif //GESTIONALETRANVIARIO_STAZIONESECONDARIA_H
