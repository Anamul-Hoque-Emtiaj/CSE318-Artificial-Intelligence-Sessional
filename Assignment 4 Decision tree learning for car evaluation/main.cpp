#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <cmath>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <sstream>
#include<queue>

using namespace std;

unordered_map<string, vector<string>> attributes =
{
    {"buying", {"vhigh", "high", "med", "low"}},
    {"maint", {"vhigh", "high", "med", "low"}},
    {"doors", {"2", "3", "4", "5","more"}},
    {"persons", {"2", "4", "more"}},
    {"lug_boot", {"small", "med", "big"}},
    {"safety", {"low", "med", "high"}}
};


struct Instance
{
    unordered_map<string,string> values;
    string classification;
};

struct TreeNode
{
    string attribute; //isLeaf=true -> classVal
    bool isLeaf;
    unordered_map<string, TreeNode*> branches; //children
    vector<Instance>dataset;
    TreeNode* parent;
};

// Function to calculate the entropy of a dataset
double calculateEntropy(const vector<Instance>& dataset)
{
    unordered_map<string, int> classCounts;
    for (const Instance& instance : dataset)
    {
        classCounts[instance.classification]++;
    }

    double entropy = 0.0;
    for (const auto& classCount : classCounts)
    {
        double prob = static_cast<double>(classCount.second) / dataset.size();
        entropy -= prob * log2(prob);
    }

    return entropy;
}

// Function to calculate the information gain for a specific attribute
double calculateInformationGain(vector<Instance>& dataset, string& attribute)
{
    double entropy_before_split = calculateEntropy(dataset);

    unordered_map<string, vector<Instance>>dividedData;
    for (Instance& instance : dataset)
    {
        dividedData[instance.values[attribute]].push_back(instance);
    }

    double remainder = 0.0;
    for (const auto& division : dividedData)
    {
        double weight = division.second.size()*1.00/dataset.size();
        remainder += (weight*calculateEntropy(division.second));
    }
    return entropy_before_split - remainder;
}

// Function to choose the best attribute for splitting
string chooseBestAttribute( vector<Instance>& dataset,  unordered_set<string>& remainingAttributes)
{
    string bestAttribute;
    double bestInfoGain = -1.0;

    for (string attribute : remainingAttributes)
    {
        double infoGain = calculateInformationGain(dataset, attribute);
        if (infoGain > bestInfoGain)
        {
            bestInfoGain = infoGain;
            bestAttribute = attribute;
        }
    }

    return bestAttribute;
}


// Function to find out plurality of given example
pair<string,int> plurality_value( vector<Instance>& dataset)
{
    unordered_map<string, int> classCounts;
    for (const Instance& instance : dataset)
    {
        classCounts[instance.classification]++;
    }

    string bestClass;
    int bestCount = -1;
    int c=0;
    for (const auto& classCount : classCounts)
    {
        if (classCount.second > bestCount)
        {
            bestCount = classCount.second;
            bestClass = classCount.first;
            c = 1;
        }
        else if(classCount.second == bestCount)
            c++;
    }

    return make_pair(bestClass,c);
}

string plurality_of_parent(TreeNode* par)
{
    pair<string,int> res = plurality_value(par->dataset);
    if(res.second==1)
        return res.first;

    return plurality_of_parent(par->parent);
}

TreeNode* buildDecisionTree(vector<Instance>& dataset, unordered_set<string> remainingAttributes, TreeNode* parent)
{

    if(dataset.empty()) //case 3: No example left
    {
        TreeNode* newNode = new TreeNode();
        newNode->isLeaf = true;
        newNode->attribute = plurality_of_parent(parent);
        newNode->parent = parent;
        return newNode;
    }
    else if(remainingAttributes.empty()) // case 4: No attribute left
    {
        TreeNode* newNode = new TreeNode();
        newNode->isLeaf = true;
        newNode->attribute = plurality_value(dataset).first;
        newNode->parent = parent;
        return newNode;
    }
    else
    {
        unordered_map<string, int> classCounts;
        for (const Instance& instance : dataset)
        {
            classCounts[instance.classification]++;
        }
        int c=0;
        for (const auto& classCount : classCounts)
        {
            if(classCount.second>0)
                c++;

            if(c==2)
                break;
        }

        if(c==1) // case 1: All example are in same classification
        {
            TreeNode* newNode = new TreeNode();
            newNode->isLeaf = true;
            newNode->attribute = dataset.front().classification;
            newNode->parent = parent;
            return newNode;
        }
        else // case 2: recursive case
        {
            string bestAttribute = chooseBestAttribute(dataset,remainingAttributes);
            TreeNode* newNode = new TreeNode();
            newNode->isLeaf = false;
            newNode->parent = parent;
            newNode->attribute = bestAttribute;
            newNode->dataset = dataset;
            remainingAttributes.erase(bestAttribute);
            for(string attr: attributes[bestAttribute])
            {
                vector<Instance> childDataset;
                for(Instance& instance : dataset)
                {
                    if(instance.values[bestAttribute]==attr)
                        childDataset.push_back(instance);
                }

                TreeNode* child = buildDecisionTree(childDataset,remainingAttributes,newNode);
                newNode->branches.insert(make_pair(attr,child));
                childDataset.clear();
            }
            return newNode;
        }
    }
}

// Function to classify a single instance using the decision tree
string classifyInstance(Instance& instance, TreeNode* currentNode)
{
    if (currentNode->isLeaf)
    {
        return currentNode->attribute;
    }

    string attributeValue = instance.values[currentNode->attribute];
    if (currentNode->branches.find(attributeValue) != currentNode->branches.end())
    {
        TreeNode* nextNode = currentNode->branches[attributeValue];
        return classifyInstance(instance, nextNode);
    }
    else
    {
        // In case of unseen attribute value, classify as plurality value of parent node
        return plurality_of_parent(currentNode);
    }
}

void deleteDecisionTree(TreeNode* node)
{
    if (node == nullptr)
    {
        return;
    }

    for (const auto& branch : node->branches)
    {
        deleteDecisionTree(branch.second);
    }

    delete node;
}

void printDecisionTree(TreeNode* node, int depth = 0)
{
    if (node == nullptr) {
        return;
    }
    // Print the current node
    for (int i = 0; i < depth; ++i) {
        cout << "  ";
    }
    if (depth > 0) {
        cout << "|-";
    }
    cout << node->attribute << endl;

    // Print branches recursively
    for (const auto& branch : node->branches) {
        // Print branch name and call the function recursively
        for (int i = 0; i < depth; ++i) {
            cout << "  ";
        }
        cout << "|-" << branch.first << endl;
        printDecisionTree(branch.second, depth + 1);
    }
}



int main()
{
    ifstream dataFile("car.data");
    vector<Instance> instances;
    if (dataFile.is_open())
    {
        string line;
        while (getline(dataFile, line))
        {
            stringstream ss(line);
            Instance instance;
            string value;
            int index = 0;

            while (getline(ss, value, ','))
            {
                if (index==0)
                {
                    instance.values["buying"] = value;
                }
                else if (index==1)
                {
                    instance.values["maint"] = value;
                }
                else if (index==2)
                {
                    instance.values["doors"] = value;
                }
                else if (index==3)
                {
                    instance.values["persons"] = value;
                }
                else if (index==4)
                {
                    instance.values["lug_boot"] = value;
                }
                else if (index==5)
                {
                    instance.values["safety"] = value;
                }
                else
                    instance.classification = value;

                index++;
            }

            instances.push_back(instance);
        }

        dataFile.close();
    }
    else
    {
        cout << "Unable to open file" << endl;
        return 1;
    }

    srand(time(0));

    const int numExperiments = 20;
    double totalAccuracy = 0.0;
    vector<pair<TreeNode*,double>> accuracies;

    // Initialize remainingAttributes set
    unordered_set<string> remainingAttributes;
    for (auto& attributeEntry : attributes)
    {
        remainingAttributes.insert(attributeEntry.first);
    }

    for (int exp = 0; exp < numExperiments; exp++)
    {
        random_shuffle(instances.begin(), instances.end()); // Shuffle the data
        int trainSize = (int)(0.8 * instances.size());
        vector<Instance> trainingSet(instances.begin(), instances.begin() + trainSize);
        vector<Instance> testSet(instances.begin() + trainSize, instances.end());

        TreeNode* root = buildDecisionTree(trainingSet, remainingAttributes, nullptr);

        int correctClassifications = 0;
        for ( Instance& instance : testSet)
        {
            string predictedClass = classifyInstance(instance, root);
            if (predictedClass == instance.classification)
            {
                correctClassifications++;
            }
        }
        double accuracy = static_cast<double>(correctClassifications) / testSet.size();
        totalAccuracy += accuracy;
        accuracies.push_back(make_pair(root,accuracy));

        trainingSet.clear();
        testSet.clear();
    }

    double meanAccuracy = totalAccuracy / numExperiments;

    double sumSquaredDeviations = 0.0;
    for (pair<TreeNode*,double> acc : accuracies)
    {
        sumSquaredDeviations += pow(acc.second - meanAccuracy, 2);
    }
    double standardDeviation = sqrt(sumSquaredDeviations / numExperiments);

    cout << "Mean Accuracy: " << meanAccuracy << endl;
    cout << "Standard Deviation of Accuracy: " << standardDeviation << endl;


    //deleting tree node
    for (pair<TreeNode*,double> acc : accuracies)
    {
        deleteDecisionTree(acc.first);
    }
}
