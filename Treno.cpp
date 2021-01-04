//Simone Peraro 1216334
#include <cmath>
#include <stdexcept>
#include "Treno.h"
#include "Stazione.h"

Treno::Treno(int id, std::list<Stazione>& Stazioni, std::vector<int>& Orari)
	:identificativo{ id }, velocità{ 0 }, posizione{ 0 }, ritardo{ 0 }, minuti_fermata{ 0 }, stato{ attesa }, Stazioni{ Stazioni }, Orari{ Orari } {
}

Treno::Treno(const Treno& treno)
	: identificativo{ treno.identificativo }, velocità{ treno.velocità }, posizione{ treno.posizione }, ritardo{ treno.ritardo }, minuti_fermata{ treno.minuti_fermata }, stato{ treno.stato }, Stazioni{ treno.Stazioni }, Orari{ treno.Orari }{
}

void Treno::muta() {
	//Il treno automaticamente muta il proprio stato in base allo stato attuale
	switch (stato){
	case attesa:
		//Se il treno è in attesa (di un qualche evento esterno o di segnali da parte dei gestori), non fa nulla
		return;
	case movimento:
		//Se il treno è in movimento, allora deve continuare a muoversi secondo la propria velocità
		avanza();
		break;
	case parcheggio:
		//Se il treno è in parcheggio, allora vi rimane e non fa nulla
		return;
	case fermata:
		//Se il treno è in fermata, allora vi rimane, ma aumenta il tempo di fermata effettuato
		aggiorna_fermata();
		break;
	}
	//Se sono avanzato, devo controllare se chiamare la stazione
}

void Treno::avanza(){
	//Aggiorno la posizione del treno, convertendo la velocità da km/h a km/minuto
	//La posizione è aggiornata all'intero superiore:
	//i chilometri vanno da 0 a 1 per il primo, da 1.1 a 2 per il secondo ecc...
	//Pertanto se un treno è al km 1.2, si trova al km 2
	posizione = std::ceil(posizione + (velocità / 60));
}

void Treno::aggiorna_fermata(){
	if (minuti_fermata == 4) {
		//Sono già stato fermo 5 minuti, devo ripartire
		minuti_fermata = 0;
		//TO-DO
	}
	//Altrimenti aggiorno il tempo
	minuti_fermata++;
}

void Treno::cambia_stato(Stato s){
	stato = s;
	//Azzero la velocità se il treno viene posto in uno stato tale da renderlo immobile
	if (stato == attesa || stato == parcheggio || stato == fermata)
		velocità = 0;
}

int Treno::get_id() const {
	return identificativo;
}

int Treno::get_velocità() const {
	return velocità;
}

int Treno::get_posizione() const {
	return posizione;
}

int Treno::get_ritardo() const {
	return ritardo;
}

void Treno::set_velocità(int v){
	if (v < 0)
		throw std::invalid_argument("La velocità non può essere negativa");
	if (v == 0) {
		velocità = 0;
		stato = attesa;
	}
	else {
		velocità = v;
		stato = movimento;
	}
}

Treno& Treno::operator=(const Treno& treno){
	identificativo = treno.identificativo;
	velocità = treno.velocità,
	posizione = treno.posizione;
	ritardo = treno.ritardo;
	minuti_fermata = treno.minuti_fermata;
	stato = treno.stato;
	Stazioni = treno.Stazioni;
	Orari = treno.Orari;
	return *this;
}

bool Treno::operator==(const Treno& treno) const{
	return identificativo == treno.identificativo;
}

Regionale::Regionale(int id, std::list<Stazione>& Stazioni, std::vector<int>& Orari)
	: Treno(id, Stazioni, Orari), MAX_SPEED{160} {
}

AltaVelocità::AltaVelocità(int id, std::list<Stazione>& Stazioni, std::vector<int>& Orari)
	: Treno(id, Stazioni, Orari), MAX_SPEED{240}{
}

SuperVelocità::SuperVelocità(int id, std::list<Stazione>& Stazioni, std::vector<int>& Orari)
	: Treno(id, Stazioni, Orari), MAX_SPEED{300}{
}
