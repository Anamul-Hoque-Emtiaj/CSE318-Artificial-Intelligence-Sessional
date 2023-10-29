#include<iostream>
#include "Player.cpp"
using namespace std;

int main()
{
    Board* board = new Board();
    int turn = 0;
    Player p1(0,12,1);
    Player p2(2,12,2);
    board->print();
    while(true)
    {

        if(board->isGameOver())
        {
            cout<<"---Game Over---\n";
            board->print();
            if(board->getWinner() == 0) cout<<"Player 1 wins\n";
            else if(board->getWinner() == 1) cout<<"Player 2 wins\n";
            else cout<<"Match Draw";
            break;
        }
        else
        {
            cout<<"Turn for player "<<turn+1<<endl;
            int mid;
            if(turn==0)
            {
                mid = p1.getMove(board,turn);
            }
            else
            {
                mid = p2.getMove(board,turn);
            }
            cout<<mid+1<<" given\n";
            if(mid == -1)
                continue;
            int freeMove = board->doMove(turn,mid);
            board->print();

            if(freeMove==1)
            {
                cout<<"Free Turn\n";
                continue;
            }
            else if(freeMove>1)
            {
                cout<<freeMove-1<<" stone captured\n";
            }
            turn^=1;
        }
    }
}
