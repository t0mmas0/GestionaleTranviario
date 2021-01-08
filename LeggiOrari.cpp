#include "LeggiOrari.h"
#include<sstream>
#include <stdexcept>
#include <algorithm>
LeggiOrari::LeggiOrari(const std::string& file) :NomeFile(file)
{
}

void LeggiOrari::leggiFile(const std::list<std::shared_ptr<Stazione>>& Stazioni) {

	Stream.open(NomeFile);//apro file
	if (!Stream) {//se file non trovato lancio eccezione
		throw std::runtime_error("Impossibile aprire file");
	}
	int orarioPartenza;
	int idTreno;
	std::string line;
	bool reverse;
	int tipoTreno;
	int n;
	for (line; std::getline(Stream, line); ) {
		bool esistente = false;
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
		for (auto i = TreniPer.begin(); i != TreniPer.end(); ++i) {
			if (idTreno == (*i)->get_id()) {
				esistente = true;
				break;
			}
		}
		if (esistente) {
			continue;
		}
		if (Stazioni.size() > orari.size()) {//se ci sono piu stazioni di orari vuol dire che 
			/*int i = 1;
			orari.resize(Stazioni.size());
			for (auto it = Stazioni.begin(); it != Stazioni.end(); ++it) {
				if (tipoTreno != 1) {//inserisco quelli mancanti ipotizzando che ci siano gia quelle principali

					if (!(*it)->isPrincipale()) {

						orari.insert(orari.begin() + i, 0);
					}
				}
				else {

				}
				i++;
			}*/
			if (tipoTreno != 1 && orari.size() < Stazioni.size()) {//caso in cuimancassero anche le principali

			}
			int origSize = orari.size();
			orari.resize(Stazioni.size(), -1);
			if (tipoTreno != 1) {
				int i = 1;
				int j = 1;
				for (auto it = ++Stazioni.begin(); it != Stazioni.end(); ++it) {
					if ((*it)->isPrincipale()) {
						if (orari[i] != 0 && i==j+1) {
							if (i ==j+1) {

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
		if (Stazioni.size() < orari.size()) {
			orari.erase(orari.begin() + Stazioni.size(), orari.end());
		}
		correggiOrari(Stazioni, orari, velocita, reverse, tipoTreno);
		//controlla se treno esiste gia
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
{
	int i = 1;
	bool fermataPrev = false;
	if (!reverse) {
		for (auto it = ++Stazioni.begin(); it != Stazioni.end(); ++it) {
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
	if (reverse) {
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

struct MyClassComparator {
	bool operator()(const std::shared_ptr<Treno>& first, const std::shared_ptr<Treno>& second) const {
		// Return true if first should go before second
		if (first->get_orario() < second->get_orario()) {

			return true;
		}
		else {
			return false;
		}
	}
};

void LeggiOrari::sortTreni()
{
	TreniDa.sort(MyClassComparator());
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

