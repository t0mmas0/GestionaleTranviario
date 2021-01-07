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
private:
    bool principale = true;
protected:
    int Km;     //km della tratta a cui si trova la stazione
    std::string nome ;

    Semaforo semBinariStazionamento;
    Semaforo semBinariStazionamentoInverso;
    std::vector <Treno> binariStazionamento{2};
    std::vector <Treno> binariStazionamentoInverso{2};
    int i = 0;
    int is = 0;

    std::vector <Treno> deposito{5};
    std::vector <Treno> depositoInverso{5};


public:

    Stazione(int km, std::string nome);
    int getDistance();
    std::string getNome();
    bool isFreeStop(Treno *t);
    void PrenotaStazionamento(Treno *t);
    void PrenotaDeposito(Treno *t);
    void liberaBinarioStazionamento(Treno *t);
    void liberaDeposito(Treno *t);
    virtual bool isPrincipale();
    virtual bool isFreePass(Treno *t)=0;
    virtual void PrenotaTransito(Treno *t)=0;
    virtual void liberaBinarioTransito(Treno *t)=0;


};


#endif //GESTIONETRANVIARIA_STAZIONE_H
