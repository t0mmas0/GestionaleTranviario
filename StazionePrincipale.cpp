/**
 * @author Tommaso Bertoldi 1218661
 * @brief Stazione della linea ferroviaria
 */
#include <iostream>
#include "StazionePrincipale.h"
#include <stdexcept>

StazionePrincipale::StazionePrincipale(int km, std::string nome) : Stazione(km, nome) {
principale = true;

}
bool StazionePrincipale::isFreePass(int id, bool reverse) {
    throw  std::runtime_error("Funzione non prevista per questa derivata");
}

void StazionePrincipale::PrenotaTransito(int id, bool reverse) {
    throw  std::runtime_error("Funzione non prevista per questa derivata");
}

void StazionePrincipale::liberaBinarioTransito(int id, bool reverse) {
    throw  std::runtime_error("Funzione non prevista per questa derivata");
}
