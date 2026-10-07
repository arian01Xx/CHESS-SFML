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

    void createBottom(sf::RenderWindow& window, int c, int r, sf::Color x){
        sf::RectangleShape bottom(sf::Vector2f(CELL, CELL));
        bottom.setFillColor(x);
        bottom.setPosition(sf::Vector2f(250+CELL*c, 150+CELL*r));

        Box _box;
        _box.box=bottom;
        _box.color=x;
        _box.coords={c,r};
        Boxes.push_back(_box);

        window.draw(bottom);

        window.draw(bottom);
    }

    sf::Sprite scale(sf::Texture& texture, int& j, int& i){
        sf::Sprite _piece(texture);
        sf::Vector2u sizePiece=texture.getSize();

        float scaleX=static_cast<float>(CELL)/sizePiece.x;
        float scaleY=static_cast<float>(CELL)/sizePiece.y;

        _piece.setScale({scaleX, scaleY});
        _piece.setPosition(sf::Vector2f(250+CELL*j, 150+CELL*i));

        return _piece;
    }

    std::pair<int,int> convertion(int& row, int& col){
        int r=(row-150)/CELL;
        int c=(col-250)/CELL;
        return {r,c};
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
