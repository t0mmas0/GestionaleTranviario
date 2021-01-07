/**
 * @author Tommaso Bertoldi 1218661
 * @brief Stazione della linea ferroviaria
 */
#include <iostream>
#include "StazionePrincipale.h"

StazionePrincipale::StazionePrincipale(int km, std::string nome) : Stazione(km, nome) {


}
bool StazionePrincipale::isFreePass(const std::shared_ptr<Treno> t) {
    return false;
}

void StazionePrincipale::PrenotaTransito(const std::shared_ptr<Treno> t) {

}

void StazionePrincipale::liberaBinarioTransito(const std::shared_ptr<Treno> t) {

}