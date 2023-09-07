#include <iostream>
#include <cmath>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <random>
#include <string>

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

float winProbabilityFunction(vector<int> knownDeck, int openCardValue,int gladosHandValue, int numberOfDraws)
{
    //probably the most important function in this program

    int possibleHandCombinations = combinationFunction(knownDeck.size(),numberOfDraws);
    int gladosHaveSuperiorHand = 0;
    int gladosHaveInferiorHand = 0;
    int equalHand = 0;
    int possibleHandValue = 0;
    int sumOfUnknown = 0;
    vector<int> combinationVector;

    float winProb;

    for(int k=0; k<possibleHandCombinations; k++)
    {
        cout<<"line 157"<<endl;
        combinationVector = chosenNumbersFunction(knownDeck,numberOfDraws,k);
        cout<<"159"<<endl;
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
    }


    possibleHandCombinations = gladosHaveSuperiorHand + gladosHaveInferiorHand;
    winProb = static_cast<float>(gladosHaveSuperiorHand)/possibleHandCombinations;
    return winProb;
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

int drawCardFunction(vector<int>& knownDeck, vector<int>& actualDeck, bool isForGlados)
{
    mt19937 mt(time(nullptr));
    int randomNumber;
            // Draw a card
            randomNumber = mt() % actualDeck.size();
            actualDeck = eraseFunction(actualDeck[randomNumber], actualDeck);

            if(isForGlados)
            {
                knownDeck = eraseFunction(actualDeck[randomNumber], knownDeck);
            }

            cout<<"card is:"<<actualDeck[randomNumber]<<endl;

            return actualDeck[randomNumber];
}

vector<int> deckCreatorFunction(vector<int> vector)
{
    //ordinary numbers
    for(int k=2; k<=10; k++)
    {
        for(int i=1; i<=4; i++)
        {
            vector.push_back(k);
        }
    }

    //k, q and j's
    for(int i=1; i<=12; i++)
    {
        vector.push_back(10);
    }

    //aces
    for(int i=1; i<=4; i++)
    {
        vector.push_back(11);
    }

    return vector;
}

int dealCardsFunction(vector<int> &knownDeck, vector<int> &actualDeck,bool isForGlados, int &playerOpenCardValue)
{
    int randomNumber;
    int handValue = 0;
    mt19937 mt(time(nullptr));

    for(int k=1; k<=2; k++)
    {
        //-1, because vector index starts at 0 in c++
        randomNumber = mt()%actualDeck.size();

        //add the values
        handValue = handValue + actualDeck[randomNumber];


        //get them out
        actualDeck = eraseFunction(actualDeck[randomNumber],actualDeck);

        if(isForGlados)
        {
            knownDeck = eraseFunction(actualDeck[randomNumber],knownDeck);
            if(k==1)
            {
                cout<<"Open card of Glados is: "<<knownDeck[randomNumber]<<endl;
            }
            else
            {
                return handValue;
            }
        }

        else
        {
            if(k==1)
            {
                knownDeck = eraseFunction(actualDeck[randomNumber],knownDeck);
                playerOpenCardValue = actualDeck[randomNumber];
                cout<<"Open card of Player is: "<<knownDeck[randomNumber]<<endl;
            }
            else
            {
                return handValue;
            }

        }

    }
}



class Player{
public:

    int wallet;
    int potMoney;
    int handValue;

    //TODO 5 card game
    int numberOfCardsInHand;

public:
    Player(int walletParameter, int handValueParameter, int potMoneyParameter)
    {
        wallet = walletParameter;
        handValue = handValueParameter;
        potMoney = potMoneyParameter;
    }

};

int main()
{
    mt19937 mt(time(nullptr));

    int startingMoney,blindBet,randomNumber,possibleHandValue,possibleHandCombinations,betRaise;
    int gladosHaveSuperiorHand,gladosHaveInferiorHand,equalHand,playerOpenCardValue,busted,notBusted,moneyInThePot = 0;
    int numberOfDrawsPlayerMade=1;
    float winProb;
    string question;
    vector<int> deckKnownToGlados;
    vector<int> deckVector;

    /*
    cout<<"Starting money?"<<endl;
    cin>>startingMoney;

    cout<<"Blind bet?"<<endl;
    cin>>blindBet;
     */

    //Glados is this program, Player is whoever playing against this program
    Player Glados(startingMoney,0,0);
    Player Player(startingMoney,0,0);

//creation of decks
    deckVector = deckCreatorFunction(deckVector);
    deckKnownToGlados = deckCreatorFunction(deckKnownToGlados);

//game begins
while(true)
{
    //deal the cards

    //for glados
    Glados.handValue = dealCardsFunction(deckKnownToGlados,deckVector,true,playerOpenCardValue);
    //for human
    Player.handValue = dealCardsFunction(deckKnownToGlados,deckVector,false,playerOpenCardValue);

    cout<<"Glados hand value: "<<Glados.handValue<<endl;
    cout<<"Human player hand value: "<<Player.handValue<<endl;

        for(int i=0; i<deckKnownToGlados.size(); i++)
        {
            cout<<deckKnownToGlados[i]<<endl;
        }

        cout<<"knownDeck.size"<<deckKnownToGlados.size()<<endl;

        //win prob??

        vector<int> magicNumbers;

       magicNumbers = chosenNumbersFunction(deckKnownToGlados,numberOfDrawsPlayerMade,0);


       for(int i=0; i<magicNumbers.size(); i++)
       {
           cout<<"mmhhmm "<<magicNumbers[i]<<endl;
       }

        break;

        //finally, the player gets to take action
        while(true)
        {
            cout<<"What are you going to do? Type draw to draw a card or type raise the bet in the pot."<<endl;
            cout<<"Of course, if you don't want to do neither of it, type stand."<<endl;
            cin>>question;
            if(question=="draw")
            {
                randomNumber = mt()%deckVector.size();
                eraseFunction(randomNumber,deckVector);
                Player.handValue = Player.handValue + deckVector[randomNumber];
                numberOfDrawsPlayerMade++;
                cout<<"You drew: "<<deckVector[randomNumber]<<" and your hand value right now is: "<<Player.handValue;

                if(Player.handValue>21)
                {
                    cout<<"You busted"<<endl;
                    return 0;
                }
            }
            else if (question=="raise")
            {
                cout<<"How much?"<<endl;
                cin>>betRaise;

                if(betRaise>Player.wallet)
                {
                    cout<<"You don't have enough money. Most you can raise is: "<<Player.wallet<<endl;
                }
                else if(betRaise>Glados.wallet)
                {
                    cout<<"Glados doesn't have enough money. Most you can raise is: "<<Glados.wallet<<endl;
                }
                else
                {
                    Player.potMoney = Player.potMoney + betRaise;
                    Player.wallet = Player.wallet - betRaise;
                }
            }
            else
            {

            }

        }



       break;
}


    return 0;
}
