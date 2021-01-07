//
// Created by Tommaso on 30/12/2020.
//

#ifndef GESTIONALETRANVIARIO_STAZIONESECONDARIA_H
#define GESTIONALETRANVIARIO_STAZIONESECONDARIA_H

#include "Stazione.h"
/**
 * @author Tommaso Bertoldi 1218661
 * @brief Stazione della linea ferroviaria
 */
class StazioneSecondaria:public Stazione {
private:
    Semaforo semBinariTransito;
    Semaforo semBinariTransitoInverso;
    std::vector <Treno> binariTransito{1};
    std::vector <Treno> binariTransitoInverso{1};
    int j=0;
    int js=0;


public:
    StazioneSecondaria(int km, std::string nome);
    bool isFreePass(Treno *t) override;
    void PrenotaTransito(Treno *t) override;
    void liberaBinarioTransito(Treno *t) override;
    bool isPrincipale()override;
};


#endif //GESTIONALETRANVIARIO_STAZIONESECONDARIA_H
