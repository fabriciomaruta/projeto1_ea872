#include <string.h>
#include <iostream>
#include <ncurses.h>
#include <unistd.h>
#include <cstdlib>
#include <time.h>
#include <chrono>
#include "model.hpp"
#include "playback.hpp"
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <thread>
#define MAX_CONEXOES 10

/*Socket variables*/
int socket_fd, connection_fd[MAX_CONEXOES], conexao_usada[MAX_CONEXOES];
socklen_t client_size;
struct sockaddr_in myself, client;

char *input;
int running ;

using namespace std;

using namespace std::chrono;
uint64_t get_now_ms() {
    return duration_cast<milliseconds>(steady_clock::now().time_since_epoch()).count();
}

int adicionar_conexao(int new_connection_fd) {
    int i;
    for (i=0; i<MAX_CONEXOES; i++) {
        if (conexao_usada[i] == 0) {
            conexao_usada[i] = 1;
            connection_fd[i] = new_connection_fd;
            return i;
        }
    }
    return -1;
}

void wait_connections() {
    int conn_fd;
    int user_id;
    while(running) {
        conn_fd = accept(socket_fd, (struct sockaddr*)&client, &client_size);
        user_id = adicionar_conexao(conn_fd);

    }
    return ;
}


int remover_conexao(int user) {
    if (conexao_usada[user]==1) {
        conexao_usada[user] = 0;
        close(connection_fd[user]);
    }
    return 1;
}


void Holds(float time) {
    float t0;
    float t1;

    t0 = get_now_ms();

    while(1) {
        t1 = get_now_ms();
        if(t1 - t0 > time) break;
    }

}

void cleanS() {
    for(int i = 0; i<25; i++) {
        move(i,0);
        printw("                                              ");
    }
}

void win() {
    for(int i = 0; i<25; i++) {
        move(i,0);
        printw("                                              ");
    }
    move(5,7);
    printw("Y O U");
    move(6,7);
    printw("W I N !");
}

void lose() {
    for(int i = 0; i<25; i++) {
        move(i,0);
        printw("                                              ");
    }
    move(5,7);
    printw("Y O U");
    move(6,7);
    printw("L O S E !");
}

int main() {
    /*Interface variables*/
    DataState *pass;
    std::string buffer(sizeof(DataContainer), ' ');
    Audio::Sample *loser;
    Audio::Sample *winner;
    loser = new Audio::Sample();
    winner = new Audio::Sample();
    loser->load("assets/loser.dat");
    winner->load("assets/win.dat");
    Audio::Player *player;
    player = new Audio::Player();
    int i,j;
    char c;
    /*Time control variables*/
    float t0;
    float t1;
    float t_base_A, t_base_B;
    float deltaT_A, deltaT_B;
    float T;
    /*Time control initialization*/
    T = get_now_ms();
    t_base_A = get_now_ms();
    t_base_B = get_now_ms();
    t1 = T;
    /*Socket variables*/
    char *input_buffer;
    char *output_buffer;
    int user_iterator;
    int msglen;
    /*Socket variables initialization*/
    client_size = (socklen_t)sizeof(client);
    for (int i=0; i<MAX_CONEXOES; i++) {
        conexao_usada[i] = 0;
    }
    running = 1;
    socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    myself.sin_family = AF_INET;
    myself.sin_port = htons(7456);
    inet_aton("127.0.0.1", &(myself.sin_addr));
    printf("Tentando abrir porta 3001\n");
    if (bind(socket_fd, (struct sockaddr*)&myself, sizeof(myself)) != 0) {
        printf("Problemas ao abrir porta\n");
        return 0;
    }
    printf("Abri porta 3001");
    listen(socket_fd, 2);
    std::thread connection_control(wait_connections); //Insert arguments
    Corpo *avatar = new Corpo('@', 6, 7,0,0);
    Enemy *enemy = new Enemy('*', 5, 5,0,0);
    Projetil *proj = new Projetil('|',0,0,0,0,0);
    enemy->init();
    avatar->init();
    Tela *tela = new Tela(avatar,enemy,proj,20,20,20,20);
    tela->init();
    pass = new DataState(avatar,enemy,proj);
    /*Run elements of interface*/
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
    cleanS();
    move(5,25);
    printw("Points: ");
    tela->initMap();
    /* ------------------------- LACO PRINCIPAL --------------------------*/
    /*Dispara thread para ouvir conexoes*/
    while (game_over != 1) {
        t0 = t1;
        t1 = get_now_ms();
        deltaT_A = t1-t_base_A;
        deltaT_B = t1-t_base_B;
        // Periodo do movimento do inimigo
        if(deltaT_A > PeriodMovEnemy) {
            deltaT_A = 0;
            t_base_A = t1;
            enemy->move();
        }
        // Se o projetil estiver ativo (em percurso)...
        if(proj->isAtivo() == 1) {
            if(deltaT_B > PeriodMovProj) {
                deltaT_B = 0;
                t_base_B = t1;
                proj->percorrer();
            }
        }

        /*-------------LER DENTRO DA THREAD?[PEGA TECLADO] ----------------------*/
        for(user_iterator = 0; user_iterator < MAX_CONEXOES; user_iterator ++) {
            if(conexao_usada[user_iterator] == 1) {
                msglen = recv(connection_fd[user_iterator], &c, 1, MSG_DONTWAIT);
                if (msglen > 0 && game_over != 1) {
                    if (c == ' ' && proj->isAtivo() == 0) {
                        int X = avatar->get_pos_X();
                        int Y = avatar->get_pos_Y();
                        proj->setAtivo(1);
                        proj->disparar(X,Y);
                    }
                    if (c == 's') {
                        avatar->moveTop();
                    }
                    if (c == 'w') {
                        avatar->moveBottom();
                    }
                    if (c == 'd') {
                        avatar->moveRight();
                    }
                    if (c == 'a') {
                        avatar->moveLeft();
                    }
                    if(c == 'q') {
                        running = 0;
                        game_over = 1;
                        break;
                    }
                    for(int ret = 0; ret<MAX_CONEXOES;ret++){
                      if(conexao_usada[ret] == 1){
                          pass->atualiza(avatar,enemy,proj);
                          tela->update();
                          pass->serialize(buffer);
                          if(send(connection_fd[user_iterator],pass,sizeof(DataContainer), MSG_NOSIGNAL) == -1){
                            remover_conexao(ret);
                          }
                      }
                    }
                }
            }
            tela->update();
        }
    }

    if(game_over == 1) {
        if(ganhou) {
            //player->play(winner);
            t0 = get_now_ms();
            while(1) {
                std::this_thread::sleep_for (std::chrono::milliseconds(1));
                t1 = get_now_ms();
                if(t1-t0 > 10) break;
            }
            winner->set_position(0);
            win();
        }
        else {
            //player->play(loser);
            t0 = get_now_ms();
            while(1) {
                std::this_thread::sleep_for (std::chrono::milliseconds(1));
                t1 = get_now_ms();
                if(t1-t0 > 10) break;
            }
            loser->set_position(0);
            lose();
        }
    }
    // Segura na tela por 1.5 seg
    t0 = get_now_ms();
    while(1) {
        t1 = get_now_ms();
        if(t1 - t0 > 1500) break;
    }
    tela->stop();
    close(socket_fd);
    connection_control.join();
    return 0;
}
