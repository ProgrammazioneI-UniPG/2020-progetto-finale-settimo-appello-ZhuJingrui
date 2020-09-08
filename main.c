/*
NOME: Jingrui
Cognome: Zhu
Progetto: The Dune
*/

#include "gamelib.h"


int main(){

  time_t t;
  srand((unsigned) time(&t));
  int page;

  //la storia del gioco
  system("clear");
  printf("\033[1;32m");
  printf("\n\nWelcome to the world of The Dune\n\n");
  sleep(1);
  printf("\033[0mYou find yourself on the only planet in the universe that produce the most precious substance which not only allows interstellar travel but also extend life and expand consciousness: ");
  printf("\033[1;32mThe SPICE\n\n");
  sleep(1);
  printf("\033[1;35mArvais \033[0mand \033[1;33mHartornen ");
  printf("\033[0mare two families who have contended for generations foe the extraction of the substance\n\n");
  sleep(1);
  printf("The extraction is troublesome and extremly dangerous like a sudden landslide or the direct conflict with rival family\n\n");
  printf("But most dangerous of all, a horrifing creature known as");
  printf(" \033[1;31mThe Sandworm...");
  sleep(2);
  printf("\n\n\n\033[0mPress any character to read game istructions: ");
  if(scanf("%ls", &page) != '\0')
    system("clear");

  //how to play
  printf("\n\n\nThe game is composed of 2 players, one represent each family\n\n");
  printf("Each player is allowed only one move only each turn\n\n");
  printf("Each player is asked to create a tunnel for its famly of at least 10 caves stating the direction and type of each cave\n\n");
  printf("At the begining of the game, the excavator tank is set to its max capacity(4 unit) while the collection tank is set to 0(max capacity 10 unit)\n\n");
  printf("Each cave has the possibility of finding 0, 1 or 2 units of enery and player will decide in which tank goes to\n\n");
  printf("As well as the possibility of encounter different type of danger in each cave\n\n");
  printf("The chance of whom starts the game is 50/50\n\n");
  printf("Each turn is allowed only one of following 2 moves: \n\n");
  printf("\tMOVE AHEAD to the next cave with out any cost of energy\n\n");
  printf("\tBREAK DOWN the cave of the different direction\n\n");
  printf("\n\n\nPress any character to start the game: ");
  if(scanf("%ls", &page) != '\0')
    system("clear");

    menu_principale();

}// end main
