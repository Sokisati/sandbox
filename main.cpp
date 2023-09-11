#include <iostream>
#include <vector>

using namespace std;

class handNode
{
public:

    int value = 0;
    int numberOfSiblings;
    double parentProbability;
    double selfProbability;
    bool finalHand;
    vector<int> cardVector;

public:

    handNode(int nOSParam, double pPParam, vector<int> cardVectorParam)
    {
        numberOfSiblings = nOSParam;
        parentProbability = pPParam;
        selfProbability = static_cast<double>(1.0/numberOfSiblings)*parentProbability;
        cardVector = cardVectorParam;

        for(int i=0; i<cardVector.size(); i++)
        {
            value = value + cardVector[i];
        }

        if(value>18)
        {
            finalHand = true;
        }
        else
        {
            finalHand = false;
        }
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

vector<handNode> startingFunction(vector<int> &knownDeck, int playerOpenCardValue, vector<handNode> &handNodeVector)
{
    vector<int> tempVector;


    for(int i=0; i<knownDeck.size(); i++)
    {
        tempVector.push_back(playerOpenCardValue);
        tempVector.push_back(knownDeck[i]);
        handNodeVector.emplace_back(knownDeck.size(),1.0,tempVector);
        tempVector.clear();
    }

    return handNodeVector;
}

void assistantFunction(vector<handNode> &handNodeVector, vector<int> knownDeck, int lastRunLastNodeID, int lastRunFirstNodeID)
{
    vector<int> originalVector;
    originalVector = knownDeck;
    vector<int> tempVector;
    int nodesCreatedThisRun = 0;
    int firstNodeID;
    int lastNodeID;

    for(int i=lastRunFirstNodeID; i<=lastRunLastNodeID; i++)
    {
        if(handNodeVector[i].value<18)
        {
            knownDeck = originalVector;

            // Remove the cards that CAN'T be drawn, starting from the last element
            for (auto it = handNodeVector[i].cardVector.begin() + 1; it != handNodeVector[i].cardVector.end(); ++it)
            {
                int cardThatWillBeRemoved = *it; // Get the current card (excluding the open card value)
                knownDeck = eraseFunction(cardThatWillBeRemoved, knownDeck);
            }



            for(int create=0; create<knownDeck.size(); create++)
            {
                tempVector = handNodeVector[i].cardVector;
                tempVector.push_back(knownDeck[create]);
                handNodeVector.emplace_back(knownDeck.size(),handNodeVector[i].selfProbability,tempVector);
                tempVector.clear();
                if(nodesCreatedThisRun==0 && create==0)
                {
                    firstNodeID = handNodeVector.size()-1;
                }
                nodesCreatedThisRun++;
            }
            knownDeck = originalVector;
        }
    }
    knownDeck = originalVector;
    lastNodeID = handNodeVector.size()-1;

    if(nodesCreatedThisRun!=0)
    {
        assistantFunction(handNodeVector,knownDeck,lastNodeID,firstNodeID);
    }


}

int main() {
    vector<int> knownDeck;
    vector<handNode> handNodeVector;

    knownDeck.push_back(6);
    knownDeck.push_back(8);
    knownDeck.push_back(10);
    knownDeck.push_back(2);
    knownDeck.push_back(2);

    int playerOpenCard = 10;

    startingFunction(knownDeck,playerOpenCard, handNodeVector);
    assistantFunction(handNodeVector,knownDeck,knownDeck.size()-1,0);


    for(int i=0; i<handNodeVector.size(); i++)
    {
        cout<<"i: "<<i<<endl;
        cout<<"value: "<<handNodeVector[i].value<<endl;
        cout<<"selfP: "<<handNodeVector[i].selfProbability<<endl;
        cout<<"number of siblings: "<<handNodeVector[i].numberOfSiblings<<endl;

        for(int k=0; k<handNodeVector[i].cardVector.size(); k++)
        {
            cout<<handNodeVector[i].cardVector[k]<<" ";
        }
        cout<<endl<<endl;
    }



    return 0;
}