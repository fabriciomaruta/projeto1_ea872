#include "model.hpp"

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
#define MAX_CONEXOES 10

int socket_fd;

void *receber_respostas(){
  /* Recebendo resposta */
  char reply[60];
  int msg_len;
  int msg_num;
  msg_num = 0;
  while(1) {
  msg_len = recv(socket_fd, reply, 12, MSG_DONTWAIT);
  if (msg_len > 0) {
    printw("aaaa:%s\n",  reply);
  }
  }
}


int main() {
  int socket_fd;
  struct sockaddr_in target;
  char com;
  int msg_len;
  int msg_num;
  DataState *received;
  Corpo *pl1 = new Corpo(' ', 3,2,0,0);
  Enemy *enemy = new Enemy(' ',5,5,0,0);
  Projetil *proj = new Projetil(' ',0,0,0,0,0);
  Tela *tela = new Tela(pl1,enemy,proj,20,20,20,20);

  //enemy->init();
  //pl1->init();
  received = new DataState(pl1,enemy,proj);
  std::string buffer(sizeof(DataContainer), ' ');
  socket_fd = socket(AF_INET, SOCK_STREAM, 0);
  printf("Socket criado\n");

  target.sin_family = AF_INET;
  target.sin_port = htons(7456);
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
  while(1){
    tela->update();
    com = getch();
    //printf("\ncomando:  %c", com );
    //if(com  != -1){
    send(socket_fd,&com,1,0);
      //printf("Comando Enviado !\n");
    //}
    msg_num = 0;
    msg_len = recv(socket_fd,(void *)buffer.c_str(), sizeof(DataContainer), MSG_DONTWAIT);
    if (msg_len > 0) {
      received->unserialize(buffer);
      pl1->set_avatar(received->jogador_get_avatar());
      proj->set_sym(received->projetil_get_avatar());
      proj->disparar(received->projetil_get_pos_X(),received->projetil_get_pos_Y());
      pl1->update(received->jogador_get_pos_X(), received->jogador_get_pos_Y());
      tela->update();
    }

    proj->percorrer();
    }
  tela->stop();
  return 0;
}
