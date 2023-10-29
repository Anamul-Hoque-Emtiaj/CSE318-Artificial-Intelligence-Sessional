#include<bits/stdc++.h>
using namespace std;

struct ObjectPtrComparator
{
    bool operator()(const Node* obj1, const Node* obj2) const
    {
        return obj1->distance+obj1->heuristicCost > obj2->distance+obj2->heuristicCost;
    }
};

struct MyObjectHash
{
    std::size_t operator()(const Node* obj) const
    {
        string str = "";
        for(int i = 0; i < obj->n; i++)
        {
            for(int j = 0; j < obj->n; j++)
            {
                str+=obj->grid[i][j];
            }
        }
        return std::hash<string> {}(str);
    }
};

class AStarSearch
{
public:
    static int explored;
    static int expanded;
    static Node* _search(Node* startNode)
    {
        explored = 0;
        expanded = 1;
        if(startNode->heuristicCost==0)
            return startNode;
        priority_queue<Node*, vector<Node*>, ObjectPtrComparator> pq;
        pq.push(startNode);
        unordered_set<Node*, MyObjectHash> st;

        while(!pq.empty())
        {
            Node* cur = pq.top();
            pq.pop();
            st.insert(cur);
            explored++;
            if(cur->heuristicCost==0)
                return cur;

            for(Node* child: cur->getChild())
            {
                auto it = st.find(child);
                if (it == st.end())
                {
                    pq.push(child);
                    expanded++;
                }

            }
        }
        return nullptr;
    }
};

int AStarSearch::explored=0;
int AStarSearch::expanded=0;


