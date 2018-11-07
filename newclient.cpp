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
  std::string buffer(sizeof(DataState), ' ');
  socket_fd = socket(AF_INET, SOCK_STREAM, 0);
  printf("Socket criado\n");

  target.sin_family = AF_INET;
  target.sin_port = htons(6004);
  inet_aton("127.0.0.1", &(target.sin_addr));
  printf("Tentando conectar\n");
  if (connect(socket_fd, (struct sockaddr*)&target, sizeof(target)) != 0) {
    printf("Problemas na conexao\n");
    return 0;
  }
  printf("Conectei ao servidor\n");
  initscr();

  while(1){
    com = getch();
    printf("\ncomando:  %c", com );
    if(com  != -1){
      send(socket_fd,&com,1,0);
      printf("Comando Enviado !\n");
    }
    msg_num = 0;
    msg_len = recv(socket_fd,(void *)buffer, sizeof(DataState), MSG_DONTWAIT);
    if (msg_len > 0) {
      received->unserialize(buffer);
      printw("aaaa:\n"  );
    }
    }

  return 0;
}
