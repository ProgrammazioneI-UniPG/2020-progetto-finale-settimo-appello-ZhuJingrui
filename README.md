# ProgrammazioneI-UniPG-Progetto-finale-2019-Sabbie
Template progetto finale per l'esame di Programmazione I con Laboratorio a.a. 2019-2020.

Inserire

Nome: Jingrui   

Cognome: Zhu  

Matricola: 320795

funzione SCONTRO: in caso di scontro tra i giocatori, in base alla quantità melassa presenti in 2 serbatoi, si trova trovare 1, 2 o 3 unità aggiuntive con la probabilità di 40/40/20, si vince chi ha più melassa; c'è una probabilità di 30% che non vince nessuno a causa di crollo; in caso di parità quantità melassa, si vincono entrambi giocatoti

funzione CHIUDI CAVERNA: assenza di tale funzione in quando il gioco si continua in modo procedurale

struttura CAVERNA: è presente solo 2 puntatori all'interno della struttura, uno che punta alla caverna prossima, uno viene usato solo se viene chiamata la funzione abbatti, invece la direzione viene indicato con il tipo enum; la motivazione di tale modifica sta nel fatto che utilizzando 3 puntatori causa lo spreco della memoria, aumento la possibilità di segfault a seguito dell'aumento della difficoltà nella programmazione
