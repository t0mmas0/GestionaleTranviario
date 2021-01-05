//Simone Peraro 1216334
#include <iostream>
#include <cmath>
#include <stdexcept>
#include "Treno.h"
#include "Stazione.h"

Treno::Treno(int id, std::list<std::shared_ptr<Stazione>>& Stazioni, std::vector<int>& Orari, bool reverse)
	: orario{ 0 }, identificativo{ id }, velocita{ 0 }, posizione{ 0 }, ritardo{ 0 }, minuti_fermata{ 0 }, stato{ attesa }, Stazioni{ Stazioni }, iteratore_stazioni{ Stazioni.begin() }, Orari{ Orari }, indice_orario{ 0 }, attivato{ false }, reverse{reverse} {
}

Treno::Treno(const Treno& treno)
	: orario{ treno.orario }, identificativo{ treno.identificativo }, velocita{ treno.velocita }, posizione{ treno.posizione }, ritardo{ treno.ritardo }, minuti_fermata{ treno.minuti_fermata }, stato{ treno.stato }, Stazioni{ treno.Stazioni }, iteratore_stazioni{ treno.iteratore_stazioni }, Orari{ treno.Orari }, indice_orario{ treno.indice_orario }, attivato{ false }, reverse{ treno.reverse }{
}

void Treno::attiva(int ora){
	if (attivato)
		throw std::logic_error("Errore. Si sta cercando di attivare un treno già attivato");
	orario = ora;
	cambia_stato(attesa);
	calcola_ritardo();
	//A questo punto il treno deve essere pronto a partire appena viene cambiato il suo stato. Devo aggiornare i riferimenti alla prossima stazione
	iteratore_stazioni++;
	indice_orario++;
}

void Treno::muta() {
	//Il treno automaticamente muta il proprio stato in base allo stato attuale
	switch (stato){
	case movimento:
		//Se il treno è in movimento, allora deve continuare a muoversi secondo la propria velocita
		avanza();
		//Deve poi controllare se deve chiamare la stazione
		if (reverse) {
			if (posizione <= (*iteratore_stazioni)->getDistance() + 5) {
				chiama_stazione();
			}
			break;
		}
		if (posizione >= (*iteratore_stazioni)->getDistance() - 5)
			chiama_stazione();
		break;
	case stazione:
		//Se il treno è in stazione, continua ad avanzare
		avanza();
	case fermata:
		//Se il treno è in fermata, allora vi rimane, ma aumenta il tempo di fermata effettuato
		aggiorna_fermata();
		calcola_ritardo();
		break;
	default:
		break;
	}
	orario++;
}

void Treno::avanza(){
	//Controllo se il treno viaggia a ritroso
	if (reverse) {
		//Aggiorno la posizione del treno, convertendo la velocita da km/h a km/minuto
		posizione = posizione - (velocita / 60);
		//Si suppone che la lunghezza della banchina sia infinita, pertanto il treno può fermarsi anche dopo il km della stazione. Poil il treno ripenderà a muoversi dal km della stazione
		if (posizione <= (*iteratore_stazioni)->getDistance()) {
			//Sono arrivato ad una stazione. Mi devo fermare?
			effettua_fermata();
		}
		return;
	}

	//Se non viaggia a ritroso, eseguo le stesse operazioni ma in avanti
	posizione = posizione + (velocita / 60);
	if (posizione >= (*iteratore_stazioni)->getDistance())
		effettua_fermata();
}
 
void Treno::aggiorna_fermata(){
	//Se sono già stato fermo 5 minuti
	if (minuti_fermata == 4) {
		//Sono già stato fermo 5 minuti, devo ripartire dal km della stazione (anche se il treno si era fermato più avanti, si ricongiunge alla linea al km di posizione della stazione)
		minuti_fermata = 0;
		posizione = (*iteratore_stazioni)->getDistance();
		
		//Richiedo l'uscita dalla stazione
		(*iteratore_stazioni)->liberaBinario(this);

		//Cambio lo stato del treno
		cambia_stato(stazione);

		//Ho effettuato la fermata, devo aggiornare gli indici
		if (reverse) {
			iteratore_stazioni--;
			indice_orario--;
		}
		else {
			iteratore_stazioni++;
			indice_orario++;
		}
		return;
	}
	//Altrimenti aggiorno il tempo
	minuti_fermata++;
}

void Treno::cambia_stato(Stato s){
	stato = s;
	//Azzero la velocita se il treno viene posto in uno stato tale da renderlo immobile
	if (stato == attesa || stato == parcheggio || stato == fermata)
		velocita = 0;
	//Imposto la velocita limite se il treno entra nella zona stazione
	else if (stato == stazione)
		velocita = 80;
	//Imposto la velocita massima se il treno viene messo in movimento
	else if (stato = movimento)
		set_velocita(); //Verrà chiamato il metodo override che imposterà la velocita massima consentita dal tipo del treno
	//TODO: Stato distrutto
}

void Treno::calcola_ritardo(){
	//Devo calcoare l'eventuale ritardo/anticipo del treno
	//TODO: Per ora la funzione è pensata per calcolarlo all'arrivo in stazione, quindi si presuppone sia in stazione
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

int Treno::get_id() const {
	return identificativo;
}

int Treno::get_velocita() const {
	return velocita;
}

int Treno::get_posizione() const {
	return posizione;
}

int Treno::get_ritardo() const {
	return ritardo;
}

void Treno::set_velocita(int v){
	if (v < 0)
		throw std::invalid_argument("La velocita non può essere negativa");
	if (v == 0) {

        velocita = 0;
		stato = attesa;
	}
	else {
        velocita = v;

		velocita = 0;
		stato = attesa;
	}

}

Treno& Treno::operator=(const Treno& treno){
	orario = treno.orario;
	identificativo = treno.identificativo;

    velocita = treno.velocita,

	ritardo = treno.ritardo;
	minuti_fermata = treno.minuti_fermata;

	posizione = treno.posizione;

	stato = treno.stato;

	Stazioni = treno.Stazioni;
	iteratore_stazioni = treno.iteratore_stazioni;

	Orari = treno.Orari;
	indice_orario = treno.indice_orario;
	return *this;
}

bool Treno::operator==(const Treno& treno) const{
	return identificativo == treno.identificativo;
}

Regionale::Regionale(int id, std::list<std::shared_ptr<Stazione>>& Stazioni, std::vector<int>& Orari, bool reverse)
	: Treno(id, Stazioni, Orari, reverse){
}

void Regionale::set_velocita(int v){
	if (v > MAX_SPEED)
		v = MAX_SPEED;
	Treno::set_velocita(v);
}

void Regionale::chiama_stazione(){
	//Il treno si deve fermare sempre
	if (!((*iteratore_stazioni)->PrenotaBinario(this))) //TODO Testare il passaggio, non sembra riconoscere il tipo
		cambia_stato(parcheggio);
}

void Regionale::effettua_fermata(){
	//Mi devo fermare sempre
	cambia_stato(fermata);
	//Ora controllo se questa era l'ultima fermata. In tal caso non devo aspettare 5 minuti, ma devo liberare il binario subito
	if (reverse) {
		if (iteratore_stazioni == Stazioni.begin()) {
			cambia_stato(distrutto);
			(*iteratore_stazioni)->liberaBinario(this);
			return;
		}
	}

	if (iteratore_stazioni == Stazioni.end() - 1) {
		cambia_stato(distrutto);
		(*iteratore_stazioni)->liberaBinario(this);
	}
}

AltaVelocita::AltaVelocita(int id, std::list<std::shared_ptr<Stazione>>& Stazioni, std::vector<int>& Orari, bool reverse)
	: Treno(id, Stazioni, Orari, reverse){
}

void AltaVelocita::set_velocita(int v){
	if (v > MAX_SPEED)
		v = MAX_SPEED;
	Treno::set_velocita(v);
}

void AltaVelocita::chiama_stazione(){
	//Il treno si deve fermare solo se è una stazione pricipale
	if ((*iteratore_stazioni)->isPrincipale()) {
		if (!((*iteratore_stazioni)->PrenotaBinario(this)) ) //TODO Testare il passaggio, non sembra riconoscere il tipo
			cambia_stato(parcheggio);
		return;
	}

	//Se la stazione non è principale, il treno richiede il transito. Se la richiesta va a buon fine, prosegue, altrimenti va in parcheggio
	if (!((*iteratore_stazioni)->RichiediTransito(this)))
		cambia_stato(parcheggio);

}

void AltaVelocita::effettua_fermata(){
	//Mi devo fermare solo se è una stazione principale
	if ((*iteratore_stazioni)->isPrincipale()) {
		cambia_stato(fermata);
		return;
	}
	//Controllo di non essere arrivato al capolinea
	if (reverse) {
		if (iteratore_stazioni == Stazioni.begin()) {
			cambia_stato(distrutto);
			return;
		}
	}
	//Eseguo lo stesso controllo per i treni che non viaggiano a ritroso
	if (iteratore_stazioni == Stazioni.end() - 1) {
		cambia_stato(distrutto);
		return;
	}

	//Altrimenti aggiorno i riferimenti per la prossima stazione
	iteratore_stazioni--;
	indice_orario--;
}

SuperVelocita::SuperVelocita(int id, std::list<std::shared_ptr<Stazione>>& Stazioni, std::vector<int>& Orari, bool reverse)
	: Treno(id, Stazioni, Orari, reverse){
}

void SuperVelocita::set_velocita(int v){
	if (v > MAX_SPEED)
		v = MAX_SPEED;
	Treno::set_velocita(v);
}

void SuperVelocita::chiama_stazione(){
	//Il treno si deve fermare solo se è una stazione pricipale
	if ((*iteratore_stazioni)->isPrincipale()) {
		if (!((*iteratore_stazioni)->PrenotaBinario(this))) //TODO Testare il passaggio, non sembra riconoscere il tipo
			cambia_stato(parcheggio);
		return;
	}

	//Se la stazione non è principale, il treno richiede il transito. Se la richiesta va a buon fine, prosegue, altrimenti va in parcheggio
	if (!((*iteratore_stazioni)->RichiediTransito(this)))
		cambia_stato(parcheggio);

}

void SuperVelocita::effettua_fermata(){
	//Mi devo fermare solo se è una stazione principale
	if ((*iteratore_stazioni)->isPrincipale()) {
		cambia_stato(fermata);
		return;
	}
	//Controllo di non essere arrivato al capolinea
	if (reverse) {
		if (iteratore_stazioni == Stazioni.begin()) {
			cambia_stato(distrutto);
			return;
		}
	}
	//Eseguo lo stesso controllo per i treni che non viaggiano a ritroso
	if (iteratore_stazioni == Stazioni.end() - 1) {
		cambia_stato(distrutto);
		return;
	}

	//Altrimenti aggiorno i riferimenti per la prossima stazione
	iteratore_stazioni--;
	indice_orario--;
}
