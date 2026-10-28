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
            return r>=0 && r<3 &&
                   c>=0 && c<3;
        }

        void initSpriteEquis(){
            sf::Texture Equis;
            if(!Equis.loadFromFile("TicTacToe/equis.png")){
                std::cerr<<"FAIL OPEN EQUIS"<<std::endl;
            }

            TEXTURE_EQUIS.push_back(Equis);
        }

        void initSpriteCircle(){
            sf::Texture Circle;
            if(!Circle.loadFromFile("TicTacToe/circulo.png")){
                std::cerr<<"FAIL OPEN CIRCULO"<<std::endl;
            }

            TEXTURE_CIRCLE.push_back(Circle);
        }

        void initEquis(){
            int row=newCoords.first;
            int col=newCoords.second;

            boardTTT[row][col]=BoardTTT::EQUIS;

            sf::Sprite EquiSprite=scale(TEXTURE_EQUIS.back(), col, row);
            SPRITE_EQUIS.push_back(EquiSprite);
        }

        void initCircle(){
            int row=newCoords.first;
            int col=newCoords.second;

            boardTTT[row][col]=BoardTTT::CIRCLE;

            sf::Sprite CircleSprite=scale(TEXTURE_CIRCLE.back(), col, row);
            SPRITE_CIRCLE.push_back(CircleSprite);
        }
    }; 

    struct RULES{
        bool team=true;

        bool click(){
            if(boardTTT[newCoords.first][newCoords.second]==BoardTTT::ANY) return true;
            return false;
        }

        bool emptySpace(){
            if(boardTTT[newCoords.first][newCoords.second]==BoardTTT::ANY) return true;
            return false;
        }

        bool rowWinning(int& x){
            auto user=boardTTT[newCoords.first][newCoords.second];

            for(int i=0; i<3; i++){
                if(user!=BoardTTT[x][i]) return false;
            }

            return true;
        }

        bool colWinning(int& y){

        }

        bool diagLeft(){

        }

        bool diagRight(){

        }

        void findWinning(){
            //aqui se trabaja con BoardTTT
            //en cada coordenada que inspeccione tanto fila, columna o diagonal
            if(rowWinning(newCoords.first) ||
               colWinning(newCoords.second) ||
               diagLeft() || diagRight()){
                //insertar imagen de felicidades al equipo ganador
            }
        }
    };
}

#endif
