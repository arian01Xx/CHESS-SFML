#ifndef DOMAIN_HPP
#define DOMAIN_HPP

#include <SFML/Window/Event.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>

#include "global.hpp"

struct DOMAIN{
    std::vector<std::vector<int>> board={
        500, std::vector<int>(500,0)
    };

    int row=board.size();
    int col=board[0].size();

    void createTable(sf::RenderWindow& window){
        for(int i=0; i<8; i++){
            for(int j=0; j<8; j++){
                if((i+j)%2==0) createBottom(window, j, i, sf::Color::White);
                else createBottom(window, j, i, sf::Color(0,100,0));
            }
        }
    }

    void createBottom(sf::RenderWindow& window, int c, int r, sf::Color x){
        sf::RectangleShape bottom(sf::Vector2f(CELL, CELL));
        bottom.setFillColor(x);
        bottom.setPosition(sf::Vector2f(COLUMN_SCREEN+CELL*c, ROW_SCREEN+CELL*r));

        Box _box;
        _box.box=bottom;
        _box.color=x;
        _box.coords={c,r};

        Boxes.push_back(_box);

        window.draw(bottom);
    }

    sf::Sprite scale(sf::Texture& texture, int& j, int& i){
        sf::Sprite _piece(texture);
        sf::Vector2u sizePiece=texture.getSize();

        float scaleX=static_cast<float>(CELL)/sizePiece.x;
        float scaleY=static_cast<float>(CELL)/sizePiece.y;

        _piece.setScale({scaleX, scaleY});
        _piece.setPosition(sf::Vector2f(COLUMN_SCREEN+CELL*j, ROW_SCREEN+CELL*i));

        return _piece;
    }

    bool insideBoard(int r, int c){
        return r>=0 && r<8 &&
               c>=0 && c<8;
    }

    void initTexturePawn(){
        for(int i=0; i<8; i++){ 
            sf::Texture PawnW; //[6][0->8]
            if(!PawnW.loadFromFile("piezas/pawnW.png")){
                std::cerr<<"FAIL OPEN PAWN WHITE"<<std::endl;
            }

            TEXTURE_PAWN.push_back(PawnW);
        }

        for(int i=0; i<8; i++){
            sf::Texture PawnB; //[1][0->8]
            if(!PawnB.loadFromFile("piezas/pawnB.png")){
                std::cerr<<"FAIL OPEN PAWN BLACK"<<std::endl;
            }

            TEXTURE_PAWN.push_back(PawnB);
        }
    }

    void initTextureQueen(){
        sf::Texture QueenW; //[7][3]
        if(!QueenW.loadFromFile("piezas/queenW.png")){
            std::cerr<<"FAIL OPEN KING WHITE"<<std::endl;
        }

        TEXTURE_QUEEN.push_back(QueenW);

        sf::Texture QueenB; //[0][3]
        if(!QueenB.loadFromFile("piezas/queenB.png")){
            std::cerr<<"FAIL OPEN KING BLACK"<<std::endl;
        }

        TEXTURE_QUEEN.push_back(QueenB);
    }

    void initTextureKing(){
        sf::Texture KingW; //[7][4]
        if(!KingW.loadFromFile("piezas/kingW.png")){
            std::cerr<<"FAIL OPEN KING WHITE"<<std::endl;
        }

        TEXTURE_KING.push_back(KingW);

        sf::Texture KingB; //[0][4]
        if(!KingB.loadFromFile("piezas/kingB.png")){
            std::cerr<<"FAIL OPEN KING BLACK"<<std::endl;
        }

        TEXTURE_KING.push_back(KingB);
    }

    void initTextureAlfil(){
        sf::Texture AlfilW; //[7][2]
        if(!AlfilW.loadFromFile("piezas/alfilW.png")){
            std::cerr<<"FAIL OPEN ALFIL WHITE"<<std::endl;
        }

        TEXTURE_ALFIL.push_back(AlfilW);

        sf::Texture AlfilW1; //[7][5]
        if(!AlfilW1.loadFromFile("piezas/alfilW.png")){
            std::cerr<<"FAIL OPEN ALFIL WHITE"<<std::endl;
        }

        TEXTURE_ALFIL.push_back(AlfilW1);

        sf::Texture AlfilB; //[0][2]
        if(!AlfilB.loadFromFile("piezas/alfilB.png")){
            std::cerr<<"FAIL OPEN ALFIL BLACK"<<std::endl;
        }

        TEXTURE_ALFIL.push_back(AlfilB);

        sf::Texture AlfilB1; //[0][5]
        if(!AlfilB1.loadFromFile("piezas/alfilB.png")){
            std::cerr<<"FAIL OPEN ALFIL BLACK"<<std::endl;
        }

        TEXTURE_ALFIL.push_back(AlfilB1);
    }

    void initTextureTower(){
        sf::Texture TowerW; //[7][0]
        if(!TowerW.loadFromFile("piezas/towerW.png")){
            std::cerr<<"FAIL OPEN TOWER WHITE"<<std::endl;
        }

        TEXTURE_TOWER.push_back(TowerW);

        sf::Texture TowerW1; //[7][7]
        if(!TowerW1.loadFromFile("piezas/towerW.png")){
            std::cerr<<"FAIL OPEN TOWER WHITE"<<std::endl;
        }

        TEXTURE_TOWER.push_back(TowerW1);

        sf::Texture TowerB; //[0][0]
        if(!TowerB.loadFromFile("piezas/towerB.png")){
            std::cerr<<"FAIL OPEN TOWER BLACK"<<std::endl;
        }

        TEXTURE_TOWER.push_back(TowerB);

        sf::Texture TowerB1; //[0][7]
        if(!TowerB1.loadFromFile("piezas/towerB.png")){
            std::cerr<<"FAIL OPEN TOWER BLACK"<<std::endl;
        }

        TEXTURE_TOWER.push_back(TowerB1);
    }

    void initTextureHorse(){
        sf::Texture HorseW; //INDICE 0 abajo izquierda [7][1]
        if(!HorseW.loadFromFile("piezas/horseW.png")){
            std::cerr<<"FAIL OPEN HORSE WHITE"<<std::endl;
        }

        TEXTURE_HORSE.push_back(HorseW);

        sf::Texture HorseW1; //INDICE 1 abajo derecha [7][6]
        if(!HorseW1.loadFromFile("piezas/horseW.png")){
            std::cerr<<"FAIL OPEN HORSE WHITE"<<std::endl;
        }

        TEXTURE_HORSE.push_back(HorseW1);

        sf::Texture HorseB; //INDICE 2 arriba izquierda [0][1]
        if(!HorseB.loadFromFile("piezas/horseB.png")){
            std::cerr<<"FAIL OPEN HORSE BLACK"<<std::endl;
        }

        TEXTURE_HORSE.push_back(HorseB); 

        sf::Texture HorseB1; //INDICE 3 arriba derecha [0][6]
        if(!HorseB1.loadFromFile("piezas/horseB.png")){
            std::cerr<<"FAIL OPEN HORSE BLACK"<<std::endl;
        }

        TEXTURE_HORSE.push_back(HorseB1);
    }
};

#endif
