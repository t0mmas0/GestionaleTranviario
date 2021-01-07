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
    std::shared_ptr<Treno>  binariTransito;
    std::shared_ptr<Treno>  binariTransitoInverso;



public:
    StazioneSecondaria(int km, std::string nome);
    bool isFreePass(const std::shared_ptr<Treno> t) override;
    void PrenotaTransito(const std::shared_ptr<Treno> t) override;
    void liberaBinarioTransito(const std::shared_ptr<Treno> t) override;

};


#endif //GESTIONALETRANVIARIO_STAZIONESECONDARIA_H
