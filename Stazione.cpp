#include <iostream>
#include "Stazione.h"
#include <algorithm>
/**
 * @author Tommaso Bertoldi 1218661
 * @brief Stazione della linea ferroviaria
 */

Stazione::Stazione(int km, std::string nome) : Km(km),nome(nome) {
    semBinariStazionamento = Semaforo();
    semBinariStazionamentoInverso = Semaforo();


}


int Stazione::getDistance() {
    return this->Km;
}

std::string Stazione::getNome() {
    return this->nome;
}

bool Stazione::isFreeStop(int id, bool reverse) {
    if(reverse){
        return semBinariStazionamentoInverso.getStatus();
    }
    return semBinariStazionamento.getStatus();
}

void Stazione::liberaDeposito(int id, bool reverse) {
    if (reverse){
        depositoInverso.erase(std::remove(depositoInverso.begin(), depositoInverso.end(), id),
                              depositoInverso.end());
        std::cout<<"Il Treno N." <<id<<"ha lasciato il deposito\n";
    }else {
        deposito.erase(std::remove(deposito.begin(), deposito.end(), id),
                       deposito.end());
        std::cout<<"Il Treno N." <<id<<"ha lasciato il deposito\n";
    }
}

void Stazione::liberaBinarioStazionamento(int id, bool reverse) {
    if (reverse){
        binariStazionamentoInverso.erase(std::remove(binariStazionamentoInverso.begin(), binariStazionamentoInverso.end(), id),
                                         binariStazionamentoInverso.end());
        std::cout << "il Treno N." << id << "ha liberato il binario\n";
        is--;
        semBinariStazionamentoInverso.setVerde();
    }else {
        binariStazionamento.erase(std::remove(binariStazionamento.begin(), binariStazionamento.end(), id),
                                  binariStazionamento.end());

        std::cout << "il Treno N." << id << "ha liberato il binario\n";
        i--;
        semBinariStazionamento.setVerde();
    }
}

void Stazione::PrenotaStazionamento(int t , bool reverse) {
    if (reverse){
        std::cout << "Accesso al Binario di Stazionamento del treno " << t << " alla Stazione" << this->nome << std::endl;
        binariStazionamentoInverso.push_back(t);
        is++;
        if (is == 2) {
            semBinariStazionamentoInverso.setRosso();
        }
    }
    else {
        std::cout << "Accesso al Binario di Stazionamento del treno " << t << " alla Stazione " << this->nome << std::endl;
        binariStazionamento[i]=t;
        i++;
        if (i == 2) {
            semBinariStazionamento.setRosso();
        }
    }
}

void Stazione::PrenotaDeposito(int t,bool reverse) {
    if(reverse){
        depositoInverso.push_back(t);
    }else{
        deposito.push_back(t);
    }
}

bool Stazione::isPrincipale() const {
    return principale;
}

