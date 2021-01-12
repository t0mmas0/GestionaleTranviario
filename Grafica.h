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
    static void accessoAlBinario(bool tipoBinario,int id, std::string nome);
    static void liberaBinario(bool tipoBinario,int id, std::string nome);
    static void uscitaDeposito(int id);
    static void attesa(int id,double pos);
    static void switchAttesa(int id);
    static void capolinea(int id);
    static void passStazione(int id,std::string nome);
    static void possibileAvanzare(int id,std::string nome);
    static void posizioneTreno(int id,double pos,int ora);






};


#endif //GESTIONALETRANVIARIO_GRAFICA_H
