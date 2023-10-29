#include "Node.cpp"
#include "Heuristic.cpp"
#include<vector>
#include <algorithm>
#include <random>
#define INF 100000000
using namespace std;

class Algorithm
{
public:
    static Node* minimax(Board *board, int turn, int depth, bool isMax,int heuristic, int stoneCaptured, int additionalMove)
    {
        if(depth == 0 || board->isGameOver())
        {
            Heuristic h(heuristic,board,stoneCaptured,additionalMove);
            return new Node(h.heuristic(turn),-1);
        }

        vector<int> moves(6);
        for (int i = 0; i < 6; i++)
        {
            moves[i] = i;
        }
        random_device rd;
        mt19937 gen(rd());
        shuffle(moves.begin(), moves.end(), gen);

        if(isMax)
        {
            Node* best = new Node(-1*INF,-1);
            for(int id=0;id<6;id++)
            {
                int cur_move = moves[id];

                int slot_id = cur_move;
                if(turn == 1)
                {
                    slot_id = slot_id + board->slot + 1;
                }

                if(board->mancala[slot_id] == 0) continue;

                Board* tempBoard = new Board(board);
                int capturedStone = tempBoard->doMove(turn,cur_move);
                Node* ret;
                if(capturedStone==0)
                    ret = minimax(tempBoard,turn,depth-1,false,heuristic,stoneCaptured, additionalMove);
                else if(capturedStone==1)
                    ret = minimax(tempBoard,turn,depth-1,true,heuristic,stoneCaptured+1, additionalMove+1);
                else
                    ret = minimax(tempBoard,turn,depth-1,false,heuristic,stoneCaptured+capturedStone, additionalMove);

                if(ret->value > best->value)
                {
                    best->value = ret->value;
                    best->bestMove = cur_move;
                }
            }
            return best;
        }
        else
        {
            Node* best = new Node(INF,-1);
            for(int id=0;id<6;id++)
            {
                int cur_move = moves[id];

                int slot_id = cur_move;
                if(turn == 1)
                {
                    slot_id = slot_id + board->slot + 1;
                }

                if(board->mancala[slot_id] == 0) continue;

                Board* tempBoard = new Board(board);
                int capturedStone = tempBoard->doMove(turn^1,cur_move);
                Node* ret;
                if(capturedStone==0)
                    ret = minimax(tempBoard,turn,depth-1,true,heuristic,stoneCaptured,additionalMove);
                else if(capturedStone==1)
                    ret = minimax(tempBoard,turn,depth-1,false,heuristic,stoneCaptured-1,additionalMove-1);
                else
                    ret = minimax(tempBoard,turn,depth-1,true,heuristic,stoneCaptured-capturedStone,additionalMove);

                if(ret->value < best->value)
                {
                    best->value = ret->value;
                    best->bestMove = cur_move;
                }
            }
            return best;
        }
    }


    static Node* minimax_alpha_beta(Board *board, int turn, int depth, bool isMax,int alpha,int beta,int heuristic, int stoneCaptured, int additionalMove)
    {
        if(depth == 0 || board->isGameOver())
        {
            Heuristic h(heuristic,board,stoneCaptured,additionalMove);
            return new Node(h.heuristic(turn),-1);
        }

        vector<int> moves(6);
        for (int i = 0; i < 6; i++)
        {
            moves[i] = i;
        }
        random_device rd;
        mt19937 gen(rd());
        shuffle(moves.begin(), moves.end(), gen);

        if(isMax)
        {
            Node* best = new Node(-1*INF,-1);
            for(int id=0;id<6;id++)
            {
                int cur_move = moves[id];

                int slot_id = cur_move;
                if(turn == 1)
                {
                    slot_id = slot_id + board->slot + 1;
                }

                if(board->mancala[slot_id] == 0) continue;

                Board* tempBoard = new Board(board);
                int capturedStone = tempBoard->doMove(turn,cur_move);
                Node* ret;
                if(capturedStone==0)
                    ret = minimax_alpha_beta(tempBoard,turn,depth-1,false,alpha,beta,heuristic,stoneCaptured,additionalMove);
                else if(capturedStone==1)
                    ret = minimax_alpha_beta(tempBoard,turn,depth-1,true,alpha,beta,heuristic,stoneCaptured+1,additionalMove+1);
                else
                    ret = minimax_alpha_beta(tempBoard,turn,depth-1,false,alpha,beta,heuristic,stoneCaptured+capturedStone,additionalMove);

                if(ret->value > best->value)
                {
                    best->value = ret->value;
                    best->bestMove = cur_move;
                }

                alpha = max(alpha,ret->value);
                if(beta <= alpha)
                    break;
            }


            return best;
        }
        else
        {
            Node* best = new Node(INF,-1);
            for(int id=0;id<6;id++)
            {
                int cur_move = moves[id];

                int slot_id = cur_move;
                if(turn == 1)
                {
                    slot_id = slot_id + board->slot + 1;
                }

                if(board->mancala[slot_id] == 0) continue;

                Board* tempBoard = new Board(board);
                int capturedStone = tempBoard->doMove(turn^1,cur_move);
                Node* ret;
                if(capturedStone==0)
                    ret = minimax_alpha_beta(tempBoard,turn,depth-1,true,alpha,beta,heuristic,stoneCaptured,additionalMove);
                else if(capturedStone==1)
                    ret = minimax_alpha_beta(tempBoard,turn,depth-1,false,alpha,beta,heuristic,stoneCaptured-1,additionalMove-1);
                else
                    ret = minimax_alpha_beta(tempBoard,turn,depth-1,true,alpha,beta,heuristic,stoneCaptured-capturedStone,additionalMove);

                if(ret->value < best->value)
                {
                    best->value = ret->value;
                    best->bestMove = cur_move;
                }
                beta = min(beta,ret->value);

                if(beta <= alpha)
                    break;
            }
            return best;
        }
    }
};
