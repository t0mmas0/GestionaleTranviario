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

bool Stazione::isFreeStop(Treno *t) {
    if(t->isReverse()){
        return semBinariStazionamentoInverso.getStatus();
    }
    return semBinariStazionamento.getStatus();
}

void Stazione::liberaDeposito(const std::shared_ptr<Treno> t) {
    deposito.erase(std::remove(deposito.begin(), deposito.end(), t), deposito.end());
    std::cout<<"Il Treno N." <<t->get_id()<<"ha lasciato il deposito";
}

void Stazione::liberaBinarioStazionamento(const std::shared_ptr<Treno> t) {
    if (t->isReverse()){
        binariStazionamentoInverso.erase(std::remove(binariStazionamentoInverso.begin(), binariStazionamentoInverso.end(), t),
                                         binariStazionamentoInverso.end());
        std::cout << "il Treno N." << t->get_id() << "ha liberato il binario";
        is--;
        semBinariStazionamentoInverso.setVerde();
    }else {
        binariStazionamento.erase(std::remove(binariStazionamento.begin(), binariStazionamento.end(), t),
                                  binariStazionamento.end());
        std::cout << "il Treno N." << t->get_id() << "ha liberato il binario";
        i--;
        semBinariStazionamento.setVerde();
    }
}

void Stazione::PrenotaStazionamento(const std::shared_ptr<Treno> t) {
    if (t->isReverse()){
        std::cout << "Accesso al Binario di Stazionamento del treno" << t->get_id() << "Alla Stazione" << this->nome;
        binariStazionamentoInverso.push_back(t);
        is++;
        if (is == 2) {
            semBinariStazionamentoInverso.setRosso();
        }
    }
    else {
        std::cout << "Accesso al Binario di Stazionamento del treno" << t->get_id() << "Alla Stazione" << this->nome;
        binariStazionamento.push_back(t);
        i++;
        if (i == 2) {
            semBinariStazionamento.setRosso();
        }
    }
}

void Stazione::PrenotaDeposito(const std::shared_ptr<Treno> t) {
    if(t->isReverse()){
        depositoInverso.push_back(t);
    }else{
        deposito.push_back(t);
    }
}

bool Stazione::isPrincipale() {
    return principale;
}

