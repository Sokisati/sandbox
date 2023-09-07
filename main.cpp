#include <iostream>
#include <vector>
#include <cmath>

using namespace std;

int combinationFunction(int samplePool, int selection)
{
    int sum = 1;
    int selectionFactorial = 1;
    int result;

    if (samplePool == selection)
    {
        return 1;
    }

    for (int i = 0; i < selection; i++)
    {
        sum = sum * (samplePool - i);
    }

    for (int i = 0; i < selection - 1; i++)
    {
        selectionFactorial = selectionFactorial * (selection - i);
    }
    result = sum / selectionFactorial;
    return result;
}

vector<int> intToBinaryFunction(int num, int size)
{
    vector<int> binary;

    binary.resize(size, 0);

    for (int i = size - 1; i >= 0; i--)
    {
        if (num & (1 << i))
        {
            binary[size - i - 1] = 1;
        }
    }

    return binary;
}

vector<int> elementPlacesFunction(const vector<int>& knownDeck, int numberOfDraws, int desiredCombinationNumber)
{
    vector<int> binaryVector;
    vector<int> tempVector;
    int tempVectorValue = 0;
    int numberOneCounter = 0;
    int combinationFound = 0;
    for(int i=0; i<pow(2,knownDeck.size()); i++)
    {
        binaryVector = intToBinaryFunction(i,knownDeck.size());
        for(int k=0; k<binaryVector.size(); k++)
        {
            if(binaryVector[k]==1)
            {
                tempVector.push_back(k);
                tempVectorValue = tempVectorValue + pow(2,k);
                numberOneCounter++;
                if(numberOneCounter>numberOfDraws)
                {
                    break;
                }
                else if (numberOneCounter==numberOfDraws && i>tempVectorValue)
                {
                    break;
                }
                else
                {
                    continue;
                }
            }
        }
        if(numberOneCounter==numberOfDraws)
        {
            if(combinationFound==desiredCombinationNumber)
            {
                return tempVector;
            }
            else
            {
                combinationFound++;
            }
        }
        numberOneCounter=0;
        tempVector.clear();
    }
    tempVector.clear();
    tempVector.push_back(404);
    return tempVector;
}



int main()
{
    vector<int> testVector;
    vector<int> chosenOnes;

    for (int i = 0; i < 19; i++)
    {
        testVector.push_back(i);
    }

    int numberOfDrawsPlayerMade = 3;

    int numberOfPossibleCombinations = combinationFunction(testVector.size(), numberOfDrawsPlayerMade);

    for (int i =0; i <numberOfPossibleCombinations; i++)
    {
        chosenOnes = elementPlacesFunction(testVector, numberOfDrawsPlayerMade, i);
        for (int k=0; k<chosenOnes.size(); k++)
        {
            cout << chosenOnes[k] << " ";
        }
        cout << endl;
    }

    return 0;
}
