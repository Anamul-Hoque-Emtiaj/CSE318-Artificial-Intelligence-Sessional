#include<iostream>
#include "Heuristic.cpp"
#include "Node.cpp"
#include "AStarSearch.cpp"
using namespace std;

void printGrid(Node* cur, int depth)
{
    if(cur!=nullptr)
    {
        printGrid(cur->parent, depth+1);
        cur->print();
        cout<<endl<<endl;
    }
    else
        cout<<"Minimum number of moves = "<<depth-1<<endl<<endl;

}

int main()
{
    int n;
    cin>>n;
    int **grid;
    grid = new int*[n];
    for(int i=0; i<n; i++)
        grid[i] = new int[n];

    for(int i=0; i<n; i++)
    {
        for(int j=0; j<n; j++)
            cin>>grid[i][j];
    }

    Node *startNode = new Node(grid,n,0, new Manhattan(),nullptr);
    //Node *startNode = new Node(grid,n,0, new Hamming(),nullptr);
    if(startNode->isSolvable())
    {
        Node *targetNode = AStarSearch::_search(startNode);
        printGrid(targetNode,0);
        //cout<<"Explored Node: "<<AStarSearch::explored<<"\nExpanded Node: "<<AStarSearch::expanded<<endl;
    }
    else
    {
        cout<<"Unsolvable puzzle"<<endl;
    }


    for(int i=0; i<n; i++)
        delete[] grid[i];
    delete[] grid;
}
