using namespace std;
class Node
{
public:

    int value;
    int bestMove;

    Node(int value, int bestMove)
    {
        this->value = value;
        this->bestMove = bestMove;
    }

    Node(const Node &obj)
    {
        this->value = obj.value;
        this->bestMove = obj.bestMove;
    }
};
