#include<iostream>
#include "Board.cpp"
using namespace std;

class Heuristic
{
    int h,stoneCaptured,additionalMove;
    Board *board;
public:
    Heuristic(int h, Board *board, int stoneCaptured, int additionalMove)
    {
        this->h = h;
        this->board = board;
        this->stoneCaptured = stoneCaptured;
        this->additionalMove = additionalMove;
    }


    int h1(int turn)
    {
        if(turn == 0)
            return board->p0Score() - board->p1Score();
        else
            return board->p1Score()- board->p0Score();
    }

    int h2(int turn)
    {
        int W1 = 10 , W2 = 2;
        if(turn == 0)
        {
            return W1 * (board->p0Score() - board->p1Score()) + W2 * (board->p0Stones() - board->p1Stones());
        }
        else
        {
            return W1 * (board->p1Score() - board->p0Score()) + W2 * (board->p1Stones() - board->p0Stones());
        }
    }

    int h3(int turn)
    {
        int W1 = 10 , W2 = 2, W3 = 4;
        if(turn == 0)
        {
            return W1 * (board->p0Score() - board->p1Score()) + W2 * (board->p0Stones() - board->p1Stones()) + W3 * additionalMove;
        }
        else
        {
            return W1 * (board->p1Score() - board->p0Score()) + W2 * (board->p1Stones() - board->p0Stones()) + W3 * additionalMove;
        }
    }

    int h4(int turn)
    {
        int W1 = 10 , W2 = 2, W3 = 4, W4 = 3;
        if(turn == 0)
        {
            return W1 * (board->p0Score() - board->p1Score()) + W2 * (board->p0Stones() - board->p1Stones()) + W3 * additionalMove + W4 * stoneCaptured;
        }
        else
        {
            return W1 * (board->p1Score() - board->p0Score()) + W2 * (board->p1Stones() - board->p0Stones()) + W3 * additionalMove + W4 * stoneCaptured;
        }
    }

    int h5(int turn)
    {
        int W1 = 10 , W2 = 2, W3 = 4, W4 = 3;
        if(turn == 0)
        {
            return W1 * (board->p0Score() - board->p1Score()) + W2 * (board->p0StonesCloseToStorage() - board->p1StonesCloseToStorage()) + W3 * additionalMove + W4 * stoneCaptured;
        }
        else
        {
            return W1 * (board->p1Score() - board->p0Score()) + W2 * (board->p1StonesCloseToStorage() - board->p0StonesCloseToStorage()) + W3 * additionalMove + W4 * stoneCaptured;
        }
    }

    int heuristic(int turn)
    {
        if(h == 1)
            return h1(turn);
        else if (h == 2)
            return h2(turn);
        else if (h == 3)
            return h3(turn);
        else if (h == 4)
            return h4(turn);
        else if (h == 5)
            return h5(turn);
        return 0;
    }
};
