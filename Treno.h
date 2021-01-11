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


class Treno: public std::enable_shared_from_this<Treno> {
public:																												//Costruttore di default
	Treno(int id, const std::list<std::shared_ptr<Stazione>>& Stazioni, std::vector<int> Orari, bool reverse = false);		//Costruttore
	//*************************Getters e setters************************************
	int get_id() const;											//Resituisce il numero del treno
	int get_velocita() const;									//Resituisce la velocità del treno
	double get_posizione() const;								//Resituisce la posizione del treno
	int get_ritardo() const;									//Resituisce il ritardo del treno (aggiornato all'ultima fermata)
	bool isReverse() const;										//Restituisce true se il treno viaggia al contrario
	int get_orario() const;										//Restituisce l'orario di partenza previsto per il treno
	virtual int max_speed() = 0;								//Restituisce la massima velocità del treno in base al tipo
	Stato get_stato() const;									//Resitituisce lo stato del treno

	virtual void set_velocita(int v);						//Imposta la velocità del treno. Poichè questa classe interfaccia non prevede limiti superiori, il valore di default è 0 nel caso in cui il parametro non venga impostato

	//*************************Inizializzazione*************************************
	virtual void attiva(int ora) = 0;							//Attiva per la prima volta il treno sulla linea

	//*************************Routine e subroutine del treno***********************
	void esegui();												//Routine del treno: deve essere chiamata ogni minuto. Esegue azioni in base allo stato
	void cambia_stato(Stato s);									//Subroutine: permette al treno di cambiare il proprio stato e regola la velocità di conseguenza

protected:
	//*************************Principali operazioni del treno**********************
	void avanza();												//Fa avanzare il treno
	void testa_ingresso_stazione();								//Controlla se il treno è entrato in zona stazione
	void testa_uscita_stazione();								//Controlla se il treno è uscito dalla zona stazione
	void testa_fermata();										//Controlla se il treno è arrivato alla banchina
	void testa_transito();										//Controlla se il treno in transito ha superato la stazione
	void aggiorna_fermata();									//Conta i minuti di fermata. Trascorsi 5 minuti, imposta lo stato a stazione e fa ripartire il treno

	//*************************Funzioni di appoggio*********************************
	void sposta_avanti();										//Sposta il treno in avanti
	void sposta_indietro();										//Sposta il treno indietro

	virtual void pre_chiamata() = 0;
	virtual void chiama_stazione() = 0;							//Chiama la stazione e agisce in base al tipo di treno e di stazione
	
	void libera_binario();										//Libera il binario sul quale si trovava il treno
	void libera_uscita();
	void aggiorna_indici();										//Calcola il ritardo con cui il treno è arrivato alla fermata

	void effettua_fermata();									//Ferma il treno alla banchina
	void calcola_ritardo();										//Calcola il ritardo del treno in stazione

	//************************Funzioni di prenotazione dei binari*******************
	void partenza(bool trans = false);							//Prenota un binario per la partenza del treno dalla stazione di partenza
	void prenota_fermata();										//Prenota un binario di fermata
	void prenota_transito();									//Prenota un binario di transito
	
	//************************Operatori utili***************************************
	bool operator ==(const Treno& treno) const;					//Operatore di confronto


	//Variabili membro
	int orario_partenza;		//Indica l'orario di partenza effettiva del treno dal capolinea
	int orario;					//Indica l'ora corrente
	int identificativo;			//Indica il numero del treno
	int velocita;				//Indica la velocità attuale del treno
	int ritardo;				//Indica il ritardo del treno
	int minuti_fermata;			//Indica il tempo trascorso in fermata

	double posizione;			//Indica la posizione del treno

	Stato stato;				//Registra lo stato attuale del treno

	const std::list<std::shared_ptr<Stazione>>& Stazioni;						//Lista di stazioni lungo la linea ferroviaria, ordinate dal km 0
	std::list<std::shared_ptr<Stazione>>::const_iterator iteratore_stazioni;	//Iteratore alla stazione corrente

	std::vector<int> Orari;		//Vettore di orari ordinato dalla stazione di partenza
	int indice_orario;			//Indice all'orario di arrivo alla prossima stazione

	bool attivato;				//Registra se il treno è già stato attivato
	bool reverse;				//Registra se il treno viaggia a velocità inversa
	bool fermata_effettuata;	//Registra se ha effettuato la fermata alla prossima stazione
	bool annunciato;
	bool transitato;
	bool liberato;
};


//************************************************************************************************************
//************************************************************************************************************


class Regionale : public Treno {
public:
	Regionale(int id, const std::list<std::shared_ptr<Stazione>>& Stazioni, std::vector<int> Orari, bool reverse = false);
	void attiva(int orario) override;
	void set_velocita(int v = MAX_SPEED) override;
	void pre_chiamata() override;
	void chiama_stazione() override;
	int max_speed() override; 
private:
	static const int MAX_SPEED = 160;
};

class AltaVelocita : public Treno {
public:
	AltaVelocita(int id, const std::list<std::shared_ptr<Stazione>>& Stazioni, std::vector<int> Orari, bool reverse = false);
	void attiva(int orario) override;
	void set_velocita(int v = MAX_SPEED) override;
	void pre_chiamata() override;
	void chiama_stazione() override;
	int max_speed() override;
private:
	static const int MAX_SPEED = 240;
};

class SuperVelocita : public Treno {
public:
	SuperVelocita(int id, const std::list<std::shared_ptr<Stazione>>& Stazioni, std::vector<int> Orari, bool reverse = false);
	void attiva(int orario) override;
	void set_velocita(int v = MAX_SPEED) override;
	void pre_chiamata() override;
	void chiama_stazione() override;
	int max_speed() override;
private:
	static const int MAX_SPEED = 300;
};


#endif //GESTIONETRANVIARIA_TRENO_H
