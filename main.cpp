#include <iostream>
#include <vector>
#include <algorithm>
#include <random>

using namespace std;


class Card{

private:
    int value;
public:
    Card(int valueParam){
        this->value=valueParam;
    }

public:
    int getValue() {
        return value;
    }

    void setValue(int newVal) {

        this->value = newVal;
    }
};


class Deck{
    vector<Card> cards;


public:
    Deck(const vector<Card> cardsParam){
        this->cards=cardsParam;
    }

    vector<Card> getCards() const {
        return cards;
    }

    Card getCard(int index) const {
        return this->cards[index];
    }

public:
    int sum(){
        int sum=0;
        for(int i=0; i<this->cards.size(); i++)
        {
            sum= sum + this->cards[i].getValue();
        }
        return sum;
    }

    int size(){
        return this->cards.size();
    }

    void deleteCard(int number){
        for(int i=0; i<this->size(); i++)
        {
            if(this->cards[i].getValue()==number)
            {
                this->cards.erase(this->cards.begin()+i);
                break;
            }
        }
    }

    Card draw(){
        Card drawnCard=this->cards[this->size()-1];
        this->cards.erase(this->cards.begin()+this->size()-1);
        return drawnCard;
    }

    void shuffle(){
        auto rng = default_random_engine {};
        std::shuffle(begin(this->cards), end(this->cards), rng);
    }
};



int main() {
    Deck knownDeck(vector<Card>{
            Card(6),
            Card(8),
            Card(10),
            Card(2),
            Card(2),
    });

    knownDeck.shuffle();
    knownDeck.shuffle();
    knownDeck.shuffle();

    cout<<knownDeck.sum()<<endl;

    for (int i = 0; i < knownDeck.size(); ++i) {
        cout<<knownDeck.getCard(i).getValue()<<endl;
    }





    return 0;
}