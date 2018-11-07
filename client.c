#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <curses.h>

int socket_fd;

void *receber_respostas(void *parametros) {
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
  pthread_t receiver;
  char com;
  char reply[60];
  int msg_len;
  int msg_num;

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
  //pthread_create(&receiver, NULL, receber_respostas,NULL);
  initscr();

  while(1){
    com = getch();
    printf("\ncomando:  %c", com );
    if(com  != -1){
      send(socket_fd,&com,1,0);
      printf("Comando Enviado !\n");
    }
    msg_num = 0;
    msg_len = recv(socket_fd, reply, 12, MSG_DONTWAIT);
    if (msg_len > 0) {
      printw("aaaa:%s\n",  reply);
    }
    //sleep(1);
    }

  return 0;
}
