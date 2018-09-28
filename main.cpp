#include <iostream>
#include <ncurses.h>
#include <unistd.h>
#include <cstdlib>
#include <time.h>
#include<chrono>
#include "model.hpp"
#include "playback.hpp"

using namespace std;

using namespace std::chrono;
uint64_t get_now_ms() {
  return duration_cast<milliseconds>(steady_clock::now().time_since_epoch()).count();
}




void Holds(float time){
    float t0;
    float t1;
    
    t0 = get_now_ms();
    
    while(1){
        t1 = get_now_ms();
        if(t1 - t0 > time) break;
    } 
    
}

void cleanS(){
    for(int i = 0; i<25; i++){
        move(i,0);
        printw("                                              ");
    }


}


void win(){
    for(int i = 0; i<25; i++){
        move(i,0);
        printw("                                              ");
    }

        move(5,7);
        printw("Y O U");


        move(6,7);
        printw("W I N !");
}

void lose(){
    for(int i = 0; i<25; i++){
        move(i,0);
        printw("                                              ");
    }

        move(5,7);
        printw("Y O U");


        move(6,7);
        printw("L O S E !");
}


int main(){
    Audio::Sample *loser;
    Audio::Sample *winner;
    loser = new Audio::Sample();
    winner = new Audio::Sample();
    loser->load("assets/loser.dat");
    winner->load("assets/win.dat");

    Audio::Player *player;
    player = new Audio::Player();
    player->init();
    int i,j;
    char c;
    Corpo *avatar = new Corpo('@', 1, 1,0,0);
    Enemy *enemy = new Enemy('*', 5, 5,0,0);
    Projetil *proj = new Projetil('|',0,0,0,0,0);

    Tela *tela = new Tela(avatar,enemy,proj,20,20,20,20);
    tela->init();
    Teclado *teclado = new Teclado();
    teclado->init();

    float t0;
    float t1;
    float t_base_A, t_base_B;
    float deltaT_A, deltaT_B;
    float T;
    T = get_now_ms();
    t_base_A = get_now_ms();
    t_base_B = get_now_ms();
    t1 = T;

    enemy->init();

    
    //cleanS();
    
    move(5,9);
    printw("* * * * * * * * * * * * *");   

    
    
    move(6,12);
    printw("P H A R A O H ' S");


    move(7,12);
    printw(". _ . _ . _ . _ . _ .");

    
    
    move(8,12);
    printw("L A B Y R I N T H");  

    
    move(9,9);
    printw("* * * * * * * * * * * * *");    
    
        Holds(2000);
    //Holds(5500);
                    
                    

    cleanS();   
    
    tela->initMap();
    
    
    move(5,25);
    printw("Points: ");
  
    

    /* ------------------------- LACO PRINCIPAL --------------------------*/

    while (1) {
        t0 = t1;
        t1 = get_now_ms();
        deltaT_A = t1-t_base_A;
        deltaT_B = t1-t_base_B;

        // Periodo do movimento do inimigo
        if(deltaT_A > PeriodMovEnemy){
            deltaT_A = 0;
            t_base_A = t1;
            enemy->move();
        }

        // Se o projetil estiver ativo (em percurso)...
        if(proj->isAtivo() == 1){
            if(deltaT_B > PeriodMovProj){
                deltaT_B = 0;
                t_base_B = t1;
                proj->percorrer();
            }
        }
        
        c = teclado->getchar();
        tela->update();




        // Somente dispara o projetil se nao houver outro em execucao
        if (c == 32 && proj->isAtivo() == 0){
            // Passa para pos. inicial do projetil a posicao
            // atual do avatar.
            int X = avatar->get_pos_X();
            int Y = avatar->get_pos_Y();
            proj->setAtivo(1);
            proj->disparar(X,Y);
        }

        if (c =='s'){
          avatar->moveTop();
          while(1){
            std::this_thread::sleep_for (std::chrono::milliseconds(1));
            t1 = get_now_ms();
            if(t1-t0 > 10) break;
          }
        }
        if (c =='w'){
          avatar->moveBottom();
          while(1){
            std::this_thread::sleep_for (std::chrono::milliseconds(1));
            t1 = get_now_ms();
            if(t1-t0 > 10) break;
          }
        }
        if (c =='d'){
          avatar->moveRight();
          while(1){
            std::this_thread::sleep_for (std::chrono::milliseconds(1));
            t1 = get_now_ms();
            if(t1-t0 > 10) break;
          }
        }
        if (c =='a'){
          avatar->moveLeft();
          while(1){
            std::this_thread::sleep_for (std::chrono::milliseconds(1));
            t1 = get_now_ms();
            if(t1-t0 > 10) break;
          }
        }

        if(c == 'q' || game_over == 1){
              t0 = get_now_ms();
          while(1){
            std::this_thread::sleep_for (std::chrono::milliseconds(1));
            t1 = get_now_ms();
            if(t1-t0 > 10) break;
          }
          break;
        }
      std::this_thread::sleep_for (std::chrono::milliseconds(10));
      //i++;

    }

    if(game_over == 1) {
      if(ganhou){
	 player->play(winner);
      t0 = get_now_ms();
      while(1){
        std::this_thread::sleep_for (std::chrono::milliseconds(1));
        t1 = get_now_ms();
        if(t1-t0 > 10) break;
      }
      winner->set_position(0);
	win();
      }
      else{
	 player->play(loser);
      t0 = get_now_ms();
      while(1){
        std::this_thread::sleep_for (std::chrono::milliseconds(1));
        t1 = get_now_ms();
        if(t1-t0 > 10) break;
      }
      loser->set_position(0);
      lose();
	lose();
      }
    }

    // Segura na tela por 1.5 seg
    t0 = get_now_ms();
    while(1){
    t1 = get_now_ms();
    if(t1 - t0 > 1500) break;
    }


  //player->stop();
  tela->stop();
  teclado->stop();
	return 0;
}
