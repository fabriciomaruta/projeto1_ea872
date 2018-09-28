#ifndef MODEL_HPP
#define MODEL_HPP
#include <vector>
#include<thread>


extern char map[24][24];
extern int ganhou;
extern int game_over;
extern float PeriodMovEnemy;
extern float PeriodMovProj;

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






#endif