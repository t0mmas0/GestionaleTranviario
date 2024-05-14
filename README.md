# GestionaleTranviario

#Appunti
Bozza struttura del progetto: https://app.creately.com/diagram/KxGNY59bkHs/view

#Cosa mostriamo in output
stato treni dato un orario(terminale) 
File di output dove viene descritto il percorso che deve fare ogni treno a seconda del traffico sulle linee 


#MacroAree

Treni:
Ogni tipo di treno deve avere una classe 
  regionale: Ferma a tutte le stazioni
  Alta velocita / super: Ferma solo alle stazioni principali
    
  Compiti
    Chiedere il permesso alla stazione 
    Comunicare se deve fermarsi o transitare
    Decidere velocita a cui viaggiare
    
Stazione:
  Gestire il traffico in ingrasso e uscita
  Gestire i treni parcheggiati prima in entrambi i sensi
  Tipo di stazione
  
Gestione Orari
  Dati i treni e i loro orari 
  Verificare se sono fattibili e modificare gli orari a seconda della fattibilità


Lettura e Scrittura File
Leggere i file timetable e parsarli a qualcosa di conosciuto da cristo
Scrivere su file timetable l'output
 

