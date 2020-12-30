/**
 * @author Tommaso Bertoldi 1218661
 * @brief come da nome la classe rappresenta un semaforo della linea ferroviaria
 */

#include "Semaforo.h"
bool Semaforo::getStatus() {
    return status;
}

Semaforo::Semaforo() {
    status = false;
}

void Semaforo::setRosso() {
    status = false;
}

void Semaforo::setVerde() {
    status = true;
}