//Simone Peraro 1216334
#include <iostream>
#include <stdexcept>
#include "Treno.h"
#include <memory>
#include "Grafica.h"

//Costruttore della classe treno richiamato dalle sottoclassi. Il passaggio del vettore Orari avviene per copia poichè la funzione leggiFile della classe LeggiOrari passa una copia del vettore che in seguito viene distrutto, in modo che il compilatore effettui una copy-elision
Treno::Treno(int id, const std::list<std::shared_ptr<Stazione>>& Stazioni, std::vector<int> Orari, bool reverse)
	:std::enable_shared_from_this<Treno>(), orario_partenza{ 0 }, orario{ 0 }, identificativo{ id }, velocita{ 0 }, 
	posizione{ 0 }, ritardo{ 0 }, minuti_fermata{ 0 }, stato{ creato }, Stazioni{ Stazioni }, iteratore_stazioni{ Stazioni.begin() }, 
	Orari{ Orari }, indice_orario{ 0 }, attivato{ false }, reverse{ reverse }, fermata_effettuata{ false }, 
	annunciato{ false }, transitato{ false }, liberato{ false } {
	if (reverse) {

		//Se il treno viaggia invertito, devo correggere gli indici in modo da partire dalla fine
		iteratore_stazioni = --(Stazioni.end());
		posizione = (*iteratore_stazioni)->getDistance();
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
		Grafica::attesa(identificativo,posizione);
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
		if (!transitato)
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
		//Devo controllare se liberare il binario di uscita dalla stazione
		if (!liberato)
			libera_uscita();
		//Deve controllare se annunciarsi alla stazione (se non lo ha già fatto)
		if (!annunciato)
			pre_chiamata();
		//Anche se si è annunciato alla stazione deve comunque proseguire indipendetemente dall'esito dell'annuncio.
		//A 5 km dalla stazione il treno effettua una nuova chiamata alla stazione, ed è a questo punto che decide  
		//se parcheggiarsi o continuare, a seconda dell'esito della nuova chiamata, che potrebbe essere diverso dall'esito dell'annuncio
		testa_ingresso_stazione();
		break;
	case distrutto:
		throw std::logic_error("Errore. Il treno e' distrutto e non può eseguire nessuna azione");
		break;
	default:
		break;
	}
	orario++;	//Trascorre il minuto
	Grafica::posizioneTreno(identificativo,posizione,orario);
}

//Subroutine: cambia lo stato del treno e regola la velocità
void Treno::cambia_stato(Stato s) {
	switch (s) {
	case creato:
		throw std::logic_error("Impossibile cambiare lo stato in creato. Questo stato e' possibile solo dopo la chiamata al costruttore");
		break;
	case attesa:
		velocita = 0;
		Grafica::switchAttesa(identificativo);
		break;
	case stazione:
		velocita = 80;
		break;
	case transito:
		set_velocita(max_speed());
		break;
	case movimento:
		set_velocita(max_speed());
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
	stato = s;
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

//Controlla se il treno è uscito dalla zona stazione. A seguito del controllo lo stato del treno sarà uno tra {movimento, stazione, transito, attesa}
void Treno::testa_uscita_stazione() {
	if (reverse) {
		if (posizione <= (*iteratore_stazioni)->getDistance() - 5) {
			//Sto uscendo dalla stazione, mi rimetto in movimento e libero il binario
			//Devo prenotare il binario di uscita, controllo se è libero
			if ((*iteratore_stazioni)->isFreeUscita()) {
				//Reimposto la velocità a quella di stazione, se per caso mi son fermato prima dell'uscita
				velocita = 80;
				(*iteratore_stazioni)->prenotaUscita(identificativo, reverse);
				libera_binario();
				cambia_stato(movimento);
				aggiorna_indici();
				fermata_effettuata = false;
				annunciato = false;
				liberato = false;
			}
			else {	//Se non è libero sto fermo
				velocita = 0;
			}
			
		}
	}
	else {
		if (posizione >= (*iteratore_stazioni)->getDistance() + 5) {
			if ((*iteratore_stazioni)->isFreeUscita()) {
				velocita = 80;
				(*iteratore_stazioni)->prenotaUscita(identificativo, reverse);
				libera_binario();
				cambia_stato(movimento);
				aggiorna_indici();
				fermata_effettuata = false;
				annunciato = false;
				liberato = false;
			}
			else {
				velocita = 0;
			}
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
			Grafica::passStazione(identificativo,(*iteratore_stazioni)->getNome());
			transitato = true;
			//Controllo se questa è la stazione capolinea
			if (iteratore_stazioni == Stazioni.begin()) {
				cambia_stato(distrutto);
				Grafica::capolinea(identificativo);
				libera_binario();
			}			
		}
	}
	else{
		if (posizione >= (*iteratore_stazioni)->getDistance()) {
            Grafica::passStazione(identificativo,(*iteratore_stazioni)->getNome());
			transitato = true;
			if (std::next(iteratore_stazioni) == Stazioni.end()) {
                Grafica::capolinea(identificativo);
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
	if (stato == transito) {
		(*iteratore_stazioni)->liberaBinarioTransito(identificativo, reverse);
		transitato = false;
	}
	else
		(*iteratore_stazioni)->liberaBinarioStazionamento(identificativo, reverse);
}

void Treno::libera_uscita(){
	if (reverse) {
		//Controllo la posizione del treno (+15km rispetto alla stazione PRECEDENTE)
		if (posizione <= (*std::next(iteratore_stazioni))->getDistance() - 15) {
			(*std::next(iteratore_stazioni))->liberaUscita(reverse);
			liberato = true;
			Grafica::possibileAvanzare(identificativo,(*std::next(iteratore_stazioni))->getNome() );
		}
	}
	else {
		if (posizione >= (*std::prev(iteratore_stazioni))->getDistance() + 15) {
			(*std::prev(iteratore_stazioni))->liberaUscita(reverse);
			liberato = true;
            Grafica::possibileAvanzare(identificativo,(*std::prev(iteratore_stazioni))->getNome() );
		}
	}
}

//Aggiorna l'indice del vettore di orari e dell'itetratore di stazioni
void Treno::aggiorna_indici() {
	if (reverse) {
		iteratore_stazioni--;
	}
	else {
		iteratore_stazioni++;
	}
	indice_orario++;
}

//Ferma il treno alla banchina
void Treno::effettua_fermata() {
	cambia_stato(fermata);
	std::cout << "Il treno " << identificativo << " e' arrivato alla stazione " << (*iteratore_stazioni)->getNome() << " ai minuti " << orario << std::endl;
	fermata_effettuata = true;
	//Controllo se era l'ultima fermata
	if (reverse) {
		if (iteratore_stazioni == Stazioni.begin()) {
			std::cout << "Il treno " << identificativo << " e' arrivato alla fermata finale e verra' distrutto" << std::endl;
			cambia_stato(distrutto);
			libera_binario();
		}
	}
	else {
		if (std::next(iteratore_stazioni, 1) == Stazioni.end()) {
			std::cout << "Il treno " << identificativo << " e' arrivato alla fermata finale e verra' distrutto" << std::endl;
			cambia_stato(distrutto);
			libera_binario();
		}
	}
}

//Calcola il ritardo con cui il treno è arrivato alla fermata
void Treno::calcola_ritardo() {
	//Devo calcoare l'eventuale ritardo/anticipo del treno
	int previsto = Orari[indice_orario];
	//if (orario - previsto == ritardo)
		//return; //Il ritardo non è variato
	ritardo = orario - previsto;
	if (ritardo >= 0)
		std::cout << "Il treno " << identificativo << " e' in ritardo di " << ritardo << " minuti alla stazione " << (*iteratore_stazioni)->getNome();
	if (ritardo < 0)
		std::cout << "Il treno " << identificativo << " e' in anticipo di " << -ritardo << " minuti alla stazione " << (*iteratore_stazioni)->getNome();
	return; //Altrimenti se ritardo = 0, non c'è annuncio ritardo
}



//*****************************Funzioni di prenotazione dei binari**************************************************

//Prenota un binario per la partenza del treno dalla stazione di partenza
void Treno::partenza(bool trans) {
	if (trans) {
		if (!((*iteratore_stazioni)->isFreePass(identificativo, reverse)))
			throw std::logic_error("Errore. Si sta cercando di far transitare se");
		(*iteratore_stazioni)->PrenotaTransito(identificativo, reverse);
		cambia_stato(transito);
	}
	else {
		if (!((*iteratore_stazioni)->isFreeStop(identificativo, reverse)))
			throw std::logic_error("Errore. Si sta cercando di far partire un treno senza che vi siano binari disponibili");
		(*iteratore_stazioni)->PrenotaStazionamento(identificativo, reverse);
		cambia_stato(stazione);
	}
}

//Prenota un binario di fermata
void Treno::prenota_fermata(){
	if ((*iteratore_stazioni)->isFreeStop(identificativo, reverse) && (Orari[indice_orario]- orario <= 4)) {
		//Se il binario è disponibile e non sono in largo anticipo , lo prenoto ed entro in stazione
		//Se sono in parcheggio libero il binario
		if (stato == parcheggio) {
			(*iteratore_stazioni)->liberaDeposito(identificativo, reverse);
		}
		(*iteratore_stazioni)->PrenotaStazionamento(identificativo, reverse);
		cambia_stato(stazione);
	}
	else if (stato != parcheggio){
		//Altrimenti mi metto in parcheggio (se non lo ero già)
		(*iteratore_stazioni)->PrenotaDeposito(identificativo, reverse);
		cambia_stato(parcheggio);
		//Sposto il treno al chilometro del parcheggio, anche se la chiamata avviene oltre
		if (reverse)
			posizione = (*iteratore_stazioni)->getDistance() + 5;
		else
			posizione = (*iteratore_stazioni)->getDistance() - 5;
	}
	if (Orari[indice_orario] - orario >= 4)
		std::cout << "Il treno " << identificativo << " e' in anticipo di " << Orari[indice_orario] - orario << " minuti, e rimarra' in parcheggio" << std::endl;
}

//Prenota un binario di transito
void Treno::prenota_transito(){
	//Se il binario è disponibile, lo prenoto ed entro in transito
	if ((*iteratore_stazioni)->isFreePass(identificativo, reverse)) {
		//Se sono in parcheggio libero il binario
		if (stato == parcheggio) {
			(*iteratore_stazioni)->liberaDeposito(identificativo, reverse);
		}
		(*iteratore_stazioni)->PrenotaTransito(identificativo, reverse);
		cambia_stato(transito);
	}
	else if (stato != parcheggio) {
		//Altrimenti mi metto in parcheggio, ma solo se non lo ero già
		(*iteratore_stazioni)->PrenotaDeposito(identificativo, reverse);
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

void Regionale::attiva(int ora){
	if (attivato)
		throw std::logic_error("Errore. Si sta attivando un treno gia' attivato");
	orario_partenza = ora;
	orario = ora;
	//Devo cercare un binario libero
	partenza();
	//Fingo di aver già effettuato la fermata
	stato = fermata;
	minuti_fermata = 4;
	fermata_effettuata = true;
	//Imposto il treno come già attivato
	attivato = true;
	esegui();
}

void Regionale::set_velocita(int v){
	if (v > MAX_SPEED)
		v = MAX_SPEED;
	Treno::set_velocita(v);
}

void Regionale::pre_chiamata() {
	if (reverse) {
		if (posizione <= (*iteratore_stazioni)->getDistance() + 20) {
			if ((*iteratore_stazioni)->isFreeStop(identificativo, reverse))
				std::cout << "Il treno regionale " << identificativo << " ha richiesto un binario di fermata presso la stazione " << (*iteratore_stazioni)->getNome() << " con esito positivo" << std::endl;
			else
				std::cout << "Il treno regionale " << identificativo << " ha richiesto un binario di fermata presso la stazione " << (*iteratore_stazioni)->getNome() << " con esito negativo" << std::endl;
			annunciato = true;
		}
	}
	else {
		if (posizione >= (*iteratore_stazioni)->getDistance() - 20) {
			if ((*iteratore_stazioni)->isFreeStop(identificativo, reverse))
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

void AltaVelocita::attiva(int ora){
	//Verifico da che tipo di stazione sto partendo
	if ((*iteratore_stazioni)->isPrincipale()) {
		//Sto partendo da una stazione principale. Devo prenotare un binario di transito
		orario_partenza = ora;
		orario = ora;
		partenza();
		//Fingo di aver già effettuato la fermata
		stato = fermata;
		minuti_fermata = 4;
		fermata_effettuata = true;
	}
	else {
		//Sto partendo da una stazione locale. Devo prenotare un binario di transito
		partenza(true);
	}
	attivato = true;
	esegui();
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
				if ((*iteratore_stazioni)->isFreeStop(identificativo, reverse))
					std::cout << "Il treno Alta Velocita' " << identificativo << " ha richiesto un binario di fermata presso la stazione " << (*iteratore_stazioni)->getNome() << " con esito positivo" << std::endl;
				else
					std::cout << "Il treno Alta Velocita' " << identificativo << " ha richiesto un binario di fermata presso la stazione " << (*iteratore_stazioni)->getNome() << " con esito negativo" << std::endl;
				annunciato = true;
			}
		}
		else {
			if (posizione >= (*iteratore_stazioni)->getDistance() - 20) {
				if ((*iteratore_stazioni)->isFreeStop(identificativo, reverse))
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
				if ((*iteratore_stazioni)->isFreePass(identificativo, reverse))
					std::cout << "Il treno Alta Velocita' " << identificativo << " ha richiesto un binario di transito presso la stazione " << (*iteratore_stazioni)->getNome() << " con esito positivo" << std::endl;
				else
					std::cout << "Il treno regionale " << identificativo << " ha richiesto un binario di transito presso la stazione " << (*iteratore_stazioni)->getNome() << " con esito negativo" << std::endl;
				annunciato = true;
			}
		}
		else {
			if (posizione >= (*iteratore_stazioni)->getDistance() - 20) {
				if ((*iteratore_stazioni)->isFreePass(identificativo, reverse))
					std::cout << "Il treno Alta Velocita' " << identificativo << " ha richiesto un binario di transito presso la stazione " << (*iteratore_stazioni)->getNome() << " con esito positivo" << std::endl;
				else
					std::cout << "Il treno Alta Velocita' " << identificativo << " ha richiesto un binario di transito presso la stazione " << (*iteratore_stazioni)->getNome() << " con esito negativo" << std::endl;
				annunciato = true;
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

void SuperVelocita::attiva(int ora){
	//Verifico da che tipo di stazione sto partendo
	if ((*iteratore_stazioni)->isPrincipale()) {
		//Sto partendo da una stazione principale. Devo prenotare un binario di transito
		orario_partenza = ora;
		orario = ora;
		partenza();
		//Fingo di aver già effettuato la fermata
		stato = fermata;
		minuti_fermata = 4;
		fermata_effettuata = true;
	}
	else {
		//Sto partendo da una stazione locale. Devo prenotare un binario di transito
		partenza(true);
	}
	attivato = true;
	esegui();
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
				if ((*iteratore_stazioni)->isFreeStop(identificativo, reverse))
					std::cout << "Il treno Alta Velocita' " << identificativo << " ha richiesto un binario di fermata presso la stazione " << (*iteratore_stazioni)->getNome() << " con esito positivo" << std::endl;
				else
					std::cout << "Il treno Alta Velocita' " << identificativo << " ha richiesto un binario di fermata presso la stazione " << (*iteratore_stazioni)->getNome() << " con esito negativo" << std::endl;
				annunciato = true;
			}
		}
		else {
			if (posizione >= (*iteratore_stazioni)->getDistance() - 20) {
				if ((*iteratore_stazioni)->isFreeStop(identificativo, reverse))
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
				if ((*iteratore_stazioni)->isFreePass(identificativo, reverse))
					std::cout << "Il treno Alta Velocita' " << identificativo << " ha richiesto un binario di transito presso la stazione " << (*iteratore_stazioni)->getNome() << " con esito positivo" << std::endl;
				else
					std::cout << "Il treno regionale " << identificativo << " ha richiesto un binario di transito presso la stazione " << (*iteratore_stazioni)->getNome() << " con esito negativo" << std::endl;
				annunciato = true;
			}
		}
		else {
			if (posizione >= (*iteratore_stazioni)->getDistance() - 20) {
				if ((*iteratore_stazioni)->isFreePass(identificativo, reverse))
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