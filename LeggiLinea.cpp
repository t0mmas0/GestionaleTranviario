//autore Marco Nardi 1218629
#include "LeggiLinea.h"
#include <sstream>


LeggiLinea::LeggiLinea(const std::string& file) :NomeFile(file)
{

}

std::list<std::shared_ptr<Stazione>> LeggiLinea::LeggiStazioni()
{
	Stream.open(NomeFile);//apro file
	if (!Stream) {//se file non trovato lancio eccezione
		throw std::runtime_error("Impossibile aprire file");
	}
	std::string line; 
	std::getline(Stream, line);//leggo prima stazione che sar� solo composta da parole
	Stazioni.push_back(std::make_shared<Stazione>(0, line));
	std::string NomeStazione;
	std::string parola;
	bool IsSecondaria;
	int kmDaOrigine;
	for (line; std::getline(Stream, line); )
	{
		NomeStazione = "";
		std::stringstream sstream(line);//bind linea allo stringstream per estrapolare i dati
		sstream >> parola;// leggo prima parola e la aggiungo
		NomeStazione += parola;
		if (sstream >> IsSecondaria) {//provo a leggere un numero
			sstream >> kmDaOrigine;
			continue;
		}
		sstream.clear();//nel caso non fosse stato letto il numero tolgo la flag di errore
		while (sstream >> parola) {//tento di leggere un'altra parola nel caso non sia stato letto un numero
			while (sstream >> IsSecondaria) {//ora che ho letto la parola tento di leggere il numero
				sstream >> kmDaOrigine;
				continue;
			}
			sstream.clear();//nel caso ci sia ancora un'altra parola tolgo flag di errore
			NomeStazione += " "+parola;//aggiungo parola al nome
		}
		if (IsSecondaria) {//se stazione secondaria uso altra classe
			Stazioni.push_back(std::make_shared<StazioneSecondaria>(kmDaOrigine, line));
		}
		else {
			Stazioni.push_back(std::make_shared<Stazione>(kmDaOrigine, line));
		}

	}//ripeto per ogni riga di file
	Stream.close();//chiudo file
	return Stazioni;
}
