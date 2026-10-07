#include <SFML/Window/Event.hpp>
#include <SFML/Graphics.hpp>
#include <algorithm>
#include <iostream>
#include <utility>
#include <cassert>
#include <vector>

#include "domain.hpp"
#include "global.hpp"
#include "pieces.hpp"

/*
 __FUNCTION__
 __LINE__ estos dos para imprimir
 */

struct RULE{
    int pre_x, pre_y; //se usa newCoords vector de coordenadas convertidas
    int new_x, new_y;

    bool click(){
        if(board[newCoords.first][newCoords.second]==Board::WHITE ||
           board[newCoords.first][newCoords.second]==Board::BLACK ||
           board[newCoords.first][newCoords.second]==Board::NONE) return true;
        return false;
    }

    bool SelectedNewPosition(){
        for(auto& coord: possiblesMoving){ //row, col
            if(coord.first==newCoords.first && coord.second==newCoords.second){
                new_x=coord.first;
                new_y=coord.second;

                pre_x=pieceSelected.first;
                pre_y=pieceSelected.second;

                return true;
            }
        }

        return false;
    }

    //esta funcion deberia ir en RENDERING!!!!!
    void actualization(){
        for(int i=0; i<OBJ_HORSE.size(); i++){

            if(!OBJ_HORSE[i].alive) continue;

            if(OBJ_HORSE[i].x==pieceSelected.first &&
               OBJ_HORSE[i].y==pieceSelected.second){

                //buscar pieza enemiga en la casilla destino
                for(int j=0; j<OBJ_HORSE.size(); j++){
                    if(i==j) continue;
                    if(!OBJ_HORSE[j].alive) continue;
                    if(OBJ_HORSE[j].x==new_x &&
                       OBJ_HORSE[j].y==new_y &&
                       OBJ_HORSE[j].team!=OBJ_HORSE[i].team) OBJ_HORSE[j].alive=false;
                }

                board[pre_x][pre_y]=Board::NONE;

                OBJ_HORSE[i].x=new_x;
                OBJ_HORSE[i].y=new_y;

                OBJ_HORSE[i].prePos={pre_x, pre_y};
                OBJ_HORSE[i].actualPos={new_x, new_y};

                //aqui hay que tener cuidado para cuando se agreguen los negros
                if(OBJ_HORSE[i].team) board[new_x][new_y]=Board::WHITE;
                else board[new_x][new_y]=Board::BLACK;
                SPRITE_HORSE[i].setPosition(sf::Vector2f(250+CELL*new_y, 150+CELL*new_x));
            }
        }

        for(int k=0; k<OBJ_HORSE.size(); k++){
            PRINT << "HORSE[" << k << "] "
            << "x=" << OBJ_HORSE[k].x
            << " y=" << OBJ_HORSE[k].y
            << " team=" << OBJ_HORSE[k].team
            << " alive=" << OBJ_HORSE[k].alive
            << END;
        }

        possiblesMoving.clear();
    }

    void calculated(){
        int _row=newCoords.first;
        int _col=newCoords.second;

        for(auto& H: OBJ_HORSE){
            if(H.alive &&
               _row==H.actualPos.first && 
               _col==H.actualPos.second) H.inspect();
        }
    }
};

struct GAME{
    void Gaming(RULE& R){
        //NO HAY PIEZA SELECCIONADA
        if(possiblesMoving.empty()){
            
            if(R.click()){
                pieceSelected=newCoords;
                R.calculated();
            }
            return;
        }

        //YA HAY PIEZA SELECCIONADA
        //1. el click corresponde a un movimiento valido?
        if(R.SelectedNewPosition()){
            R.actualization();
            return;
        }

        //2. no es un movimiento valido
        //es otra pieza de mi equipo?
        if(board[newCoords.first][newCoords.second]==
           board[pieceSelected.first][pieceSelected.second]){
            //cancelar seleccion anterior
            possiblesMoving.clear();

            //seleccionar nueva pieza
            pieceSelected=newCoords;

            //calcular sus movimientos
            R.calculated();

            return;
        }

        //3. es NONE o una pieza enemiga pero no es movimiento valido
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
            b.setPosition(sf::Vector2f(250+CELL*_coord.second, 150+CELL*_coord.first));

            window.draw(b);
        }
    }
};

int main(){

    RULE R;
    GAME G;
    DOMAIN D;
    RENDERING RE;

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
        for(int i=0; i<OBJ_TOWER.size(); i++){ 
            if(OBJ_TOWER[i].alive) OBJ_TOWER[i].paint(window, SPRITE_TOWER[i]); 
        }
        for(int i=0; i<OBJ_HORSE.size(); i++){ 
            if(OBJ_HORSE[i].alive) OBJ_HORSE[i].paint(window, SPRITE_HORSE[i]); 
        }

        RE.coloredFuture(window);

        window.display();
    }

    return 0;
}
