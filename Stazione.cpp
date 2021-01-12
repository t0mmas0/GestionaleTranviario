#include <iostream>
#include "Stazione.h"
#include "Grafica.h"
#include <algorithm>

/**
 * @author Tommaso Bertoldi 1218661
 * @brief Stazione della linea ferroviaria
 */

Stazione::Stazione(int km, std::string nome) : Km(km), nome(nome), binariStazionamento(2),
                                               binariStazionamentoInverso(2) {
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
    if (reverse) {
        return semBinariStazionamentoInverso.getStatus();
    }
    return semBinariStazionamento.getStatus();
}

void Stazione::liberaDeposito(int id, bool reverse) {
    if (reverse) {
        depositoInverso.erase(std::remove(depositoInverso.begin(), depositoInverso.end(), id),depositoInverso.end());
    } else {
        deposito.erase(std::remove(deposito.begin(), deposito.end(), id),deposito.end());
    }
    Grafica::uscitaDeposito(id);
}

void Stazione::liberaBinarioStazionamento(int id, bool reverse) {
    if (reverse) {
        binariStazionamentoInverso.erase(
                std::remove(binariStazionamentoInverso.begin(), binariStazionamentoInverso.end(), id),
                binariStazionamentoInverso.end());
        is--;
        semBinariStazionamentoInverso.setVerde();
    } else {
        binariStazionamento.erase(std::remove(binariStazionamento.begin(), binariStazionamento.end(), id),
                                  binariStazionamento.end());
        i--;
        semBinariStazionamento.setVerde();
    }
    Grafica::liberaBinario(true, id, this->nome);

}

void Stazione::PrenotaStazionamento(int t, bool reverse) {
    if (reverse) {
        Grafica::accessoAlBinario(false, t, this->nome);
        binariStazionamentoInverso.push_back(t);
        is++;
        if (is == 2) {
            semBinariStazionamentoInverso.setRosso();
        }
    } else {
        Grafica::accessoAlBinario(false, t, this->nome);
        binariStazionamento.push_back(t);
        i++;
        if (i == 2) {
            semBinariStazionamento.setRosso();
        }
    }
}

void Stazione::PrenotaDeposito(int t, bool reverse) {
    if (reverse) {
        depositoInverso.push_back(t);
    } else {
        deposito.push_back(t);
    }
}

bool Stazione::isPrincipale() const {
    return principale;
}

bool Stazione::isFreeUscita() {
    return uscita.getStatus();
}

void Stazione::prenotaUscita(int t, bool reverse) {
    if (reverse) {
        binarioPostStazioneInverso = t;
        uscitaInversa.setRosso();
    } else {
        binarioPostStazione = t;
        uscita.setRosso();
    }
}

void Stazione::liberaUscita(bool reverse) {
    if (reverse) {
        binarioPostStazioneInverso = -1;
        uscitaInversa.setVerde();
    } else {
        binarioPostStazione = -1;
        uscita.setVerde();
    }
}

