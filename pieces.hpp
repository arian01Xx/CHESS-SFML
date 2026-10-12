#ifndef PIECES_HPP
#define PIECES_HPP

#include <SFML/Window/Event.hpp>
#include <SFML/Graphics.hpp>
#include <algorithm>
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

struct PAWN: PIECE{
    int firstTime=0;

    std::vector<int> _x={-1,-2}; //FIRST TIME WHITE
    std::vector<int> _y={0,0};

    std::vector<int> _x2={1,2}; //FIRST TIME BLACK
    std::vector<int> _y2={0,0};

    PAWN(int& _row, int& _col, sf::Texture& image, bool _team):
        PIECE(_row, _col, image, _team){
            actualPos.first=_row;
            actualPos.second=_col;
    }

    void inspect(){
        if(team && firstTime==0){
            for(int i=0; i<2; i++){
                int temp_x=newCoords.first+_x[i];
                int temp_y=newCoords.second+_y[i];

                if(temp_x>=0 && temp_x<8 && temp_y>=0 && temp_y<8 &&
                   board[temp_x][temp_y]==Board::NONE)
                    possiblesMoving.push_back({temp_x, temp_y});
            }
        }else if(team && firstTime!=0){
            int temp_x=newCoords.first-1;
            int temp_y=newCoords.second+1;

            if(temp_x>=0 && temp_x<8 && 
               temp_y>=0 && temp_y<8 &&
               board[temp_x][temp_y]==Board::BLACK) 
                possiblesMoving.push_back({temp_x, temp_y});

            int temp_x2=newCoords.first-1;
            int temp_y2=newCoords.second;

            if(temp_x2>=0 && temp_x2<8 && 
               temp_y2>=0 && temp_y2<8 &&
               board[temp_x2][temp_y2]==Board::NONE)
                possiblesMoving.push_back({temp_x2, temp_y2});

            int temp_x3=newCoords.first-1;
            int temp_y3=newCoords.second-1;

            if(temp_x3>=0 && temp_x3<8 && 
               temp_y3>=0 && temp_y3<8 &&
               board[temp_x3][temp_y3]==Board::BLACK)
                possiblesMoving.push_back({temp_x3, temp_y3});
        }

        if(!team && firstTime==0){
            for(int i=0; i<2; i++){
                int temp_x2=newCoords.first+_x2[i];
                int temp_y2=newCoords.second+_y2[i];

                if(temp_x2>=0 && temp_x2<8 && temp_y2>=0 && temp_y2<8 &&
                   board[temp_x2][temp_y2]==Board::NONE)
                    possiblesMoving.push_back({temp_x2, temp_y2});
            }
        }else if(!team && firstTime!=0){
            int temp_x=newCoords.first+1;
            int temp_y=newCoords.second+1;

            if(temp_x>=0 && temp_x<8 &&
               temp_y>=0 && temp_y<8 &&
               board[temp_x][temp_y]==Board::WHITE)
                possiblesMoving.push_back({temp_x, temp_y});

            int temp_x2=newCoords.first+1;
            int temp_y2=newCoords.second;

            if(temp_x2>=0 && temp_x2<8 &&
               temp_y2>=0 && temp_y2<8 &&
               board[temp_x2][temp_y2]==Board::NONE)
                possiblesMoving.push_back({temp_x2, temp_y2});

            int temp_x3=newCoords.first+1;
            int temp_y3=newCoords.second-1;

            if(temp_x3>=0 && temp_x3<8 &&
               temp_y3>=0 && temp_y3<8 &&
               board[temp_x3][temp_y3]==Board::WHITE)
                possiblesMoving.push_back({temp_x3, temp_y3});
        }
    }
};

struct QUEEN: PIECE{
    std::vector<int> R={-1,-1,-1,0,0,1,1,1};
    std::vector<int> C={-1,0,1,-1,1,-1,0,1};

    QUEEN(int& _row, int& _col, sf::Texture& image, bool _team):
        PIECE(_row, _col, image, _team){
            actualPos.first=_row;
            actualPos.second=_col;
    }

    void inspectTower(){
        int _i=newCoords.first;
        int _j=newCoords.second;

        //ABAJO
        for(int r=_i+1; r<8; ++r){
            if(board[r][_j]==Board::NONE){
                possiblesMoving.push_back({r,_j});
            }else{
                if(board[r][_j]!=(team ? Board::WHITE : Board::BLACK)){
                    std::pair<int,int> pos{r,_j};
                    add(pos);
                }
                break;
            }
        }

        //ARRIBA
        for(int r=_i-1; r>=0; --r){
            if(board[r][_j]==Board::NONE){
                possiblesMoving.push_back({r,_j});
            }else{
                if(board[r][_j]!=(team ? Board::WHITE : Board::BLACK)){
                    std::pair<int,int> pos{r,_j};
                    add(pos);
                }
                break;
            }
        }

        //DERECHA
        for(int c = _j + 1; c < 8; ++c){
            if(board[_i][c] == Board::NONE){
                possiblesMoving.push_back({_i, c});
            }else{
                if(board[_i][c] != (team ? Board::WHITE : Board::BLACK)){
                    std::pair<int,int> pos{_i,c};
                    add(pos);
                }
                break;
            }
        }

        //IZQUIERDA
        for(int c = _j - 1; c >= 0; --c){
            if(board[_i][c] == Board::NONE){
                possiblesMoving.push_back({_i, c});
            }else{
                if(board[_i][c] != (team ? Board::WHITE : Board::BLACK)){
                    std::pair<int,int> pos{_i,c};
                    add(pos);
                }
                break;
            }
        }

    }

    void add(std::pair<int,int>& pos){
        if(std::find(possiblesMoving.begin(),
                     possiblesMoving.end(),
                     pos)==possiblesMoving.end())
            possiblesMoving.push_back(pos);
    }

    void inspectKing(){
        for(int i=0; i<8; i++){
            int new_x=newCoords.first+R[i];
            int new_y=newCoords.second+C[i];

            if(new_x>=0 && new_x<8 && 
               new_y>=0 && new_y<8){
                if(board[new_x][new_y]==Board::NONE){
                    possiblesMoving.push_back({new_x, new_y});
                    //continue;
                }
                if(board[new_x][new_y]!=(team ? Board::WHITE: Board::BLACK)){
                    std::pair<int,int> pos{new_x,new_y};

                    if(std::find(possiblesMoving.begin(),
                                 possiblesMoving.end(),
                                 pos)==possiblesMoving.end())
                        possiblesMoving.push_back(pos);
                }
            }
        }
    }

    void inspectAlfil(){
        constexpr int directions[4][2]={
            {1,1},
            {-1,-1},
            {-1,1},
            {1,-1}
        };

        int row=newCoords.first;
        int col=newCoords.second;

        for(const auto& dir: directions){
            int dr=dir[0];
            int dc=dir[1];

            for(int i=1; i<8; ++i){
                int r=row+dr*i;
                int c=col+dc*i;

                if(r<0 || r>=8 || c<0 || c>=8) break;
                if(board[r][c]==Board::NONE){
                    std::pair<int,int> pos{r,c};

                    if(std::find(possiblesMoving.begin(),
                                 possiblesMoving.end(),
                                 pos)==possiblesMoving.end())
                        possiblesMoving.push_back(pos);
                    continue;
                }

                if(board[r][c]!=(team ? Board::WHITE: Board::BLACK)){
                    std::pair<int,int> pos{r,c};

                    if(std::find(possiblesMoving.begin(),
                                 possiblesMoving.end(),
                                 pos)==possiblesMoving.end())
                        possiblesMoving.push_back(pos);
                }
                break;
            }
        }
    }

    void inspect(){
        //MOVIMIENTOS DE LA TORRE, DEL REY Y DEL ALFIL
        inspectTower();
        inspectKing();
        inspectAlfil();
    }
};

struct KING: PIECE{
    std::vector<int> R={-1,-1,-1,0,0,1,1,1};
    std::vector<int> C={-1,0,1,-1,1,-1,0,1};

    KING(int& _row, int& _col, sf::Texture& image, bool _team):
        PIECE(_row, _col, image, _team){
            actualPos.first=_row;
            actualPos.second=_col;
    }

    void inspect(){
        for(int i=0; i<8; i++){
            int new_x=newCoords.first+R[i];
            int new_y=newCoords.second+C[i];

            if(new_x>=0 && new_x<8 && 
               new_y>=0 && new_y<8){
                if(board[new_x][new_y]==Board::NONE){
                    possiblesMoving.push_back({new_x, new_y});
                    //continue;
                }
                if(board[new_x][new_y]!=(team ? Board::WHITE: Board::BLACK))
                    possiblesMoving.push_back({new_x, new_y});
            }
        }
    }
};

struct ALFIL: PIECE{
    ALFIL(int& _row, int& _col, sf::Texture& image, bool _team): 
        PIECE(_row, _col, image, _team){
            actualPos.first=_row;
            actualPos.second=_col;
    }

    void inspect(){
        constexpr int directions[4][2]={
            {1,1},
            {-1,-1},
            {-1,1},
            {1,-1}
        };

        int row=newCoords.first;
        int col=newCoords.second;

        for(const auto& dir: directions){
            int dr=dir[0];
            int dc=dir[1];

            for(int i=1; i<8; ++i){
                int r=row+dr*i;
                int c=col+dc*i;

                if(r<0 || r>=8 || c<0 || c>=8) break;
                if(board[r][c]==Board::NONE){
                    possiblesMoving.push_back({r,c});
                    continue;
                }

                if(board[r][c]!=(team ? Board::WHITE: Board::BLACK))
                    possiblesMoving.push_back({r,c});
                break;
            }
        }
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

void initSpritePawn(DOMAIN& D){
    int six=6, one=1;

    for(int i=0; i<8; i++){
        PAWN PW(six, i, TEXTURE_PAWN[i], true);
        board1[six][i]=PIB::_PAWN;
        board[six][i]=Board::WHITE;
        sf::Sprite PawnWhite=PW.init(D);

        OBJ_PAWN.push_back(PW);
        SPRITE_PAWN.push_back(PawnWhite);
    }

    for(int i=0; i<8; i++){
        PAWN PB(one, i, TEXTURE_PAWN[8+i], false);
        board1[one][i]=PIB::_PAWN;
        board[one][i]=Board::BLACK;
        sf::Sprite PawnBlack=PB.init(D);

        OBJ_PAWN.push_back(PB);
        SPRITE_PAWN.push_back(PawnBlack);
    }
}

void initSpriteQueen(DOMAIN& D){
    int seven=7, three=3, zero=0;

    QUEEN QW(seven, three, TEXTURE_QUEEN[0], true);
    board1[seven][three]=PIB::_QUEEN;
    board[seven][three]=Board::WHITE;
    sf::Sprite QueenWhite=QW.init(D);

    OBJ_QUEEN.push_back(QW);
    SPRITE_QUEEN.push_back(QueenWhite);

    QUEEN QB(zero, three, TEXTURE_QUEEN[1], false);
    board1[zero][three]=PIB::_QUEEN;
    board[zero][three]=Board::WHITE;
    sf::Sprite QueenBlack=QB.init(D);

    OBJ_QUEEN.push_back(QB);
    SPRITE_QUEEN.push_back(QueenBlack);
}

void initSpriteKing(DOMAIN& D){
    int seven=7, four=4, zero=0;

    KING KW(seven, four, TEXTURE_KING[0], true);
    board1[seven][four]=PIB::_KING;
    board[seven][four]=Board::WHITE;
    sf::Sprite KingWhite=KW.init(D);

    OBJ_KING.push_back(KW);
    SPRITE_KING.push_back(KingWhite);

    KING KB(zero, four, TEXTURE_KING[1], false);
    board1[zero][four]=PIB::_KING;
    board[zero][four]=Board::BLACK;
    sf::Sprite KingBlack=KB.init(D);

    OBJ_KING.push_back(KB);
    SPRITE_KING.push_back(KingBlack);
}

void initSpriteAlfil(DOMAIN& D){
    int seven=7, zero=0;
    int two=2, five=5;

    ALFIL AW(seven, two, TEXTURE_ALFIL[0], true);
    board1[seven][two]=PIB::_ALFIL;
    board[seven][two]=Board::WHITE;
    sf::Sprite AlfilWhite=AW.init(D);

    OBJ_ALFIL.push_back(AW);
    SPRITE_ALFIL.push_back(AlfilWhite);

    ALFIL AW1(seven, five, TEXTURE_ALFIL[1], true);
    board1[seven][five]=PIB::_ALFIL;
    board[seven][five]=Board::WHITE;
    sf::Sprite AlfilWhite1=AW1.init(D);

    OBJ_ALFIL.push_back(AW1);
    SPRITE_ALFIL.push_back(AlfilWhite1);

    ALFIL AB(zero, two, TEXTURE_ALFIL[2], false);
    board1[zero][two]=PIB::_ALFIL;
    board[zero][two]=Board::BLACK;
    sf::Sprite AlfilBlack=AB.init(D);

    OBJ_ALFIL.push_back(AB);
    SPRITE_ALFIL.push_back(AlfilBlack);

    ALFIL AB1(zero, five, TEXTURE_ALFIL[3], false);
    board1[zero][five]=PIB::_ALFIL;
    board[zero][five]=Board::BLACK;
    sf::Sprite AlfilBlack1=AB1.init(D);

    OBJ_ALFIL.push_back(AB1);
    SPRITE_ALFIL.push_back(AlfilBlack1);
}

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
