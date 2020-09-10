/*
Nome: Jingrui
Cognome: Zhu
Progetto: The Dune
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <unistd.h>

  void menu_principale();
  void gioca();

  enum tipo_imprevisto{nessun_imprevisto, crollo, baco};
  enum quantita_melassa{nessuna, poca, molta};
  enum tipo_caverna{normale, speciale, accidentale, uscita};
  enum direzione{avanti, destra, sinistra};

  typedef struct Caverna{
    enum tipo_imprevisto TipoImprevisto;
    enum quantita_melassa QuantitaMelassa;
    enum tipo_caverna TipoCaverna;
    enum direzione Direzione;
    struct Caverna* prossima;
    struct Caverna* nuova; //in caso della abbatti
  }Tcaverna;

  typedef struct Scavatrice{
    Tcaverna* posizione;
    int serbatoio_energia;
    int serbatoio_raccolta;
  }Tscavatrice;
