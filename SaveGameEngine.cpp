#include <SaveGameEngine.h>

SaveGameEngine() {}

virtual ~SaveGameEngine() {}

//Serializes to a file
static SaveGameEngine* serialize(QString filename, Game *game, Board *board, QVector<Pawn*> pawns) {
    QFile file = {filename, QIODevice::WriteOnly};
    stream = {file};

    game->serializeInto(this);
    board->serializeInto(this);
    stream << pawns.size(); //We need this
    for(auto p : pawns)
        p->serializeInto(this);
}

//Deserializes from a file
static SaveGameEngine* deserialize(QString filename) {
    QFile file = {filename, QIODevice::ReadOnly};
    stream = {file};

    s_game = new Game(this);
    s_board = new Board(this);
    int pawn_n {};
    for (size_t pawn_n = 0; pawn_n < count; pawn_n++)
        s_pawns.append(new Pawn(this));
}

Game* getGame() {
    return s_game;
}

Board* getBoard() {
    return s_board;
}

QVector<Pawn*> getPawns() {
    return s_pawns;
}
