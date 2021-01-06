#include <iostream>
#include "Stazione.h"
#include <algorithm>
/**
 * @author Tommaso Bertoldi 1218661
 * @brief Stazione della linea ferroviaria
 */

Stazione::Stazione(int km, std::string nome) : Km(km),nome(nome) {
    semBinariStazionamento = Semaforo();
    semBinariStazionamento.setVerde();

}


int Stazione::getDistance() {
    return this->Km;
}

std::string Stazione::getNome() {
    return this->nome;
}

bool Stazione::isFreeStop() {
    return semBinariStazionamento.getStatus();
}

void Stazione::liberaDeposito(Treno t) {
    deposito.erase(std::remove(deposito.begin(), deposito.end(), t), deposito.end());
    std::cout<<"Il Treno N." <<t.get_id()<<"ha lasciato il deposito";
}

void Stazione::liberaBinarioStazionamento(Treno t) {
    binariStazionamento.erase(std::remove(binariStazionamento.begin(), binariStazionamento.end(), t), binariStazionamento.end());
    std::cout<<"il Treno N." <<t.get_id()<<"ha liberato il binario";
    i--;
    semBinariStazionamento.setVerde();
}

void Stazione::PrenotaStazionameto(Treno t) {
    std::cout<<"Accesso al Binario di Stazionamento del treno" << t.get_id() << "Alla Stazione" << this->nome;
    binariStazionamento.push_back(t);
    i++;
    if (i==4) {
        semBinariStazionamento.setRosso();
    }

}

void Stazione::PrenotaDeposito(Treno t) {
    deposito.push_back(t);
}
