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
    std::string nome;


    Semaforo semBinariStazionamento;
    Semaforo semBinariStazionamentoInverso;
    Semaforo uscita;
    Semaforo uscitaInversa;
    std::vector<int> binariStazionamento;
    std::vector<int> binariStazionamentoInverso;
    int i = 0;
    int is = 0;
    int binarioPostStazione;
    int binarioPostStazioneInverso;
    std::vector<int> deposito{ 5 };
    std::vector<int> depositoInverso{ 5 };


public:

    Stazione(int km, std::string nome);
    ///Restituisce a che distanza si trova la stazione sulla linea
    int getDistance();
    ///Restituisce il nome della stazione
    std::string getNome();
    ///indica se ci si puo farmare in stazione
    bool isFreeStop(int id, bool reverse);
    ///Prenota un binario di stazionemento
    void PrenotaStazionamento(int id, bool reverse);
    ///Prenota un binario nel deposito
    void PrenotaDeposito(int id, bool reverse);
    ///libera un binario di stazionamento
    void liberaBinarioStazionamento(int id, bool reverse);
    ///libera un binario del deposito
    void liberaDeposito(int id, bool reverse);
    ///restituisce se la stazione è principale
    bool isPrincipale() const;
    ///restituisce se si può uscire nel binario unico della linea
    bool isFreeUscita();
    ///prenota uscita sul binario
    void prenotaUscita(int t, bool reverse);
    ///il primo tratto del binario è libero
    void liberaUscita(bool reverse);
    ///metodo della classe stazioen secondaria chiede se c'è un binario di transito libero
    virtual bool isFreePass(int id, bool reverse) = 0;
    ///prenota un binario di transito
    virtual void PrenotaTransito(int id, bool reverse) = 0;
    ///libera un binario di transito
    virtual void liberaBinarioTransito(int id, bool reverse) = 0;


};


#endif //GESTIONETRANVIARIA_STAZIONE_H