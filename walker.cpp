#include <ncurses.h>
#include <unistd.h>
#include <cstdlib>
#include <time.h>

int main()
{
	
	char matrix[15][15];
	
	int posX;
	int posY;
	int B_posX, B_posY;
	
  int i, j, i_, j_;
  char c;

	initscr();			       /* Start curses mode 		*/
	raw();				         /* Line buffering disabled	*/
	keypad(stdscr, TRUE);	 /* We get F1, F2 etc..		*/
	noecho();			         /* Don't echo() while we do getch */
  curs_set(0);           /* Do not display cursor */

 	i = 0;
 	j = 0;

	

	 
 
 for (int k=0; c != 'q'; k++) {
 	
	 srand(time(NULL));
	 B_posX = posX;
	 B_posY = posY;
    posX = (rand() % 15);
 	 posY = (rand() % 15);
 	 matrix[posX][posY] = '*';
    move(B_posX, B_posY);   /* Move back cursor to position */ 	 
 	 echochar(' ');
    move(posX, posY);   /* Move cursor to position */
    echochar('*');
  	
 	
 	
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
    if (j<0) j=15;
    if (j>15) j=0;



    move(i_, j_);   /* Move cursor back to position */
    echochar(' ');  /* Erase character by printing a space */
    
 	if(matrix[i][j] == '*') c = 'q'; 
  }

	refresh();			/* Refresh screen */
	endwin();			  /* End curses mode		  */

	return 0;
}
