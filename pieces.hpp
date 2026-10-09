#ifndef PIECES_HPP
#define PIECES_HPP

#include <SFML/Window/Event.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <utility>
#include <cassert>
#include <vector>

#include "domain.hpp"
#include "global.hpp"

struct PIECE{
    int x, y; //fila - columna
    bool team;
    bool alive=true;
    sf::Texture* image;

    std::pair<int,int> prePos; //row - col
    std::pair<int,int> actualPos; //row - col puedo usar newCoords normalmente
    
    PIECE(int _x, int _y, sf::Texture& _image, bool _team): x(_x), y(_y), 
                                                           image(&_image), team(_team) {} 

    sf::Sprite init(DOMAIN& D){
        return D.scale(*image,y,x); //SFML procesa en -> columna - fila
    }

    void paint(sf::RenderWindow& window, sf::Sprite figure){
        window.draw(figure);
    }
};

struct TOWER: PIECE{
    TOWER(int& _row, int& _col, sf::Texture& image, bool _team):
        PIECE(_row, _col, image, _team){
            actualPos.first=_row;
            actualPos.second=_col;
    } 

    void inspect(){
        int _i=newCoords.first;
        int _j=newCoords.second;

        //ABAJO
        for(int r=_i+1; r<8; ++r){
            if(board[r][_j]==Board::NONE){
                possiblesMoving.push_back({r,_j});
            }else{
                if(board[r][_j]!=(team ? Board::WHITE : Board::BLACK))
                    possiblesMoving.push_back({r, _j}); //captura enemiga
                break;
            }
        }

        //ARRIBA
        for(int r=_i-1; r>=0; --r){
            if(board[r][_j]==Board::NONE){
                possiblesMoving.push_back({r,_j});
            }else{
                if(board[r][_j]!=(team ? Board::WHITE : Board::BLACK))
                    possiblesMoving.push_back({r, _j}); //captura enemiga
                break;
            }
        }

        //DERECHA
        for(int c = _j + 1; c < 8; ++c){
            if(board[_i][c] == Board::NONE){
                possiblesMoving.push_back({_i, c});
            }else{
                if(board[_i][c] != (team ? Board::WHITE : Board::BLACK))
                    possiblesMoving.push_back({_i, c});
                break;
            }
        }

        //IZQUIERDA
        for(int c = _j - 1; c >= 0; --c){
            if(board[_i][c] == Board::NONE){
                possiblesMoving.push_back({_i, c});
            }else{
                if(board[_i][c] != (team ? Board::WHITE : Board::BLACK))
                    possiblesMoving.push_back({_i, c});
                break;
            }
        }
    }
};

struct HORSE: PIECE{
    std::vector<int> inspect_row={2,1,-1,-2,-2,-1,1,2};
    std::vector<int> inspect_col={-1,-2,-2,-1,1,2,2,1};

    HORSE(int& _row, int& _col, sf::Texture& image, bool _team): 
        PIECE(_row, _col, image,_team){
            actualPos.first=_row;
            actualPos.second=_col;
    }

    void inspect(){
        for(int i=0; i<8; i++){
            int _x=newCoords.first+inspect_row[i];
            int _y=newCoords.second+inspect_col[i];
            
            if(team){ //de esta forma tambien considerarà a los enemigos

                if(_x>=0 && _x<8 && _y>=0 && _y<8 &&
                   (board[_x][_y]==Board::NONE ||
                    board[_x][_y]==Board::BLACK)){
                    possiblesMoving.push_back({_x,_y});
                }else continue;
            }else if(!team){

                if(_x>=0 && _x<8 && _y>=0 && _y<8 &&
                   (board[_x][_y]==Board::NONE ||
                    board[_x][_y]==Board::WHITE)){
                    possiblesMoving.push_back({_x,_y});
                }else continue;
            }
        }
    }
};

void initSpriteTower(DOMAIN& D){ //2
    int seven=7, zero=0;
    TOWER TW(seven, zero, TEXTURE_TOWER[0], true);
    board1[seven][zero]=PIB::_TOWER;
    board[seven][zero]=Board::WHITE;
    sf::Sprite TowerWhite=TW.init(D);

    OBJ_TOWER.push_back(TW);
    SPRITE_TOWER.push_back(TowerWhite);

    TOWER TW1(seven, seven, TEXTURE_TOWER[1], true);
    board1[seven][seven]=PIB::_TOWER;
    board[seven][seven]=Board::WHITE;
    sf::Sprite TowerWhite1=TW1.init(D);

    OBJ_TOWER.push_back(TW1);
    SPRITE_TOWER.push_back(TowerWhite1);

    TOWER TB(zero, zero, TEXTURE_TOWER[2], false);
    board1[zero][zero]=PIB::_TOWER;
    board[zero][zero]=Board::BLACK;
    sf::Sprite TowerBlack=TB.init(D);

    OBJ_TOWER.push_back(TB);
    SPRITE_TOWER.push_back(TowerBlack);

    TOWER TB1(zero, seven, TEXTURE_TOWER[3], false);
    board1[zero][seven]=PIB::_TOWER;
    board[zero][seven]=Board::BLACK;
    sf::Sprite TowerBlack1=TB1.init(D);

    OBJ_TOWER.push_back(TB1);
    SPRITE_TOWER.push_back(TowerBlack1);
}

void initSpriteHorse(DOMAIN& D){ //1
    int seven=7, one=1;
    HORSE HW(seven, one, TEXTURE_HORSE[0], true);
    board1[seven][one]=PIB::_HORSE;
    board[seven][one]=Board::WHITE;
    sf::Sprite HorseWhite=HW.init(D);

    OBJ_HORSE.push_back(HW);
    SPRITE_HORSE.push_back(HorseWhite);

    int six=6;
    HORSE HW1(seven, six, TEXTURE_HORSE[1], true);
    board1[seven][six]=PIB::_HORSE;
    board[seven][six]=Board::WHITE;
    sf::Sprite HorseWhite1=HW1.init(D);

    OBJ_HORSE.push_back(HW1);
    SPRITE_HORSE.push_back(HorseWhite1);

    //0-1
    int zero=0;
    HORSE HB(zero, one, TEXTURE_HORSE[2], false);
    board1[zero][one]=PIB::_HORSE;
    board[zero][one]=Board::BLACK;
    sf::Sprite HorseBlack=HB.init(D);

    OBJ_HORSE.push_back(HB);
    SPRITE_HORSE.push_back(HorseBlack);

    //0-6
    HORSE HB1(zero, six, TEXTURE_HORSE[3], false);
    board1[zero][six]=PIB::_HORSE;
    board[zero][six]=Board::BLACK;
    sf::Sprite HorseBlack1=HB1.init(D);

    OBJ_HORSE.push_back(HB1);
    SPRITE_HORSE.push_back(HorseBlack1);
}

#endif
