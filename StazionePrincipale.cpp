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
bool StazionePrincipale::isFreePass(const std::shared_ptr<Treno> t) {
    throw  std::runtime_error("Funzione non prevista per questa derivata");
}

void StazionePrincipale::PrenotaTransito(const std::shared_ptr<Treno> t) {
    throw  std::runtime_error("Funzione non prevista per questa derivata");
}

void StazionePrincipale::liberaBinarioTransito(const std::shared_ptr<Treno> t) {
    throw  std::runtime_error("Funzione non prevista per questa derivata");
}