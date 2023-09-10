#include <iostream>
#include <vector>

using namespace std;

class handNode
{
public:

    int value = 0;
    int numberOfSiblings;
    float parentProbability;
    float selfProbability;
    bool finalHand;
    vector<int> cardVector;

public:

    handNode(int nOSParam, float pPParam, vector<int> cardVectorParam)
    {
        numberOfSiblings = nOSParam;
        parentProbability = pPParam;
        selfProbability = static_cast<float>(1.0/numberOfSiblings)*parentProbability;
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

vector<handNode> startingFunction(vector<int> &imaginaryDeck, int playerOpenCardValue, vector<handNode> &handNodeVector)
{
    vector<int> tempVector;


        for(int i=0; i<imaginaryDeck.size(); i++)
        {
            tempVector.push_back(playerOpenCardValue);
            tempVector.push_back(imaginaryDeck[i]);
            handNodeVector.emplace_back(imaginaryDeck.size(),1.0,tempVector);
            tempVector.clear();
        }

        return handNodeVector;
}

void assistantFunction(vector<handNode> &handNodeVector, vector<int> imaginaryDeck, int lastRunLastNodeID, int lastRunFirstNodeID)
{
    vector<int> originalVector;
    originalVector = imaginaryDeck;
    vector<int> tempVector;
    int nodesCreatedThisRun = 0;
    int firstNodeID;
    int lastNodeID;

    for(int i=lastRunFirstNodeID; i<=lastRunLastNodeID; i++)
    {
        if(handNodeVector[i].value<18)
        {
            imaginaryDeck = originalVector;

            //remove the cards that CAN'T be drawn
            for(int k=0; k<handNodeVector[i].cardVector.size()-1; k++)
            {
                int cardThatWillBeRemoved;
                cardThatWillBeRemoved = handNodeVector[i].cardVector.back();
                cout<<"card that will be removed is: "<<cardThatWillBeRemoved<<endl;
                imaginaryDeck = eraseFunction(cardThatWillBeRemoved,imaginaryDeck);
            }

            cout<<"i: "<<i<<endl;
            cout<<"imaginary deck: "<<endl;
            for(int d=0; d<imaginaryDeck.size(); d++)
            {
                cout<<imaginaryDeck[d]<<" ";
            }
            cout<<endl;

            for(int create=0; create<imaginaryDeck.size(); create++)
            {
                tempVector = handNodeVector[i].cardVector;
                tempVector.push_back(imaginaryDeck[create]);
                handNodeVector.emplace_back(imaginaryDeck.size(),handNodeVector[i].selfProbability,tempVector);
                tempVector.clear();
                if(nodesCreatedThisRun==0 && create==0)
                {
                    firstNodeID = handNodeVector.size()-1;
                }
                nodesCreatedThisRun++;
            }
            imaginaryDeck = originalVector;
        }
    }
    imaginaryDeck = originalVector;
    lastNodeID = handNodeVector.size()-1;
    cout<<"last node id: "<<lastNodeID<<endl;
    cout<<"first node id: "<<firstNodeID<<endl;

    if(nodesCreatedThisRun!=0)
    {
        assistantFunction(handNodeVector,imaginaryDeck,lastNodeID,firstNodeID);
    }


}

int main() {
    vector<int> knownDeck;
    vector<handNode> handNodeVector;

    knownDeck.push_back(6);
    knownDeck.push_back(8);
    knownDeck.push_back(2);
    knownDeck.push_back(3);
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
