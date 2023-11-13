#include <iostream>
#include <cmath>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <random>
#include <string>
#include <algorithm>
#include <windows.h>


using namespace std;

mt19937 mt(time(nullptr));

class Deck
{
private:
    vector<int> cards;
    int totalValue;
    int gameValue;
    int numberOfCards;
    int openCardValue;

public:
    int getTotalValue()
    {
        int sum=0;
        for(int i=0; i<cards.size(); i++)
        {
            sum = sum + cards[i];
        }
        this->totalValue = sum;
        return totalValue;
    }

    int getNumberOfCards()
    {
        int number;
        number = cards.size();
        this->numberOfCards = number;
        return numberOfCards;
    }

    int getGameValue()
    {
        int value;

        if(getTotalValue()>21)
        {
            value = 0;
        }
        else
        {
            if(getNumberOfCards()==5)
            {
                value = 21;
            }
            else
            {
                value = getTotalValue();
            }
        }
        this->gameValue = value;
        return gameValue;
    }

    void removeCard(int cardToRemove)
    {
        for(int i=0; i<cards.size(); i++)
        {
            if(cards[i]==cardToRemove)
            {
                cards.erase(cards.begin()+i);
                break;
            }
        }
    }

    int getOpenCard()
    {
        int open;
        open = cards[0];
        openCardValue = open;
        return openCardValue;
    }

    void addCard(int cardToAdd)
    {
        this->cards.push_back(cardToAdd);
    }

    void clearCards()
    {
     this->cards.clear();
    }

    vector<int> getCards()
    {
        return cards;
    }

    void printCards()
    {
        for(int i=0; i<cards.size(); i++)
        {
            cout<<cards[i]<<" ";
        }
        cout<<endl;
    }

    int getElementI(int indexNumber)
    {
        return cards[indexNumber];
    }

    void createLargeDeck()
    {
        this->cards.clear();
        //ordinary numbers
        for(int k=2; k<=10; k++)
        {
            for(int i=1; i<=4; i++)
            {
                cards.push_back(k);
            }
        }

        //k, q and j's
        for(int i=1; i<=12; i++)
        {
            cards.push_back(10);
        }
    }

};

class Player{

private:
    int wallet;
    int potMoney = 0;
    int gameValue;
    int openCardValue;
    int numberOfCards;
    int totalValue;
    Deck cardsInsideHand;


public:
    Player(int walletParameter)
    {
        wallet = walletParameter;
    }

public:

    int getTotalNumberOfCards()
    {
        int number;
        number = cardsInsideHand.getNumberOfCards();
        this->numberOfCards = number;
        return numberOfCards;
    }

    int getTotalValueOfHand()
    {
        this->totalValue = cardsInsideHand.getTotalValue();
        return totalValue;
    }

    void printSubjectCards()
    {
        cardsInsideHand.printCards();
    }

    int getPlayerOpenCardValue()
    {
       this->openCardValue = cardsInsideHand.getOpenCard();
       return openCardValue;
    }

    int getPlayerGameValue()
    {
        this->gameValue = cardsInsideHand.getGameValue();
        return gameValue;
    }

    void putMoneyInPot(int amount)
    {
        this->wallet = wallet - amount;
        this->potMoney = potMoney + amount;
    }


    int getWallet()
    {
        return this->wallet;
    }

    int getPotMoney()
    {
        return this->potMoney;
    }

    vector<int> getCards()
    {
        return cardsInsideHand.getCards();
    }

    void victory(int opponentPotMoney)
    {
        this->wallet = wallet + opponentPotMoney + potMoney;
        this->potMoney = 0;
        cardsInsideHand.clearCards();
    }

    void defeat()
    {
        this->potMoney = 0;
        cardsInsideHand.clearCards();
    }

    void draw()
    {
        this->wallet = wallet + potMoney;
        this->potMoney = 0;
        cardsInsideHand.clearCards();
    }

    void drawCard(bool isForGlados, Deck &knownDeck, Deck &actualDeck)
    {
        int randomCard;
        int index = mt()%actualDeck.getNumberOfCards();
        randomCard = actualDeck.getElementI(index);
        this->cardsInsideHand.addCard(randomCard);

        if(isForGlados)
        {
            knownDeck.removeCard(randomCard);
            actualDeck.removeCard(randomCard);
        }
        else
        {
            actualDeck.removeCard(randomCard);
            cout<<"Card drawn is: "<<randomCard<<endl;
        }
    }

    int getNumberOfUnknownCards()
    {
        return getTotalNumberOfCards()-1;
    }

    Deck getUnknownDeck()
    {
        Deck unKnownDeck;
        for(int i=1; i<cardsInsideHand.getNumberOfCards(); i++)
        {
            unKnownDeck.addCard(cardsInsideHand.getElementI(i));
        }
        return unKnownDeck;
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

vector<handNode> humanTreeInitialFunction(Deck knownDeck, int playerOpenCardValue, vector<handNode> handNodeVector)
{
    vector<int> tempVector;

    for(int i=0; i<knownDeck.getNumberOfCards(); i++)
    {
        tempVector.push_back(playerOpenCardValue);
        tempVector.push_back(knownDeck.getElementI(i));
        handNodeVector.emplace_back(knownDeck.getNumberOfCards(),1.0,tempVector);
        tempVector.clear();
    }

    return handNodeVector;
}

void humanTreeRecursiveFunction(vector<handNode> &handNodeVector, Deck knownDeck, int lastRunLastNodeID, int lastRunFirstNodeID, int limit)
{
    vector<int> tempVector;
    vector<int> originalVector;
    vector<int> knownDeckCopy;
    knownDeckCopy = knownDeck.getCards();
    originalVector = knownDeckCopy;
    int nodesCreatedThisRun = 0;
    int firstNodeID;
    int lastNodeID;

    for(int i=lastRunFirstNodeID; i<=lastRunLastNodeID; i++)
    {
        if(handNodeVector[i].value<limit)
        {
            knownDeckCopy = originalVector;

            // Remove the cards that CAN'T be drawn, starting from the last element
            for (auto it = handNodeVector[i].cardVector.begin() + 1; it != handNodeVector[i].cardVector.end(); ++it)
            {
                int cardThatWillBeRemoved = *it; // Get the current card (excluding the open card value)
                knownDeckCopy = eraseFunction(cardThatWillBeRemoved,knownDeckCopy);
            }

            for(int create=0; create<knownDeckCopy.size(); create++)
            {
                tempVector = handNodeVector[i].cardVector;
                tempVector.push_back(knownDeckCopy[create]);
                handNodeVector.emplace_back(knownDeckCopy.size(),handNodeVector[i].selfProbability,tempVector);
                tempVector.clear();
                if(nodesCreatedThisRun==0 && create==0)
                {
                    firstNodeID = handNodeVector.size()-1;
                }
                nodesCreatedThisRun++;
            }
            knownDeckCopy = originalVector;
        }
    }
    knownDeckCopy = originalVector;
    lastNodeID = handNodeVector.size()-1;

    if(nodesCreatedThisRun!=0)
    {
        humanTreeRecursiveFunction(handNodeVector,knownDeck,lastNodeID,firstNodeID,limit);
    }

}

vector<handNode> humanTreeFunction(Deck knownDeck, int playerOpenCardValue, vector<handNode> &handNodeVectorParameter, int satisfactionValue)
{
    vector<handNode> handNodeVector;
    vector<handNode> finalNodeVector;
    handNodeVector = humanTreeInitialFunction(knownDeck,playerOpenCardValue,handNodeVectorParameter);
    humanTreeRecursiveFunction(handNodeVector,knownDeck,knownDeck.getNumberOfCards()-1,0,satisfactionValue);

    for(int i=0; i<handNodeVector.size(); i++)
    {
        if(handNodeVector[i].finalHand)
        {
            finalNodeVector.push_back(handNodeVector[i]);
        }
    }

    return finalNodeVector;
}

double humanTreeWinProbability(Deck knownDeck, int playerOpenCardValue, vector<handNode> &handNodeVectorParameter, int satisfactionValue, int gladosHandValue)
{
    vector<handNode> finalNodes;
    finalNodes = humanTreeFunction(knownDeck,playerOpenCardValue,handNodeVectorParameter,satisfactionValue);
    double gladosHavingSuperiorHandProb = 0;
    double gladosHavingInferiorHandProb = 0;
    double equalHandProb = 0;

    if(gladosHandValue>21)
    {
        for (int i = 0; i < finalNodes.size(); i++)
        {
            if (finalNodes[i].value<=21)
            {
                gladosHavingInferiorHandProb = gladosHavingInferiorHandProb + finalNodes[i].selfProbability;
            } else if (finalNodes[i].value>21)
            {
                gladosHavingSuperiorHandProb = gladosHavingSuperiorHandProb + finalNodes[i].selfProbability;
            }
        }
    }

    else
    {
        for (int i = 0; i < finalNodes.size(); i++)
        {
                if (finalNodes[i].value > gladosHandValue && finalNodes[i].value <= 21)
                {
                    gladosHavingInferiorHandProb = gladosHavingInferiorHandProb + finalNodes[i].selfProbability;
                } else if (finalNodes[i].value < gladosHandValue || finalNodes[i].value > 21)
                {
                    gladosHavingSuperiorHandProb = gladosHavingSuperiorHandProb + finalNodes[i].selfProbability;
                } else
                {
                    equalHandProb = equalHandProb + finalNodes[i].selfProbability;
                }
        }
    }

    //clear the cache
    handNodeVectorParameter.clear();

    return gladosHavingSuperiorHandProb + equalHandProb;

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

void generateCombinations(Deck knownDeck, int cardDraw, int start_index, vector<int>& current_combination, vector<vector<int>>& combinations) {
    if (current_combination.size() == cardDraw) {
        combinations.push_back(current_combination);
        return;
    }

    for (int i = start_index; i < knownDeck.getNumberOfCards(); ++i) {
        current_combination.push_back(knownDeck.getElementI(i));
        generateCombinations(knownDeck, cardDraw, i + 1, current_combination, combinations);
        current_combination.pop_back();
    }
}

vector<vector<int>> getCombinations(Deck knownDeck, int cardDraw) {
    vector<int> current_combination;
    vector<vector<int>> combinations;
    generateCombinations(knownDeck, cardDraw, 0, current_combination, combinations);
    return combinations;
}

double winProbabilityFunction(Deck knownDeck, int openCardValue,int gladosHandValue, int numberOfUnknownCards)
{
    //probably the most important function in this program
    int possibleHandCombinations = combinationFunction(knownDeck.getNumberOfCards(),numberOfUnknownCards);
    int gladosHaveSuperiorHand = 0;
    int gladosHaveInferiorHand = 0;
    int equalHand = 0;
    int possibleHandValue = 0;
    int sumOfUnknown = 0;
    double winProb;
    vector<int> combinationVector;
    vector<vector<int>> allCombinations = getCombinations(knownDeck, numberOfUnknownCards);

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

double expectedValueFunction(Deck knownDeck, int gladosHandValue, int playerOpenCardValue, int numberOfUnknownCards)
{
    //IMPORTANT NOTE: SOMEHOW, THE PROBABILITY OF DRAWING A SPECIFIC CARD IS SAME, IRRELEVANT FROM NUMBER OF UNKNOWN OPPONENT CARDS.
    //HOW? NO IDEA, BUT I DID THE MATH AND IT'S ALWAYS THE SAME

    vector<int> originalDeck = knownDeck.getCards();
    double expectedValue=0;
    double eachProbability = 1/static_cast<double>(knownDeck.getNumberOfCards());
    double instance;
    double sumOfProbabilites;
    double pastWinProbability;
    double updatedWinProbability;
    double instanceSum = 0.0;
    int pickedCard;
    int updatedGladosHandValue;

    pastWinProbability = winProbabilityFunction(knownDeck,playerOpenCardValue,gladosHandValue,numberOfUnknownCards);

    for(int i=0; i<knownDeck.getNumberOfCards(); i++)
    {
        pickedCard = knownDeck.getElementI(i);
        knownDeck.removeCard(pickedCard);
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
        knownDeck.getCards() = originalDeck;
    }

    return instanceSum;

}

void dealCardsFunction(mt19937& rng, Deck &knownDeck, Deck &actualDeck, Player &SubjectGlados, Player &SubjectHuman)
{
    int randomNumber;
    int handValue = 0;
    int randomCard;

    //for glados
    for(int i=1; i<=2; i++)
    {
        randomNumber = mt()%actualDeck.getNumberOfCards();
        randomCard = actualDeck.getElementI(randomNumber);
        SubjectGlados.drawCard(true,knownDeck,actualDeck);
    }

    //for human
    for(int i=1; i<=2; i++)
    {
        randomNumber = mt()%actualDeck.getNumberOfCards();
        randomCard = actualDeck.getElementI(randomNumber);
        SubjectHuman.drawCard(false,knownDeck,actualDeck);
    }

    knownDeck.removeCard(SubjectHuman.getPlayerOpenCardValue());

}

bool gladosStandFunction(double winProb, int blindBet, int startingMoney, int betRange, int betRaiseSum)
{
    int analogToleranceLimit;
    int digitalToleranceLimit;
    bool gladosStands;
    int toleranceBarDistance;
    int toleranceBarDivide;
    vector<int> toleranceBar;

    toleranceBarDivide = 4;

    //compensation is needed because when the blind bet is high (relative to starting money), you can't just keep folding, waiting for the best moment
    //I mean, with the discrete tolerance states, is it really needed? I really don't know
    double compensation = static_cast<double>(blindBet)/(startingMoney);

    analogToleranceLimit = betRange*(winProb+compensation);
    //as toleranceBarDivide approaches infinity, betRange becomes more and more continuous
    toleranceBarDistance = betRange/toleranceBarDivide;

    for(int i=0; i<toleranceBarDivide; i++)
    {
        int bar = (i+1) * (toleranceBarDistance);
        toleranceBar.push_back(bar);
    }

    if(toleranceBar.back()<=analogToleranceLimit)
    {
        digitalToleranceLimit = betRange;
    }
    else
    {
        //find out which sector it's between
        for(int i=0; i<toleranceBarDivide; i++)
        {
            if(toleranceBar[i]>analogToleranceLimit)
            {
                //which is closest, above or below?
                if (abs(toleranceBar[i] - analogToleranceLimit) <= abs(toleranceBar[i - 1] - analogToleranceLimit))
                {
                    digitalToleranceLimit = toleranceBar[i];
                    break;
                }
                else
                {
                    digitalToleranceLimit = analogToleranceLimit;
                    break;
                }
            }
        }
    }

    //check if the bet raise is in acceptable ground
    if(winProb<0.5)
    {
        gladosStands = false;
    }
    else
    {
        if(winProb>=0.75)
        {
            gladosStands = true;
        }
        else
        {
            if(betRaiseSum>digitalToleranceLimit)
            {
                gladosStands = false;
            }
            else
            {
                gladosStands = true;
            }
        }
    }


    return gladosStands;
}

double humanBustedProbabilityFunction(Player SubjectHuman, Deck knownDeck, int numberOfUnknownCards)
{
    int possibleHandCombinations = combinationFunction(knownDeck.getNumberOfCards(),numberOfUnknownCards);
    int sumOfUnknown = 0;
    int busted = 0;
    int notBusted = 0;
    int possibleHandValue;
    double bustedProb;
    vector<int> combinationVector;
    vector<vector<int>> allCombinations = getCombinations(knownDeck, numberOfUnknownCards);

        for(int k=0; k<possibleHandCombinations; k++)
        {
            combinationVector = allCombinations[k];
            for(int i=0; i<combinationVector.size(); i++)
            {
                sumOfUnknown = sumOfUnknown + combinationVector[i];
            }
            possibleHandValue = SubjectHuman.getPlayerOpenCardValue() + sumOfUnknown;

            if(possibleHandValue>21)
            {
                busted++;
            }
            else
            {
                notBusted++;
            }
            sumOfUnknown = 0;
        }
        bustedProb = static_cast<double>(busted)/static_cast<double>(busted+notBusted);

        return bustedProb;
}

int gladosBetRaiseFunction(mt19937& rng, double winProb, int limit)
{
    int betRaise;
    int randomNumber = mt();
    //should I add a bluff option? I don't know
    if(winProb<0.75)
    {
        if(winProb<0.50)
        {
            if(randomNumber%3==0)
            {
                int lowest = limit*0.75;
                int highest = limit - lowest;
                betRaise = (mt()%highest) + lowest;
            }
            else
            {
                betRaise = 0;
            }
        }
        else
        {
            betRaise = 0;
        }
    }
    else
    {
        betRaise = limit*winProb;
    }
    return betRaise;
}

int betRaiseLimitFunction(int subjectMoneyInPot, int gladosWallet, int humanWallet, int maxBetRaise)
{
    int warning;
    int raiseLimit;
    raiseLimit = maxBetRaise - subjectMoneyInPot;
    warning = min({gladosWallet,humanWallet,raiseLimit});
    return warning;
}

void roundEndFunction(Player &SubjectGlados, Player &SubjectHuman, bool gladosWon, bool draw, Deck &knownDeck)
{
    Deck unKnownDeck;
    unKnownDeck = SubjectHuman.getUnknownDeck();

    for(int i=0; i<unKnownDeck.getNumberOfCards(); i++)
    {
        knownDeck.removeCard(unKnownDeck.getElementI(i));
    }


    if(gladosWon && !draw)
    {
        cout<<"Glados won. It's hand was: "<<endl;
        SubjectGlados.printSubjectCards();
        SubjectGlados.victory(SubjectHuman.getPotMoney());
        SubjectHuman.defeat();
        cout<<endl;
    }
    else if(!gladosWon && !draw)
    {
        cout<<"Glados lost. It's hand was: "<<endl;
        SubjectGlados.printSubjectCards();
        SubjectHuman.victory(SubjectGlados.getPotMoney());
        SubjectGlados.defeat();
        cout<<endl;
    }
    else
    {
        cout<<"It's a draw. Glados's hand was:"<<endl;
        SubjectGlados.printSubjectCards();
        SubjectGlados.draw();
        SubjectHuman.draw();
        cout<<endl;
    }
}

int main()
{
    mt19937 mt_rng(time(nullptr));
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);

    int startingMoney,blindBet,maxBetRaiseForPlayer;
    int betRaise,moneyInPot, gladosInitialBetRaise,gladosExtraBetRaise = 0;
    int playerSatistactionValue = 18;
    int limit,betRange;
    double winProb,riskOfDrawingCard;
    double expectedValue;
    double humanTreeWinProb;
    double bustedProb;
    double compensationValue;
    bool humanNewCard;

    string question;
    vector<handNode> handNodeVector;
    bool gladosStands;
    bool messageDisplayed;
    bool matchInitialRaise;
    bool matchExtraRaise;

    startingMoney = 100;
    blindBet = 15;
    maxBetRaiseForPlayer = 50;
    betRange = maxBetRaiseForPlayer - blindBet;
    compensationValue = (static_cast<double>(maxBetRaiseForPlayer-blindBet)/(startingMoney))/2;

    cout<<"c"<<compensationValue<<endl;

    //create the players
    Player Glados(startingMoney);
    Player Human(startingMoney);
    Deck actualDeck;
    Deck deckKnownToGlados;

    //create the mutual, large decks
    actualDeck.createLargeDeck();
    deckKnownToGlados.createLargeDeck();

    while(Glados.getWallet()>blindBet && Human.getWallet()>blindBet)
    {
        SetConsoleTextAttribute(h,15);
        gladosInitialBetRaise = 0;
        gladosExtraBetRaise = 0;
        humanNewCard = false;
        matchInitialRaise = false;
        matchExtraRaise = false;
        messageDisplayed = false;
        cout<<"Your wallet: "<<Human.getWallet()<<endl;
        cout<<"Glados's wallet: "<<Glados.getWallet()<<endl;
        cout<<endl;

        //check if deck is depleted
        //4, because this is a 2 player game and 2 cards are dealt for each player. hence 2*2 is...4
        if(actualDeck.getNumberOfCards()<=4)
        {
            SetConsoleTextAttribute(h,5);
            cout<<"Opening a new deck..."<<endl;
            actualDeck.createLargeDeck();
            deckKnownToGlados.createLargeDeck();
        }
        //deal the cards
        dealCardsFunction(mt_rng,deckKnownToGlados,actualDeck,Glados,Human);

        SetConsoleTextAttribute(h,6);
        cout<<"Glados open card value is: "<<Glados.getPlayerOpenCardValue()<<endl;
        SetConsoleTextAttribute(h,15);

        //place the blind bets, no backing up now
        Glados.putMoneyInPot(blindBet);
        Human.putMoneyInPot(blindBet);

        cout<<"Your cards are: "<<endl;
        Human.printSubjectCards();
        cout<<endl;

        //glados shall draw a card as long as it's logical to do so
        expectedValue = expectedValueFunction(deckKnownToGlados,Glados.getTotalValueOfHand(),Human.getPlayerOpenCardValue(),Human.getNumberOfUnknownCards());
        while(expectedValue>0.0-compensationValue && Glados.getPlayerGameValue()>0)
        {
            Glados.drawCard(true,deckKnownToGlados,actualDeck);
            SetConsoleTextAttribute(h,14);
            cout<<"Glados drew a card"<<endl;
            expectedValue = expectedValueFunction(deckKnownToGlados,Glados.getTotalValueOfHand(),Human.getPlayerOpenCardValue(),Human.getNumberOfUnknownCards());
        }

        winProb = winProbabilityFunction(deckKnownToGlados,Human.getPlayerOpenCardValue(),Glados.getTotalValueOfHand(),Human.getNumberOfUnknownCards());
        gladosStands = gladosStandFunction(winProb,blindBet,startingMoney,betRange,Glados.getPotMoney()-blindBet);
        limit = betRaiseLimitFunction(Glados.getPotMoney(),Glados.getWallet(),Human.getWallet(),maxBetRaiseForPlayer);
        humanTreeWinProb = humanTreeWinProbability(deckKnownToGlados,Human.getPlayerOpenCardValue(),handNodeVector,playerSatistactionValue,Glados.getTotalValueOfHand());
        gladosInitialBetRaise = gladosBetRaiseFunction(mt_rng,humanTreeWinProb,limit);
        moneyInPot = 2*blindBet;

        if(Glados.getTotalNumberOfCards()==2)
        {
            SetConsoleTextAttribute(h,8);
            cout<<"Glados doesn't draw additional card this round"<<endl;
        }

        if(gladosInitialBetRaise==0)
        {
            SetConsoleTextAttribute(h,8);
            cout<<"Glados doesn't increase the bet this round"<<endl;
        }
        else
        {
            SetConsoleTextAttribute(h,11);
            Glados.putMoneyInPot(gladosInitialBetRaise);
            cout<<"Glados increases bet by: "<<gladosInitialBetRaise<<endl;
        }

        //finally, player gets to take action
        while(true)
        {
            SetConsoleTextAttribute(h,15);
            cout<<"Your total value of hand is: "<<Human.getTotalValueOfHand()<<endl;
            if(Human.getTotalNumberOfCards()==5 && Human.getTotalValueOfHand()<=21)
            {
                SetConsoleTextAttribute(h,10);
                cout<<"5 card game! Your hand is considered 21 now"<<endl;
                SetConsoleTextAttribute(h,15);
            }
            cout<<"Draw, raise, stand or fold"<<endl;
            cin>>question;

            if(question=="draw")
            {
                if(Human.getTotalNumberOfCards()==5)
                {
                    cout<<"You can't draw anymore!"<<endl;
                    continue;
                }
                else if(actualDeck.getNumberOfCards()==0)
                {
                    cout<<"Deck is depleted, you can't draw anymore for this round"<<endl;
                    continue;
                }
                humanNewCard = true;
                Human.drawCard(false,deckKnownToGlados,actualDeck);
                //update win probability
                winProb = winProbabilityFunction(deckKnownToGlados,Human.getPlayerOpenCardValue(),Glados.getTotalValueOfHand(),Human.getNumberOfUnknownCards());
                limit = betRaiseLimitFunction(Glados.getPotMoney(),Glados.getWallet(),Human.getWallet(),maxBetRaiseForPlayer);

                //corner him if he is expected to be busted
                bustedProb = humanBustedProbabilityFunction(Human,deckKnownToGlados,Human.getNumberOfUnknownCards());
                gladosExtraBetRaise = gladosBetRaiseFunction(mt_rng,bustedProb,limit);
                Glados.putMoneyInPot(gladosExtraBetRaise);

                //if winProb is off the roof, raise the bet to force the human to fold or think twice when standing
                if(gladosInitialBetRaise==0 && gladosExtraBetRaise!=0 && !messageDisplayed)
                {
                    SetConsoleTextAttribute(h,11);
                    cout<<"Glados raises bet by: "<<gladosExtraBetRaise<<endl;
                    messageDisplayed = true;
                    matchExtraRaise = false;
                }
            }
            else if(question=="raise")
            {
                if(!matchInitialRaise)
                {
                    Human.putMoneyInPot(gladosInitialBetRaise);
                    matchInitialRaise = true;
                }
                else if(!matchExtraRaise)
                {
                    Human.putMoneyInPot(gladosExtraBetRaise);
                    matchExtraRaise=true;
                }
                limit = betRaiseLimitFunction(Human.getPotMoney(),Glados.getWallet(),Human.getWallet(),maxBetRaiseForPlayer);
                if(limit<=0)
                {
                    cout<<"You can't raise right now"<<endl;
                    continue;
                }
                else
                {
                    cout<<"Most you can raise right now is: "<<limit<<endl;
                    cout<<"How much you want to raise?"<<endl;
                    cin>>betRaise;
                    if(betRaise>limit)
                    {
                        cout<<"You went above and beyond (even though you were warned), try again"<<endl;
                        continue;
                    }

                    Human.putMoneyInPot(betRaise);

                    //will glados match this raise?

                    if(gladosInitialBetRaise==0)
                    {
                        cout<<"stance updated"<<endl;
                        gladosStands = gladosStandFunction(winProb,blindBet,startingMoney,betRange,Human.getPotMoney()-blindBet);
                    }

                    if(gladosStands || gladosInitialBetRaise!=0)
                    {
                        cout<<"Glados matches your bet"<<endl;
                        Glados.putMoneyInPot(betRaise);
                    }
                    else
                    {
                        //draw a card, what's the worst that could happen? we were about to fold anyway
                        if(Glados.getPlayerGameValue()>0)
                        {
                            Glados.drawCard(true,deckKnownToGlados,actualDeck);
                        }
                        winProb = winProbabilityFunction(deckKnownToGlados,Human.getPlayerOpenCardValue(),Glados.getTotalValueOfHand(),Human.getNumberOfUnknownCards());
                        gladosStands = gladosStandFunction(winProb,blindBet,startingMoney,betRange,Human.getPotMoney()-blindBet);

                        if(gladosStands)
                        {
                            cout<<"Glados matches your bet"<<endl;
                            Glados.putMoneyInPot(betRaise);
                        }
                        else
                        {
                            SetConsoleTextAttribute(h,2);
                            cout<<"Glados folds"<<endl;
                            roundEndFunction(Glados,Human, false, false,deckKnownToGlados);
                            break;
                        }
                    }
                }
            }
            else if(question=="stand")
            {
                Human.putMoneyInPot(Glados.getPotMoney()-blindBet);
                if(Glados.getPlayerGameValue()==Human.getPlayerGameValue())
                {
                    SetConsoleTextAttribute(h,15);
                    roundEndFunction(Glados,Human,false, true,deckKnownToGlados);
                    break;
                }
                else if(Glados.getPlayerGameValue()>Human.getPlayerGameValue())
                {
                    SetConsoleTextAttribute(h,4);
                    roundEndFunction(Glados,Human,true,false,deckKnownToGlados);
                    break;
                }
                else
                {
                    SetConsoleTextAttribute(h,2);
                    roundEndFunction(Glados,Human,false,false,deckKnownToGlados);
                    break;
                }
            }
            else if(question=="fold")
            {
                SetConsoleTextAttribute(h,4);
                roundEndFunction(Glados,Human, true, false,deckKnownToGlados);
                break;
            }
            else
            {
                cout<<"You have a typo, try again"<<endl;
            }
        }
    }

    if(Human.getWallet()>Glados.getWallet())
    {
        cout<<"This wasn't supposed to happen, but here we are. Congrats..."<<endl;
    }
    else
    {
        cout<<"Did you really thought you stood a chance?"<<endl;
    }

    return 0;
}
