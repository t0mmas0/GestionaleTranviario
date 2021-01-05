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

class Stazione {
protected:
    int Km;     //km della tratta a cui si trova la stazione
    std::string nome ;
    Semaforo accessoStazione;
    Semaforo uscitaDeposito;
    std::vector <Treno> binarioOrdinario{4};
    std::vector <Treno> deposito{ 5 };
    int i = 0;
    void liberaDeposito(Treno t);


public:

    Stazione(int km, std::string nome);
    int getDistance();
    bool PrenotaBinario(Treno t);
    void liberaBinario(Treno t);
    bool UscitaDeposito(Treno t);




};


#endif //GESTIONETRANVIARIA_STAZIONE_H
