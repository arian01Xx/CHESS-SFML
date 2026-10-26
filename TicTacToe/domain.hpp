#ifndef DOMAIN_TTT_HPP
#define DOMAIN_TTT_HPP

#include <SFML/Window/Event.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>

#include "../global.hpp"

namespace TTT{
    struct DOMAIN{
        std::vector<std::vector<int>> board={
           500, std::vector<int>(500,0)
        };

        int row=board.size();
        int col=board[0].size();

        void createTable(sf::RenderWindow& window){
            for(int i=0; i<3; i++){
                for(int j=0; j<3; j++){
                    if((i+j)%2==0) createBottom(window, j, i, sf::Color::White);
                    else createBottom(window, j, i, sf::Color(128,0,128));
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

            BoxesTTT.push_back(_box);

            window.draw(bottom);
        }

        bool insideBoard(int r, int c){
            return r>=0 && r<3 &&
                   c>=0 && c<8;
        }

        void initTextureEquis(){
            sf::Texture Equis;
            if(!Equis.loadFromFile("equis.png")){
                std::cerr<<"FAIL OPEN EQUIS"<<std::endl;
            }

            TEXTURE_EQUIS.push_back(Equis);
        }

        void initTextureCircle(){
            sf::Texture Circle;
            if(!Circle.loadFromFile("circulo.png")){
                std::cerr<<"FAIL OPEN CIRCULO"<<std::endl;
            }

            TEXTURE_CIRCLE.push_back(Circle);
        }
    };

    struct PIECE{
        int x, y;
        sf::Texture* image;

    };

    struct EQUIS: TTT::PIECE{

    };

    struct CIRCLE: TTT::PIECE{

    };

    struct RULES{
        bool click(){
            if(boardTTT[newCoords.first][newCoords.second]==BoardTTT::ANY) return true;
            else if(boardTTT[newCoords.first][newCoords.second]==BoardTTT::CIRCLE ||
                    boardTTT[newCoords.first][newCoords.second]==BoardTTT::EQUIS)
                return false;
        }
    };
}

#endif
