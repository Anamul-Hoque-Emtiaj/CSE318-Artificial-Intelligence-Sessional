#include<iostream>
#include "Player.cpp"
using namespace std;

int main()
{
    cout<<"h1 h2 d1 d2 w\n";
    int p1Win = 0, p2Win = 0, h1=5, h2=4;
    for(int d1=9; d1<=11; d1++)
    {
        for(int d2=9; d2<=11; d2++)
        {
            Board* board = new Board();
            int turn = 0;
            Player p1(2,d1,h1);
            Player p2(2,d2,h2);
            while(true)
            {

                if(board->isGameOver())
                {
                    if(board->getWinner() == 0) p1Win++;
                    else if(board->getWinner() == 1) p2Win++;
                    cout<<h1<<" "<<h2<<" "<<d1<<" "<<d2<<" "<<board->getWinner()+1<<endl;
                    break;

                }
                else
                {
                    int mid;
                    if(turn==0)
                    {
                        mid = p1.getMove(board,turn);
                    }
                    else
                    {
                        mid = p2.getMove(board,turn);
                    }

                    if(mid == -1)
                        continue;
                    int freeMove = board->doMove(turn,mid);

                    if(freeMove==1)
                    {
                        continue;
                    }
                    turn^=1;
                }
            }
        }
    }
    cout<<"H"<<h1<<":H"<<h2<<" --- "<<p1Win<<":"<<p2Win<<endl;
}
