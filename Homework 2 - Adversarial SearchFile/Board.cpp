#include<iostream>
using namespace std;

class Board
{
public:
    int slot = 6;
    int stone = 4;
    int boardSize = 2*slot + 2;
    int *mancala;
    int p0Storage = 6;
    int p1Storage = 13;


    Board()
    {
        mancala = new int[boardSize];
        for(int i=0;i<boardSize; i++)
        {
            if(i==p0Storage || i==p1Storage)
                mancala[i]=0;
            else
                mancala[i]=stone;
        }
    }
    ~Board()
    {
        delete[] mancala;
    }
    Board(Board* obj)
    {
        slot = obj->slot;
        stone = obj->stone;
        boardSize = obj->boardSize;
        p0Storage = obj->p0Storage;
        p1Storage = obj->p1Storage;
        mancala = new int[boardSize];
        for(int i=0;i<boardSize; i++)
        {
            mancala[i]=obj->mancala[i];

        }
    }



    bool isValidMove(int turn,int slot_id)
    {
        if(slot_id < 0 || slot_id > 5)
            return false;

        if(turn == 1)
            slot_id = slot_id + slot + 1;

        if(mancala[slot_id] <= 0)
            return  false;

        return true;
    }

    int getWinner()
    {
        if(mancala[p0Storage] > mancala[p1Storage])
            return 0;
        else if(mancala[p1Storage] > mancala[p0Storage])
            return 1;
        return 2; // draw
    }


    int p0Stones()
    {
        int st1 = 0,cnt = 0;
        for(int i=st1;i<st1+slot;i++)
        {
             cnt += mancala[i];
        }
        return cnt;
    }

    int p1Stones()
    {
        int st2 = slot+1,cnt = 0;
        for(int i=st2;i<st2+slot;i++)
        {
            cnt += mancala[i];
        }
        return cnt;
    }

    int p0Score()
    {
        return mancala[p0Storage];
    }

    int p1Score()
    {
        return mancala[p1Storage];
    }

    int p0StonesCloseToStorage()
    {
        int st1 = 0,cnt = 0;
        for(int i=st1;i<st1+slot;i++)
        {
            if(mancala[i] > (p0Storage - i))
            {
                cnt += mancala[i] - (p0Storage - i);
            }
            else
                cnt += mancala[i];
        }
        return cnt;
    }

    int p1StonesCloseToStorage(){
        int st2 = slot+1,cnt = 0;
        for(int i=st2;i<st2+slot;i++)
        {
            if(mancala[i] > (p1Storage - i))
            {
                cnt += mancala[i] - (p1Storage - i);
            }
            else
                cnt += mancala[i];
        }
        return cnt;
    }

    bool isGameOver()
    {
        int cnt = 0;
        for(int i=0;i<slot;i++)
        {
            cnt += mancala[i];
        }

        if(cnt == 0)
        {

            int st2 = slot+1;
            for(int i=st2;i<st2+slot;i++){
                mancala[p1Storage] += mancala[i];
                mancala[i] = 0;
            }

            return true;
        }

        cnt = 0;
        int st2 = slot+1;
        for(int i=st2;i<st2+slot;i++)
        {
            cnt += mancala[i];
        }

        if(cnt == 0){
            int st1 = 0;
            for(int i=st1;i<st1+slot;i++){
                mancala[p0Storage] += mancala[i];
                mancala[i] = 0;
            }
            return true;
        }

        return false;
    }

    int doMove(int turn,int slot_id)
    {
        if(turn == 0)
        {
            //player 0 turn
            int num_stone = mancala[slot_id];
            int cur = slot_id+1;

            while(num_stone > 0)
            {
                if(cur == p1Storage)
                {
                    cur = (cur+1)%boardSize;
                    continue;
                }

                mancala[cur]++;
                mancala[slot_id]--;
                num_stone--;

                if(num_stone == 0)
                {
                    if(cur == p0Storage)
                        return 1; // extra move

                    int opposite_slot_id = abs(2*slot - cur);
                    if(mancala[cur] == 1 && cur >= 0 && cur <= 5 && mancala[opposite_slot_id] > 0) // capture
                    {
                        int captured = mancala[opposite_slot_id];
                        mancala[p0Storage] += mancala[opposite_slot_id];
                        mancala[p0Storage] += 1;
                        mancala[opposite_slot_id] = 0;
                        mancala[cur] = 0;
                        return captured+1;
                    }
                }

                cur = (cur+1)%boardSize;
            }


        }
        else{

            //player 1 turn

            slot_id = slot_id + slot + 1;
            int num_stone = mancala[slot_id];
            int cur = slot_id+1;

            while(num_stone > 0)
            {
                if(cur == p0Storage)
                {
                    cur = (cur+1)%boardSize;
                    continue;
                }

                mancala[cur]++;
                mancala[slot_id]--;
                num_stone--;

                if(num_stone == 0) // last move
                {
                    if(cur == p1Storage)
                        return 1; // free move

                    int opposite_slot_id = abs(2*slot - cur);
                    if(mancala[cur] == 1 && cur >= 7 && cur <= 12 && mancala[opposite_slot_id] > 0) // capture
                    {
                        int captured = mancala[opposite_slot_id];
                        mancala[p1Storage] += mancala[opposite_slot_id];
                        mancala[p1Storage] += 1;
                        mancala[opposite_slot_id] = 0;
                        mancala[cur] = 0;
                        return captured+1;
                    }
                }

                cur = (cur+1)%boardSize;
            }
        }
        return 0;
    }

    void print()
    {
        int st2 = slot+1;
        cout<<" ";
        for(int i=st2+slot-1;i>=st2;i--)
            cout<<mancala[i]<<" ";
        cout<<endl;

        cout<<mancala[p1Storage]<<"            "<<mancala[p0Storage]<<endl;

        int st1 = 0;
        cout<<" ";
        for(int i=st1;i<st1+slot;i++)
            cout<<mancala[i]<<" ";
        cout<<endl;
    }

};
