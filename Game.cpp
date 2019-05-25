#include <Game.h>

#include <Board.h>
#include <Pawn.h>
#include <PlayerColor.h>

#include <QDebug>

Game::Game(unsigned int players) :
    players_count(players), mBoard(new Board {players}),
    currentSequence(new QVector<PlayerColor> {}), current (0),
    random (QRandomGenerator::securelySeeded()) {

    switch(players) {
        case 2:
            currentSequence->push_back(PlayerColor::RED);
            currentSequence->push_back(PlayerColor::BLUE); //So that pawns are at opposite corners
            break;
        case 3:
            currentSequence->push_back(PlayerColor::RED);
            currentSequence->push_back(PlayerColor::YELLOW);
            currentSequence->push_back(PlayerColor::BLUE);
            break;
        case 4:
            currentSequence->push_back(PlayerColor::RED);
            currentSequence->push_back(PlayerColor::YELLOW);
            currentSequence->push_back(PlayerColor::BLUE);
            currentSequence->push_back(PlayerColor::GREEN);
            break;
    }
}

Game::~Game() {
    delete currentSequence;
    delete mBoard;
}

int Game::rollDice() {
    //TODO:: Generate a random number between 1 to 6

    lastDiceValue = random.bounded(1, 7);
    return lastDiceValue;
}

PlayerColor Game::getCurrentPlayer() {
    return (*currentSequence)[current];
}

Board* Game::getGameBoard() {
    return mBoard;
}

QVector<Pawn*> Game::getPlayablePawns(int diceFace) {
    if(diceFace < 1 || diceFace > 6)
        throw std::string {"Invalid dice value"};

    QVector<Pawn*> result;
    QVector<Pawn*> pawns = mBoard->getAllPawns();

    for(auto p : pawns) {
        if(p->getColor() != getCurrentPlayer())
            continue;
        if(p->isAtHome() && diceFace != 6 && SIX_FOR_HOME)
            continue;
        if(p->hasReachedDestination())
            continue;
        if(p->getRelPosition() + diceFace > Pawn::DEST)
            continue;

        result.append(p);
    }

    return result;
}

int Game::predictRel(Pawn* pawn, int diceFace) {
    qInfo() << "Game::predictRel(Pawn*, int)";

    if(pawn->isAtHome() && diceFace != 6 && SIX_FOR_HOME)
        throw std::string {"Invalid move"};
    if(pawn->getRelPosition() + diceFace > Pawn::DEST)
        throw std::string {"Invalid move"};

    if(pawn->isAtHome()) //Just get out of home by one step
        return pawn->getRelPosition() + 1;
    else
        return pawn->getRelPosition() + diceFace;
}

bool Game::playMove(Pawn* pawn, int diceFace) {
    qInfo() << "Game::playMove(Pawn*, int)";

    //Set it to the number by which we're gonna move
    unsigned int futureRel {Game::predictRel(pawn, diceFace)};

    //Will the player get a turn again?
    bool re_turn = diceFace == 6;

    //Pawn which was hit by this one while moving
    Pawn* toClash {nullptr};

    //If there is only one pawn at this new location, send it back home
    //This should be done before moving the current pawn
    QVector<Pawn*> pawnsThere = mBoard->getPawnsAt(
        mBoard->getPawnCoordinates(
            pawn->getColor(),
            futureRel //Our future position
        )
    );
    if(pawnsThere.size() == 1 && pawnsThere[0]->getColor() != pawn->getColor()) {
        toClash = pawnsThere[0];
        re_turn = true;
    }

    //We can now move the pawn
    pawn->changePosition(futureRel);

    //And send the clashed one home
    if(toClash != nullptr)
        toClash->goBackHome();

    return re_turn;
}

void Game::changeCurrentPlayer() {
    if(current >= (currentSequence->size())-1) {
        current = 0;
        return;
    }

    current++;
}

QVector<PlayerColor> Game::getCurrentPlayerSequence() {
    return *currentSequence;
}

unsigned int Game::getLastDiceValue() {
    return lastDiceValue;
}
