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

    if(counter) {

        std::cout << "  _____  _      _       _                         _                     ____                 _     \n";
        std::cout << " |  __ \\(_)    | |     | |                       (_)                   / __ \\               (_)  _ \n";
        std::cout << " | |__) |_  ___| | __ _| |__   ___  _ __ __ _ _____  ___  _ __   ___  | |  | |_ __ __ _ _ __ _  (_)\n";
        std::cout << " |  _  /| |/ _ \\ |/ _` | '_ \\ / _ \\| '__/ _` |_  / |/ _ \\| '_ \\ / _ \\ | |  | | '__/ _` | '__| |    \n";
        std::cout << " | | \\ \\| |  __/ | (_| | |_) | (_) | | | (_| |/ /| | (_) | | | |  __/ | |__| | | | (_| | |  | |  _ \n";
        std::cout << " |_|  \\_\\_|\\___|_|\\__,_|_.__/ \\___/|_|  \\__,_/___|_|\\___/|_| |_|\\___|  \\____/|_|  \\__,_|_|  |_| (_)\n\n\n";
        std::cout << "TRENO" << "\t\t" << "ORARIO LETTO" << "\t\t" << "ORARIO CORRETTO" << "\t\t" << "STAZIONE\n"<< std::endl;
    }
    std::cout  <<"N."<< id << "\t\t" << orarioOre <<":"<<orariomin << "\t\t\t" << orarioCorrOre <<":"<<orarioCorrmin <<"\t\t\t" << nome <<  std::endl;
    counter = false;
}

void Grafica::accessoAlBinario(bool tipoBinario, int id, std::string nome) {

    if (tipoBinario) {
        std::cout << "Accesso al Binario di Transito:" << std::endl;
        std::cout << "Treno:\t" << id << "\tStazione:\t" << nome << std::endl;
    }
    else{
        std::cout << "Accesso al Binario di Stazionamento:"<<std::endl;
        std::cout << "Treno:\t" << id << "\tStazione:\t" << nome << std::endl;
    }
}

void Grafica::liberaBinario(bool tipoBinario, int id, std::string nome) {
    if (tipoBinario)
        std::cout << "il Treno N." << id << "ha liberato il binario di stazionamento\n";
    else{
        std::cout << "il Treno N." << id << "ha liberato il binario di transito\n";
    }
}

void Grafica::uscitaDeposito(int id) {
    std::cout<<"Il Treno N." <<id<<"ha lasciato il deposito\n";
}

void Grafica::attesa(int id, double pos) {
    std::cout << "Il treno " << id << " e' fermo in attesa al chilometro " << pos << std::endl;
}

void Grafica::posizioneTreno(int id, double pos,int ora) {
    std::cout << "Il treno " << id << " e' al km " << pos << " e segna le ore " << ora << std::endl;
}

void Grafica::switchAttesa(int id) {
    std::cout << "Il treno " << id << " e' stato messo in attesa" << std::endl;
}

void Grafica::capolinea(int id) {
    std::cout << "Il treno " << id << " ha superato il capolinea ed e' stato distrutto";
}

void Grafica::passStazione(int id, std::string nome) {
    std::cout << "Il treno " << id << " in transito, ha superato la stazione " << nome << std::endl;
}

void Grafica::possibileAvanzare(int id, std::string nome) {
    std::cout << "Il treno " << id << " ha liberato il binario di uscita dalla stazione " << nome<< std::endl;
}

void Grafica::distruggi(int id) {
    std::cout << "Il treno " << id << " e' arrivato alla fermata finale e verra' distrutto" << std::endl;
}

void Grafica::ingressoStazione(int id, std::string nome, double orario) {
    std::cout << "Il treno " << id << " e'entrato alla stazione"<<nome<<" all' orario" <<orario<< std::endl;
}

void Grafica::ritardo(int id, int ritardo, std::string nome) {
    std::cout << "Il treno " << id << " e' in ritardo di " << ritardo << " minuti alla stazione " <<nome<< std::endl;
}

void Grafica::anticipo(int id, int min) {
    std::cout << "Il treno " << id << " e' in anticipo di " << min << " minuti, e rimarra' in parcheggio" << std::endl;
}
