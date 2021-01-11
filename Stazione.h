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

protected:
    bool principale;
    int Km;     //km della tratta a cui si trova la stazione
    std::string nome ;

    Semaforo semBinariStazionamento;
    Semaforo semBinariStazionamentoInverso;
    std::vector <int> binariStazionamento{2};
    std::vector <int> binariStazionamentoInverso{2};
    int i = 0;
    int is = 0;

    std::vector <int> deposito{5};
    std::vector <int> depositoInverso{5};


public:

    Stazione(int km, std::string nome);
    int getDistance();
    std::string getNome();
    bool isFreeStop(const std::shared_ptr<Treno> t);
    void PrenotaStazionamento(int id, bool reverse);
    void PrenotaDeposito(int id, bool reverse);
    void liberaBinarioStazionamento(int id, bool reverse);
    void liberaDeposito(int id, bool reverse);
    bool isPrincipale() const;
    virtual bool isFreePass(const std::shared_ptr<Treno> t)=0;
    virtual void PrenotaTransito(const std::shared_ptr<Treno> t)=0;
    virtual void liberaBinarioTransito(const std::shared_ptr<Treno> t)=0;


};


#endif //GESTIONETRANVIARIA_STAZIONE_H
