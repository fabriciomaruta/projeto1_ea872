#include <ncurses.h>
#include <unistd.h>
#include <cstdlib>
#include <time.h>

using namespace std;


class enemy{
public:
    char body;
    int posX;
    int posY;
    

public:
    enemy(char c,int pX,int pY);
    void init(int posicaoX, int posicaoY);
    void move(int *matrix);
};

enemy::enemy(char c,int pX,int pY);
{
    this->body = c;
    this->posX = pX;
    this->posY = pY;
};

enemy::init(int posicaoX, int posicaoY){
    this->posX = posicaoX;
    this->posY = posicaoY;
    matrix[posX][posY] = '*';   
    
    move(posX, posY);   /* Move cursor to position */
    echochar('X');  
};

enemy::move(int *matrix){
    srand(time(NULL));
    int B_posX = this.posX;
    int B_posY = this.posY;
    int movement;
    
    movement = rand()%4;
    
    // Direita
    if(movement == 0){
        posX = B_posX + 1;
        posY = B_posY;
    // Esquerda
    }else if(movement == 1){
        posX = B_posX - 1;
        posY = B_posY;
    // Cima
    }else if(movement == 2){
        posX = B_posX;
        posY = B_posY + 1;
    // Baixo
    }else{
        posX = B_posX;
        posY = B_posY - 1;
    }
    
    
    matrix[posX][posY] = '*';
    
    
    move(B_posX, B_posY);   /* Move back cursor to position */ 	 
    echochar(' ');
    move(posX, posY);   /* Move cursor to position */
    echochar('*');
    
};





int main()
{
	
	char matrix[15][15];
	

	
  int i, j, i_, j_;
  char c;

	initscr();			       /* Start curses mode 		*/
	raw();				         /* Line buffering disabled	*/
	keypad(stdscr, TRUE);	 /* We get F1, F2 etc..		*/
	noecho();			         /* Don't echo() while we do getch */
        curs_set(0);           /* Do not display cursor */

 	i = 0;
 	j = 0;

        
enemy E;
E.init(3,3);
	 
 
 for (int k=0; c != 'q'; k++) {

 	
 	
    i_ = i;
    j_ = j;
    move(i_, j_);   /* Move cursor to position */
    echochar('#');  /* Prints character, advances a position */
    refresh();      /* Refresh screen */

    c = getch();

    if (c=='s') i++;
    if (c=='w') i--;
    if (c=='d') j++;
    if (c=='a') j--;
    if (i<0) i=15;
    if (i>15) i=0;
    if (j>15) j=0;

    if (j<0) j=15;


    move(i_, j_);   /* Move cursor back to position */
    echochar(' ');  /* Erase character by printing a space */
    
 	if(matrix[i][j] == '*') c = 'q';
}

	refresh();			/* Refresh screen */
	endwin();			  /* End curses mode		  */

	return 0;
}