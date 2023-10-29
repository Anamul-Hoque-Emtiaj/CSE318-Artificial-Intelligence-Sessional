#include <iostream>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include<random>
#include <unordered_map>
#include <unordered_set>

using namespace std;

random_device rd;
mt19937 gen(rd());

// Define the range for random numbers
double lowerBound = 0.5;
double upperBound = 0.95;

// Create a uniform real distribution
uniform_real_distribution<double> dist(lowerBound, upperBound);
double getRand()
{
    return dist(gen);
}

struct Edge
{
    int u, v;
    double weight;
};

double getCut(const unordered_set<int>&cutX, const unordered_set<int>&cutY, const vector<Edge>& edges)
{
    double cut = 0;
    for (const Edge& edge : edges)
    {
        if ((cutX.count(edge.u)>0 && cutY.count(edge.v)>0) || (cutX.count(edge.v)>0 && cutY.count(edge.u)>0))
        {
            cut += edge.weight;
        }
    }
    return cut;
}

//local-search heuristic
double delta(int v, const unordered_set<int> &cutX, const unordered_set<int> &cutY, const vector<Edge>& edges)
{
    double deltaOne = 0, deltaTwo=0;
    for (const Edge& edge : edges)
    {
        if (edge.u==v || edge.v==v)
        {
            if((cutX.count(edge.u)>0 && cutX.count(edge.v)>0) || (cutY.count(edge.u)>0 && cutY.count(edge.v)>0))
                deltaOne+=edge.weight;
            else
                deltaTwo+=edge.weight;
        }
    }
    return deltaOne-deltaTwo;
}

//semi-greedy heuristic
double calSigma(const vector<Edge>& edges, const unordered_set<int>&cutX, const unordered_set<int>&cutY, int v, int x)
{
    double val=0;
    for (const Edge& edge : edges)
    {
        if(edge.u==v)
        {
            if((x==1 && cutY.count(edge.v)>0) || (x==0 && cutX.count(edge.v)>0))
                val+=edge.weight;
        }
        else if(edge.v==v)
        {
            if((x==1 && cutY.count(edge.u)>0) || (x==0 && cutX.count(edge.u)>0))
                val+=edge.weight;
        }
    }
    return val;
}

pair<unordered_set<int>,unordered_set<int>> semiGreedy(unordered_map<int, vector<Edge>> &adjList, const vector<Edge>& edges, double alpha, int n)
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

    vector<Edge> RCLe;
    for (const Edge& edge : edges)
    {
        if(edge.weight>=miu)
            RCLe.push_back(edge);
    }
    int rand_int = rand()%RCLe.size();
    Edge edge = RCLe[rand_int];
    unordered_set<int> cutX,cutY,unexplored;
    cutX.insert(edge.u);
    cutY.insert(edge.v);
    for(int i=1; i<=n; i++)
    {
        if(!(i==edge.u || i==edge.v))
            unexplored.insert(i);
    }
    RCLe.clear();

    while(!unexplored.empty())
    {
        vector<pair<int,pair<int,int>>> sigma; //nodeNo,sigmaX,sigmaY
        double max_sigma = -100000000, min_sigma = 100000000;
        for (const auto& nodeNo : unexplored)
        {
            double x = calSigma(adjList[nodeNo],cutX,cutY,nodeNo,1);
            double y = calSigma(adjList[nodeNo],cutX,cutY,nodeNo,0);
            sigma.push_back(make_pair(nodeNo,make_pair(x,y)));
            max_sigma = max(max_sigma,max(x,y));
            min_sigma = min(min_sigma,min(x,y));
        }

        miu = min_sigma + alpha*(max_sigma-min_sigma);
        vector<pair<int,pair<int,int>>> RCLv;  //nodeNo,sigmax,sigmay
        for(pair<int,pair<int,int>> cur: sigma)
        {
            if(max(cur.second.first,cur.second.second)>=miu)
            {
                RCLv.push_back(cur);
            }
        }

        rand_int = rand()%RCLv.size();
        pair<int,pair<int,int>> v = RCLv[rand_int];

        if(v.second.first>v.second.second)
        {
            cutX.insert(v.first);
            unexplored.erase(unexplored.find(v.first));
        }
        else
        {
            cutY.insert(v.first);
            unexplored.erase(unexplored.find(v.first));
        }

        RCLv.clear();
        sigma.clear();
    }
    return make_pair(cutX,cutY);
}

pair<int,pair<unordered_set<int>,unordered_set<int>>> localSearch(pair<unordered_set<int>,unordered_set<int>> &cut, unordered_map<int, vector<Edge>> &adjList, int n)
{
    bool change = true;
    int itr_count=0;
    unordered_set<int> cutX = cut.first;
    unordered_set<int> cutY = cut.second;
    while (change)
    {
        change = false;

        for (int v = 1; v<=n; v++)
        {

            if (delta(v,cutX,cutY,adjList[v]) > 0)
            {
                if(cutX.count(v)>0)
                {
                    cutX.erase(cutX.find(v));
                    cutY.insert(v);
                }
                else
                {
                    cutY.erase(cutY.find(v));
                    cutX.insert(v);
                }

                change = true;
                break;
            }
        }
        itr_count++;
    }
    return make_pair(itr_count,make_pair(cutX,cutY));
}

pair<unordered_set<int>,unordered_set<int>> graspMaxCut(unordered_map<int, vector<Edge>> &adjList,const vector<Edge>& edges, int iterations, int n)
{
    pair<unordered_set<int>,unordered_set<int>> bestSolution;
    double bestCut = 0;

    for (int iter = 1; iter <=iterations; iter++)
    {
        pair<unordered_set<int>,unordered_set<int>> solution = semiGreedy(adjList,edges, getRand(),n);
        solution = localSearch(solution, adjList,n).second;
        double cut = getCut(solution.first,solution.second, edges);

        if (cut > bestCut)
        {
            bestCut = cut;
            bestSolution = solution;
        }
        //cout<<"grasp: "<<iter<<" "<<getCut(solution.first,solution.second,edges)<<endl;
    }

    return bestSolution;
}


int main(int argc, char* argv[])
{
    //FILE* file_input = freopen("input.txt", "r", stdin);
    FILE* file_input = freopen(argv[1], "r", stdin);
    int n, m;
    cin >> n >> m;
    cout<<n<<","<<m<<",";
    vector<Edge> edges(m);

    unordered_map<int, vector<Edge>> adjList(n);
    for(int i=1; i<=n; i++)
        adjList[i]=vector<Edge>();
    int u,v;
    double wght;
    for (int i = 0; i < m; i++)
    {
        cin>>u>>v>>wght;
        edges[i].u = u;
        edges[i].v = v;
        edges[i].weight = wght;

        vector<Edge> adl = adjList[u];
        adl.push_back(edges[i]);
        adjList[u] = adl;

        adl = adjList[v];
        adl.push_back(edges[i]);
        adjList[v] = adl;
    }

    int iterations = 20; // Number of iterations
    pair<unordered_set<int>,unordered_set<int>> solution;

    //Simple Randomized
    double totalCut=0;
    for(int i=1; i<=iterations; i++)
    {
        solution = semiGreedy(adjList,edges, 0,n); //alpha=0
        totalCut += getCut(solution.first,solution.second, edges);
    }

    cout<<totalCut/iterations<<",";

    //Simple Greedy
    solution = semiGreedy(adjList,edges, 1,n); //alpha=1
    cout<<getCut(solution.first,solution.second, edges)<<",";

    //Semi-Greedy
    totalCut=0;
    for(int i=1; i<=iterations; i++)
    {
        solution = semiGreedy(adjList,edges, getRand(),n); //alpha=random value 0.5 to 0.95
        totalCut += getCut(solution.first,solution.second, edges);
    }
    cout<<totalCut/iterations<<",";

    //Local search
    double bestCut=-100000000;
    int itr_cnt=0;
    for(int i=1; i<=iterations; i++)
    {
        unordered_set<int> cutX,cutY;
        // Initialize vertices randomly to X or Y
        for (int i = 1; i <=n; i++)
        {
            if (rand() % 2 == 0)
            {
                cutX.insert(i);
            }
            else
            {
                cutY.insert(i);
            }
        }
        pair<unordered_set<int>,unordered_set<int>> cut = make_pair(cutX,cutY);
        pair<int,pair<unordered_set<int>,unordered_set<int>>> sol = localSearch(cut, adjList,n);
        double cut_val = getCut(sol.second.first,sol.second.second, edges);
        if(cut_val>bestCut)
        {
            bestCut = cut_val;
            itr_cnt = sol.first;
        }
    }
    cout<<itr_cnt<<","<<bestCut<<",";

    //GRASP
    solution = graspMaxCut(adjList,edges, iterations,n);
    cout << iterations<<"," <<getCut(solution.first,solution.second, edges)<<","<<endl;
}
