//

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
    bool isFreePass(const std::shared_ptr<Treno> t) override;
    void PrenotaTransito(const std::shared_ptr<Treno> t) override;
    void liberaBinarioTransito(const std::shared_ptr<Treno> t) override;
};



#endif //GESTIONALETRANVIARIO_STAZIONEPrincipale_H
