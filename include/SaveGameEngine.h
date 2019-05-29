#ifndef SAVE_GAME_ENGINE_H
#define SAVE_GAME_ENGINE_H

#include <QtGlobal>
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
    static void serialize(QString filename, QVector<Pawn*> pawns, Board *board, Game *game);

    /* Deserializes from a file and returns the state handle constructed on heap
     * NOTE: This class/function does not holds the ownership of the returned
     * object */
    static SaveGameEngine* deserialize(QString filename);

    //Reads an integer from the stream
    int readInt();

    //Writes an integer to the stream
    void writeInt(int i);

    //Reads a qreal from the stream
    qreal readReal();

    //Writes a qreal to the stream
    void writeReal(qreal r);

    Game* getGame();
    Board* getBoard();
    QVector<Pawn*> getPawns();

private:
    explicit SaveGameEngine(QFile *file);

    QDataStream *stream {};

    Game *s_game {}; //Serialized game
    Board *s_board {}; //Serialized board
    QVector<Pawn*> s_pawns {}; //Serialized pawns
};

#endif //SAVE_GAME_ENGINE_H
