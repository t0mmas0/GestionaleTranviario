
#ifndef GESTIONALETRANVIARIO_STAZIONEPrincipale_H
#define GESTIONALETRANVIARIO_STAZIONEPrincipale_H

#include "Stazione.h"
/**
 * @author Tommaso Bertoldi 1218661
 * @brief Stazione della linea ferroviaria
 */
class StazionePrincipale:public Stazione {
public:
    StazionePrincipale(int km, std::string nome);
    bool isFreePass(int id, bool reverse) override;
    void PrenotaTransito(int id, bool reverse) override;
    void liberaBinarioTransito(int id, bool reverse) override;

};



#endif //GESTIONALETRANVIARIO_STAZIONEPrincipale_H
