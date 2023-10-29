#include<iostream>
using namespace std;

class Heuristic
{
public:

    virtual int h_of(int **grid, int n) =0;
};

class Manhattan: public Heuristic
{
public:

    int h_of(int **grid, int n)
    {
        int res = 0;
        for(int i=0; i<n; i++)
        {
            for(int j=0; j<n; j++)
            {
                int v = grid[i][j];
                if(v!=0)
                {
                    v--;
                    res += (abs((v/n)-i)+abs((v%n)-j));
                }
            }
        }
        return res;
    }
};

class Hamming: public Heuristic
{
public:

    int h_of(int **grid, int n)
    {
        int res = 0;
        int c = 1;
        for(int i=0; i<n; i++)
        {
            for(int j=0; j<n; j++)
            {
                int v = grid[i][j];
                if(v!=0)
                {
                    if(v!=c)
                        res++;
                }
                c++;
            }
        }
        return res;
    }
};
