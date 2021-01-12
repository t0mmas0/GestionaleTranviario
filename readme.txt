Suddivisone dei file:
	Marco Nardi:
		LeggiOrari.cpp, LeggiLinea.cpp
	Tommaso Bertoldi:
		Grafica.cpp, Semaforo.cpp, Stazione.cpp, StazionePrincipale.cpp, StazioneSecondaria.cpp, main.cpp
	Simone Peraro:
		Treno.cpp
	Note: La classe Treno crea un treno intelligente che cambia il proprio stato a seconda della posizione
		lungo la linea. Al momento della creazione del treno lo stato del treno è creato, in seguito viene
		attivato e posizionato alla prima (o ultima) stazione, su un binario di transito o di stazionamento
		disponibile. Il treno inizia subito a muoversi per uscire dalla stazione e raggiunto il punto di raccordo
		con la linea ferroviaria (a 5 km) chiede alla stazione il permesso di uscire, eventualmente fermandosi.
		Il treno poi prosegue lungo la linea verso la stazione successiva. A 20 km da questa, il treno stampa 
		in output un annuncio di avvicinamento alla stazione e controlla se il binario di transito o stazionamento
		è disponibile, ma qualunque sia la risposta, il treno prosegue fino a 5 km dalla stazione, dove effettua
		una nuova chiamata alla stazione ed eventualmente accede alla zona di parcheggio, dove continua a chiamare
		la stazione per cercare un binario libero. Una volta ottenuto il binario, il treno prosegue ed effettua
		la fermata o il transito. Questo avviene per tutte le stazioni, e alla fine il treno viene distrutto.
	
	Il file LineaFerroviaria.cpp è stato scritto da Tommaso Bertoldi, in collaborazione con tutti gli altri membri
