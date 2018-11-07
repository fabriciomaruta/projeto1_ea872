#include <iostream>
#include "model.hpp"
#include <ncurses.h>
#include <vector>
#include <cstring>
#include <string>
using namespace std;


// GLOBAIS
int apagar = 0;
int points = 0;
int max_points = 7;
int ganhou = 0;
int isEnemyAlive = 1;
int game_over = 0;
float PeriodMovEnemy = 500; // Periodo do movimento do inimigo
float PeriodMovProj = 250;  // Periodo do movimento do Projetil


char map[24][24] = {
                {'#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#'},
                {'#',' ',' ',' ',' ',' ',' ',' ',' ',' ','#',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','#'},
                {'#',' ',' ',' ',' ',' ',' ',' ',' ',' ','#',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','#'},
                {'#',' ',' ','.',' ',' ',' ',' ',' ',' ','#',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','#'},
                {'#',' ',' ',' ',' ',' ',' ',' ',' ',' ','#',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','#'},
                {'#',' ',' ',' ',' ',' ',' ',' ',' ',' ','#','.',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','#'},
                {'#',' ',' ',' ',' ',' ',' ',' ',' ',' ','#',' ',' ',' ',' ',' ',' ',' ',' ','.',' ',' ',' ','#'},
                {'#',' ',' ',' ',' ',' ',' ',' ',' ',' ','#',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','#'},
                {'#',' ',' ',' ',' ',' ',' ',' ','#','#','#','#','#',' ',' ',' ',' ','.',' ',' ',' ',' ',' ','#'},
                {'#',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','#'},
                {'#',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','#'},
                {'#',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','#'},
                {'#',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','#'},
                {'#',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','#'},
                {'#',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','#'},
                {'#',' ',' ','.',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','#'},
                {'#',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','#','#','#','#','#',' ',' ',' ','#'},
                {'#',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','#',' ',' ',' ','#'},
                {'#',' ',' ',' ',' ',' ',' ',' ',' ',' ','.',' ',' ',' ',' ',' ',' ','.',' ','#',' ',' ',' ','#'},
                {'#','.',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','#',' ',' ',' ','#'},
                {'#',' ',' ',' ','#',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','#',' ','.',' ','#'},
                {'#',' ',' ',' ','#',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','#',' ',' ',' ','#'},
                {'#',' ',' ',' ','#',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','#',' ',' ',' ','#'},
                {'#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#'},
                };

Projetil::Projetil(char sym, int ativo, int posX, int posY, int old_posX, int old_posY) {
    this->sym = sym;
    this->ativo = ativo;
    this->posX = posX;
    this->posY = posY;
    this->old_posX = old_posX;
    this->old_posY = old_posY;
}

void Projetil::disparar(int origemX, int origemY){
    this->posX = origemX;
    this->posY = origemY;
    this->old_posX = 0;
    this->old_posY = 0;
    this->ativo = 1;
}

int Projetil::get_old_posX() {
  return this->old_posX;
}

int Projetil::get_old_posY() {
  return this->old_posY;
}

int Projetil::get_posX() {
  return this->posX;
}

int Projetil::get_posY() {
  return this->posY;
}

char Projetil::get_sym() {
  return this->sym;
}

int Projetil::isAtivo(){
  return this->ativo;
}

void Projetil::percorrer(){
    int X = this->get_posX();
    int Y = this->get_posY();
    //map[X][Y] = ' ';
    this->old_posX = X;
    this->old_posY = Y;



    // Se o projetil nao colidir com parede move
    // caso contrario, desativa.
    if(map[X][Y-1] != '#'){
        int nY = this->get_posY() - 1;
        this->posY = nY;
        if(map[X][nY] == '*'){
            isEnemyAlive = 0;
            PeriodMovEnemy = 125;
        }
    }else{
        this->setAtivo(0);
        apagar = 1;
    }
}

void Projetil::setAtivo(int value){
    this->ativo = value;
}


Corpo::Corpo(char avatar, int pos_X, int pos_Y, int old_pos_X, int old_pos_Y) {
  this->avatar = avatar;/*bonequinho do player ou inimigo*/
  this->pos_X = pos_X;
  this->pos_Y = pos_Y;
  this->old_pos_X = old_pos_X;
  this->old_pos_Y = old_pos_Y;
}
void Corpo::init(){
    int X = get_pos_X();
    int Y = get_pos_Y();
    map[X][Y] = get_avatar();
};
void Corpo::update(int X, int Y) {
  this->old_pos_X = this->pos_X; // Holds _
  this->old_pos_Y = this->pos_Y; // last position
  this->pos_X = X;  // Updates positon
  this->pos_Y = Y;

  if(map[X][Y] == '.'){
    map[X][Y] = ' ';
    points++;
    PeriodMovProj -= 30;
  }else if(map[X][Y] == '*'){
    game_over = 1;
    ganhou = 0;
    //points--;
  }else if(map[X][Y] == '+'){
    game_over = 1;
    ganhou = 1;
    //points--;
  }


}

int Corpo::get_pos_X() {
  return this->pos_X;
}

int Corpo::get_pos_Y() {
  return this->pos_Y;
}

int Corpo::get_old_pos_X() {
  return this->old_pos_X;
}

int Corpo::get_old_pos_Y() {
  return this->old_pos_Y;
}

char Corpo::get_avatar() {
  return this->avatar;
}

void Corpo::moveTop(){
    int x = this->get_pos_X();
    int y = this->get_pos_Y()+1;

    // Verifica viabilidade do movimento
    if(map[x][y] != '#'){
        this->update(x,y);
    }
}

void Corpo::moveBottom(){
    int x = this->get_pos_X();
    int y = this->get_pos_Y()-1;

    // Verifica viabilidade do movimento
    if(map[x][y] != '#'){
        this->update(x,y);
    }
}


void Corpo::moveRight(){
    int x = this->get_pos_X()+1;
    int y = this->get_pos_Y();

    // Verifica viabilidade do movimento
    if(map[x][y] != '#'){
        this->update(x,y);
    }
}

void Corpo::moveLeft(){
    int x = this->get_pos_X()-1;
    int y = this->get_pos_Y();

    // Verifica viabilidade do movimento
    if(map[x][y] != '#'){
        this->update(x,y);
    }
}


ListaDeCorpos::ListaDeCorpos() {
  this->corpos = new std::vector<Corpo *>(0);
}
void ListaDeCorpos::add_corpo(Corpo *c) {
  (this->corpos)->push_back(c);
}
void ListaDeCorpos::hard_copy(ListaDeCorpos *ldc) {
  std::vector<Corpo *> *corpos = ldc->get_corpos();

  for (int k=0; k<corpos->size(); k++) {
    Corpo *c = new Corpo( (*corpos)[k]->get_avatar(), (*corpos)[k]->get_pos_X(), (*corpos)[k]->get_pos_Y(), (*corpos)[k]->get_old_pos_X(), (*corpos)[k]->get_old_pos_Y());
    this->add_corpo(c);

  }

}
std::vector<Corpo*> *ListaDeCorpos::get_corpos() {
  return (this->corpos);
}


Tela::Tela(Corpo *corpo,Enemy *enemy,Projetil *proj,int maxI, int maxJ, float maxX, float maxY){
 this->corpo = corpo;
 this->enemy = enemy;
 this->proj = proj;
 this->maxI = maxI;
 this->maxJ = maxJ;
 this->maxX = maxX;
 this->maxY = maxY;

}

void Tela::stop() {
  endwin();
}

Tela::~Tela() {
  this->stop();;
}
void threadfun (char *keybuffer, int *control)
{
  char c;
  while ((*control) == 1) {
    c = getch();
    if (c!=ERR) (*keybuffer) = c;
    else (*keybuffer) = 0;
    std::this_thread::sleep_for (std::chrono::milliseconds(10));
  }
  return;
}

void Tela::init(){
    initscr(); /*Start curses mode*/
    raw();
    curs_set(0);

}

void Tela::initMap(){
    for(int x=0;x<24;x++){
        for(int y=0;y<24;y++){
                move(x, y);
                echochar(map[y][x]);
            }
    }
}

void Tela::update(){
  int linha, coluna;
  int i,j;
  int i_enemy,j_enemy;
  int i_proj, j_proj;
  int x, y;
  int x_proj, y_proj;
  int x_enemy, y_enemy;



    if((this->proj)->isAtivo()){
        i_proj = (int) ((this->proj)->get_old_posX());//*(this->maxI/this->maxX);
        j_proj = (int) ((this->proj)->get_old_posY());//*(this->maxJ/this->maxY);

        /* Apaga a posicao antiga do projetil da tela */
        move(j_proj,i_proj);   /* Move cursor to position */
        echochar(' ');


        x_proj = this->proj->get_posX();
        y_proj = this->proj->get_posY();

        move(y_proj, x_proj);
        echochar(this->proj->get_sym());
    }

    if(apagar == 1){
        i_proj = (int) ((this->proj)->get_old_posX());//*(this->maxI/this->maxX);
        j_proj = (int) ((this->proj)->get_old_posY());//*(this->maxJ/this->maxY);

        /* Apaga a posicao antiga do projetil da tela */
        move(j_proj,i_proj);   /* Move cursor to position */
        echochar(' ');
        apagar = 0;
    }






  getmaxyx(stdscr, linha, coluna); /*linha e coluna armazenam o tamanho da tela*/
  i = (int) ((this->corpo)->get_old_pos_X());//*(this->maxI/this->maxX);
  j = (int) ((this->corpo)->get_old_pos_Y());//*(this->maxJ/this->maxY);

    /* Apaga a posicao antiga do avatar da tela */
    move(j,i);   /* Move cursor to position */
    echochar(' ');

  i_enemy = (int) ((this->enemy)->get_old_pos_X());//*(this->maxI/this->maxX);
  j_enemy = (int) ((this->enemy)->get_old_pos_Y());//*(this->maxJ/this->maxY);

    /* Apaga a posicao antiga do inimigo da tela */
    move(j_enemy,i_enemy );   /* Move cursor to position */
    echochar(' ');




  x = this->corpo->get_pos_X();
  y = this->corpo->get_pos_Y();

    move(y, x);
    echochar(this->corpo->get_avatar());



  x_enemy = this->enemy->get_pos_X();
  y_enemy = this->enemy->get_pos_Y();

    move(y_enemy, x_enemy);
    echochar(this->enemy->get_avatar());

  refresh();



  // Imprime a pontuacao na tela
  move(6,25);
  echochar(48+points);


}

Teclado::Teclado(){
}
Teclado::~Teclado(){
}

void Teclado::init(){
  //Inicializa ncurses
  raw();
  keypad(stdscr,TRUE);
  noecho();
  curs_set(0);

  this->rodando = 1;
  std::thread newthread(threadfun, &(this->ultima_captura), &(this->rodando));
  (this->kb_thread).swap(newthread);
}

void Teclado::stop(){
  this->rodando = 0;
  (this->kb_thread).join();
}

char Teclado::getchar(){
  char c = this->ultima_captura;
  this->ultima_captura = 0;
  return c;
}







Enemy::Enemy(char c,int pX,int pY,int old_pX, int old_pY){
    this->avatar = c;
    this->pos_X = pX;
    this->pos_Y = pY;
    this->old_pos_X = old_pX;
    this->old_pos_Y = old_pY;
};

void Enemy::init(){
    int X = get_pos_X();
    int Y = get_pos_Y();
    map[X][Y] = '*';
};



int Enemy::get_pos_X() {
  return this->pos_X;
}

int Enemy::get_pos_Y() {
  return this->pos_Y;
}

int Enemy::get_old_pos_X() {
  return this->old_pos_X;
}

int Enemy::get_old_pos_Y() {
  return this->old_pos_Y;
}

char Enemy::get_avatar() {
  return this->avatar;
}


void Enemy::move(){
    srand(time(NULL));

    // Armazena a posicao atual na antiga
    // pois a pos. sera atualizada a seguir
    int B_pos_X = this->get_pos_X();
    int B_pos_Y = this->get_pos_Y();
    this->old_pos_X = B_pos_X;
    this->old_pos_Y = B_pos_Y;


    int movement;
    int mov_permitido = 0;
    int X, Y;
    map[B_pos_X][B_pos_Y] = ' ';



    /* Calcula um movimento aleatorio e que permitido
        ou seja, dentro dos limites do mapa.
     */

    if(!isEnemyAlive) this->avatar = '+';

    while(!mov_permitido){
        movement = rand()%4;
        // Direita
        if(movement == 0){
            X = get_old_pos_X() + 1;
            Y = get_old_pos_Y();
        // Esquerda
        }else if(movement == 1){
            X = get_old_pos_X() - 1;
            Y = get_old_pos_Y();
        // Cima
        }else if(movement == 2){
            X = get_old_pos_X();
            Y = get_old_pos_Y() + 1;
        // Baixo
        }else{
            X = get_old_pos_X();
            Y = get_old_pos_Y() - 1;
        }

        if(map[X][Y] != '.' && map[X][Y] != '#') mov_permitido = 1;
    }

    this->pos_X = X;
    this->pos_Y = Y;

    map[X][Y] = this->get_avatar();

};


DataState::DataState(){

};
DataState::DataState(Corpo *jogador, Enemy *inimigo, Projetil *projetil){
  this->data.jogador_X = jogador->get_pos_X();
  this->data.inimigo_X = inimigo->get_pos_X();
  this->data.projetil_X = projetil->get_posX();
}

DataState::DataState(std::string buffer_in){
  this->unserialize(buffer_in);
}

void DataState::serialize(std::string &buffer_out){
    std::memcpy((void*)buffer_out.c_str(), &(this->data), sizeof(DataContainer));
}
void DataState::unserialize(std::string buffer_in) {
  std::memcpy(&(this->data), (void*)buffer_in.c_str(), sizeof(DataContainer));
}
