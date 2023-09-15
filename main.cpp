#include <iostream>
#include <cmath>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <random>
#include <string>

using namespace std;

mt19937 mt(time(nullptr));

class Player{
private:
    int wallet;
    int potMoney = 0;
    int handValue;
    int openCardValue;
    vector<int> cardsInsideHand;
    //TODO 5 card game
    int numberOfCardsInHand;

public:
    Player(int walletParameter)
    {
        wallet = walletParameter;
    }

public:
    int getTotalNumberOfCards()
    {
        return cardsInsideHand.size();
    }

    int getTotalValueOfHand()
    {
        int sum=0;
        for(int i=0; i<cardsInsideHand.size(); i++)
        {
            sum = sum + cardsInsideHand[i];
        }

        this->handValue = sum;
        return sum;
    }

    int getPlayerOpenCardValue()
    {
        return cardsInsideHand[0];
    }

    void addCardToPlayersHand(int card)
    {
        cardsInsideHand.push_back(card);
    }

    void printCards()
    {
        for(int i=0; i<cardsInsideHand.size(); i++)
        {
            cout<<cardsInsideHand[i]<<" ";
        }
        cout<<endl;
    }

    void raiseBet(int amount)
    {
        this->wallet = wallet - amount;
        this->potMoney = potMoney + amount;
    }

    vector<int> getCards()
    {
        return this->cardsInsideHand;
    }

    int getWallet()
    {
        return this->wallet;
    }

    int getPotMoney()
    {
        return this->potMoney;
    }

    void clearHand()
    {
        this->cardsInsideHand.clear();
    }

    void victory(int opponentPotMoney)
    {
        this->wallet = wallet + opponentPotMoney + potMoney;
        this->potMoney = 0;
        clearHand();
    }

    void defeat()
    {
        this->potMoney = 0;
        clearHand();
    }

    void draw()
    {
        this->wallet = wallet + potMoney;
        this->potMoney = 0;
        clearHand();
    }

    int getNumberOfUnknownCards()
    {
        int nouc = cardsInsideHand.size()-1;
        return nouc;
    }

    vector<int> getUnknownCards()
    {
        vector<int> unKnownCards;
        for(int i=1; i<cardsInsideHand.size(); i++)
        {
            unKnownCards.push_back(cardsInsideHand[i]);
        }
        return unKnownCards;
    }

    int getGameValue()
    {
        int rawValue = getTotalValueOfHand();
        if(rawValue>21)
        {
            return 0;
        }
        else
        {
            if(getTotalNumberOfCards()==5)
            {
                return 21;
            }
            else
            {
                return rawValue;
            }
        }
    }


};
class handNode
{
public:

    int value = 0;
    int numberOfSiblings;
    double parentProbability;
    double selfProbability;
    bool finalHand;
    vector<int> cardVector;
    int openCardValue;

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

        if(value>=18)
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

vector<handNode> startingFunction(vector<int> knownDeck, int playerOpenCardValue, vector<handNode> handNodeVector)
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

void assistantFunction(vector<handNode> &handNodeVector, vector<int> knownDeck, int lastRunLastNodeID, int lastRunFirstNodeID, int limit)
{
    vector<int> originalVector;
    originalVector = knownDeck;
    vector<int> tempVector;
    int nodesCreatedThisRun = 0;
    int firstNodeID;
    int lastNodeID;

    for(int i=lastRunFirstNodeID; i<=lastRunLastNodeID; i++)
    {
        if(handNodeVector[i].value<limit)
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
        assistantFunction(handNodeVector,knownDeck,lastNodeID,firstNodeID,limit);
    }

}

double playerKeepsDrawing(vector<int> knownDeck, int playerOpenCardValue, vector<handNode> &handNodeVectorParameter, int satisfactionValue, int gladosHandValue)
{
    vector<handNode> handNodeVector;
    handNodeVector = startingFunction(knownDeck,playerOpenCardValue,handNodeVectorParameter);
    assistantFunction(handNodeVector,knownDeck,knownDeck.size()-1,0,satisfactionValue);


    double gladosHavingSuperiorHandProb = 0;
    double gladosHavingInferiorHandProb = 0;
    double equalHandProb = 0;


    for(int i=0; i<handNodeVector.size(); i++)
    {
        if(handNodeVector[i].finalHand)
        {
            if(handNodeVector[i].value>gladosHandValue && handNodeVector[i].value<=21)
            {
                gladosHavingInferiorHandProb = gladosHavingInferiorHandProb + handNodeVector[i].selfProbability;
            }
            else if(handNodeVector[i].value<gladosHandValue || handNodeVector[i].value>21)
            {
                gladosHavingSuperiorHandProb = gladosHavingSuperiorHandProb + handNodeVector[i].selfProbability;
            }
            else
            {
                equalHandProb = equalHandProb + handNodeVector[i].selfProbability;
            }
        }
    }

    cout<<"sup: "<<gladosHavingSuperiorHandProb<<endl;
    cout<<"inf: "<<gladosHavingInferiorHandProb<<endl;
    cout<<"tie: "<<equalHandProb<<endl;

    return gladosHavingSuperiorHandProb;

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

void generate_combinations(const vector<int>& knownDeck, int cardDraw, int start_index, vector<int>& current_combination, vector<vector<int>>& combinations) {
    if (current_combination.size() == cardDraw) {
        combinations.push_back(current_combination);
        return;
    }

    for (int i = start_index; i < knownDeck.size(); ++i) {
        current_combination.push_back(knownDeck[i]);
        generate_combinations(knownDeck, cardDraw, i + 1, current_combination, combinations);
        current_combination.pop_back();
    }
}

vector<vector<int>> get_combinations(const vector<int>& knownDeck, int cardDraw) {
    vector<int> current_combination;
    vector<vector<int>> combinations;
    generate_combinations(knownDeck, cardDraw, 0, current_combination, combinations);
    return combinations;
}

double winProbabilityFunction(vector<int> knownDeck, int openCardValue,int gladosHandValue, int numberOfUnknownCards)
{
    //probably the most important function in this program
    int possibleHandCombinations = combinationFunction(knownDeck.size(),numberOfUnknownCards);
    int gladosHaveSuperiorHand = 0;
    int gladosHaveInferiorHand = 0;
    int equalHand = 0;
    int possibleHandValue = 0;
    int sumOfUnknown = 0;
    double winProb;
    vector<int> combinationVector;
    vector<vector<int>> allCombinations = get_combinations(knownDeck, numberOfUnknownCards);

    if(gladosHandValue>21)
    {
        for(int k=0; k<possibleHandCombinations; k++)
        {
            combinationVector = allCombinations[k];
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
    }
    else
    {
        for(int k=0; k<possibleHandCombinations; k++)
        {
            combinationVector = allCombinations[k];
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

double expectedValueFunction(vector<int> knownDeck, int gladosHandValue, int playerOpenCardValue, int numberOfUnknownCards)
{
    //IMPORTANT NOTE: SOMEHOW, THE PROBABILITY OF DRAWING A SPECIFIC CARD IS SAME, IRRELEVANT FROM NUMBER OF UNKNOWN OPPONENT CARDS.
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
            updatedGladosHandValue = 0;
        }

        updatedWinProbability = winProbabilityFunction(knownDeck,playerOpenCardValue,updatedGladosHandValue,numberOfUnknownCards);



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

void drawCardFunction(mt19937& rng, vector<int>& knownDeck, vector<int>& actualDeck, bool isForGlados, Player &SubjectName)
{
    int randomNumber;
    int randomCard;
    // Draw a card
    randomNumber = mt() % actualDeck.size();
    randomCard = actualDeck[randomNumber];

    actualDeck = eraseFunction(randomCard, actualDeck);

    if(isForGlados)
    {
        knownDeck = eraseFunction(randomCard, knownDeck);
    }
    else
    {
        cout<<"card is: "<<randomCard<<endl;
    }

    SubjectName.addCardToPlayersHand(randomCard);
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

    //aces for later
    return vector;
}

void dealCardsFunction(mt19937& rng, vector<int> &knownDeck, vector<int> &actualDeck, Player &SubjectGlados, Player &SubjectHuman)
{
    int randomNumber;
    int handValue = 0;
    int randomCard;

    for(int i=1; i<=2; i++)
    {
        randomNumber = mt()%actualDeck.size();
        randomCard = actualDeck[randomNumber];

        //get them out
        actualDeck = eraseFunction(randomCard,actualDeck);
        knownDeck = eraseFunction(randomCard,knownDeck);
        SubjectGlados.addCardToPlayersHand(randomCard);
    }

    for(int i=1; i<=2; i++)
    {
        randomNumber = mt()%actualDeck.size();
        randomCard = actualDeck[randomNumber];

        //get them out
        actualDeck = eraseFunction(randomCard,actualDeck);
        SubjectHuman.addCardToPlayersHand(randomCard);
    }

    //don't forget the human players open card
    knownDeck = eraseFunction(SubjectHuman.getPlayerOpenCardValue(),knownDeck);


}

bool gladosStandFunction(float winProb)
{
    mt19937 mt(time(nullptr));
    int randomNumber;
    bool gladosStands;

    randomNumber = mt()%100;

    if(winProb>=0.75)
    {
        //yes
        gladosStands=true;
    }
    else if(winProb<0.75 && winProb>=0.5)
    {
        //maybe

        if(randomNumber<winProb*100)
        {
            //yeah
            gladosStands=true;
        }
        else
        {
            //nah
            gladosStands=false;
        }
    }
    else
    {
        //no
        gladosStands=false;
    }
    return gladosStands;
}

void printDeckFunction(vector<int> desiredVector)
{
    for(int i=0; i<desiredVector.size(); i++)
    {
        cout<<desiredVector[i]<<endl;
    }
    cout<<endl;
}

int gladosBetRaiseFunction(mt19937& rng, double winProb, int maxBetRaise)
{
    int randomNumber;
    randomNumber = mt()%100;


    //should I add a bluff option? I don't know
    if(winProb<0.66)
    {
    return 0;
    }

    else
    {
        if(randomNumber<winProb*100)
        {
            randomNumber = (rng() % maxBetRaise) + 1;
            return randomNumber;
        }
        else
        {
            return 0;
        }
    }
}


int main()
{
    mt19937 mt_rng(time(nullptr));

    int startingMoney,blindBet,maxBetRaise;
    int betRaise,sumOfBet, gladosBetRaiseSum = 0;
    int playerSatistactionValue = 18;
    int gladosBetRaise;
    double winProb,riskOfDrawingCard;
    double expectedValue;
    bool humanNewCard = false;

    string question;
    string earlyQuestion;
    vector<int> deckKnownToGlados;
    vector<int> actualDeck;
    vector<int> playersUnknownCards;
    vector<handNode> handNodeVector;
    bool gladosStands;
    bool messageDisplayed;

    startingMoney = 100;
    blindBet = 10;
    maxBetRaise = 40;

    actualDeck = deckCreatorFunction(actualDeck);
    deckKnownToGlados = actualDeck;

    //create the players
    Player Glados(startingMoney);
    Player Human(startingMoney);


    while(actualDeck.size()>4 && Glados.getWallet()>=blindBet && Human.getWallet()>=blindBet)
    {
        cout<<"Another round begins"<<endl;
        cout<<"glados wallet: "<<Glados.getWallet()<<endl;
        cout<<"human wallet: "<<Human.getWallet()<<endl;
        //deal the cards
        dealCardsFunction(mt_rng,deckKnownToGlados,actualDeck,Glados,Human);

        //place the blind bet, no backing now
        Glados.raiseBet(blindBet);
        Human.raiseBet(blindBet);

        //glados takes action
        expectedValue = expectedValueFunction(deckKnownToGlados,Glados.getTotalValueOfHand(),Human.getPlayerOpenCardValue(),Human.getNumberOfUnknownCards());

        cout<<"Open card of Glados is: "<<Glados.getPlayerOpenCardValue()<<endl;
        cout<<"Your cards are: "<<endl;
        printDeckFunction(Human.getCards());

        while(expectedValue>0)
        {
            drawCardFunction(mt_rng, deckKnownToGlados,actualDeck, true,Glados);
            cout<<"Glados draws a card"<<endl;
            expectedValue = expectedValueFunction(deckKnownToGlados,Glados.getTotalValueOfHand(),Human.getPlayerOpenCardValue(),Human.getNumberOfUnknownCards());
        }

        winProb = winProbabilityFunction(deckKnownToGlados,Human.getPlayerOpenCardValue(),Glados.getTotalValueOfHand(),Human.getNumberOfUnknownCards());
        cout<<"win prob intial: "<<winProb<<endl;
        cout<<"glados cards: "<<endl;
        printDeckFunction(Glados.getCards());

        gladosStands = gladosStandFunction(winProb);

        sumOfBet = 0;

        //human can't draw a card yet
        humanNewCard = false;

        //player takes action
        while(true)
        {
            cout<<"Type draw to draw a card. Type raise to raise bet. Type stand if you want to do neither."<<endl;
            cout<<"Or if you realized that resistance is futile, type retreat to... well, retreat?"<<endl;
            cout<<"Your total hand value: "<<Human.getTotalValueOfHand()<<endl;
            cin>>question;

            if(question=="draw")
            {
                drawCardFunction(mt_rng,deckKnownToGlados,actualDeck,false,Human);
                humanNewCard = true;
                cout<<"calculating..."<<endl;
                winProb = winProbabilityFunction(deckKnownToGlados,Human.getPlayerOpenCardValue(),Glados.getTotalValueOfHand(),Human.getNumberOfUnknownCards());
                printDeckFunction(Human.getCards());
            }
            else if(question=="raise")
            {

                while(true)
                {
                    cout<<"How much?"<<endl;
                    cin>>betRaise;
                    if(betRaise>Glados.getWallet())
                    {
                        cout<<"Hold your horses, Glados doesn't have that much money. Most you can raise is: "<<Glados.getWallet()<<endl;
                    }
                    else if(betRaise>Human.getWallet())
                    {
                        cout<<"Hold your horses, you don't have that much money. Most you can raise is: "<<Human.getWallet()<<endl;
                    }
                    else if(sumOfBet + betRaise>maxBetRaise)
                    {
                        cout<<"Hold your horses, you went below the allowed bet raise for this round. Most you can raise is: "<<maxBetRaise-sumOfBet<<endl;
                    }
                    else
                    {
                        break;
                    }
                }

                sumOfBet = sumOfBet + betRaise;
                Human.raiseBet(betRaise);

                //will glados see?
                //probably the most important part of the program

                //update the winProb if player picked a new card
                if(humanNewCard)
                {
                    cout<<"glados stand updated"<<endl;
                    gladosStands = gladosStandFunction(winProb);
                    humanNewCard = false;
                }

                if(gladosStands)
                {
                    cout<<"Glados matches your bet"<<endl;
                    Glados.raiseBet(betRaise);
                    cout<<endl;
                }
                else
                {
                    cout<<"Glados retreats, Human won"<<endl;
                    cout<<"Glados hand: "<<endl;
                    printDeckFunction(Glados.getCards());
                    cout<<endl;
                    deckKnownToGlados = actualDeck;
                    Human.victory(Glados.getPotMoney());
                    Glados.defeat();
                    cout<<endl;
                    break;
                }

            }
            else if(question=="stand")
            {
                if(Glados.getGameValue()==Human.getGameValue())
                {
                    cout<<"It's a draw"<<endl;
                    cout<<"Glados hand: "<<endl;
                    printDeckFunction(Glados.getCards());
                    cout<<endl;
                    deckKnownToGlados = actualDeck;
                    Glados.draw();
                    Human.draw();
                    cout<<endl;
                    break;
                }
                else if(Glados.getGameValue()>Human.getGameValue())
                {

                    cout<<"Glados won"<<endl;
                    cout<<"Glados hand: "<<endl;
                    printDeckFunction(Glados.getCards());
                    cout<<endl;
                    deckKnownToGlados = actualDeck;
                    Glados.victory(Human.getPotMoney());
                    Human.defeat();
                    cout<<endl;
                    break;
                }
                else
                {

                    cout<<"Human won"<<endl;
                    cout<<"Glados hand: "<<endl;
                    printDeckFunction(Glados.getCards());
                    cout<<endl;
                    deckKnownToGlados = actualDeck;
                    Human.victory(Glados.getPotMoney());
                    Glados.defeat();
                    cout<<endl;
                    break;
                }
            }
            else
            {
                cout<<"Glados won"<<endl;
                cout<<"Glados hand: "<<endl;
                printDeckFunction(Glados.getCards());
                cout<<endl;
                deckKnownToGlados = actualDeck;
                Glados.victory(Human.getPotMoney());
                Human.defeat();
                cout<<endl;
                break;
            }
        }
    }

    if(Glados.getWallet()>Human.getWallet())
    {
        cout<<"Glados won the match"<<endl;
    }
    else
    {
        cout<<"Human won the match"<<endl;
    }



    return 0;
}
