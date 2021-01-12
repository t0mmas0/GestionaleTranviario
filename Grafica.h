//
// Created by Tommaso on 12/01/2021.
//

#ifndef GESTIONALETRANVIARIO_GRAFICA_H
#define GESTIONALETRANVIARIO_GRAFICA_H

#include "string"
class Grafica {
private:
public:
    static bool counter;

    static void stampaModificheOrari(int id,std::string nome,int orario , int orarioCorretto);
};


#endif //GESTIONALETRANVIARIO_GRAFICA_H
