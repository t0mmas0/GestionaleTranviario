#include "LeggiOrari.h"
#include <iostream>
#include<sstream>
#include <stdexcept>
#include <algorithm>


bool treniCompare(const std::shared_ptr<Treno>& primo, const std::shared_ptr<Treno>& secondo) {
	// Returna true se primo va prima del secondo
	if (primo->get_orario() < secondo->get_orario()) {

		return true;
	}
	else {
		return false;
	}
}

std::list<std::shared_ptr<Treno>> LeggiOrari::getTreniDa()
{
	TreniDa.sort(treniCompare);//ordino rispetto orario di partenza
	return TreniDa;
}

std::list<std::shared_ptr<Treno>> LeggiOrari::getTreniPer(){
	TreniPer.sort(treniCompare);//ordino rispetto orario di partenza
	return TreniPer;
}

LeggiOrari::LeggiOrari(const std::string& file) :NomeFile(file)
{
	
}

void LeggiOrari::leggiFile(const std::list<std::shared_ptr<Stazione>>& Stazioni) {

	Stream.open(NomeFile);//apro file
	if (!Stream) {//se file non trovato lancio eccezione
		throw std::runtime_error("Impossibile aprire file");
	}
	int idTreno;
	std::string line;
	bool reverse;//tiene conto il verso del treno
	int tipoTreno = 1;
	int n;//variabile dove salvo dati letti da file
	for (line; std::getline(Stream, line); ) {
		bool esistente = false;//variabile nel caso il esista gia un trenocon lo stesso id
		std::vector<int> orari;
		std::stringstream sstream(line);//bind linea allo stringstream per estrapolare i dati
		int i = 0;
		while (sstream >> n) {
			if (i == 0) {//se e' il primo numero e' l'id
				idTreno = n;
			}
			else if (i == 1) {//se e' il secondo numero e' il verso
				reverse = n;
			}
			else if (i == 2) {//se e' il terzo numero e' il tipo di treno
				tipoTreno = n;
			}
			else {//il resto sono orari
				orari.push_back(n);
				std::cout << "Pushato elemento\nSize:" << orari.size() << std::endl;
			}
			i++;
		}
		//setto velocita per i rispettivi treni
		int velocita = getVelocita(tipoTreno);

		if (orari.size() == 0) {// se non c'era neanche un orario
			throw std::logic_error("serve almeno l'orario di partenza nel file");
		}
		for (auto i = TreniDa.begin(); i != TreniDa.end(); ++i) {
			if (idTreno == (*i)->get_id()) {
				esistente = true;
				break;
			}
		}
		if (esistente) {
			for (auto i = TreniPer.begin(); i != TreniPer.end(); ++i) {
				if (idTreno == (*i)->get_id()) {
					esistente = true;
					break;
				}
			}
		}
		if (esistente) {
			continue;
		}
		if (Stazioni.size() > orari.size()) {//algoritmo nel caso manchino orari (riempe con 0 gli orari vuoti) 
			int origSize = orari.size();
			orari.resize(Stazioni.size(), 0);
			if (tipoTreno != 1) {//qua serve solo per alta/super velocita
				int i = 1;
				int j = 1;
				for (auto it = ++Stazioni.begin(); it != Stazioni.end(); ++it) {
					if ((*it)->isPrincipale()) {
						if (orari[i] != 0 && i == j + 1) {
							if (i == j + 1) {

								orari.resize(orari.size() - 1);
								orari.insert(orari.begin() + j, 0);
								j += 2;
							}
							else {
								j++;
							}
						}
						else {

							std::swap(orari[j], orari[i]);
							j++;
						}

					}
					i++;
				}
			}

		}
		if (Stazioni.size() < orari.size()) {//se ci sono piu orari di stazioni tolgo gli ultimi
			orari.erase(orari.begin() + Stazioni.size(), orari.end());
		}
		correggiOrari(Stazioni, orari, velocita, reverse, tipoTreno);//chiamo funzione correggi orari
		//per ogni treno inserisco col giusto costruttore nella giusta lista
		if (tipoTreno == 1) {
			if (reverse) {
				TreniPer.emplace_back(std::make_shared<Regionale>(idTreno, Stazioni, orari, reverse));
			}
			else {
				TreniDa.emplace_back(std::make_shared<Regionale>(idTreno, Stazioni, orari, reverse));
			}
		}
		else if (tipoTreno == 2) {
			if (reverse) {
				TreniPer.emplace_back(std::make_shared<AltaVelocita>(idTreno, Stazioni, orari, reverse));
			}
			else {
				TreniDa.emplace_back(std::make_shared<AltaVelocita>(idTreno, Stazioni, orari, reverse));
			}
		}
		else {
			if (reverse) {
				TreniPer.emplace_back(std::make_shared<SuperVelocita>(idTreno, Stazioni, orari, reverse));
			}
			else {
				TreniDa.emplace_back(std::make_shared<SuperVelocita>(idTreno, Stazioni, orari, reverse));
			}
		}


	}
	Stream.close();
}


void LeggiOrari::correggiOrari(const std::list<std::shared_ptr<Stazione>>& Stazioni, std::vector<int>& orari, int velocita, bool reverse, int tipoTreno)
{//putroppo non sono riuscito a creare un algoritmo unico per treni reverse e non reverse in quanto non si può assegnare un reverse iterator a un iterator
	int i = 1;
	bool fermataPrev = false;//bool che tiene conto se il treno si e' fermato nella stazione prima, in quel caso aggiungo 5 in al tempo
	if (!reverse) {
		for (auto it = ++Stazioni.begin(); it != Stazioni.end(); ++it) {
			//ottengo i km in mezzo e calcolo il tempo 
			auto prev = std::prev(it, 1);
			int km = (*it)->getDistance();

			int kmPrima = (*(prev))->getDistance();
			int kmInMezzo = km - kmPrima;
			int tempoInMezzo = 0;
			if (fermataPrev) {
				tempoInMezzo += 5;
			}
			if (tipoTreno != 1) {//alta e super velocita
				if (!(*it)->isPrincipale()) {//caso secondaria
					tempoInMezzo += 10 + kmInMezzo / velocita;
					fermataPrev = false;
				}
				else {//caso principale
					tempoInMezzo += 10 + (kmInMezzo - 10) / velocita + 10 / 80;
					fermataPrev = true;
				}
			}
			else {//regionale
				tempoInMezzo += 10 + (kmInMezzo - 10) / velocita + 10 / 80;
				fermataPrev = true;
			}
			if (orari[i] - orari[i - 1] < tempoInMezzo) {
				orari[i] = tempoInMezzo + orari[i - 1];
			}

			i++;
		}
	}
	if (reverse) {//stesso algoritmo ma con dati al contrario
		for (auto it = ++Stazioni.rbegin(); it != Stazioni.rend(); ++it) {
			auto prev = std::prev(it, 1);
			int km = (*it)->getDistance();

			int kmPrima = (*(prev))->getDistance();
			int kmInMezzo = kmPrima - km;
			int tempoInMezzo = 0;
			if (fermataPrev) {
				tempoInMezzo += 5;
			}
			if (tipoTreno != 1) {//alta e super velocita
				if (!(*it)->isPrincipale()) {//caso secondaria
					tempoInMezzo += 10 + kmInMezzo / velocita;
					fermataPrev = false;
				}
				else {//caso principale
					tempoInMezzo += 10 + (kmInMezzo - 10) / velocita + 10 / 80;
					fermataPrev = true;
				}
			}
			else {//regionale
				tempoInMezzo += 10 + (kmInMezzo - 10) / velocita + 10 / 80;
				fermataPrev = true;
			}
			if (orari[i] - orari[i - 1] < tempoInMezzo) {
				orari[i] = tempoInMezzo + orari[i - 1];
			}

			i++;
		}
	}
}


int LeggiOrari::getVelocita(int tipoTreno)
{
	if (tipoTreno == 1) {
		return 160;
	}
	else if (tipoTreno == 2) {
		return 240;
	}
	else {
		return 300;
	}
}

