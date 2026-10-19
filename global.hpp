#pragma once

#define PRINT std::cout
#define END '\n'

#define ANCHURA 65.f
#define ALTURA 35.f

#define COL_TEXT 50

#define ROW_SCREEN 150
#define COLUMN_SCREEN 50

struct Box;
struct HORSE;
struct TOWER;
struct ALFIL;
struct KING;
struct QUEEN;
struct PAWN;

constexpr int TILE=6;
constexpr int CELL=TILE*10;

int col, row;
bool dragging=false;
sf::Vector2i lastMouse;
std::vector<Box> Boxes;

enum Board{ BLACK, WHITE, NONE };
enum PLAY{TICTACTOE, SNAKE, CHESS, PONG, NO_ELECTION};
enum PIB{ EMPTY, _HORSE, _ALFIL, _TOWER, _PAWN, _QUEEN, _KING };

Board currentTurn = Board::WHITE;
PLAY currentPlay = PLAY::CHESS;

struct Box{
    sf::RectangleShape box;
    sf::Color color;
    std::pair<int,int> coords;
};

struct CB{ 
    int x, y; //FILA COLUMNA
    CB(int _x, int _y): x(_x), y(_y) {}
};

std::vector<CB> CBs; //FILA - COLUMNA

std::pair<int,int> pieceSelected; //row - col

std::pair<int,int> newCoords; //row - col coordenadas convertidas
std::vector<std::pair<int,int>> possiblesMoving; //row - col
std::vector<std::vector<Board>> board(8, std::vector<Board>(8,Board::NONE));
std::vector<std::vector<PIB>> board1(8, std::vector<PIB>(8, PIB::EMPTY));

std::vector<sf::Texture> TEXTURE_PAWN;
std::vector<sf::Sprite> SPRITE_PAWN;
std::vector<PAWN> OBJ_PAWN;

std::vector<sf::Texture> TEXTURE_QUEEN;
std::vector<sf::Sprite> SPRITE_QUEEN;
std::vector<QUEEN> OBJ_QUEEN;

std::vector<sf::Texture> TEXTURE_KING;
std::vector<sf::Sprite> SPRITE_KING;
std::vector<KING> OBJ_KING;

std::vector<sf::Texture> TEXTURE_ALFIL;
std::vector<sf::Sprite> SPRITE_ALFIL;
std::vector<ALFIL> OBJ_ALFIL;

std::vector<sf::Texture> TEXTURE_HORSE;
std::vector<sf::Sprite> SPRITE_HORSE;
std::vector<HORSE> OBJ_HORSE;

std::vector<sf::Texture> TEXTURE_TOWER;
std::vector<sf::Sprite> SPRITE_TOWER;
std::vector<TOWER> OBJ_TOWER;

//CAJAS DE TEXTO
std::vector<sf::Text> textBoxes;
std::vector<sf::RectangleShape> BoxForText;
std::vector<std::pair<int,int>> coordBoxes;
