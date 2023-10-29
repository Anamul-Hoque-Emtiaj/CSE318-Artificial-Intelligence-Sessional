#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>
#include <ctime>
#include <cstdlib>

using namespace std;

struct Edge
{
    int u, v;
    double weight;
};

double getCut(const vector<int>& S, const vector<Edge>& edges)
{
    double cut = 0;
    for (const Edge& edge : edges)
    {
        if ((S[edge.u]==0 && S[edge.v]==1) || (S[edge.u]==1 && S[edge.v]==0))
        {
            cut += edge.weight;
        }
    }
    return cut;
}

double delta(int v, const vector<int>& S, const vector<Edge>& edges)
{
    double deltaOne = 0, deltaTwo=0;
    for (const Edge& edge : edges)
    {
        if (edge.u==v)
        {
            if(S[v]^S[edge.v]==0)
                deltaOne+=edge.weight;
            else
                deltaTwo+=edge.weight;
        }
        else if(edge.v==v)
        {
            if(S[v]^S[edge.u]==0)
                deltaOne+=edge.weight;
            else
                deltaTwo+=edge.weight;
        }
    }
    return deltaOne-deltaTwo;
}

double sigma(const vector<Edge>& edges, const vector<int>& S, int v, int x)
{
    double val=0;
    for (const Edge& edge : edges)
    {
        if((edge.u==v && S[edge.u]==x) || (edge.v==v && S[edge.v]==x))
            val+=edge.weight;
    }
    return val;
}

vector<int> semiGreedy(const vector<Edge>& edges, double alpha, int n)
{
    double max_weight = -100000000, min_weight = 100000000;
    for (const Edge& edge : edges)
    {
        if(edge.weight > max_weight)
            max_weight = edge.weight;

        if(edge.weight < min_weight)
            min_weight = edge.weight;
    }
    double miu = min_weight+alpha*(max_weight-min_weight);
    //cout<<"miu1: "<<miu<<endl;
    vector<Edge> RCLe;
    for (const Edge& edge : edges)
    {
        if(edge.weight>=miu)
            RCLe.push_back(edge);
    }
    vector<int> solution(n+1, -1);
    int rand_int = rand()%RCLe.size();
    Edge edge = RCLe[rand_int];
    solution[edge.u]=0; //X
    solution[edge.v]=1; //Y
    RCLe.clear();

    while(true)
    {
        bool flag = false;
        for(int i=1; i<=n;i++)
        {
            if(solution[i]==-1)
            {
                flag=true;
                break;
            }
        }
        if(!flag)
            break;
        vector<pair<int,int>> sigmaX,sigmaY; //nodeNo,value
        double max_sigma = -100000000, min_sigma = 100000000;
        for(int i=1; i<=n;i++)
        {
            if(solution[i]==-1)
            {
                double x = sigma(edges,solution,i,1);
                double y = sigma(edges,solution,i,0);
                sigmaX.push_back(make_pair(i,x));
                sigmaY.push_back(make_pair(i,y));
                max_sigma = max(max_sigma,max(x,y));
                min_sigma = min(min_sigma,min(x,y));
            }
        }
        miu = min_sigma + alpha*(max_sigma-min_sigma);
        vector<pair<int,pair<int,int>>> RCLv;  //nodeNo,sigmax,sigmay
        for(int i=0; i<sigmaX.size(); i++)
        {
            if(max(sigmaX[i].second,sigmaY[i].second)>=miu)
            {
                RCLv.push_back(make_pair(sigmaX[i].first,make_pair(sigmaX[i].second,sigmaY[i].second)));
            }
        }
        rand_int = rand()%RCLv.size();
        pair<int,pair<int,int>> v = RCLv[rand_int];

        if(v.second.first>v.second.second)
            solution[v.first] =  0;
        else
            solution[v.first] =  1;

        RCLv.clear();
        sigmaX.clear();
        sigmaY.clear();
        cout<<"semi2: "<<v.first<<" "<<getCut(solution,edges)<<endl;
    }
    cout<<"semi: "<<getCut(solution,edges)<<endl;
    return solution;
}

int localSearch(vector<int>& S, const vector<Edge>& edges)
{
    int n = S.size()-1;
    bool change = true;
    int itr_count=0;

    while (change)
    {
        change = false;

        for (int v = 1; v<=n; v++)
        {

            if (delta(v,S,edges) > 0)
            {
                S[v] = 1 - S[v];
                change = true;
                break;
            }
        }
        itr_count++;
    }
    cout<<"local: "<<getCut(S,edges)<<endl;
    return itr_count;
}

pair<vector<int>, int> graspMaxCut(const vector<Edge>& edges, double alpha, int iterations, int n)
{
    vector<int> bestSolution(n+1, 0);
    double bestCut = 0;

    srand(time(0));

    for (int iter = 0; iter < iterations; iter++)
    {
        vector<int> solution = semiGreedy(edges, alpha,n);
        int itr = localSearch(solution, edges);
        double cut = getCut(solution, edges);

        if (cut > bestCut)
        {
            bestCut = cut;
            bestSolution = solution;
        }
        solution.clear();
    }

    return make_pair(bestSolution, bestCut);
}


int main()
{
    ifstream inputFile;
    inputFile.open("input.txt");
    int n, m;
    inputFile >> n >> m;
    vector<Edge> edges(m);
    int u,v;
    double wght;
    for (int i = 0; i < m; i++)
    {
        inputFile>>u>>v>>wght;
        edges[i].u = u;
        edges[i].v = v;
        edges[i].weight = wght*1.0;
    }
    inputFile.close();

    double alpha = 0.95; // Adjust alpha value as needed
    int iterations = 5; // Number of GRASP iterations

    pair<vector<int>, int> result = graspMaxCut(edges, alpha, iterations,n);

    cout << "Best Solution S: ";
    for (int i : result.first)
    {
        cout << i << " ";
    }
    cout << endl;

    cout << "Max Cut Value: " << result.second << endl;
}
