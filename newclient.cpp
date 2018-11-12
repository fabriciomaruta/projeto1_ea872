#include <string.h>
#include <iostream>
#include <ncurses.h>
#include <unistd.h>
#include <cstdlib>
#include <time.h>
#include <chrono>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <thread>
#include "model.hpp"

#define MAX_CONEXOES 10

int socket_fd;
ListaDeCorpos *jogadores = new ListaDeCorpos();
void mandar_resposta(){
  char com;
  com = getch();
  send(socket_fd,&com,1,0);

}


int main() {
  //std::thread wait_kb(mandar_resposta);
  int socket_fd;
  struct sockaddr_in target;
  int old_x = 0;
  int old_y = 0;
  int msg_len;
  int msg_num;
  char com;
  DataState *received;
  //Corpo * c[MAX_CONEXOES];
  Corpo *pl1 = new Corpo(' ', 10,2,0,0,0);
  Enemy *enemy = new Enemy(' ',5,5,0,0);
  Projetil *proj = new Projetil(' ',0,0,0,0,0);
  Tela *tela = new Tela(pl1,enemy,proj,20,20,20,20);
  Corpo *player[MAX_CONEXOES];
  Teclado *teclado = new Teclado();
  teclado->init();
  //enemy->init();
  //pl1->init();
  received = new DataState(pl1,enemy,proj);
  std::string buffer(sizeof(DataContainer), ' ');
  socket_fd = socket(AF_INET, SOCK_STREAM, 0);
  printf("Socket criado\n");
  target.sin_family = AF_INET;
  target.sin_port = htons(7466);
  inet_aton("127.0.0.1", &(target.sin_addr));
  printf("Tentando conectar\n");

  if (connect(socket_fd, (struct sockaddr*)&target, sizeof(target)) != 0) {
    printf("Problemas na conexao\n");
    return 0;
  }

  printf("Conectei ao servidor\n");
  //initscr();
  tela->init();
  tela->initMap();
  Corpo *c = new Corpo(' ',0, 0,0,0, 0);
  while(1){
    //tela->update();
    com = teclado->getchar();
    send(socket_fd,&com,1,0);
    msg_num = 0;
    msg_len = recv(socket_fd,(void *)buffer.c_str(), sizeof(DataContainer), MSG_NOSIGNAL);

    if (msg_len > 0) {
      received->unserialize(buffer);

      c->set_avatar(received->jogador_get_avatar());
      c->set_pos_X(received->jogador_get_pos_X());
      c->set_pos_Y(received->jogador_get_pos_Y());

      //player[received->jogador_get_indice()] = c ;
      tela->set_Corpo(c);
      //(player)[received->jogador_get_indice()]->set_avatar(received->jogador_get_avatar());
      //(player)[received->jogador_get_indice()]->update(received->jogador_get_pos_X(), received->jogador_get_pos_Y());
      //old_x = received->jogador_get_pos_X();
      //old_y = received->jogador_get_pos_Y();
    }
    tela->update();
    proj->percorrer();
    }
  teclado->stop();
  tela->stop();
  //wait_kb.join();
  return 0;
}
