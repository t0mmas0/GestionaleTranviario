/**
 * @author Tommaso Bertoldi 1218661
 * @brief Stazione della linea ferroviaria
 */


#ifndef GESTIONETRANVIARIA_STAZIONE_H
#define GESTIONETRANVIARIA_STAZIONE_H

#include <string>
#include "Treno.h"
#include "vector"
#include "Semaforo.h"
class Treno;
class Stazione {
protected:
    int Km;     //km della tratta a cui si trova la stazione
    std::string nome ;

    Semaforo semBinariStazionamento;
    std::vector <Treno> binariStazionamento{4};
    int i = 0;

    std::vector <Treno> deposito{5};



public:

    Stazione(int km, std::string nome);
    int getDistance();
    std::string getNome();
    bool isFreeStop();
    void PrenotaStazionameto(Treno t);
    void PrenotaDeposito(Treno t);
    void liberaBinarioStazionamento(Treno t);
    void liberaDeposito(Treno t);




};


#endif //GESTIONETRANVIARIA_STAZIONE_H
