//Simone Peraro 1216334
#include <iostream>
#include <cmath>
#include <stdexcept>
#include "Treno.h"
#include "Stazione.h"

Treno::Treno(int id, std::list<std::shared_ptr<Stazione>>& Stazioni, std::vector<int>& Orari, bool reverse)
	: orario{ 0 }, identificativo{ id }, velocit�{ 0 }, posizione{ 0 }, ritardo{ 0 }, minuti_fermata{ 0 }, stato{ attesa }, Stazioni{ Stazioni }, iteratore_stazioni{ Stazioni.begin() }, Orari{ Orari }, indice_orario{ 0 }, attivato{ false }, reverse{reverse} {
}

Treno::Treno(const Treno& treno)
	: orario{ treno.orario }, identificativo{ treno.identificativo }, velocit�{ treno.velocit� }, posizione{ treno.posizione }, ritardo{ treno.ritardo }, minuti_fermata{ treno.minuti_fermata }, stato{ treno.stato }, Stazioni{ treno.Stazioni }, iteratore_stazioni{ treno.iteratore_stazioni }, Orari{ treno.Orari }, indice_orario{ treno.indice_orario }, attivato{ false }, reverse{ treno.reverse }{
}

void Treno::attiva(int ora){
	if (attivato)
		throw std::logic_error("Errore. Si sta cercando di attivare un treno gi� attivato");
	orario = ora;
	cambia_stato(attesa);
}

void Treno::muta() {
	
	//Il treno automaticamente muta il proprio stato in base allo stato attuale
	switch (stato){
	case movimento:
		//Se il treno � in movimento, allora deve continuare a muoversi secondo la propria velocit�
		avanza();
		//Deve poi controllare se deve chiamare la stazione
		//TODO: Controllo al rovescio
		if (posizione >= (*iteratore_stazioni)->Km - 5)
			//Devo limitare la velocit� e chiamare la stazione
			velocit� = 80;
			chiama_stazione();
		break;
	case fermata:
		//Se il treno � in fermata, allora vi rimane, ma aumenta il tempo di fermata effettuato
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
	//Aggiorno la posizione del treno, convertendo la velocit� da km/h a km/minuto
	posizione = posizione + (velocit� / 60);
	//Si suppone che la lunghezza della banchina sia infinita, pertanto il treno pu� fermarsi anche dopo il km della stazione. Poil il treno ripender� a muoversi dal km della stazione
	if (posizione >= (*iteratore_stazioni)->Km)
		cambia_stato(fermata);
}

 
void Treno::aggiorna_fermata(){
	if (minuti_fermata == 4) {
		//Sono gi� stato fermo 5 minuti, devo ripartire dal km della stazione (anche se il treno si era fermato pi� avanti)
		minuti_fermata = 0;
		posizione = (*iteratore_stazioni)->Km;
		
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
	//Azzero la velocit� se il treno viene posto in uno stato tale da renderlo immobile
	if (stato == attesa || stato == parcheggio || stato == fermata)
		velocit� = 0;
	//Imposto la velocit� limite se il treno entra nella zona stazione
	else if (stato == stazione)
		velocit� = 80;
	//Imposto la velocit� massima se il treno viene messo in movimento
	else if (stato = movimento)
		set_velocit�(); //Verr� chiamato il metodo override che imposter� la velocit� massima consentita dal tipo del treno
	//TODO: Stato distrutto
}

void Treno::calcola_ritardo(){
	//Devo calcoare l'eventuale ritardo/anticipo del treno
	//TODO: Per ora la funzione � pensata per calcolarlo all'arrivo in stazione, quindi si presuppone sia in stazione
	int previsto = Orari[indice_orario];
	if (previsto - orario == ritardo)
		return; //Il ritardo non � variato
	ritardo = previsto - orario;
	if (ritardo > 0)
		std::cout << "Il treno " << identificativo << " � in ritardo di " << ritardo << " minuti alla stazione " << (*iteratore_stazioni)->nome ; //TODO: Aggiungere nome stazioni
	if (ritardo < 0)
		std::cout << "Il treno " << identificativo << " � in anticipo di " << -ritardo << " minuti alla stazione " << (*iteratore_stazioni)->nome;
	return; //Altrimenti se ritardo = 0, non c'� annuncio ritardo
}

void Treno::chiama_stazione(){
	if (!((*iteratore_stazioni)->PrenotaBinario(this))) //TODO Passaggio per puntatore o per riferimento?
		cambia_stato(parcheggio);
}

int Treno::get_id() const {
	return identificativo;
}

int Treno::get_velocit�() const {
	return velocit�;
}

int Treno::get_posizione() const {
	return posizione;
}

int Treno::get_ritardo() const {
	return ritardo;
}

void Treno::set_velocit�(int v){
	if (v < 0)
		throw std::invalid_argument("La velocit� non pu� essere negativa");
	if (v == 0) {
		velocit� = 0;
		stato = attesa;
	}
	else {
		velocit� = v;
		stato = movimento;
	}
}

Treno& Treno::operator=(const Treno& treno){
	orario = treno.orario;
	identificativo = treno.identificativo;
	velocit� = treno.velocit�,
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

void Regionale::set_velocit�(int v){
	if (v > MAX_SPEED)
		v = MAX_SPEED;
	Treno::set_velocit�(v);
}

AltaVelocit�::AltaVelocit�(int id, std::list<std::shared_ptr<Stazione>>& Stazioni, std::vector<int>& Orari, bool reverse)
	: Treno(id, Stazioni, Orari, reverse){
}

void AltaVelocit�::set_velocit�(int v){
	if (v > MAX_SPEED)
		v = MAX_SPEED;
	Treno::set_velocit�(v);
}

SuperVelocit�::SuperVelocit�(int id, std::list<std::shared_ptr<Stazione>>& Stazioni, std::vector<int>& Orari, bool reverse)
	: Treno(id, Stazioni, Orari, reverse){
}

void SuperVelocit�::set_velocit�(int v){
	if (v > MAX_SPEED)
		v = MAX_SPEED;
	Treno::set_velocit�(v);
}
