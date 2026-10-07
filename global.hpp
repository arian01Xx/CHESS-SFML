#pragma once

#define PRINT std::cout
#define END '\n'

struct Box;
struct HORSE;
struct TOWER;
constexpr int TILE=6;
constexpr int CELL=TILE*10;

int col, row;
bool dragging=false;
sf::Vector2i lastMouse;
std::vector<Box> Boxes;

enum Board{
    BLACK, WHITE, NONE
};

struct Box{
    sf::RectangleShape box;
    sf::Color color;
    std::pair<int,int> coords;
};

std::pair<int,int> pieceSelected; //row - col

std::pair<int,int> newCoords; //row - col coordenadas convertidas
std::vector<std::pair<int,int>> possiblesMoving; //row - col
std::vector<std::vector<Board>> board(8, std::vector<Board>(8,Board::NONE));

std::vector<sf::Texture> TEXTURE_HORSE;
std::vector<sf::Sprite> SPRITE_HORSE;
std::vector<HORSE> OBJ_HORSE;

std::vector<sf::Texture> TEXTURE_TOWER;
std::vector<sf::Sprite> SPRITE_TOWER;
std::vector<TOWER> OBJ_TOWER;
