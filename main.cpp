#include <iostream>
#include <vector>

using namespace std;

class handNode
{
public:

    int value;
    int numberOfSiblings;
    float parentProbability;
    float selfProbability;

public:

    handNode(int nOSParam, float pPParam, int valueParam)
    {
        numberOfSiblings = nOSParam;
        parentProbability = pPParam;
        value = valueParam;
        selfProbability = static_cast<float>(1.0/numberOfSiblings)*parentProbability;
    }

};

vector<int> eraseFunction(int number, vector<int> vector)
{
    for(int i=0; i<vector.size(); i++)
    {
        if(vector[i]==number)
        {
            vector.erase(vector.begin()+i);
            break;
        }
    }
    return vector;
}

vector<handNode> startingFunction(vector<int> &imaginaryDeck, int playerOpenCardValue)
{
    vector<handNode> handNodeVector;

    int possibleSum = 0;

        for(int i=0; i<imaginaryDeck.size(); i++)
        {
            possibleSum = playerOpenCardValue + imaginaryDeck[i];
            handNodeVector.emplace_back(imaginaryDeck.size(),1,possibleSum);
        }

        return handNodeVector;
}

void assistantFunction(vector<handNode> &handNodeVector, vector<int> &imaginaryDeck, int pastValue)
{
    int possibleSum;
    for(int i=0; i<handNodeVector.size(); i++)
    {
        if(handNodeVector[i].value < 18)
        {
            eraseFunction(handNodeVector[i].value-pastValue,imaginaryDeck);

            for(int k=0; k<imaginaryDeck.size(); k++)
            {
                possibleSum = handNodeVector[i].value + imaginaryDeck[k];
                handNodeVector.emplace_back(imaginaryDeck.size(),handNodeVector[i].selfProbability,possibleSum);
            }
        }
        else
        {
            continue;
        }
    }
}

int main() {
    vector<int> knownDeck;

    knownDeck.push_back(6);
    knownDeck.push_back(8);
    knownDeck.push_back(2);


    return 0;
}
