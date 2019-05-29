#include <SaveGameEngine.h>

#include <Pawn.h>
#include <Game.h>
#include <Board.h>

SaveGameEngine::SaveGameEngine(QFile *file) {
	stream = new QDataStream(file);
}

SaveGameEngine::~SaveGameEngine() {}

//Serializes to a file
void SaveGameEngine::serialize(QString filename, QVector<Pawn*> pawns, Board *board, Game *game) {
	QFile file(filename);
    file.open(QIODevice::WriteOnly);
    SaveGameEngine *save = new SaveGameEngine(&file);

    *(save->stream) << pawns.size(); //Save it for later use!
    for(auto p : pawns)
        p->serializeInto(save);
    board->serializeInto(save);
    game->serializeInto(save);

    delete save;
}

//Deserializes from a file
SaveGameEngine* SaveGameEngine::deserialize(QString filename) {
    QFile file(filename);
    file.open(QIODevice::ReadOnly);
    SaveGameEngine *save = new SaveGameEngine(&file);

	int pawn_n {};
	*(save->stream) >> pawn_n; //Retrive number of pawns
    for (int i = 0; i < pawn_n; i++)
        save->s_pawns.append(new Pawn(save));
    save->s_board = new Board(save);
    save->s_game = new Game(save);

    return save;
}

int SaveGameEngine::readInt() {
    int i {};
    *stream >> i;
    return i;
}

void SaveGameEngine::writeInt(int i) {
    *stream << i;
}

qreal SaveGameEngine::readReal() {
    qreal r;
    *stream >> r;
    return r;
}

void SaveGameEngine::writeReal(qreal r) {
    *stream << r;
}

Game* SaveGameEngine::getGame() {
    return s_game;
}

Board* SaveGameEngine::getBoard() {
    return SaveGameEngine::s_board;
}

QVector<Pawn*> SaveGameEngine::getPawns() {
    return s_pawns;
}
