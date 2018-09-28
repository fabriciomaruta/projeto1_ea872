#include <ncurses.h>
#include <unistd.h>
#include <cstdlib>
#include <time.h>
#include <oo_model.hpp>

using namespace std;




int main(){
    
    int i;
    
    Corpo avatar = new Corpo('$', 1, 1);
    Tela *tela = new Tela();
    tela->init();
    
    while (c != 'q') {

        refresh();      /* Refresh screen */

        
        c = getch();
        
        if (c=='s') avatar->update(avatar->get_pos_X() + 1, avatar->get_pos_Y() - 1);
       // if (c=='w') i--;
       // if (c=='d') j++;
       // if (c=='a') j--;
        
        if (i<0) i=15;
        if (i>15) i=0;
        if (j>15) j=0;
        if (j<0) j=15;
        
        tela->update(avatar);
        
        //if(matrix[i][j] == '*') c = 'q';
        
    }
    

	return 0;
}