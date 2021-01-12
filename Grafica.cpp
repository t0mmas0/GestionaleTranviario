//
// Created by Tommaso on 12/01/2021.
//

#include "Grafica.h"
#include "iostream"
bool Grafica::counter = true;
void  Grafica::stampaModificheOrari(int id, std::string nome, int orario, int orarioCorretto) {

    std::string orarioOre="";
    if (orario/60 < 10){
        orarioOre+="0";
    }
    orarioOre += std::to_string(orario/60);
    std::string orariomin="";
    if (orario%60 < 10){
        orariomin+="0";
    }
    orariomin += std::to_string(orario%60);
    std::string orarioCorrOre="";
    if (orarioCorretto/60 < 10){
        orarioCorrOre+="0";
    }
    orarioCorrOre += std::to_string(orarioCorretto/60);
    std::string orarioCorrmin="";
    if (orarioCorretto%60 < 10){
        orarioCorrmin+="0";
    }
    orarioCorrmin += std::to_string(orarioCorretto%60);

    if(counter)
        std::cout << "TRENO"<<"\t\t"<<"ORARIO LETTO"<< "\t\t"<<"ORARIO CORRETTO"<<"\t\t"<<"STAZIONE"<<std::endl;
    std::cout  <<"N."<< id << "\t\t" << orarioOre <<":"<<orariomin << "\t\t\t" << orarioCorrOre <<":"<<orarioCorrmin <<"\t\t\t" << nome <<  std::endl;
    counter = false;

}
