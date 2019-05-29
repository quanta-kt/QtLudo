#include <Game.h>

#include <QDebug>

#include <Board.h>
#include <Pawn.h>
#include <PlayerColor.h>
#include <SaveGameEngine.h>
#include <ValueError.h>

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

Game::Game(SaveGameEngine *save) {
    this->players_count = save->readInt();
    
    this->currentSequence = new QVector<PlayerColor> {};
    for(unsigned int i = 0; i < players_count; i++)
        this->currentSequence->append(static_cast<PlayerColor>(save->readInt()));
    
    this->current = save->readInt();
    this->lastDiceValue = save->readInt();
    
    this->mBoard = save->getBoard();
    
    this->random = QRandomGenerator::securelySeeded();
}

void Game::serializeInto(SaveGameEngine *save) {
    save->writeInt(this->players_count);
    
    for(auto p : *(this->currentSequence))
        save->writeInt(static_cast<int>(p));
    
    save->writeInt(this->current);
    save->writeInt(this->lastDiceValue);
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
        ValueError::raise_new(QString("Invalid dice value : %1").arg(diceFace));

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

unsigned int Game::predictRel(Pawn* pawn, unsigned int diceFace) {
    qInfo() << "Game::predictRel(Pawn*, int)";

    if(pawn->isAtHome() && diceFace != 6 && SIX_FOR_HOME)
        ValueError::raise_new(QString("Invalid move"));
    if(pawn->getRelPosition() + diceFace > Pawn::DEST)
        ValueError::raise_new(QString("Invalid move"));

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

    //This pawn couldn't have been clashed with anybody, because it has reached home
    if(!pawn->hasReachedDestination()) {

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
    } else {
        //But we do get a turn again,..
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
    if(current >= (unsigned int)(currentSequence->size())-1) {
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
