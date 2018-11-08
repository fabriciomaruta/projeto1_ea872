#ifndef MODEL_HPP
#define MODEL_HPP
#include <vector>
#include<thread>
#include <fstream>
#include <iostream>
#include <string>



extern char map[24][24];
extern int ganhou;
extern int game_over;
extern float PeriodMovEnemy;
extern float PeriodMovProj;
struct DataContainer {
  int jogador_X;
  int jogador_Y;
  char jogador_avatar;

  int inimigo_X;
  int inimigo_Y;
  char inimigo_avatar;

  int projetil_X;
  int projetil_Y;
  char projetil_avatar;

};

class Projetil {
    private:
    char sym;
    int ativo;
    int posX;
    int posY;
    int old_posX;
    int old_posY;

    public:
    Projetil(char syym, int ativo, int posX, int posY, int old_posX, int old_posY);
    void disparar(int origemX, int origemY);
    void percorrer();
    void set_sym(char sym);
    int get_old_posX();
    int get_old_posY();
    int get_posX();
    int get_posY();
    char get_sym();
    int isAtivo();
    void setAtivo(int value);
};


class Corpo { /*Classe para criar o personagem e as caracteristicas*/
  private:
  char avatar; /*Caractere que define o personagem*/
  int pos_X;
  int pos_Y;
  int old_pos_X;
  int old_pos_Y;

  public:
    Corpo(char avatar, int pos_X, int pos_Y, int old_pos_X, int old_pos_Y);
    void init();
    void set_avatar(char avatar);
    void update(int X, int Y);
    int get_pos_X();
    int get_pos_Y();
    int get_old_pos_X();
    int get_old_pos_Y();
    char get_avatar();
    void moveTop();
    void moveBottom();
    void moveRight();
    void moveLeft();
};

class ListaDeCorpos { /*Classe para controlar lista de "personagens"*/
 private:
    std::vector<Corpo*> *corpos;

  public:
    ListaDeCorpos();
    void hard_copy(ListaDeCorpos *ldc);
    void add_corpo(Corpo *c);
    std::vector<Corpo*> *get_corpos();
};



class Enemy{
public:
    char avatar;
    int pos_X;
    int pos_Y;
    int old_pos_X;
    int old_pos_Y;

public:
    Enemy(char c,int pX,int pY, int old_pX, int old_pY);
    void init();
    int get_pos_X();
    int get_pos_Y();
    int get_old_pos_X();
    int get_old_pos_Y();
    char get_avatar();
    void move();
};




/*Classe para controlar a tela*/
class Tela{
  private:
    //ListaDeCorpos *lista, *lista_anterior;
    Corpo *corpo;
    Enemy *enemy;
    Projetil *proj;
    int maxI, maxJ;
    float maxX, maxY;


  public:
    Tela(Corpo *corpo,Enemy *enemy,Projetil *proj, int maxI, int maxJ, float maxX, float maxY);
    ~Tela();
    void init();
    void initMap();
    void stop();
    void update();

};

void threadfun(char *keybuffer, int *control);

class Teclado {
private:
  char ultima_captura;
  int rodando;

  std::thread kb_thread;

public:
  Teclado();
  ~Teclado();
  void stop();
  void init();
  char getchar();
};

class DataState {
private:
  DataContainer data;
public:
  DataState();
  DataState(Corpo *jogador, Enemy *inimigo, Projetil *projetil);
  DataState(std::string buffer_in);
  void atualiza(Corpo *jogador, Enemy *inimigo, Projetil *projetil);
  void serialize(std::string &buffer_out);
  void unserialize(std::string buffer_in);
  char jogador_get_avatar();
  int jogador_get_pos_X();
  int jogador_get_pos_Y();
  char projetil_get_avatar();
  int projetil_get_pos_X();
  int projetil_get_pos_Y();
};




#endif
