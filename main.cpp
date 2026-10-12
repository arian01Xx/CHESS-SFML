#include <SFML/Window/Event.hpp>
#include <SFML/Graphics.hpp>
#include <algorithm>
#include <stdexcept>
#include <iostream>
#include <utility>
#include <cassert>
#include <string>
#include <vector>

#include "domain.hpp"
#include "global.hpp"
#include "pieces.hpp"
#include "rules.hpp"

/*
 __FUNCTION__
 __LINE__ estos dos para imprimir
 */

/*
 UI/
│   ├── TextRenderer.hpp
│   ├── SidePanel.hpp
│   └── Buttons.hpp
 */
struct TEXT_RENDERING{
    sf::Font font;

    sf::Text initTextureLetters(const std::string & letters, int row){
        if(!font.openFromFile("UI/main.ttf")){
            throw std::runtime_error("ERROR DE CARGA DE LA FUENTE");
        }

        sf::Text text(font);
        text.setString(letters);
        text.setCharacterSize(30);
        text.setFillColor(sf::Color(0,128,40));

        text.setPosition(sf::Vector2f(COL_TEXT, row));

        return text;
    }
};

struct GAME{
    void Gaming(RULE& R){
        //NO HAY PIEZA SELECCIONADA
        if(possiblesMoving.empty()){ 
            if(R.click()){
                if(board[newCoords.first][newCoords.second]==currentTurn){
                    pieceSelected=newCoords;
                    R.calculated();
                }
            }
            return;
        }

        if(R.SelectedNewPosition()){
            R.actualization();

            currentTurn=(currentTurn==Board::WHITE)
                ? Board::BLACK : Board::WHITE;

            return;
        }

        if(board[newCoords.first][newCoords.second]==
           board[pieceSelected.first][pieceSelected.second]){
            possiblesMoving.clear();
            pieceSelected=newCoords;
            R.calculated();

            return;
        }

        possiblesMoving.clear();
    }
};

struct RENDERING{
//se encarga de dibujar lo entregado por RULE

    void coloredFuture(sf::RenderWindow& window){
        sf::Color redTrans(255,0,0,170);

        if(possiblesMoving.empty()) return;

        for(auto& _coord: possiblesMoving){
            sf::RectangleShape b(sf::Vector2f(CELL, CELL));
            b.setFillColor(redTrans);
            b.setPosition(sf::Vector2f(COLUMN_SCREEN+CELL*_coord.second, ROW_SCREEN+CELL*_coord.first));

            window.draw(b);
        }
    }
};

int main(){

    RULE R;
    GAME G;
    DOMAIN D;
    RENDERING RE;

    TEXT_RENDERING TXR;

    sf::RenderWindow window{
        sf::VideoMode({
            static_cast<unsigned>(TILE*D.col),
            static_cast<unsigned>(TILE*D.row)
        }), "CHESS"
    };

    window.setFramerateLimit(50);

    /**********************************************/
    /*CREACION DEL TABLERO UNA VEZ*/
    for(int i=0; i<8; i++){
        for(int j=0; j<8; j++){
            if((i+j)%2==0) D.createBottom(window, j, i, sf::Color::White);
            else D.createBottom(window, j, i, sf::Color(0,100,0));
        }
    }
    /**********************************************/
    std::string textMain="NEURAL'S NETWORK";
    sf::Text Title=TXR.initTextureLetters(textMain, 20); //con eso ya deberia crearse un texto

    std::string textMain1="G.D.INTELECTUAL";
    sf::Text Title1=TXR.initTextureLetters(textMain1, 70);

    /**********************************************/
    D.initTexturePawn();
    initSpritePawn(D);

    D.initTextureQueen();
    initSpriteQueen(D);

    D.initTextureKing();
    initSpriteKing(D);

    D.initTextureAlfil();
    initSpriteAlfil(D);

    D.initTextureTower();
    initSpriteTower(D);

    D.initTextureHorse();
    initSpriteHorse(D);

    while(window.isOpen()){
        while(const std::optional event=window.pollEvent()){
            if(event->is<sf::Event::Closed>()) window.close();
            if(event->is<sf::Event::MouseButtonPressed>()){
                if(event->getIf<sf::Event::MouseButtonPressed>()->button==
                   sf::Mouse::Button::Left){
                    dragging=true;
                    lastMouse=sf::Mouse::getPosition(window);
                }
            }
            if(event->is<sf::Event::MouseButtonReleased>()){
                if(event->getIf<sf::Event::MouseButtonReleased>()->button==
                   sf::Mouse::Button::Left){
                    dragging=false;
                }
            }
            if(const auto* mouseButtonPressed=event->getIf<sf::Event::MouseButtonPressed>()){
                if(mouseButtonPressed->button==sf::Mouse::Button::Left){
                    col=mouseButtonPressed->position.x;
                    row=mouseButtonPressed->position.y;

                    PRINT<<"------------------------"<<END;
                    PRINT<<"COORDENADAS MOUSE"<<END;
                    PRINT<<"col: "<<col<<" - row: "<<row<<END;

                    newCoords=D.convertion(row, col);

                    PRINT<<"-----------------------"<<END;
                    PRINT<<"COORDENADAS ENTERIZADAS"<<END;
                    PRINT<<"col: "<<newCoords.second<<" - row: "<<newCoords.first<<END;

                    G.Gaming(R);
                }
            }
        }

        window.clear();

        for(auto& x: Boxes){ window.draw(x.box); }
        for(int i=0; i<OBJ_KING.size(); i++){ if(OBJ_KING[i].alive) OBJ_KING[i].paint(window, SPRITE_KING[i]); }
        for(int i=0; i<OBJ_ALFIL.size(); i++){ if(OBJ_ALFIL[i].alive) OBJ_ALFIL[i].paint(window, SPRITE_ALFIL[i]); }
        for(int i=0; i<OBJ_TOWER.size(); i++){ if(OBJ_TOWER[i].alive) OBJ_TOWER[i].paint(window, SPRITE_TOWER[i]); }
        for(int i=0; i<OBJ_HORSE.size(); i++){ if(OBJ_HORSE[i].alive) OBJ_HORSE[i].paint(window, SPRITE_HORSE[i]); }
        for(int i=0; i<OBJ_QUEEN.size(); i++){ if(OBJ_QUEEN[i].alive) OBJ_QUEEN[i].paint(window, SPRITE_QUEEN[i]); }
        for(int i=0; i<OBJ_PAWN.size(); i++){ if(OBJ_PAWN[i].alive) OBJ_PAWN[i].paint(window, SPRITE_PAWN[i]); }

        RE.coloredFuture(window);

        window.draw(Title);
        window.draw(Title1);

        window.display();
    }

    return 0;
}
