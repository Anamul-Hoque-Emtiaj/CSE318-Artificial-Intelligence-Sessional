#include<iostream>
#include "Algorithm.cpp"
using namespace std;

class Player
{
    int type; // 0->Human, 1->minmax, 2->minmax with pruning
    int depth;
    int heuristic;
public:
    Player(int type, int depth, int heuristic)
    {
        this->type = type;
        this->depth = depth;
        this->heuristic = heuristic;
    }

    int getMove(Board *board, int turn)
    {
        if(type == 0)
        {
            int slot_id;
            cout<<"Enter your move\n";
            cin>>slot_id;
            slot_id--;

            if(!board->isValidMove(turn,slot_id))
            {
                cout<<"Invalid Move\n";
                return -1;
            }
            return slot_id;
        }
        else if(type == 1)
        {

            Node* ret = Algorithm::minimax(board,turn,depth,true,heuristic,0,0);

            if(!board->isValidMove(turn,ret->bestMove))
            {
                cout<<"Invalid Move\n";
                return -1;
            }
            return ret->bestMove;
        }
        else if(type == 2)
        {
            Node* ret = Algorithm::minimax_alpha_beta(board,turn,depth,true,-1*INF,INF,heuristic,0,0);

            if(!board->isValidMove(turn,ret->bestMove))
            {
                cout<<"Invalid Move\n";
                return -1;
            }
            return ret->bestMove;
        }
        return -1;
    }
};
