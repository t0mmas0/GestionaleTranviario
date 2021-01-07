//Simone Peraro 1216334
//Interfaccia Treno e sottoclassi derivate

#ifndef GESTIONETRANVIARIA_TRENO_H
#define GESTIONETRANVIARIA_TRENO_H

#include <list>
#include <vector>
#include <memory>
#include "Stazione.h"

//Forward declaration. Poichè la classe Stazione include la classe Treno, 
//e la classe Treno include la classe Stazione, è necessario anticipare la dichiarazione della classe Stazione
class Stazione;

//Enumeratore stato treno
enum Stato :int { creato = 0, attesa = 1, stazione = 2, transito = 3, movimento = 4, parcheggio = 5, fermata = 6, distrutto = 7 };
//Creato: il treno è stato appena creato. Tuttavia non è ancora stato inserito nel traffico tranviario
//In attesa: il treno è fermo sul binario e non esegue alcuna operazione
//In stazione: il treno è in zona stazione e rallenta la propria velocità
//In transito: il treno è nel binario di transito di una stazione
//In movimento: il treno si muove lungo i binari
//In parcheggio: il treno è in un binario di parcheggio presso una stazione
//In fermata: il treno è fermo per la salita/discesa dei passeggeri
//Distrutto: il treno si è fermato o ha superato la stazione finale

//Ciclo di vita di un treno (con modalità di cambio stato)
//																┌-----------------------------------------------------------------------------------------------┐
//			(manuale)						(automatico)		|	    (automatico)				(automatico)			 (automatico)				   (auto|matico)
//(creato)		-->		(stazione/transito)		--->		(movimento)		 ┬--->			(stazione)	--->		(fermata)	--->	(stazione)				├--->		(distrutto)
//																	   oppure|					^																|
//																			 └--->(parcheggio) -┤oppure															|
//																								v							 (automatico)						|
//																							(transito) ----------------------------------------------------------
//In ogni momento, l'attività del treno può essere sospesa impostando lo stato ad attesa


class Treno {
public:

	Treno();																											//Costruttore di default
	Treno(int id, std::list<std::shared_ptr<Stazione>>& Stazioni, std::vector<int>& Orari, bool reverse = false);		//Costruttore
	Treno(const Treno& treno);																							//Costruttore di copia

	// TODO: Copia e Move dovrebbero essere virtuali per gestire i tipi specifici?
	// TODO: Move

	
	void esegui();												//Muta automaticamento lo stato del treno
	void sposta_avanti(int v);
	void sposta_indietro(int v);
	void testa_ingresso_stazione();
	void testa_uscita_stazione();
	void testa_fermata();
	void effettua_fermata();
	void aggiorna_fermata();									//Conta il tempo di fermata
	void cambia_stato(Stato s);									//Cambia lo stato del treno e imposta la velocita
	void calcola_ritardo();										//Calcola il ritardo del treno in stazione
	void prenota_fermata();
	void prenota_transito();
	void libera_binario();
	void partenza(bool transito = false);
	void aggiorna_indici();

	virtual void avanza(int v = 0);											//Fa avanzare il treno
	virtual void attiva(int ora) = 0;										//Attiva per la prima volta il treno sulla linea
	virtual void chiama_stazione() = 0;										//Chiama stazione

	int get_id() const;				//Resituisce l'identificativo
	int get_velocita() const;		//Resituisce la velocita
	int get_posizione() const;		//Resituisce la posizione
	int get_ritardo() const;		//Resituisce il ritardo

	virtual void set_velocita(int v = 0);	//Imposta la velocita. Poichè questa interfaccia non ha una velocita limite predefinita, essa viene posta a 0 di default

	Treno& operator=(const Treno& treno);						//Assegnamento di copia
	bool Treno::operator ==(const Treno& treno) const;			//Operatore di confronto

protected:
	int orario;
	int identificativo;
	int velocita;
	int ritardo;
	int minuti_fermata;

	double posizione;

	Stato stato;

	std::list<std::shared_ptr<Stazione>>& Stazioni;
	std::list<std::shared_ptr<Stazione>>::iterator iteratore_stazioni;

	std::vector<int>& Orari;
	int indice_orario;

	bool attivato;
	bool reverse;
	bool velocita_limitata;
	bool fermata_effettuata;
};

class Regionale : public Treno {
public:
	Regionale(int id, std::list<std::shared_ptr<Stazione>>& Stazioni, std::vector<int>& Orari, bool reverse = false);
	void attiva(int orario) override;
	void set_velocita(int v = MAX_SPEED) override;
	void avanza(int v = MAX_SPEED) override;
	void chiama_stazione() override;
private:
	static const int MAX_SPEED = 160;
};

class AltaVelocita : public Treno {
public:
	AltaVelocita(int id, std::list<std::shared_ptr<Stazione>>& Stazioni, std::vector<int>& Orari, bool reverse = false);
	void attiva(int orario) override;
	void set_velocita(int v = MAX_SPEED) override;
	void avanza(int v = MAX_SPEED) override;
	void chiama_stazione() override;
private:
	static const int MAX_SPEED = 240;
};

class SuperVelocita : public Treno {
public:
	SuperVelocita(int id, std::list<std::shared_ptr<Stazione>>& Stazioni, std::vector<int>& Orari, bool reverse = false);
	void attiva(int orario) override;
	void set_velocita(int v = MAX_SPEED) override;
	void avanza(int v = MAX_SPEED) override;
	void chiama_stazione() override;
private:
	static const int MAX_SPEED = 300;
};


#endif //GESTIONETRANVIARIA_TRENO_H
