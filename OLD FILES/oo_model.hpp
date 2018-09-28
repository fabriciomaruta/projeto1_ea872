#ifndef MODEL_HPP
#define MODEL_HPP

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
    
};


/*Classe para controlar a tela*/
class Tela{

  public:
    Tela();
    ~Tela();
    void init();
    void stop();
    void update(Corpo *corpo);

};

#endif