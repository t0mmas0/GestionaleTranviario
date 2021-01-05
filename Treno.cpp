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
		//TODO: Controllo al rovescio
		if (posizione >= (*iteratore_stazioni)->getDistance() - 5)
			//Devo limitare la velocita e chiamare la stazione
			velocita = 80;
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

//TODO: Il treno avanza anche se deve percorrere la tratta al contrario - decidere se rovesciare i km delle stazioni
void Treno::avanza(){
	//Aggiorno la posizione del treno, convertendo la velocita da km/h a km/minuto
	posizione = posizione + (velocita / 60);
	//Si suppone che la lunghezza della banchina sia infinita, pertanto il treno può fermarsi anche dopo il km della stazione. Poil il treno ripenderà a muoversi dal km della stazione
	if (posizione >= (*iteratore_stazioni)->getDistance())
		cambia_stato(fermata);
}

 
void Treno::aggiorna_fermata(){
	if (minuti_fermata == 4) {
		//Sono già stato fermo 5 minuti, devo ripartire dal km della stazione (anche se il treno si era fermato più avanti)
		minuti_fermata = 0;
		posizione = (*iteratore_stazioni)->getDistance();
		
		//Richiedo l'uscita dalla stazione
		(*iteratore_stazioni)->liberaBinario(this);

		//Cambio lo stato del treno
		cambia_stato(stazione);

		//Ho effettuato la fermata, devo aggiornare gli indici
		iteratore_stazioni++;
		indice_orario++;

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

void Treno::chiama_stazione(){
	//TODO: verificare che il treno debba fermarsi
	if (!((*iteratore_stazioni)->PrenotaBinario(this))) //TODO Passaggio per puntatore o per riferimento?
		cambia_stato(parcheggio);
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
		throw std::invalid_argument("La velocità non può essere negativa");
	if (v == 0) {
		velocita = 0;
		stato = attesa;
	}
	else {
		velocita = v;
		stato = movimento;
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

AltaVelocità::AltaVelocità(int id, std::list<std::shared_ptr<Stazione>>& Stazioni, std::vector<int>& Orari, bool reverse)
	: Treno(id, Stazioni, Orari, reverse){
}

void AltaVelocità::set_velocita(int v){
	if (v > MAX_SPEED)
		v = MAX_SPEED;
	Treno::set_velocita(v);
}

SuperVelocità::SuperVelocità(int id, std::list<std::shared_ptr<Stazione>>& Stazioni, std::vector<int>& Orari, bool reverse)
	: Treno(id, Stazioni, Orari, reverse){
}

void SuperVelocità::set_velocita(int v){
	if (v > MAX_SPEED)
		v = MAX_SPEED;
	Treno::set_velocita(v);
}
