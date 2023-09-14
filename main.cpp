#include <iostream>
#include <vector>
#include <algorithm>
#include <random>

using namespace std;

void printDeckFunction(vector<int> desiredVector)
{
    for(int i=0; i<desiredVector.size(); i++)
    {
        cout<<desiredVector[i]<<endl;
    }
    cout<<endl;
}

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

void generate_combinations(const vector<int>& knownDeck, int cardDraw, int combinationNumber, int start_index, vector<int>& current_combination, vector<vector<int>>& combinations) {
    if (current_combination.size() == cardDraw) {
        combinations.push_back(current_combination);
        return;
    }

    for (int i = start_index; i < knownDeck.size(); ++i) {
        current_combination.push_back(knownDeck[i]);
        generate_combinations(knownDeck, cardDraw, combinationNumber, i + 1, current_combination, combinations);
        current_combination.pop_back();
    }
}

vector<int> get_combinationFunction(const vector<int>& knownDeck, int cardDraw, int combinationNumber) {
    vector<int> current_combination;
    vector<vector<int>> combinations;
    generate_combinations(knownDeck, cardDraw, combinationNumber, 0, current_combination, combinations);

    if (combinationNumber >= 0 && combinationNumber < combinations.size()) {
        return combinations[combinationNumber];
    } else {
        return {};
    }
}

double winProbabilityFunction(vector<int> knownDeck, int openCardValue,int gladosHandValue, int numberOfUnknownCards)
{
    //probably the most important function in this program
    int possibleHandCombinations = combinationFunction(knownDeck.size(),numberOfUnknownCards);
    if(possibleHandCombinations>50000)
    {
        numberOfUnknownCards--;
        possibleHandCombinations = combinationFunction(knownDeck.size(),numberOfUnknownCards);
    }

    int gladosHaveSuperiorHand = 0;
    int gladosHaveInferiorHand = 0;
    int equalHand = 0;
    int possibleHandValue = 0;
    int sumOfUnknown = 0;
    vector<int> combinationVector;

    double winProb;

    for(int k=0; k<possibleHandCombinations; k++)
    {
        combinationVector = get_combinationFunction(knownDeck,numberOfUnknownCards,k);

        for(int i=0; i<combinationVector.size(); i++)
        {
            sumOfUnknown = sumOfUnknown + combinationVector[i];
        }
        possibleHandValue = openCardValue + sumOfUnknown;

        if(possibleHandValue>21 || possibleHandValue<gladosHandValue)
        {
            gladosHaveSuperiorHand++;
        }
        else if(possibleHandValue>gladosHandValue)
        {
            gladosHaveInferiorHand++;
        }
        else
        {
            equalHand++;
        }
        sumOfUnknown = 0;
    }

    possibleHandCombinations = gladosHaveSuperiorHand + gladosHaveInferiorHand + equalHand;
    winProb = static_cast<double>(gladosHaveSuperiorHand+equalHand)/static_cast<double>(possibleHandCombinations);
    return winProb;
}

double riskOfDrawingCardFunction(vector<int> knownDeck, int gladosHandValue)
{
    int possibleHandValue;
    int busted  = 0;
    int notBusted = 0;
    double probability;

    for(int i=0; i<knownDeck.size(); i++)
    {
        possibleHandValue = gladosHandValue + knownDeck[i];

        if(possibleHandValue>21)
        {
            busted++;
        }
        else
        {
            notBusted++;
        }
    }

    probability = static_cast<double>(busted)/(notBusted+busted);
    return probability;
}

double expectedValueFunctionAssistant(vector<int> knownDeck, int openCardValue,int gladosHandValue, int numberOfUnknownCards)
{
    int possibleHandCombinations = combinationFunction(knownDeck.size(),numberOfUnknownCards);
    if(possibleHandCombinations>50000)
    {
        numberOfUnknownCards--;
        possibleHandCombinations = combinationFunction(knownDeck.size(),numberOfUnknownCards);
    }

    int gladosHaveSuperiorHand = 0;
    int gladosHaveInferiorHand = 0;
    int possibleHandValue = 0;
    int sumOfUnknown = 0;
    vector<int> combinationVector;

    double winProb;

    for(int k=0; k<possibleHandCombinations; k++)
    {
        combinationVector = get_combinationFunction(knownDeck, numberOfUnknownCards, k);
        for(int i=0; i<combinationVector.size(); i++)
        {
            sumOfUnknown = sumOfUnknown + combinationVector[i];
        }
        possibleHandValue = openCardValue + sumOfUnknown;

        if(possibleHandValue>21)
        {
            gladosHaveSuperiorHand++;
        }
        else
        {
            gladosHaveInferiorHand++;
        }
        sumOfUnknown = 0;
    }

    possibleHandCombinations = gladosHaveSuperiorHand + gladosHaveInferiorHand;
    winProb = static_cast<double>(gladosHaveSuperiorHand)/possibleHandCombinations;
    return winProb;
}

double expectedValueFunction(vector<int> knownDeck, int gladosHandValue, int playerOpenCardValue, int numberOfUnknownCards)
{
    //VERY IMPORTANT NOTE: SOMEHOW, THE PROBABILITY OF DRAWING A SPECIFIC CARD IS SAME, IRRELEVANT FROM NUMBER OF UNKNOWN OPPONENT CARDS.
    //HOW? NO IDEA, BUT I DID THE MATH AND IT'S ALWAYS THE SAME

    vector<int> originalDeck = knownDeck;
    double expectedValue=0;
    double eachProbability = 1/static_cast<double>(knownDeck.size());
    double instance;
    double sumOfProbabilites;
    double pastWinProbability;
    double updatedWinProbability;
    double instanceSum = 0.0;
    int pickedCard;
    int updatedGladosHandValue;

    pastWinProbability = winProbabilityFunction(knownDeck,playerOpenCardValue,gladosHandValue,numberOfUnknownCards);

    for(int i=0; i<knownDeck.size(); i++)
    {
        pickedCard = knownDeck[i];
        knownDeck = eraseFunction(pickedCard,knownDeck);
        updatedGladosHandValue = gladosHandValue + pickedCard;

        //god forbid this to be true, it's wide open to bugs...
        if(updatedGladosHandValue>21)
        {
            updatedWinProbability = expectedValueFunctionAssistant(knownDeck,playerOpenCardValue,updatedGladosHandValue,numberOfUnknownCards);
        }
        else
        {
            updatedWinProbability = winProbabilityFunction(knownDeck,playerOpenCardValue,updatedGladosHandValue,numberOfUnknownCards);
        }


        sumOfProbabilites = static_cast<double>(updatedWinProbability-pastWinProbability);

        instance = eachProbability * sumOfProbabilites;
        instanceSum = instanceSum + instance;
        knownDeck = originalDeck;
    }

    const double tolerance = 1e-12; // Set a small tolerance
    if (fabs(instanceSum) < tolerance) {
        instanceSum = 0.0;
    }
    return instanceSum;

}

int main() {

    vector<int> testVector;
    int gladosHandValue = 16;
    int playerOpenCardValue = 10;
    int numberOfUnknownCards = 2;
    double riskOfDrawing;
    double winProb;
    double expectedValue;
    double x,y;
    x = 0.111;
    y = -0.111;

    testVector.push_back(10);
    testVector.push_back(3);
    testVector.push_back(7);



    winProb = winProbabilityFunction(testVector,playerOpenCardValue,gladosHandValue,numberOfUnknownCards);
    riskOfDrawing = riskOfDrawingCardFunction(testVector,gladosHandValue);
    expectedValue = expectedValueFunction(testVector,gladosHandValue,playerOpenCardValue,numberOfUnknownCards);
    cout<<expectedValue<<endl;


    return 0;
}