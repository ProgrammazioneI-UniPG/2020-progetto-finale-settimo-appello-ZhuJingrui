/*
NOME: Jingrui
Cognome: Zhu
Progetto: The Dune
*/

//Non c'è la funzione chiudi_caverna in quando il progetto è procedurale
//Esiste solo un puntatore per il prossimo nodo, il motivo è creando 3 puntatori causa lo spreco della memoria e aumenta la possibilità segfault ma anche aumenta la difficoltà di progammazione
//Scontro si definisce con la quantità totale di melassa che ha in entrambi serbatoti piu unita di melassa trovata casualmente (da 1 a 3 unità), vince chi ha piu melassa, però c'è la probabiità di 30% che entrambi giocatori si muoiono, in caso di pareggio la quantità, entrambi si vincono


#include "gamelib.h"

//----------------------------------------------------------------------------- DICHIARAZIONI
  static Tcaverna* prima_caverna_arvais = NULL;
  static Tcaverna* prima_caverna_hartornen = NULL;
  static int scelta_famiglia;
  static int scelta_commando;
  static int cunicolo_arvais;
  static int cunicolo_hartornen;
  static Tscavatrice scavatrice_arvais = {NULL, 4, 0};
  static Tscavatrice scavatrice_hartornen = {NULL, 4, 0};
  static int vittoria_arvais;
  static int vittoria_hartornen;
  static int uscita_arvais;
  static int uscita_hartornen;
  static int prob_incontro;
  static int turno;
  static int cambia_direzione;

  static void crea_mappa(); //decide quale famiglia
  static void menu_crea_cunicoli(); //il menu dei comandi sulle operazioni del cunicolo
  static void ins_caverna(Tcaverna** cave);
  static void canc_caverna(Tcaverna* cave);  //elimina ultimo nodo della lista
  static void stampa_caverna(Tcaverna* cave);
  static void empty_caverna(Tcaverna** cave); //elimina intera lista
  static void state_cave(); //stampa lo stato di ogni caverna
  static void result(); //stampa il vincitore
  static void reset(); //risetta il gioco
  static void rematch(); //chiede ai giocatori di una nuova partita
  static void avanza(); //avanza alla prossima caverna
  static void abbatti(); //abbatti una caverna nella direzione non scelta
  static void aggira(); //chimata solo nel caso di crollo imprevisto che crea una caverna tra la corrente e prossima
  static void scontro(); //in caso dello scontro tra 2 famiglie
  static void esci(); //invoca l'uscita
//----------------------------------------------------------------- ------------DEFINIZIONI

void menu_principale(){ //OK

  int scelta;

  printf("\033[1;32m");
  printf("\t\t\t\t\t****************************************\n");
  printf("\t\t\t\t\t*                                      *\n");
  printf("\t\t\t\t\t*                                      *\n");
  printf("\t\t\t\t\t*            1.Create Game Map         *\n");
  printf("\t\t\t\t\t*            2.Play                    *\n");
  printf("\t\t\t\t\t*            3.Exit                    *\n");
  printf("\t\t\t\t\t*                                      *\n");
  printf("\t\t\t\t\t*                                      *\n");
  printf("\t\t\t\t\t****************************************\n\n\n");

  do{
    printf("\n\033[1;34mWhat are you going to do: \033[0m");
    scanf("%d", &scelta);
    while(getchar() != '\n');
  }while(scelta < 1 || scelta > 3);

  switch(scelta){
    case 1:
      system("clear");
      crea_mappa();
      break;
    case 2:
      if(cunicolo_arvais == 0 || cunicolo_hartornen == 0){
        printf("\n  YOU NEED TO CREATE GAME MAPS BEFORE PLAYING\n");
        sleep(1);
        system("clear");
        menu_principale();
      }else{
        system("clear");
        gioca();
      }
      break;
    case 3:
      reset();
      printf("\n\n\033[1;32mSEE YOU NEXT TIME\n");
      sleep(1);
      system("clear");
      exit(0);
      break;
    default:
      break;

  }//end switch

}//end menu_principale

static void crea_mappa(){ //OK

  printf("\033[1;33m");
  printf("\t\t\t\t\t+++++++++++++++++++++++++++++++++++++++\n");
  printf("\t\t\t\t\t+                                     +\n");
  printf("\t\t\t\t\t+                                     +\n");
  printf("\t\t\t\t\t+            1. Arvais                +\n");
  printf("\t\t\t\t\t+            2. Hartornen             +\n");
  printf("\t\t\t\t\t+            3. Back to menu          +\n");
  printf("\t\t\t\t\t+                                     +\n");
  printf("\t\t\t\t\t+                                     +\n");
  printf("\t\t\t\t\t+++++++++++++++++++++++++++++++++++++++\n");

  do{
    printf("\n\033[1;34mChoose a family you are playing for: \033[0m");
    scanf("%d", &scelta_famiglia);
    while(getchar() != '\n');
  }while(scelta_famiglia < 1 || scelta_famiglia > 3);

  switch (scelta_famiglia) {
    case 1:
      system("clear");
      menu_crea_cunicoli();
      break;
    case 2:
      system("clear");
      menu_crea_cunicoli();
      break;
    case 3:
      if(cunicolo_arvais == 0 || cunicolo_hartornen == 0){
        printf("You need to create one game map for family\n");
        sleep(2);
        system("clear");
        crea_mappa();
      }else{
        system("clear");
        menu_principale();
      }
      break;
    default:
      break;
  }//end switch

}//end crea_mappa

static void menu_crea_cunicoli(){ //OK

  int scelta_caverna;
  int conferma;

  printf("\033[1;33m");
  printf("\t\t\t\t\t+++++++++++++++++++++++++++++++++++++++\n");
  printf("\t\t\t\t\t+                                     +\n");
  printf("\t\t\t\t\t+                                     +\n");
  printf("\t\t\t\t\t+           1. Insert caves           +\n");
  printf("\t\t\t\t\t+           2. Eliminate cave         +\n");
  printf("\t\t\t\t\t+           3. Print out caves        +\n");
  printf("\t\t\t\t\t+           4. Back to menu           +\n");
  printf("\t\t\t\t\t+                                     +\n");
  printf("\t\t\t\t\t+                                     +\n");
  printf("\t\t\t\t\t+++++++++++++++++++++++++++++++++++++++\n");

  do{
    printf("\033[1;34m");
    printf("\nWhat do you want to do: ");
    printf("\033[0m");
    scanf("%d", &scelta_caverna);
    while(getchar() != '\n');
  }while(scelta_caverna < 1 || scelta_caverna > 4);

  switch (scelta_caverna) {
    case 1:
      if(scelta_famiglia == 1){
        if(cunicolo_arvais == 0){
          ins_caverna(&prima_caverna_arvais);
        } else if(cunicolo_arvais == 1){
          printf("There is already a tunnel for Arvais\n");
          printf("You will lose the tunnel created if you choose this command again\n");
          do{
            printf("What are you going to do?\n(0 = I miss clicked, 1 = I want a new tunnel): ");
            scanf("%d", &conferma);
            while(getchar() != '\n');
          }while(conferma < 0 || conferma > 1);
          if(conferma == 0){
            system("clear");
            menu_crea_cunicoli();
          } else{
            system("clear");
            cunicolo_arvais = 0;
            ins_caverna(&prima_caverna_arvais);
          }
        }
      } else if(scelta_famiglia == 2){
        if(cunicolo_hartornen == 0){
          ins_caverna(&prima_caverna_hartornen);
        } else if(cunicolo_hartornen == 1){
          printf("There is already a tunnel for Hartornen\n");
          printf("You will lose the tunnel created if you choose this command again\n");
          do{
            printf("What are you going to do?\n(0 = I miss clicked, 1 = I want a new tunnel): ");
            scanf("%d", &conferma);
            while(getchar() != '\n');
          }while(conferma < 0 || conferma > 1);
          if(conferma == 0){
            system("clear");
            menu_crea_cunicoli();
          } else{
            system("clear");
            cunicolo_arvais = 0;
            ins_caverna(&prima_caverna_hartornen);
          }
        }
      }
      break;
    case 2:
      if(scelta_famiglia == 1 && cunicolo_arvais == 0){
        printf("You need to creat a tunnel first\n");
        sleep(1);
        system("clear");
        menu_crea_cunicoli();
      } else if(scelta_famiglia == 1 && cunicolo_arvais == 1){
        system("clear");
        canc_caverna(prima_caverna_arvais);
      } else if(scelta_famiglia == 2 && cunicolo_hartornen == 0){
        printf("You need to creat a tunnel first\n");
        sleep(1);
        system("clear");
        menu_crea_cunicoli();
      } else if(scelta_famiglia == 2 && cunicolo_hartornen == 1){
        system("clear");
        canc_caverna(prima_caverna_hartornen);
      }
      break;
    case 3:
      if(scelta_famiglia == 1 && cunicolo_arvais == 0){
        printf("There isn't a map\n");
        sleep(1);
        system("clear");
        menu_crea_cunicoli();
      } else if(scelta_famiglia == 1 && cunicolo_arvais == 1){
        system("clear");
        stampa_caverna(prima_caverna_arvais);
      } else if(scelta_famiglia == 2 && cunicolo_hartornen == 0){
        printf("There isn't a map\n");
        sleep(1);
        system("clear");
        menu_crea_cunicoli();
      } else if(scelta_famiglia == 2 && cunicolo_hartornen == 1){
        system("clear");
        stampa_caverna(prima_caverna_hartornen);
      }
      break;
    case 4:
      if(scelta_famiglia == 1 && cunicolo_arvais == 0){
        printf("You need to creat a tunnel for Arvais in order to get back\n");
        sleep(1);
        system("clear");
        menu_crea_cunicoli();
      } else if(scelta_famiglia == 2 && cunicolo_hartornen == 0){
        printf("You need to creat a tunnel for Hartornen in order to get back\n");
        sleep(1);
        system("clear");
        menu_crea_cunicoli();
      } else{
        system("clear");
        crea_mappa();
      }
      break;
    default:
      break;
  }

}//end menu_crea_cunicoli

static void ins_caverna(Tcaverna** cave){ //OK

  //in caso che utente decide di eliminare la mappa già creata e ne costruirla un'altra
  if(scelta_famiglia == 1)
    empty_caverna(&prima_caverna_arvais);
  else if(scelta_famiglia == 2)
    empty_caverna(&prima_caverna_hartornen);

  int numero_caverne;
  int conta_caverne;
  int typecave;
  int direzione;
  Tcaverna* new = NULL;

  system("clear");
  do{
    printf("\nHow many caves do you want for the game(at least 10): ");
    scanf("%d", &numero_caverne);
    while(getchar() != '\n');
  }while(numero_caverne < 10);
  system("clear");

  //il ciclo per creare il cunicolo definendo ogni caverna
  for(conta_caverne = 0; conta_caverne < numero_caverne; conta_caverne++){

      Tcaverna* next = (Tcaverna*)malloc(sizeof(Tcaverna));
      int prob_melassa = 1 + (rand() % 100);
      int prob_imprevisto = 1 +(rand() % 100);
      next -> nuova = NULL;

      //definire la probabilità della quantità melassa si trova in caverna
      if(prob_melassa > 0 && prob_melassa <= 50){
        next -> QuantitaMelassa = 0;
      } else if(prob_melassa > 50 && prob_melassa <= 80){
        next -> QuantitaMelassa = 1;
      } else if(prob_melassa > 80 && prob_melassa <= 100){
        next -> QuantitaMelassa = 2;
      }

      //definire la probabilità di imprevisto della caverna
      if(prob_imprevisto > 0 && prob_imprevisto <= 50){
        next -> TipoImprevisto = 0;
      } else if(prob_imprevisto > 50 && prob_imprevisto <= 85){
        next -> TipoImprevisto = 1;
      } else if(prob_imprevisto > 85 && prob_imprevisto <= 100){
        next -> TipoImprevisto = 2;
      }

      printf("\n\nCave n.%d\n", conta_caverne + 1);
      //definire il tipo della caverna
      do{
        printf("\nChoose the type of cave\n(0 = Normal, 1 = Special, 2 = Accidental): ");
        scanf("%d", &typecave);
        while(getchar() != '\n');
        next -> TipoCaverna = typecave;
      }while(typecave < 0 || typecave > 2);

      //definire la direzione della prossima caverna
      do{
        printf("\nChoose the direction of cave compared to the last one\n(0 = Ahead, 1 = Right, 2 = Left): ");
        scanf("%d", &direzione);
        while(getchar() != '\n');
        next -> Direzione = direzione;
      }while(direzione < 0 || direzione > 2);

      next -> prossima = NULL;

      if(new == NULL){
        new = next;
      }else{
        next -> prossima = (Tcaverna*)new;
        new = next;
      }

    }//end for

    *cave = new;

    if(scelta_famiglia == 1){
      cunicolo_arvais = 1;
      scavatrice_arvais.posizione = *cave;
    }else if(scelta_famiglia == 2){
      cunicolo_hartornen = 1;
      scavatrice_hartornen.posizione = *cave;
    }

  printf("\n\n\033[1;36mTunnel creation with success, back to the last menu\n");
  sleep(1);
  system("clear");
  printf("\033[0m");
  menu_crea_cunicoli();

}//end ins_caverna

static void canc_caverna(Tcaverna* cave){ //OK

  int scelta;
  Tcaverna* temp = NULL;
  printf("\033[1;33mThis command eliminates the last cave in the tunnel, are you sure?\n\033[0m(0 = No 1 = Yes):");
  scanf("%d", &scelta);
  if(scelta == 1){
    for(int i = 0; (cave  -> prossima -> prossima) != NULL; i++){
      cave = cave -> prossima;
    }
    temp = cave;
    free(temp -> prossima);
    temp -> prossima = NULL;
    cave = temp;

    puts("");
    printf("\033[1;36mElimination with success, go back to the last menu\n\033[0m");
    sleep(1);
    system("clear");
    menu_crea_cunicoli();
  }else{
    puts("");
    printf("\033[1;36mGood call\n\033[0m");
    sleep(1);
    system("clear");
    menu_crea_cunicoli();
  }

}//end canc_caverna

static void stampa_caverna(Tcaverna* cave){ //OK

  int page;
  puts("");

  for(int i = 0; cave != NULL; i++){
    printf("\n\tcave n.%d |", i+1);
    switch (cave -> TipoCaverna) {
      case 0:
        printf(" cave type: Normal     |");
        break;
      case 1:
        printf(" cave type: Special    |");
        break;
      case 2:
        printf(" cave type: Accidental |");
        break;
      default:
        break;
    }
    switch(cave -> Direzione){
      case 0:
        printf(" direction: Ahead\n");
        break;
      case 1:
        printf(" direction: Right\n");
        break;
      case 2:
        printf(" direction: Left\n");
        break;
      default:
        break;
    }
    cave = cave -> prossima;
  }

  printf("\n\n\nDigit any character in order to go back to the last menu:  ");
  if(scanf("%ls", &page) != '\0'){
    system("clear");
    menu_crea_cunicoli();
  }

}//end stampa_caverna

static void empty_caverna(Tcaverna** cave){  //OK

    Tcaverna* current = *cave;
    Tcaverna* temp = NULL;

    while(current != NULL){
      temp = current -> prossima;
      free(current);
      current = temp;
    }

    *cave = NULL;

}//end empty_caverna

static void avanza(Tscavatrice* excavator){  //OK

  system("clear");
  excavator -> posizione = excavator -> posizione -> prossima;

}//end avanza

static void abbatti(Tscavatrice** excavator){ //OK

    system("clear");

    Tscavatrice* player = *excavator;
    if(player -> serbatoio_energia >= 1){
      player -> serbatoio_energia -= 1;
    }else if(player -> serbatoio_raccolta >= 1 && player -> serbatoio_energia <= 0){
      player -> serbatoio_raccolta -= 1;
    }

    int prob_melassa = 1 + (rand() % 100);
    int prob_imprevisto = 1 + (rand() % 100);
    int prob_caverna = 1 + (rand() % 100);

    Tcaverna* temp = player -> posizione -> prossima -> prossima;
    free(player -> posizione -> prossima);
    player -> posizione -> prossima = NULL;
    Tcaverna* new = (Tcaverna*)malloc(sizeof(Tcaverna));

    new -> Direzione = cambia_direzione;
    new -> nuova = NULL;

    if(prob_melassa > 0 && prob_melassa <= 40){
      new -> QuantitaMelassa = 0;
    } else if(prob_melassa > 40 && prob_melassa <= 80){
      new -> QuantitaMelassa = 1;
    } else if(prob_melassa > 80 && prob_melassa <= 100){
      new -> QuantitaMelassa = 2;
    }

    if(prob_imprevisto > 0 && prob_imprevisto <= 40){
      new -> TipoImprevisto = 0;
    } else if(prob_imprevisto > 40 && prob_imprevisto <= 80){
      new -> TipoImprevisto = 1;
    } else if(prob_imprevisto > 80 && prob_imprevisto <= 100){
      new -> TipoImprevisto = 2;
    }

    if(turno == 1){
      if(prob_caverna > 0 && prob_caverna <= 20){
        new -> TipoCaverna = 2;
      }else if(prob_caverna > 20 && prob_caverna <= (20 + uscita_arvais)){
        new -> TipoCaverna = 3;
        vittoria_arvais = 1;
        esci();
      }else if(prob_caverna > (20 + uscita_arvais) && prob_caverna < ((100 - (20 + uscita_arvais)) / 2)){
        new -> TipoCaverna = 0;
      }else if(prob_caverna >= ((100 - (20 + uscita_arvais)) / 2) && prob_caverna < 100){
        new -> TipoCaverna = 1;
      }
    }else if(turno == 2){
      if(prob_caverna > 0 && prob_caverna <= 20){
        new -> TipoCaverna = 2;
      }else if(prob_caverna > 20 && prob_caverna <= (20 + uscita_hartornen)){
        new -> TipoCaverna = 3;
        vittoria_hartornen = 1;
        esci();
      }else if(prob_caverna > (20 + uscita_hartornen) && prob_caverna < ((100 - (20 + uscita_hartornen)) / 2)){
        new -> TipoCaverna = 0;
      }else if(prob_caverna >= ((100 - (20 + uscita_hartornen)) / 2) && prob_caverna < 100){
        new -> TipoCaverna = 1;
      }
    }

    new -> nuova = NULL;
    new -> prossima = temp;
    player -> posizione -> prossima = new;

    *excavator = player;

}//end abbatti

static void aggira(Tscavatrice** excavator){ //OK

   system("clear");

   Tscavatrice* player = *excavator;
   int prob_melassa = 1 + (rand() % 100);
   int prob_imprevisto = 1 +(rand() % 100);

   Tcaverna* new = (Tcaverna*)malloc(sizeof(Tcaverna));
   Tcaverna* temp = player -> posizione -> prossima;

   if(prob_melassa > 0 && prob_melassa <= 50){
     new -> QuantitaMelassa = 0;
   } else if(prob_melassa > 50 && prob_melassa <= 80){
     new -> QuantitaMelassa = 1;
   } else if(prob_melassa > 80 && prob_melassa <= 100){
     new -> QuantitaMelassa = 2;
   }

   if(prob_imprevisto > 0 && prob_imprevisto <= 50){
     new -> TipoImprevisto = 0;
   } else if(prob_imprevisto > 50 && prob_imprevisto <= 85){
     new -> TipoImprevisto = 1;
   } else if(prob_imprevisto > 85 && prob_imprevisto <= 100){
     new -> TipoImprevisto = 2;
   }

   new -> TipoCaverna = player -> posizione -> TipoCaverna;
   new -> Direzione = 0;
   new -> nuova = NULL;
   new -> prossima = temp;
   player -> posizione -> prossima = new;

   *excavator = player;

}//end aggira

static void scontro(){ //OK

  int prob_arvais = 1 + (rand() % 100);
  int prob_hartornen = 1 + (rand() % 100);
  int tot_arvais = 0;
  int tot_harotrnen = 0;
  int prob_incendio = 1 + (rand() % 100);
  int page;

  system("clear");
  printf("\033[0mYou now both in the same cave, either of you could get back to the cave you came from due to the pride of defending your family\nSo you decide to duel");
  printf("\nYou agreed to recharge your tank with the energy you find in this cave\nThen you are going to battle each other using your excavator\n\n");
  sleep(3);

  if(prob_arvais >= 0 && prob_arvais < 40){
    printf("Arvais has found 1 unit of SPICE\n");
    tot_arvais = scavatrice_arvais.serbatoio_energia + scavatrice_arvais.serbatoio_raccolta + 1;
  }else if(prob_arvais >= 40 && prob_arvais < 80){
    printf("Arvais has found 2 units of SPIECE\n");
    tot_arvais = scavatrice_arvais.serbatoio_energia + scavatrice_arvais.serbatoio_raccolta + 2;
  }else if(prob_arvais >= 80 && prob_arvais <= 100){
    printf("Arvais has found 3 units of SPICE\n");
    tot_arvais = scavatrice_arvais.serbatoio_energia + scavatrice_arvais.serbatoio_raccolta + 3;
  }

  if(prob_hartornen >= 0 && prob_hartornen < 40){
    printf("Hartornen has found 1 unit of SPICE\n");
    tot_harotrnen = scavatrice_hartornen.serbatoio_energia + scavatrice_hartornen.serbatoio_raccolta + 1;
  }else if(prob_hartornen >= 40 && prob_hartornen < 80){
    printf("Hartornen has found 2 units of SPIECE\n");
    tot_harotrnen = scavatrice_hartornen.serbatoio_energia + scavatrice_hartornen.serbatoio_raccolta + 2;
  }else if(prob_hartornen >= 80 && prob_hartornen <= 100){
    printf("Hartornen has found 3 units of SPICE\n");
    tot_harotrnen = scavatrice_hartornen.serbatoio_energia + scavatrice_hartornen.serbatoio_raccolta + 3;
  }
  sleep(2);

  if(prob_incendio >= 70){
    printf("\nYou are in a fair fight, Arvais operates with technique while Hartornen operates the latest model of excavator which is more powerful\n");
    sleep(2);
    printf("But either of you noticed that the sparks caused by the friction between excavators has lighted an un-know substance on fire\n");
    printf("Before you know, you are surrounded by the fire and you are hearing the explosion...\n\n");
    sleep(3);
    printf("\033[1;31mBoom.....\n");
    sleep(1);

  }else{
    if(tot_arvais > tot_harotrnen){
      printf("\n\033[1;35mHartornen's excavator is more powerful than Arvais, it seems like Hartornen is winning, but Arvais has more experience on the extraction of SPICE and operating an excavator\n");
      sleep(2);
      printf("Arvais noticed a small vibration, soon he induced Hartornen to the vibration\n");
      sleep(2);
      printf("All the sudden, a Sandworm came from the side of cave and hit Hartornen right in the tanks\nLuckily, no-one got hurt but it's clear that Hartornrn lost the fight..\n");
      sleep(2);
      vittoria_arvais = 1;
    }else if(tot_arvais < tot_harotrnen){
      printf("\n\033[1;33mArvais has got more experience on operating an excavator, soon he got cocky and starts toying with Hartornen, convinced that he is winning\n");
      sleep(2);
      printf("Hartornen noticed the change of attidue in Arvais, so he decided to play along waiting for the moment of opportunity using least units of energy possible\n");
      sleep(2);
      printf("He waited till the moment when Arvais is on his back to a cave and starts pushing him into the tunnel just when it's collapsing...\n");
      sleep(2);
      vittoria_hartornen = 1;
    }else if(tot_arvais = tot_harotrnen){
      printf("\n\033[1;34mYou are in a fair fight, Arvais operates with technique while Hartornen operates the latest model of excavator which is more powerful\n");
      sleep(2);
      printf("Both is moving with caution trying to be in one step ahead of each other\n");
      sleep(2);
      printf("Just when you are about to make next move, you hear something moving in the walls\nYou looked at each other and without saying a word, you are moving towards to the next cave\n");
      sleep(2);
      printf("You are going as fast as you can, you barely cared about anything else and then\n\033[1;32mThere are no more walls, just an open field...\n\033[0m");
      sleep(3);
      vittoria_arvais = 1;
      vittoria_hartornen = 1;
    }
  }

  printf("\n\n\n\033[0mPlease digit any character to continue: ");
  if(scanf("%ls", &page) != '\0'){
    system("clear");
    result();
  }

}//end scontro

static void esci(Tscavatrice** excavator){ //OK

    int page;
    system("clear");

    Tscavatrice* player = *excavator;
    free(player -> posizione -> prossima);
    player -> posizione -> prossima = NULL;
    player -> posizione -> prossima = (Tcaverna*)malloc(sizeof(Tcaverna));
    player -> posizione -> prossima -> TipoCaverna = 3;
    player -> posizione -> prossima -> TipoImprevisto = 0;
    player -> posizione -> prossima -> QuantitaMelassa = 0;
    player -> posizione -> prossima -> Direzione = 0;
    player -> posizione -> prossima -> prossima = NULL;
    player -> posizione -> prossima -> nuova = NULL;
    *excavator = player;
    printf("You are going through the tunnel hearing the sound of collapsing\nYou are terrified of what could happen at any moment but you took a deep breath and heading to the next cave firmly\n");
    sleep(2);
    printf("You thought you are arriving to the next cave, instead, you see a light, you are stunned for a second then realize\n\033[1;33mYou are going home...\n");
    sleep(4);
    if(turno == 1){
      vittoria_arvais = 1;
    }else if(turno == 2){
      vittoria_hartornen = 1;
    }

    printf("\n\n\n\033[0mPlease digit any character to continue: ");
    if(scanf("%ls", &page) != '\0'){
      system("clear");
      result();
    }

}//end esci

static void result(){ //OK

  system("clear");
  if(vittoria_arvais == 1 && vittoria_hartornen == 0){
    printf("\n\t\t\033[1;32mThe winner is");
    printf("\033[1;35m Arvais\n");
    printf("\t\t\033[1;32mCONGRATULATION!!\n");
    sleep(2);
  }else if(vittoria_arvais == 0 && vittoria_hartornen == 1){
    printf("\n\t\t\033[1;32mThe winner is");
    printf("\033[1;33m Hartornen\n");
    printf("\t\t\033[1;32mCONGRATULATION!!\n");
    sleep(2);
  }else if(vittoria_arvais == 0 && vittoria_hartornen == 0){
    printf("\n\t\t\033[1;36mYOU BOTH ARE DEAD\n");
    sleep(2);
  }else if(vittoria_arvais == 1 && vittoria_hartornen == 1){
    printf("\n\t\t\033[1;36mCONGRATULATIONS, YOU BOTH SURVIVED THE GAME!!\n");
    sleep(2);
  }
  reset();
  rematch();

}// end result

static void rematch(){ //OK

    int scelta_rematch;

    do{
      printf("\n\n\n\033[0mDo you want to play another game? (0 = NO, 1 = YES): ");
      scanf("%d", &scelta_rematch);
      while(getchar() != '\n');
    }while(scelta_rematch < 0 || scelta_rematch > 1);
    if(scelta_rematch == 0){
      printf("\033[1;36m\nSee you next time\n");
      sleep(1);
      reset();
      system("clear");
      exit(0);
    }else if(scelta_rematch == 1){
      reset();
      printf("\n\033[1;36mGame maps will reset\n");
      sleep(1);
      system("clear");
      menu_principale();
    }

}//end rematch

static void reset(){ //OK

    empty_caverna(&prima_caverna_arvais);
    empty_caverna(&prima_caverna_hartornen);
    cunicolo_arvais = 0;
    cunicolo_hartornen = 0;
    vittoria_arvais = 0;
    vittoria_hartornen = 0;
    uscita_arvais = 0;
    uscita_hartornen = 0;
    prob_incontro = 0;
    turno = 1;
    scavatrice_arvais.posizione = NULL;
    scavatrice_arvais.serbatoio_energia = 4;
    scavatrice_arvais.serbatoio_raccolta = 0;
    scavatrice_hartornen.posizione = NULL;
    scavatrice_hartornen.serbatoio_energia = 4;
    scavatrice_hartornen.serbatoio_raccolta = 0;

}//end reset

static void state_cave(Tscavatrice* excavator){ //OK

    int scelta_serbatotio;
    int scelta_direzione;
    int scelta_charge;
    int n_charge = 0;
    int prob_crollo = 1 + (rand() % 100); //gestisce la probabiità di crollo imprevisto
    int n_cave = 1; //stampa il corrente numero della caverna
    prob_incontro += 3; //la probabilità di incontro con la famiglia rivale si aumenta di 3% ogni turno

    if(turno == 1){
      uscita_arvais += 5;
    }else if(turno == 2){
      uscita_hartornen += 5;
    }

    //probabiità di trovare uscita
    if(turno == 1){
      if(uscita_arvais > prob_crollo && uscita_arvais > prob_incontro){
        printf("\n\nYou are in the tunnel and you have this strange feeling...\n");
        sleep(2);
        esci(&excavator);
      }
    }else if(turno == 2){
      if(uscita_hartornen > prob_crollo && uscita_hartornen > prob_incontro){
        printf("\n\nYou are in the tunnel and you have this strange feeling...\n");
        sleep(2);
        esci(&excavator);
      }
    }

    //probabilità di incontro con la famiglia rivale
    if(prob_incontro > uscita_arvais && prob_incontro > prob_crollo){
      puts("");
      printf("\n\033[0mAh oh, you have crossed the tunnel, you now find rival family in the same cave as you\n");
      sleep(2);
      scontro();
    }

    switch(excavator -> posizione -> TipoCaverna) {
      case 0:
        printf(" cave type: Normal |");
        break;
      case 1:
        printf(" cave type: Special |");
        break;
      case 2:
        printf(" cave type: Accidental |");
        break;
      default:
        break;
    }
    printf(" SPICE found: %d units\n", (excavator -> posizione -> QuantitaMelassa));
    printf("\033[0m");
    printf("excavator tank: %d/4 units | collection tank: %d/10 units\n\n", excavator -> serbatoio_energia, excavator -> serbatoio_raccolta);
    sleep(1);

    //diversi tipi di caverna(normale, accidentale, speciale)
    puts("");
    switch (excavator -> posizione -> TipoCaverna){
      case 0:
        printf("This cave seems normal, nothing happened\n");
        break;
      case 1:
        printf("\033[1;32mThe Weirdest thing just happened, you found 1 unit of SPICE right after you entered the cave, you put it in the tank\n");
        printf("\033[0m");
        if(excavator -> serbatoio_raccolta < 10){
          excavator -> serbatoio_raccolta += 1;
        }else if(excavator -> serbatoio_raccolta == 10){
          excavator -> serbatoio_energia += 1;
        }else if(excavator -> serbatoio_energia == 4 && excavator -> serbatoio_raccolta == 10){
          printf("But both of your tanks are full, you have to leave the extra unit of SPIECE in the cave\n");
        }
        break;
      case 2:
        printf("\033[1;33mAH OH, your tank got damaged during the moving, it's leaking energy, you fixed it right away, but it still leaked 1 unit of energy\n");
        printf("\033[0m");
        if(excavator -> serbatoio_raccolta >= 1){
          excavator -> serbatoio_raccolta -= 1;
        }else if(excavator -> serbatoio_energia >=1){
          excavator -> serbatoio_energia -= 1;
        }else{
          printf("\033[1;31mNow you have no energy in either tanks and your excavator is damaged, you lost the game\033[0m\n");
          sleep(2);
          if(turno == 1){
            vittoria_hartornen = 1;
          }else if(turno == 2){
            vittoria_arvais = 1;
          }
          result();
        }
        break;
      default:
        break;
    }
    sleep(1);

    //mettere unita di energia da collezione a serbatotio d'energia
    if(excavator -> serbatoio_raccolta >= 1 && excavator -> serbatoio_energia < 4){
      while(scelta_charge < 0 || scelta_charge > 1){
        printf("Do you want to charge excavator tank with spare energy?\n(0 = No, 1 = Yes): ");
        scanf("%d", &scelta_charge);
        while(getchar() != '\n');
      }
      if(scelta_charge == 1){
        do{
          printf("How many units do you want to charge in the excavator tank from collection tank? ");
          scanf("%d", &n_charge);
          excavator -> serbatoio_raccolta = excavator -> serbatoio_raccolta - n_charge;
          excavator -> serbatoio_energia = excavator -> serbatoio_energia + n_charge;
          if(excavator -> serbatoio_energia > 4){
            printf("You realized that excavator tank is full, so you put excessive units back into the collection tank\n");
            n_charge = excavator -> serbatoio_energia - 4;
            excavator -> serbatoio_energia = 4;
            excavator -> serbatoio_raccolta = excavator -> serbatoio_raccolta + n_charge;
          }
        }while(n_charge > 4 || n_charge < 1);
      }
    }

    //se trova unita di melassa in caverna chiede in quale serbatotio la vuole mettere
    if((excavator -> posizione -> QuantitaMelassa) != 0){
      puts("");
      do{
        printf("You have found %d unit of SPICE\nWhich tank do you want put it in?\n(0 = excavator tank, 1 = collection tank): ", (excavator -> posizione -> QuantitaMelassa));
        scanf("%d", &scelta_serbatotio);
        while(getchar() != '\n');
      }while(scelta_serbatotio < 0 || scelta_serbatotio > 1);

      if(scelta_serbatotio == 0 && excavator -> serbatoio_energia < 4){
        excavator -> serbatoio_energia = excavator -> serbatoio_energia + excavator -> posizione -> QuantitaMelassa;
        if(excavator -> serbatoio_energia > 4){
          printf("\nYou have 4 units of energy in the excavator tank, it is at its max capacity\nExcessive units of energy went to the collection tank\n");
          excavator -> serbatoio_raccolta = excavator -> serbatoio_raccolta + ((excavator -> serbatoio_energia) - 4);
          excavator -> serbatoio_energia  = 4;
          if(excavator -> serbatoio_raccolta > 10){
            printf("Your collection tank is also at its max capacity, you have no choice but leave excessive units of energy in the cave\n");
            excavator -> serbatoio_raccolta = 10;
          }
        }else if(excavator -> serbatoio_energia <= 4){
          printf("\n\033[1;32mYou have successfully collected %d units of SPICE\n", excavator -> posizione -> QuantitaMelassa);
        }
      }else if(scelta_serbatotio == 0 && excavator -> serbatoio_energia >= 4){
        printf("\nYour excavator tank is at its full capacity, you had to put units of energy found in the collection tank\n");
        excavator -> serbatoio_raccolta = excavator -> serbatoio_raccolta + excavator -> posizione -> QuantitaMelassa;
        if(excavator -> serbatoio_raccolta > 10){
          printf("Your collection tank is full, you have no choice but leave excessive units of SPICE in the cave\n");
          excavator -> serbatoio_raccolta = 10;
        }else if(excavator -> serbatoio_raccolta <= 10){
          printf("\n\033[1;32mYou have successfully collected %d units of SPICE\n", excavator -> posizione -> QuantitaMelassa);
        }
      }else if(scelta_serbatotio == 1 && excavator-> serbatoio_raccolta < 10){
        excavator -> serbatoio_raccolta = excavator -> serbatoio_raccolta + excavator -> posizione -> QuantitaMelassa;
        if(excavator -> serbatoio_raccolta > 10){
          printf("\nYou have 10 units of energy in the collection tank, it is at its full capacity\nExcessive units of energy went to the excavator tank\n");
          excavator -> serbatoio_energia = excavator -> serbatoio_energia + ((excavator -> serbatoio_raccolta) - 10);
          excavator -> serbatoio_raccolta = 10;
          if(excavator -> serbatoio_energia > 4){
            printf("Your excavator tank is also at its max capacity, you have no choice but leave excessive units of energy in the cave\n");
            excavator -> serbatoio_energia = 4;
          }
        }else if(excavator -> serbatoio_raccolta < 10){
          printf("\n\033[1;32mYou have successfully collected %d units of SPICE\n", excavator -> posizione -> QuantitaMelassa);
        }
      }else if(scelta_serbatotio == 1 && excavator -> serbatoio_raccolta >= 10){
        printf("Your collection tank is at its max capacity, you had to put units of energy found in the excavator tank\n");
        excavator -> serbatoio_energia = excavator -> serbatoio_energia + (excavator -> posizione -> QuantitaMelassa);
        if(excavator -> serbatoio_energia > 4){
          printf("Your excavator tank is also at its max capacity, you have no choice but leave excessive units of energy in the cave\n");
          excavator -> serbatoio_energia = 4;
        }else if(excavator -> serbatoio_energia <= 4){
          printf("\n\033[1;32mYou have successfully collected %d units of SPICE\n", excavator -> posizione -> QuantitaMelassa);
        }
      }
    }else{
      printf("\nYou didn't find any unit of energy in this cave, try next one\n");
    }
    sleep(1);

    printf("\nYou now have:\n\033[1;33mexcavator tank \033[1;32m%d\033[0m/4 units of energy | \033[1;33mcollection tank \033[1;32m%d\033[0m/10 units of energy\n", excavator -> serbatoio_energia, excavator -> serbatoio_raccolta);

    //diversi casi di imprevisto(nessuno, crollo, baco)
    puts("");
    switch(excavator -> posizione -> TipoImprevisto){
      case 0:
        printf("\033[0mAfter collected all the SPICE in this cave, you heard something moving but nothing happened\n");
        break;
      case 1:
        printf("\033[1;33mSuddenly, the cave starts to collapse, you couldn't make to the next cave in time and you can't go back to the last cave\nYou are buried alive, but your excavator is fine, so you dig through to the next cave using 1 unit of energy\n");
        sleep(1);
        printf("\033[0m");
        if(excavator -> serbatoio_energia >= 1){
          excavator -> serbatoio_energia -= 1;
        }else if(excavator -> serbatoio_raccolta >= 1){
          excavator -> serbatoio_raccolta -= 1;
        }else{
          printf("\033[1;31mNow you have no energy in either tanks and your excavator is damaged, you lost the game\033[0m\n");
          sleep(2);
          if(turno == 1){
            vittoria_hartornen = 1;
          }else if(turno == 2){
            vittoria_arvais = 1;
          }
          result();
        }
        break;
      case 2:
        printf("\033[1;31mOH NO, you have encountered the worst possibile danger in the SPICE extraction, the Sandworm\n\033[0mLuckily it is not interested in your life but it took all your SPICE in both tanks\n");
        printf("\033[0m");
        sleep(1);
        excavator -> serbatoio_energia = 0;
        excavator -> serbatoio_raccolta = 0;
        break;
      default:
        break;
    }

    puts("");
    if(excavator -> posizione -> TipoImprevisto == 0){
      if(prob_crollo >= 75 && prob_crollo > uscita_arvais && prob_crollo > prob_incontro){
        printf("\033[1;33mYou are about to go ahead to next cave, but all the sudden the way to the next cave collapsed\nSo you decide to get around and you discover a new cave that may connect to the next one\n");
        do{
          printf("\n\033[1;34mPlease insert 2 to next turn: \033[0m");
          scanf("%d", &scelta_commando);
          while(getchar() != '\n');
        }while(scelta_commando != 2);
      }else{
        if(excavator -> serbatoio_energia >= 1 || excavator -> serbatoio_raccolta >= 1){
          printf("\033[0mNow you have a choice to make: \n");
          printf(" 0 = GO AHEAD\n 1 = BREAK DOWN A NEW CAVE (cost 1 unit of energy)\n");
          do{
            printf("\033[1;34m\nWhat's your move: \033[0m");
            scanf("%d", &scelta_commando);
            while (getchar() != '\n');
          }while(scelta_commando < 0 || scelta_commando > 1);
        }else if(excavator -> serbatoio_energia <= 0 && excavator -> serbatoio_raccolta <= 0){
          printf("\033[0mYou have no energy at all in either tank, only thing you can do is go to next cave\n");
          do{
            printf("\n\033[1;34mPlease insert 0 to next turn: \033[0m");
            scanf("%d", &scelta_commando);
            while(getchar() != '\n');
          }while(scelta_commando != 0);
        }
      }
        if(scelta_commando == 1){
          if(excavator -> posizione -> Direzione == 0){
            printf(" 1 = Right\n 2 = Left\n");
          }else if(excavator -> posizione -> Direzione == 1){
            printf(" 0 = Ahead\n 2 = Left\n");
          }else if(excavator -> posizione -> Direzione == 2){
            printf(" 0 = Ahead\n 1 = Right\n");
          }
          do{
            printf("Which was is it going to be: ");
            scanf("%d", &cambia_direzione);
            while(getchar() != '\n');
          }while(cambia_direzione < 0 || cambia_direzione > 2);
        }
    }else if(excavator -> posizione -> TipoImprevisto == 1 || excavator -> posizione -> TipoImprevisto == 2){
      printf("\033[0mThe only thing you can do right now is go to the next cave\n");
      do{
        printf("\n\033[1;34mPlease insert 0 to next turn: \033[0m");
        scanf("%d", &scelta_commando);
        while(getchar() != '\n');
      }while(scelta_commando != 0);
    }

    if(scelta_commando == 0){
      avanza(excavator);
      if(turno == 1){
        turno = 2;
      }else if(turno == 2){
        turno = 1;
      }
    }else if(scelta_commando == 1){
      abbatti(&excavator);
      if(turno == 1){
        turno = 2;
      }else if(turno == 2){
        turno = 1;
      }
    }else if(scelta_commando == 2){
      aggira(&excavator);
      if(turno == 1){
        turno = 2;
      }else if(turno == 2){
        turno = 1;
      }
    }

}//end status_cave

void gioca(){

  int n_cave_arvais = 1;
  int n_cave_hartornen = 1;
  int prob_turno = 1 + (rand() % 2);

  if(prob_turno == 1){
    turno = 1;
    printf("Arvais makes first move\n\n");
  }else if(prob_turno == 2){
    turno = 2;
    printf("Hartornen makes first move\n\n");
  }

  do{
    if(turno == 1){
      puts("");
      printf("\033[1;35mTurn Arvais\n\n");
      printf("\033[1;34mcave n. %d |", n_cave_arvais++);
      state_cave(&scavatrice_arvais);
    }else if(turno == 2){
      puts("");
      printf("\033[1;33mTurn Hartornen\n\n");
      printf("\033[1;34mcave n. %d |", n_cave_hartornen++);
      state_cave(&scavatrice_hartornen);
    }
  }while(1);

}//end gioco
