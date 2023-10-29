#include<bits/stdc++.h>
#define ll long long int
using namespace std;

class Solvability
{
    ll *oneDarray;
    ll n;
    int blankRow;
    int c,k;

    ll *merged(ll *a, ll *b, ll n, ll m)
    {
        ll *res = new ll[n+m];
        ll i=0,j=0,k=0;
        for(ll t=0; t<m; t++)
        {
            ll x = upper_bound(a,a+n,b[t])-a;
            c+=(n-x);
        }
        while(1)
        {
            if(a[i]<=b[j]&& i<n && j<m)
            {
                res[k++] = a[i++];
            }
            else if(a[i]>b[j] && i<n && j<m)
            {
                res[k++] = b[j++];
            }
            else if(i==n && j<m)
            {
                res[k++] = b[j++];
            }
            else if(i<n && j==m)
            {
                res[k++] = a[i++];
            }
            else
                break;
        }
        return res;
    }

    ll *mergedSort(ll arr[], ll l, ll h)
    {
        if(l==h)
            return &arr[l];
        ll n = h-l+1;
        ll *r1 = mergedSort(arr,l, ((l+h)/2));
        ll  *r2 = mergedSort(arr, ((l+h)/2)+1, h);
        return merged(r1,r2,n-n/2,n/2);
    }

    int numOfInversion()
    {
        c=0;
        ll *r = mergedSort(oneDarray, 0, n-1);
        return c;
    }

public:
    Solvability(int **grid, int m)
    {
        this->n =  (m*m)-1;
        k = m;
        oneDarray = new ll[this->n];
        int ind=0;
        c = 0;
        for(int i=0; i<m; i++)
        {
            for(int j=0; j<m; j++)
            {

                if(grid[i][j]==0)
                {
                    blankRow = i;
                }
                else
                    oneDarray[ind++] = grid[i][j];
            }
        }

    }

    ~Solvability()
    {
        delete[] oneDarray;
    }

    bool isSolvable()
    {
        int inversionCount = numOfInversion();
        if(k % 2 == 1)
        {
            return inversionCount % 2 == 0;
        }
        int distFromLast = k - blankRow;
        return ((distFromLast % 2 == 0 && inversionCount % 2 == 1) || (distFromLast % 2 == 1 && inversionCount % 2 == 0));
    }

};

class Node
{
    void cloneGrid(int **source, int **dest)
    {
        for(int i=0; i<n; i++)
        {
            for(int j=0; j<n; j++)
                dest[i][j] = source[i][j];
        }
    }
public:
    int n;
    int distance;
    int heuristicCost;
    Heuristic* h;
    int **grid;
    Node *parent;
    Node(int **g2, int n, int g, Heuristic* h, Node *par)
    {
        this->n = n;
        this->distance = g;
        this->h = h;
        this->parent = par;
        grid = new int*[n];
        for(int i=0; i<n; i++)
            grid[i] = new int[n];
        cloneGrid(g2, this->grid);
        this->heuristicCost = h->h_of(grid,n);

    }
    ~Node()
    {
        for(int i=0; i<n; i++)
            delete[] grid[i];
        delete[] grid;
    }

    vector<Node*> getChild()
    {
        vector<Node*>childs;
        int i,j;
        bool f = false;
        for( i=0; i < n; ++i)
        {
            for( j = 0; j < n; ++j)
            {
                if(grid[i][j] == 0)
                {
                    f = true;
                    break;
                }
            }
            if(f) break;
        }

        vector<pair<int,int>> probablePositions;
        if (i - 1 >= 0) probablePositions.push_back(make_pair(i - 1, j));
        if(i + 1 < n) probablePositions.push_back(make_pair(i + 1, j));
        if(j - 1 >= 0) probablePositions.push_back(make_pair(i, j - 1));
        if(j + 1 < n) probablePositions.push_back(make_pair(i, j + 1));

        for(pair<int,int> v : probablePositions)
        {
            if(parent != nullptr && parent->grid[v.first][v.second] == 0) continue;
            int **g2;
            g2 = new int*[n];
            for(int i=0; i<n; i++)
                g2[i] = new int[n];
            cloneGrid(grid,g2);
            g2[i][j] = g2[v.first][v.second];
            g2[v.first][v.second] = 0;
            childs.push_back(new Node(g2,n,distance + 1,h,this));
            for(int i=0; i<n; i++)
                delete[] g2[i];
            delete[] g2;

        }

        return childs;
    }

    bool isSolvable()
    {
        Solvability *slv = new Solvability(this->grid,this->n);
        return slv->isSolvable();
    }

    void print()
    {
        for(int i=0; i<n; i++)
        {
            for(int j=0; j<n; j++)
                cout<<grid[i][j]<<" ";
            cout<<endl;
        }
    }


    //for hashset
    bool operator==(const Node* t) const
    {
        for(int i = 0; i < n; i++)
        {
            for(int j = 0; j < n; j++)
            {
                if(t->grid[i][j] != grid[i][j]) return false;
            }
        }
        return true;
    }

};
