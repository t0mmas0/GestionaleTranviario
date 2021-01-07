//Simone Peraro 1216334
#include <iostream>
#include <cmath>
#include <stdexcept>
#include "Treno.h"
#include "Stazione.h"

Treno::Treno(int id, const std::list<std::shared_ptr<Stazione>>& Stazioni, std::vector<int>& Orari, bool reverse)
	: orario_partenza{ 0 }, orario{ 0 }, identificativo{ id }, velocita{ 0 }, posizione{ 0 }, ritardo{ 0 }, minuti_fermata{ 0 }, stato{ creato }, Stazioni{ Stazioni }, iteratore_stazioni{ Stazioni.begin() }, Orari{ Orari }, indice_orario{ 0 }, attivato{ false }, reverse{ reverse }, velocita_limitata{ false }, fermata_effettuata{ false } {
	if (reverse) {
		//Se il treno viaggia invertito, devo correggere gli indici in modo da partire dalla fine
		iteratore_stazioni = --(Stazioni.end());
		posizione = (*iteratore_stazioni)->getDistance();
		indice_orario = Orari.size() - 1;
	}
	orario_partenza = Orari[indice_orario];
}

Treno::Treno(const Treno& treno)
	: orario{ treno.orario }, identificativo{ treno.identificativo }, velocita{ treno.velocita }, posizione{ treno.posizione }, ritardo{ treno.ritardo }, minuti_fermata{ treno.minuti_fermata }, stato{ treno.stato }, Stazioni{ treno.Stazioni }, iteratore_stazioni{ treno.iteratore_stazioni }, Orari{ treno.Orari }, indice_orario{ treno.indice_orario }, attivato{ false }, reverse{ treno.reverse }{
}

//Il treno esegue automaticamente alcune cose in base al proprio stato attuale
void Treno::esegui() {
	switch (stato){
	case creato:
		std::logic_error("Impossibile far eseguire operazioni ad un treno creato ma non attivato");
		break;
	case attesa:
		//Se il treno è in attesa, non fa assolutamente nulla, ma avvisa di essere fermo
		std::cout << "Il treno " << identificativo << " e' fermo in attesa al chilometro " << posizione << std::endl;
	case fermata:
		//Se sono fermo alla fermata, devo controllare se è trascorso abbastanza tempo da poter proseguire e calcolare l'eventuale ritardo
		aggiorna_fermata();
		calcola_ritardo();
		break;
	case stazione:
		//Se sono in una zona stazione, continuo ad avanzare a velocità limitata
		avanza(80);
		//Se non mi sono ancora fermato, controllo di non essere arrivato alla fermata. Altrimenti controllo di non essere uscito dalla zona stazione
		if (!fermata_effettuata)
			testa_fermata();
		else
			testa_uscita_stazione();
		break;
	case transito:
		//Se sono in transito non ho restrizioni sulla velocità e posso continuare ad avanzare
		avanza();
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
		//Deve poi controllare se deve chiamare la stazione
		testa_ingresso_stazione();
		break;
	case distrutto:
		std::logic_error("Errore. Il treno è distrutto e non può eseguire nessuna azione");
		break;
	default:
		break;
	}
	orario++;
}

void Treno::avanza(int v){
	//Controllo se ho ricevuto precedenti limitazioni sulla velocità - in tal caso rimangono valide, ma solo se è minore di quella prevista.
	if (velocita_limitata) {
		if (velocita < v)
			v = velocita;
	}
	//Controllo in che direzione si muove il treno
	if (reverse)
		sposta_indietro(v);
	else
		sposta_avanti(v);
}

void Treno::sposta_avanti(int v){
	//Aggiorno la posizione del treno, convertendo la velocita da km/h a km/minuto
	posizione = posizione + (v / 60.0);
}
 
void Treno::sposta_indietro(int v){
	//Aggiorno la posizione del treno, convertendo la velocita da km/h a km/minuto
	posizione = posizione - (v / 60.0);
}

void Treno::testa_ingresso_stazione(){
	if (reverse) {
		if (posizione <= (*iteratore_stazioni)->getDistance() + 5) {
			chiama_stazione();
		}
	}
	else {
		if (posizione >= (*iteratore_stazioni)->getDistance() - 5)
			chiama_stazione();
	}
}

void Treno::testa_uscita_stazione(){
	if (reverse) {
		if (posizione <= (*iteratore_stazioni)->getDistance() - 5) {
			//Sto uscendo dalla stazione, mi rimetto in movimento e libero il binario
			libera_binario();
			cambia_stato(movimento);
			aggiorna_indici();
			fermata_effettuata = false;
		}
	}
	else {
		if (posizione >= (*iteratore_stazioni)->getDistance() + 5) {
			libera_binario();
			cambia_stato(movimento);
			aggiorna_indici();
			fermata_effettuata = false;
		}
	}
}

void Treno::testa_fermata(){
	//Controllo come si muove il treno
	if (reverse) {
		if (posizione <= (*iteratore_stazioni)->getDistance()) {
			//Sono arrivato alla banchina, devo effettuare la fermata
			effettua_fermata();
		}
	}
	else {
		if (posizione >= (*iteratore_stazioni)->getDistance()) {
			//Sono arrivato alla banchina, devo effettuare la fermata
			effettua_fermata();
		}
	}
}

void Treno::effettua_fermata(){
	cambia_stato(fermata);
	fermata_effettuata = true;
}

void Treno::aggiorna_fermata(){
	//Se sono già stato fermo 5 minuti
	if (minuti_fermata >= 4) {
		//Devo ripartire dal km della stazione (anche se il treno si era fermato più avanti, si ricongiunge alla linea al km di posizione della stazione)
		minuti_fermata = 0;
		posizione = (*iteratore_stazioni)->getDistance();

		//Cambio lo stato del treno
		cambia_stato(stazione);
		return;
	}
	//Altrimenti aggiorno il tempo
	minuti_fermata++;
}

//TODO: gestire le velocità limitate
void Treno::cambia_stato(Stato s){
	switch (s){
	case creato:
		std::logic_error("Impossibile cambiare lo stato in creato. Questo stato è possibile solo dopo la chiamata al costruttore");
		break;
	case attesa:
		velocita = 0;
		std::cout << "Il treno " << identificativo << " e' stato messo in attesa" << std::endl;
		break;
	case stazione:
 		break;
	case transito:
		break;
	case movimento:
		break;
	case parcheggio:
		break;
	case fermata:
		break;
	case distrutto:
		break;
	default:
		break;
	}
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

void Treno::prenota_fermata(){
	if ((*iteratore_stazioni)->isFreeStop()) {
		//Se il binario è disponibile, lo prenoto ed entro in stazione
		(*iteratore_stazioni)->PrenotaStazionameto(this);
		cambia_stato(stazione);
	}
	else if (stato != parcheggio){
		//Altrimenti mi metto in parcheggio (se non lo ero già)
		(*iteratore_stazioni)->PrenotaDeposito(this);
		cambia_stato(parcheggio);
	}
}

void Treno::prenota_transito(){
	//Se il binario è disponibile, lo prenoto ed entro in transito
	if ((*iteratore_stazioni)->isFreePass()) {
		(*iteratore_stazioni)->PrenotaTransito(this);
		cambia_stato(transito);
	}
	else if (stato != parcheggio) {
		//Altrimenti mi metto in parcheggio, ma solo se non lo ero già
		(*iteratore_stazioni)->PrenotaDeposito(this);
		cambia_stato(parcheggio);
	}
}

void Treno::libera_binario(){
	//Quale tipo di binario devo liberare?
	if (stato == transito)
		(*iteratore_stazioni)->liberaBinarioTransito(this);
	else
		(*iteratore_stazioni)->liberaBinarioStazionamento(this);
}

void Treno::partenza(bool transito){
	if (transito) {
		if (!((*iteratore_stazioni)->isFreePass()))
			std::logic_error("Errore. Si sta cercando di far transitare");
		(*iteratore_stazioni)->PrenotaTransito(this);
		cambia_stato(transito);
	}
	else {
		if (!((*iteratore_stazioni)->isFreeStop()))
			std::logic_error("Errore. Si sta cercando di far partire un treno senza che vi siano binari disponibili");
		(*iteratore_stazioni)->PrenotaStazionameto(this);
		cambia_stato(stazione);
	}
}

void Treno::aggiorna_indici(){
	if (reverse) {
		indice_orario--;
		iteratore_stazioni--;
	}
	else {
		indice_orario++;
		iteratore_stazioni++;
	}
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

bool Treno::isReverse() const{
	return reverse;
}

int Treno::get_orario() const{
	return orario_partenza;
}

void Treno::set_velocita(int v){
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

Regionale::Regionale(int id, const std::list<std::shared_ptr<Stazione>>& Stazioni, std::vector<int>& Orari, bool reverse)
	: Treno(id, Stazioni, Orari, reverse){
}

void Regionale::attiva(int orario){
	if (attivato)
		std::logic_error("Errore. Si sta attivando un treno già attivato");
	//Devo cercare un binario libero
	partenza();
	//Fingo di aver già effettuato la fermata
	stato = fermata;
	minuti_fermata = 4;
	//Imposto il treno come già attivato
	attivato = true;
}

void Regionale::set_velocita(int v){
	if (v >= MAX_SPEED)
		velocita_limitata = false;
	if (v < MAX_SPEED)
		velocita_limitata = true;
	if (v > MAX_SPEED)
		v = MAX_SPEED;
	Treno::set_velocita(v);
}

void Regionale::avanza(int v){
	if (v > MAX_SPEED)
		v = MAX_SPEED;
	Treno::avanza(v);
}

void Regionale::chiama_stazione(){
	//Il treno si deve fermare sempre
	prenota_fermata();
}

AltaVelocita::AltaVelocita(int id, const std::list<std::shared_ptr<Stazione>>& Stazioni, std::vector<int>& Orari, bool reverse)
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
	if (v >= MAX_SPEED)
		velocita_limitata = false;
	if (v < MAX_SPEED)
		velocita_limitata = true;
	if (v > MAX_SPEED)
		v = MAX_SPEED;
	Treno::set_velocita(v);
}

void AltaVelocita::avanza(int v){
	if (v > MAX_SPEED)
		v = MAX_SPEED;
	Treno::avanza(v);
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

SuperVelocita::SuperVelocita(int id, const std::list<std::shared_ptr<Stazione>>& Stazioni, std::vector<int>& Orari, bool reverse)
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
	if (v >= MAX_SPEED)
		velocita_limitata = false;
	if (v < MAX_SPEED)
		velocita_limitata = true;
	if (v > MAX_SPEED)
		v = MAX_SPEED;
	Treno::set_velocita(v);
}

void SuperVelocita::avanza(int v){
	if (v > MAX_SPEED)
		v = MAX_SPEED;
	Treno::avanza(v);
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
