#include "model.hpp"
#include <ncurses.h>

using namespace std;
Corpo::Corpo(char avatar, int pos_X, int pos_Y, int old_pos_X, int old_pos_Y) {
  this->avatar = avatar;/*bonequinho do player ou inimigo*/
  this->pos_X = pos_X;
  this->pos_Y = pos_Y;
  this->old_pos_X = old_pos_X;
  this->old_pos_Y = old_pos_Y;  
}

void Corpo::update(int X, int Y) {
  this->old_pos_X = this->pos_X; // Holds _
  this->old_pos_Y = this->pos_Y; // last position
  this->pos_X = X;  // Updates positon
  this->pos_Y = Y;    
}

int Corpo::get_pos_X() {
  return this->pos_X;
}

int Corpo::get_pos_Y() {
  return this->pos_Y;
}

int Corpo::get_old_pos_X() {
  return this->pos_X;
}

int Corpo::get_old_pos_Y() {
  return this->pos_Y;
}



char Corpo::get_avatar() {
  return this->avatar;
}




Tela::Tela(){

}


void Tela::init(){
  initscr(); /*Start curses mode*/
  raw();
  curs_set(0);
}

void Tela::update(Corpo corpo){
    int i ;
    int x, y;

    x = corpo->get_old_pos_X();
    y = corpo->get_old_pos_Y();

    move(x,y);
    echochar(' ');


    /*Desenhar corpos na tela*/
    x = corpo->pos_X;
    y = corpo->pos_Y;
    move(x, y);   /* Move cursor to position */
    echochar(corpo->get_avatar());  /* Prints character, advances a position */
    

    //Atualiza tela
    refresh();

}





// ))))))))))))))))))))))))))))))))))))) NEW CLASSES (((((((((((((((((((((((((((((((((((((((((((((((((




