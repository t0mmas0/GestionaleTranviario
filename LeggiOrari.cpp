#include "LeggiOrari.h"
#include<sstream>
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
	std::vector<int> orari;
	std::string line = " ";
	bool reverse;
	int tipoTreno;
	int n;
	for (line; std::getline(Stream, line); ) {
		std::stringstream sstream(line);//bind linea allo stringstream per estrapolare i dati
		int i = 0;
		while (sstream >> n) {
			if (i == 0) {
				idTreno = n;
			}
			else if (i == 1) {
				reverse = n;
			}
			else if (i == 2) {
				tipoTreno = n;
			}
			else {
				orari.push_back(n);
			}
		}
		int velocita = 0;
		if (tipoTreno == 1) {
			velocita = 160;
		}
		else if (tipoTreno == 2) {
			velocita = 240;
		}
		else {
			velocita = 300;
		}
		if (Stazioni.size() > orari.size()) {
			int i = 0;
			for (auto it = Stazioni.begin(); it != Stazioni.end(); ++it) {

				if (!(*it)->isPrincipale()) {
					
					int tempoInMezzo = 0;
					orari.insert(orari.begin() + i, tempoInMezzo);
				}
				i++;
			}
		}
		if (Stazioni.size() < orari.size()) {
			orari.erase(orari.begin() + Stazioni.size(), orari.end());
		}
		correggiOrari(Stazioni, orari, velocita);
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

void LeggiOrari::correggiOrari(const std::list<std::shared_ptr<Stazione>>& Stazioni, std::vector<int> orari, int velocita)
{
	int i = 1;
	for (auto it = ++Stazioni.begin(); it != Stazioni.end(); ++it) {
		auto prev = std::prev(it, 1);
		int km = (*it)->getDistance();

		int kmPrima = (*(prev))->getDistance();
		int kmInMezzo = km - kmPrima;
		int tempoInMezzo = 10 + 5 + (kmInMezzo - 10) / velocita + 10 / 80;
		if (orari[i] < tempoInMezzo) {
			orari[i] = tempoInMezzo;
		}

		i++;
	}
}
