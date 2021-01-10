//Simone Peraro 1216334
#include <iostream>
#include <cmath>
#include <stdexcept>
#include "Treno.h"
#include "Stazione.h"
#include <memory>

//Costruttore della classe treno richiamato dalle sottoclassi. Il passaggio del vettore Orari avviene per copia poichè la funzione leggiFile della classe LeggiOrari passa una copia del vettore che in seguito viene distrutto, in modo che il compilatore effettui una copy-elision
Treno::Treno(int id, const std::list<std::shared_ptr<Stazione>>& Stazioni, std::vector<int> Orari, bool reverse)
	:std::enable_shared_from_this<Treno>(), orario_partenza{ 0 }, orario{ 0 }, identificativo{ id }, velocita{ 0 }, posizione{ 0 }, ritardo{ 0 }, minuti_fermata{ 0 }, stato{ creato }, Stazioni{ Stazioni }, iteratore_stazioni{ Stazioni.begin() }, Orari{ Orari }, indice_orario{ 0 }, attivato{ false }, reverse{ reverse }, fermata_effettuata{ false }, annunciato{ false } {
	if (reverse) {

		//Se il treno viaggia invertito, devo correggere gli indici in modo da partire dalla fine
		iteratore_stazioni = --(Stazioni.end());
		posizione = (*iteratore_stazioni)->getDistance();
		indice_orario = Orari.size() - 1;
	}
	orario_partenza = Orari[indice_orario];
}


//******************************Getters e Setters******************************************************************

int Treno::get_id() const {
	return identificativo;
}

int Treno::get_velocita() const {
	return velocita;
}

double Treno::get_posizione() const {
	return posizione;
}

int Treno::get_ritardo() const {
	return ritardo;
}

bool Treno::isReverse() const {
	return reverse;
}

int Treno::get_orario() const {
	return orario_partenza;
}

Stato Treno::get_stato() const{
	return stato;
}

void Treno::set_velocita(int v) {
	if (v < 0)
		throw std::invalid_argument("La velocita non può essere negativa");
	if (v == 0) {
		velocita = 0;
		cambia_stato(attesa);
	}
	else {
		velocita = v;
	}
}



//******************************Routine e subroutine principali del treno*******************************************

//Routine: il treno esegue alcune azioni in base allo stato in cui si trova
void Treno::esegui() {
	switch (stato){
	case creato:
		throw std::logic_error("Impossibile far eseguire operazioni ad un treno creato ma non attivato");
		break;
	case attesa:
		//Se il treno è in attesa, non fa assolutamente nulla, ma avvisa di essere fermo
		std::cout << "Il treno " << identificativo << " e' fermo in attesa al chilometro " << posizione << std::endl;
	case fermata:
		//Se sono fermo alla fermata, devo controllare se è trascorso abbastanza tempo da poter proseguire e calcolare l'eventuale ritardo
		aggiorna_fermata();
		break;
	case stazione:
		//Se sono in una zona stazione, continuo ad avanzare a velocità limitata
		avanza();
		//Se non mi sono ancora fermato, controllo di non essere arrivato alla fermata. Altrimenti controllo di non essere uscito dalla zona stazione
		if (!fermata_effettuata)
			testa_fermata();
		else
			testa_uscita_stazione();
		break;
	case transito:
		//Se sono in transito non ho restrizioni sulla velocità e posso continuare ad avanzare
		avanza();
		//Avviso in caso di superamento della stazione
		testa_transito();
		//Devo controllare però di non essere uscito dalla zona stazione
		testa_uscita_stazione();
		break;
	case parcheggio:
		//Se il treno è in parcheggio deve verificare se può uscirne chiamando la stazione
		chiama_stazione();
		break;
	case movimento:
		//Se il treno è in movimento, allora deve continuare a muoversi secondo la propria velocità
		avanza();
		//Deve controllare se annunciarsi alla stazione (se non lo ha già fatto)
		if (!annunciato)
			pre_chiamata();
		//Anche se si è annunciato alla stazione deve comunque proseguire indipendetemente dall'esito dell'annuncio.
		//A 5 km dalla stazione il treno effettua una nuova chiamata alla stazione, ed è a questo punto che decide  
		//se parcheggiarsi o continuare, a seconda dell'esito della nuova chiamata, che potrebbe essere diverso dall'esito dell'annuncio
		testa_ingresso_stazione();
		break;
	case distrutto:
		throw std::logic_error("Errore. Il treno è distrutto e non può eseguire nessuna azione");
		break;
	default:
		break;
	}
	orario++;	//Trascorre il minuto
}

//Subroutine: cambia lo stato del treno e regola la velocità
void Treno::cambia_stato(Stato s) {
	switch (s) {
	case creato:
		throw std::logic_error("Impossibile cambiare lo stato in creato. Questo stato è possibile solo dopo la chiamata al costruttore");
		break;
	case attesa:
		velocita = 0;
		std::cout << "Il treno " << identificativo << " e' stato messo in attesa" << std::endl;
		break;
	case stazione:
		velocita = 80;
		break;
	case transito:
		set_velocita();
		break;
	case movimento:
		set_velocita();
		break;
	case parcheggio:
		velocita = 0;
		break;
	case fermata:
		velocita = 0;
		break;
	case distrutto:
		velocita = 0;
		break;
	default:
		break;
	}
}



//*****************************Principali operazioni del treno******************************************************

//Sposta il treno a seconda della direzione in cui viaggia
void Treno::avanza(){
	//Controllo in che direzione si muove il treno
	if (reverse)
		sposta_indietro();
	else
		sposta_avanti();
}

//Controlla se il treno è entrato in zona stazione. A seguito del controllo lo stato del treno sarà uno tra: {movimento, stazione, transito, parcheggio}
void Treno::testa_ingresso_stazione() {
	if (reverse) {
		if (posizione <= (*iteratore_stazioni)->getDistance() + 5)
			chiama_stazione();
	}
	else {
		if (posizione >= (*iteratore_stazioni)->getDistance() - 5)
			chiama_stazione();
	}
}

//TODO: Controlla il semaforo prima di uscire
//Controlla se il treno è uscito dalla zona stazione. A seguito del controllo lo stato del treno sarà uno tra {movimento, stazione, transito, attesa}
void Treno::testa_uscita_stazione() {
	if (reverse) {
		if (posizione <= (*iteratore_stazioni)->getDistance() - 5) {
			//Sto uscendo dalla stazione, mi rimetto in movimento e libero il binario
			libera_binario();
			cambia_stato(movimento);
			aggiorna_indici();
			fermata_effettuata = false;
			annunciato = false;
		}
	}
	else {
		if (posizione >= (*iteratore_stazioni)->getDistance() + 5) {
			libera_binario();
			cambia_stato(movimento);
			aggiorna_indici();
			fermata_effettuata = false;
			annunciato = false;
		}
	}
}

//Controlla se il treno è arrivato alla banchina. Viene chiamato solo se il treno è in modalità stazione. A seguito del controllo lo stato del treno sarà uno tra: {stazione, fermata}
void Treno::testa_fermata() {
	//Controllo come si muove il treno
	if (reverse) {
		if (posizione <= (*iteratore_stazioni)->getDistance()) {
			//Sono arrivato alla banchina, devo effettuare la fermata
			effettua_fermata();
			//A questo punto posso calcolare che ritardo ho sulla tabella di marcia
			calcola_ritardo();
			//Anche se il treno si è fermato più avanti, virtualmente si trova al km della stazione
			posizione = (*iteratore_stazioni)->getDistance();
		}
	}
	else {
		if (posizione >= (*iteratore_stazioni)->getDistance()) {
			//Sono arrivato alla banchina, devo effettuare la fermata
			effettua_fermata();
			//A questo punto posso calcolare che ritardo ho sulla tabella di marcia
			calcola_ritardo();
			//Anche se il treno si è fermato più avanti, virtualmente si trova al km della stazione
			posizione = (*iteratore_stazioni)->getDistance();
		}
	}
}

//Controlla se il treno ha superato la stazione lungo il binario di transito
void Treno::testa_transito(){
	if (reverse) {
		if (posizione <= (*iteratore_stazioni)->getDistance()) {
			std::cout << "Il treno " << identificativo << " in transito, ha superato la stazione " << (*iteratore_stazioni)->getNome() << std::endl;
			//Controllo se questa è la stazione capolinea
			if (iteratore_stazioni == Stazioni.begin()) {
				cambia_stato(distrutto);
				std::cout << "Il treno " << identificativo << " ha superato il capolinea ed è stato distrutto";
				libera_binario();
			}			
		}
	}
	else{
		if (posizione >= (*iteratore_stazioni)->getDistance()) {
			std::cout << "Il treno " << identificativo << " in transito, ha superato la stazione " << (*iteratore_stazioni)->getNome() << std::endl;
			if (std::next(iteratore_stazioni) == Stazioni.end()) {
				std::cout << "Il treno " << identificativo << " ha superato il capolinea ed è stato distrutto";
				libera_binario();
			}
		}
	}
}

//Conta i minuti di fermata. Trascorsi 5 minuti, imposta lo stato a stazione e fa ripartire il treno
void Treno::aggiorna_fermata() {
	//Se sono già stato fermo 5 minuti
	if (minuti_fermata >= 4) {
		//Azzero i minuti di fermata e cambio lo stato del treno da fermata a stazione
		minuti_fermata = 0;
		cambia_stato(stazione);
		return;
	}
	//Altrimenti aumento di un minuto il tempo
	minuti_fermata++;
}



//*****************************Funzioni di appoggio*****************************************************************

//Sposta il treno avanti
void Treno::sposta_avanti() {
	//Aggiorno la posizione del treno, convertendo la velocita da km/h a km/minuto
	posizione = posizione + (velocita / 60.0);
}

//Sposta il treno indietro
void Treno::sposta_indietro() {
	//Aggiorno la posizione del treno, convertendo la velocita da km/h a km/minuto
	posizione = posizione - (velocita / 60.0);
}

//Libera il binario su cui si trovava il treno
void Treno::libera_binario() {
	//Quale tipo di binario devo liberare?
	if (stato == transito)
		(*iteratore_stazioni)->liberaBinarioTransito(std::shared_ptr<Treno>(this));
	else
		(*iteratore_stazioni)->liberaBinarioStazionamento(std::shared_ptr<Treno>(this));
}

//Aggiorna l'indice del vettore di orari e dell'itetratore di stazioni
void Treno::aggiorna_indici() {
	if (reverse) {
		indice_orario--;
		iteratore_stazioni--;
	}
	else {
		indice_orario++;
		iteratore_stazioni++;
	}
}

//Ferma il treno alla banchina
void Treno::effettua_fermata() {
	cambia_stato(fermata);
	std::cout << "Il treno " << identificativo << " e' arrivato alla stazione " << (*iteratore_stazioni)->getNome() << " ai minuti " << orario << std::endl;
	fermata_effettuata = true;
	//Controllo se era l'ultima fermata
	if (reverse) {
		if (iteratore_stazioni == Stazioni.begin()) {
			std::cout << "Il treno " << identificativo << " è arrivato alla fermata finale e verrà distrutto" << std::endl;
			cambia_stato(distrutto);
			libera_binario();
		}
	}
	else {
		if (std::next(iteratore_stazioni, 1) == Stazioni.end()) {
			std::cout << "Il treno " << identificativo << " è arrivato alla fermata finale e verrà distrutto" << std::endl;
			cambia_stato(distrutto);
			libera_binario();
		}
	}
}

//Calcola il ritardo con cui il treno è arrivato alla fermata
void Treno::calcola_ritardo() {
	//Devo calcoare l'eventuale ritardo/anticipo del treno
	int previsto = Orari[indice_orario];
	if (previsto - orario == ritardo)
		return; //Il ritardo non è variato
	ritardo = previsto - orario;
	if (ritardo > 0)
		std::cout << "Il treno " << identificativo << " è in ritardo di " << ritardo << " minuti alla stazione " << (*iteratore_stazioni)->getNome();
	if (ritardo < 0)
		std::cout << "Il treno " << identificativo << " è in anticipo di " << -ritardo << " minuti alla stazione " << (*iteratore_stazioni)->getNome();
	return; //Altrimenti se ritardo = 0, non c'è annuncio ritardo
}



//*****************************Funzioni di prenotazione dei binari**************************************************

//Prenota un binario per la partenza del treno dalla stazione di partenza
void Treno::partenza(bool trans) {
	if (trans) {
		if (!((*iteratore_stazioni)->isFreePass(std::shared_ptr<Treno>(this))))
			throw std::logic_error("Errore. Si sta cercando di far transitare se");
		(*iteratore_stazioni)->PrenotaTransito(std::shared_ptr<Treno>(this));
		cambia_stato(transito);
	}
	else {
		if (!((*iteratore_stazioni)->isFreeStop(shared_from_this())))
			throw std::logic_error("Errore. Si sta cercando di far partire un treno senza che vi siano binari disponibili");
		(*iteratore_stazioni)->PrenotaStazionamento(std::shared_ptr<Treno>(this));
		cambia_stato(stazione);
	}
}

//Prenota un binario di fermata
void Treno::prenota_fermata(){
	if ((*iteratore_stazioni)->isFreeStop(std::shared_ptr<Treno>(this))) {
		//Se il binario è disponibile, lo prenoto ed entro in stazione
		(*iteratore_stazioni)->PrenotaStazionamento(std::shared_ptr<Treno>(this));
		cambia_stato(stazione);
	}
	else if (stato != parcheggio){
		//Altrimenti mi metto in parcheggio (se non lo ero già)
		(*iteratore_stazioni)->PrenotaDeposito(std::shared_ptr<Treno>(this));
		cambia_stato(parcheggio);
		//Sposto il treno al chilometro del parcheggio, anche se la chiamata avviene oltre
		if (reverse)
			posizione = (*iteratore_stazioni)->getDistance() + 5;
		else
			posizione = (*iteratore_stazioni)->getDistance() - 5;
	}
}

//Prenota un binario di transito
void Treno::prenota_transito(){
	//Se il binario è disponibile, lo prenoto ed entro in transito
	if ((*iteratore_stazioni)->isFreePass(std::shared_ptr<Treno>(this))) {
		(*iteratore_stazioni)->PrenotaTransito(std::shared_ptr<Treno>(this));
		cambia_stato(transito);
	}
	else if (stato != parcheggio) {
		//Altrimenti mi metto in parcheggio, ma solo se non lo ero già
		(*iteratore_stazioni)->PrenotaDeposito(std::shared_ptr<Treno>(this));
		cambia_stato(parcheggio);
		//Sposto il treno al chilometro del parcheggio, anche se la chiamata avviene oltre
		if (reverse)
			posizione = (*iteratore_stazioni)->getDistance() + 5;
		else
			posizione = (*iteratore_stazioni)->getDistance() - 5;
	}
}



//*****************************Operatori utili**********************************************************************

bool Treno::operator==(const Treno& treno) const{
	return identificativo == treno.identificativo;
}



//-------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------



Regionale::Regionale(int id, const std::list<std::shared_ptr<Stazione>>& Stazioni, std::vector<int> Orari, bool reverse)
	: Treno(id, Stazioni, Orari, reverse){
}

void Regionale::attiva(int orario){
	if (attivato)
		throw std::logic_error("Errore. Si sta attivando un treno già attivato");
	//Devo cercare un binario libero
	partenza();
	//Fingo di aver già effettuato la fermata
	stato = fermata;
	minuti_fermata = 4;
	//Imposto il treno come già attivato
	attivato = true;
}

void Regionale::set_velocita(int v){
	if (v > MAX_SPEED)
		v = MAX_SPEED;
	Treno::set_velocita(v);
}

void Regionale::pre_chiamata() {
	if (reverse) {
		if (posizione <= (*iteratore_stazioni)->getDistance() + 20) {
			if ((*iteratore_stazioni)->isFreeStop(std::shared_ptr<Treno>(this)))
				std::cout << "Il treno regionale " << identificativo << " ha richiesto un binario di fermata presso la stazione " << (*iteratore_stazioni)->getNome() << " con esito positivo" << std::endl;
			else
				std::cout << "Il treno regionale " << identificativo << " ha richiesto un binario di fermata presso la stazione " << (*iteratore_stazioni)->getNome() << " con esito negativo" << std::endl;
			annunciato = true;
		}
	}
	else {
		if (posizione >= (*iteratore_stazioni)->getDistance() - 20) {
			if ((*iteratore_stazioni)->isFreeStop(std::shared_ptr<Treno>(this)))
				std::cout << "Il treno regionale " << identificativo << " ha richiesto un binario di fermata presso la stazione " << (*iteratore_stazioni)->getNome() << " con esito positivo" << std::endl;
			else
				std::cout << "Il treno regionale " << identificativo << " ha richiesto un binario di fermata presso la stazione " << (*iteratore_stazioni)->getNome() << " con esito negativo" << std::endl;
			annunciato = true;
		}
	}
}

void Regionale::chiama_stazione(){
	//Il treno si deve fermare sempre
	prenota_fermata();
}

int Regionale::max_speed() {
	return MAX_SPEED;
}



AltaVelocita::AltaVelocita(int id, const std::list<std::shared_ptr<Stazione>>& Stazioni, std::vector<int> Orari, bool reverse)
	: Treno(id, Stazioni, Orari, reverse){
}

void AltaVelocita::attiva(int orario){
	//Verifico da che tipo di stazione sto partendo
	if ((*iteratore_stazioni)->isPrincipale()) {
		//Sto partendo da una stazione principale. Devo prenotare un binario di transito
		partenza();
		//Fingo di aver già effettuato la fermata
		stato = fermata;
		minuti_fermata = 4;
	}
	else {
		//Sto partendo da una stazione locale. Devo prenotare un binario di transito
		partenza(true);
	}
	attivato = true;
}

void AltaVelocita::set_velocita(int v){
	if (v > MAX_SPEED)
		v = MAX_SPEED;
	Treno::set_velocita(v);
}

void AltaVelocita::pre_chiamata() {
	if ((*iteratore_stazioni)->isPrincipale()) {
		if (reverse) {
			if (posizione <= (*iteratore_stazioni)->getDistance() + 20) {
				if ((*iteratore_stazioni)->isFreeStop(std::shared_ptr<Treno>(this)))
					std::cout << "Il treno Alta Velocita' " << identificativo << " ha richiesto un binario di fermata presso la stazione " << (*iteratore_stazioni)->getNome() << " con esito positivo" << std::endl;
				else
					std::cout << "Il treno Alta Velocita' " << identificativo << " ha richiesto un binario di fermata presso la stazione " << (*iteratore_stazioni)->getNome() << " con esito negativo" << std::endl;
				annunciato = true;
			}
		}
		else {
			if (posizione >= (*iteratore_stazioni)->getDistance() - 20) {
				if ((*iteratore_stazioni)->isFreeStop(std::shared_ptr<Treno>(this)))
					std::cout << "Il treno Alta Velocita' " << identificativo << " ha richiesto un binario di fermata presso la stazione " << (*iteratore_stazioni)->getNome() << " con esito positivo" << std::endl;
				else
					std::cout << "Il treno Alta Velocita' " << identificativo << " ha richiesto un binario di fermata presso la stazione " << (*iteratore_stazioni)->getNome() << " con esito negativo" << std::endl;
				annunciato = true;
			}
		}
	}
	else {
		if (reverse) {
			if (posizione <= (*iteratore_stazioni)->getDistance() + 20) {
				if ((*iteratore_stazioni)->isFreePass(std::shared_ptr<Treno>(this)))
					std::cout << "Il treno Alta Velocita' " << identificativo << " ha richiesto un binario di transito presso la stazione " << (*iteratore_stazioni)->getNome() << " con esito positivo" << std::endl;
				else
					std::cout << "Il treno regionale " << identificativo << " ha richiesto un binario di transito presso la stazione " << (*iteratore_stazioni)->getNome() << " con esito negativo" << std::endl;
			}
		}
		else {
			if (posizione >= (*iteratore_stazioni)->getDistance() - 20) {
				if ((*iteratore_stazioni)->isFreePass(std::shared_ptr<Treno>(this)))
					std::cout << "Il treno Alta Velocita' " << identificativo << " ha richiesto un binario di transito presso la stazione " << (*iteratore_stazioni)->getNome() << " con esito positivo" << std::endl;
				else
					std::cout << "Il treno Alta Velocita' " << identificativo << " ha richiesto un binario di transito presso la stazione " << (*iteratore_stazioni)->getNome() << " con esito negativo" << std::endl;
			}
		}
	}
	
}

void AltaVelocita::chiama_stazione(){
	//Il treno si deve fermare solo se è una stazione pricipale
	if ((*iteratore_stazioni)->isPrincipale()) {
		prenota_fermata();
	}
	else {
		//Se la stazione non è principale, il treno richiede il transito
		prenota_transito();
	}
}

int AltaVelocita::max_speed() {
	return MAX_SPEED;
}



SuperVelocita::SuperVelocita(int id, const std::list<std::shared_ptr<Stazione>>& Stazioni, std::vector<int> Orari, bool reverse)
	: Treno(id, Stazioni, Orari, reverse){
}

void SuperVelocita::attiva(int orario){
	//Verifico da che tipo di stazione sto partendo
	if ((*iteratore_stazioni)->isPrincipale()) {
		//Sto partendo da una stazione principale. Devo prenotare un binario di transito
		partenza();
		//Fingo di aver già effettuato la fermata
		stato = fermata;
		minuti_fermata = 4;
	}
	else {
		//Sto partendo da una stazione locale. Devo prenotare un binario di transito
		partenza(true);
	}
	attivato = true;
}

void SuperVelocita::set_velocita(int v){
	if (v > MAX_SPEED)
		v = MAX_SPEED;
	Treno::set_velocita(v);
}

void SuperVelocita::pre_chiamata() {
	if ((*iteratore_stazioni)->isPrincipale()) {
		if (reverse) {
			if (posizione <= (*iteratore_stazioni)->getDistance() + 20) {
				if ((*iteratore_stazioni)->isFreeStop(std::shared_ptr<Treno>(this)))
					std::cout << "Il treno Alta Velocita' " << identificativo << " ha richiesto un binario di fermata presso la stazione " << (*iteratore_stazioni)->getNome() << " con esito positivo" << std::endl;
				else
					std::cout << "Il treno Alta Velocita' " << identificativo << " ha richiesto un binario di fermata presso la stazione " << (*iteratore_stazioni)->getNome() << " con esito negativo" << std::endl;
				annunciato = true;
			}
		}
		else {
			if (posizione >= (*iteratore_stazioni)->getDistance() - 20) {
				if ((*iteratore_stazioni)->isFreeStop(std::shared_ptr<Treno>(this)))
					std::cout << "Il treno Alta Velocita' " << identificativo << " ha richiesto un binario di fermata presso la stazione " << (*iteratore_stazioni)->getNome() << " con esito positivo" << std::endl;
				else
					std::cout << "Il treno Alta Velocita' " << identificativo << " ha richiesto un binario di fermata presso la stazione " << (*iteratore_stazioni)->getNome() << " con esito negativo" << std::endl;
				annunciato = true;
			}
		}
	}
	else {
		if (reverse) {
			if (posizione <= (*iteratore_stazioni)->getDistance() + 20) {
				if ((*iteratore_stazioni)->isFreePass(std::shared_ptr<Treno>(this)))
					std::cout << "Il treno Alta Velocita' " << identificativo << " ha richiesto un binario di transito presso la stazione " << (*iteratore_stazioni)->getNome() << " con esito positivo" << std::endl;
				else
					std::cout << "Il treno regionale " << identificativo << " ha richiesto un binario di transito presso la stazione " << (*iteratore_stazioni)->getNome() << " con esito negativo" << std::endl;
				annunciato = true;
			}
		}
		else {
			if (posizione >= (*iteratore_stazioni)->getDistance() - 20) {
				if ((*iteratore_stazioni)->isFreePass(std::shared_ptr<Treno>(this)))
					std::cout << "Il treno Alta Velocita' " << identificativo << " ha richiesto un binario di transito presso la stazione " << (*iteratore_stazioni)->getNome() << " con esito positivo" << std::endl;
				else
					std::cout << "Il treno Alta Velocita' " << identificativo << " ha richiesto un binario di transito presso la stazione " << (*iteratore_stazioni)->getNome() << " con esito negativo" << std::endl;
				annunciato = true;
			}
		}
	}
}

void SuperVelocita::chiama_stazione(){
	//Il treno si deve fermare solo se è una stazione pricipale
	if ((*iteratore_stazioni)->isPrincipale()) {
		prenota_fermata();
	}
	else {
		//Se la stazione non è principale, il treno richiede il transito
		prenota_transito();
	}
}

int SuperVelocita::max_speed() {
	return MAX_SPEED;
}