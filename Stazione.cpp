#include <iostream>
#include "Stazione.h"
#include <algorithm>
/**
 * @author Tommaso Bertoldi 1218661
 * @brief Stazione della linea ferroviaria
 */

Stazione::Stazione(int km, std::string nome) : Km(km),nome(nome) {
    accessoStazione = Semaforo();
    accessoStazione.setVerde();
    uscitaDeposito = Semaforo();
    uscitaDeposito.setVerde();
}


bool Stazione::UscitaDeposito(Treno t) {
    if(uscitaDeposito.getStatus()){
        return PrenotaBinario(t);
    }
    return false;
}

bool Stazione::PrenotaBinario(Treno t) {
    std::cout<<"Richiesta di accesso alla Stazione";
    if(accessoStazione.getStatus()){
        std::cout<<"Richiesta Binario dal treno" << t.get_id() << "Alla Stazione" << this->nome;
        if(i<4){
            std::cout << "Richiesta Accettata e Confermata";
            binarioOrdinario.push_back(t);
            i++;
            if (i=4) {
                accessoStazione.setRosso();
                uscitaDeposito.setRosso();
            }
            return true;
        }
    }
        std::cout << "Stazione piena reindirizzamento al deposito";
        deposito.push_back(t);
        //Verificando col semaforo non si dovrebbe mai arrivare qua
        return false;
}

void Stazione::liberaBinario(Treno t) {
    std::cout<<"Richiesta Partenza dal treno" << "t.nome" << "Dalla Stazione" << this->nome;
    binarioOrdinario.erase(std::remove(binarioOrdinario.begin(), binarioOrdinario.end(), t), binarioOrdinario.end());
    std::cout<<"Richiesta Accettata";
    i--;
}

void Stazione::liberaDeposito(Treno t) {
    deposito.erase(std::remove(binarioOrdinario.begin(), binarioOrdinario.end(), t), binarioOrdinario.end());
    std::cout<<"Richiesta Accettata";
}

int Stazione::getDistance() {
    return this->Km;
}

