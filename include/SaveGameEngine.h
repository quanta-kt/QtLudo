#ifndef SAVE_GAME_ENGINE_H
#define SAVE_GAME_ENGINE_H

#include <QDataStream>
#include <QFile>
#include <QVector>
#include <QString>
#include <QIODevice>

class Game;
class Board;
class Pawn;

/* This class provides a mechanism to save/load game from/to a file(s)
 * It releys on QDataStream for serialization */
class SaveGameEngine {

public:
    virtual ~SaveGameEngine();

    //Serializes to a file
    static SaveGameEngine* serialize(QString filename, Game *game, Board *board, QVector<Pawn*> pawns);

    //Deserializes from a file
    static SaveGameEngine* deserialize(QString filename);

    Game* getGame();
    Board* getBoard();
    QVector<Pawn*> getPawns();

private:
    explicit SaveGameEngine();

    static QDataStream *stream {};
    QFile *mSavefile {};

    Game *s_game {}; //Serialized game
    Board *s_board {}; //Serialized board
    QVector<Pawn*> s_pawns {}; //Serialized pawns
};

#endif //SAVE_GAME_ENGINE_H
