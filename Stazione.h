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
#include <memory>
class Treno;
class Stazione {
private:
    bool principale = true;
protected:
    int Km;     //km della tratta a cui si trova la stazione
    std::string nome ;

    Semaforo semBinariStazionamento;
    Semaforo semBinariStazionamentoInverso;
    std::vector <std::shared_ptr<Treno>> binariStazionamento{2};
    std::vector <std::shared_ptr<Treno>> binariStazionamentoInverso{2};
    int i = 0;
    int is = 0;

    std::vector <std::shared_ptr<Treno>> deposito{5};
    std::vector <std::shared_ptr<Treno>> depositoInverso{5};


public:

    Stazione(int km, std::string nome);
    int getDistance();
    std::string getNome();
    bool isFreeStop(Treno *t);
    void PrenotaStazionamento(const std::shared_ptr<Treno> t);
    void PrenotaDeposito(const std::shared_ptr<Treno> t);
    void liberaBinarioStazionamento(const std::shared_ptr<Treno> t);
    void liberaDeposito(const std::shared_ptr<Treno> t);
    virtual bool isPrincipale();
    virtual bool isFreePass(const std::shared_ptr<Treno> t)=0;
    virtual void PrenotaTransito(const std::shared_ptr<Treno> t)=0;
    virtual void liberaBinarioTransito(const std::shared_ptr<Treno> t)=0;


};


#endif //GESTIONETRANVIARIA_STAZIONE_H
