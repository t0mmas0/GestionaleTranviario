

#ifndef GESTIONETRANVIARIA_LINEAFERROVIARIA_H
#define GESTIONETRANVIARIA_LINEAFERROVIARIA_H

#include "Stazione.h"
#include "vector"
/**
 * @author Tommaso Bertoldi 1218661
 * @brief  linea ferroviaria
 */

class LineaFerroviaria {
private:
    std::list<std::shared_ptr<Stazione>> linea;
    std::list<std::shared_ptr<Treno>> treniAndata;
    std::list<std::shared_ptr<Treno>> treniRitorno;
    std::list<std::shared_ptr<Treno>> treniAttiviAndata;
    std::list<std::shared_ptr<Treno>> treniAttiviRitorno;
    int orario;
    bool static compara(std::shared_ptr<Treno> a, std::shared_ptr<Treno> b);
    bool static comparaInversi(std::shared_ptr<Treno> a, std::shared_ptr<Treno> b);

public:
    LineaFerroviaria(std::string fileOrari,std::string fileLinea );
    void esegui();

    void sort();

    void controlloTamponamenti();

    void attivaTreni();

    void gestioneMovimento();
};


#endif //GESTIONETRANVIARIA_LINEAFERROVIARIA_H
