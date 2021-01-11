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
    int  binariTransito;
    int  binariTransitoInverso;



public:
    StazioneSecondaria(int km, std::string nome);
    bool isFreePass(const std::shared_ptr<Treno> t) override;
    void PrenotaTransito(int id, bool reverse) override;
    void liberaBinarioTransito(int id, bool reverse) override;

};


#endif //GESTIONALETRANVIARIO_STAZIONESECONDARIA_H
