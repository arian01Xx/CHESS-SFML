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

    void actualization(){
        if(board1[pieceSelected.first][pieceSelected.second]==PIB::_HORSE) actualizationHorse();
        else if(board1[pieceSelected.first][pieceSelected.second]==PIB::_TOWER) actualizationTower();
    }
    void actualizationTower(){
        for(int i=0; i<OBJ_TOWER.size(); i++){

            if(!OBJ_TOWER[i].alive) continue;

            if(OBJ_TOWER[i].x==pieceSelected.first &&
               OBJ_TOWER[i].y==pieceSelected.second){

                //buscar pieza enemiga en la casilla destino
                if(board1[new_x][new_y]==PIB::_TOWER){
                    for(auto& T: OBJ_TOWER){
                        if(!T.alive) continue;
                        
                        if(T.x==new_x && T.y==new_y){
                            if(T.team==OBJ_TOWER[i].team) return;
                            T.alive=false;
                            break;
                        }
                    }
                }else if(board1[new_x][new_y]==PIB::_HORSE){
                    for(std::size_t j=0; j<OBJ_HORSE.size(); ++j){
                        if(j==static_cast<std::size_t>(i)) continue;

                        auto& H=OBJ_HORSE[j];
                        if(!H.alive) continue;

                        if(H.x==new_x && H.y==new_y){
                            if(H.team==OBJ_TOWER[i].team) return;

                            H.alive=false;
                            break;
                        }
                    }
                }  

                board1[pre_x][pre_y]=PIB::EMPTY;
                board[pre_x][pre_y]=Board::NONE;

                OBJ_TOWER[i].x=new_x;
                OBJ_TOWER[i].y=new_y;

                OBJ_TOWER[i].prePos={pre_x, pre_y};
                OBJ_TOWER[i].actualPos={new_x, new_y};

                if(OBJ_TOWER[i].team){
                    board[new_x][new_y]=Board::WHITE;
                    board1[new_x][new_y]=PIB::_TOWER;
                }else{
                    board[new_x][new_y]=Board::BLACK;
                    board1[new_x][new_y]=PIB::_TOWER;
                }
                
                SPRITE_TOWER[i].setPosition(sf::Vector2f(250+CELL*new_y, 150+CELL*new_x));
            }
        }

        possiblesMoving.clear();
    }

    void actualizationHorse(){
        for(int i=0; i<OBJ_HORSE.size(); i++){

            if(!OBJ_HORSE[i].alive) continue;

            if(OBJ_HORSE[i].x==pieceSelected.first &&
               OBJ_HORSE[i].y==pieceSelected.second){

                //buscar pieza enemiga en la casilla destino
                if(board1[new_x][new_y]==PIB::_TOWER){
                    for(auto& T: OBJ_TOWER){
                        if(!T.alive) continue;
                        
                        if(T.x==new_x && T.y==new_y){
                            if(T.team==OBJ_HORSE[i].team) return;
                            T.alive=false;
                            break;
                        }
                    }
                }else if(board1[new_x][new_y]==PIB::_HORSE){
                    for(std::size_t j=0; j<OBJ_HORSE.size(); ++j){
                        if(j==static_cast<std::size_t>(i)) continue;

                        auto& H=OBJ_HORSE[j];
                        if(!H.alive) continue;

                        if(H.x==new_x && H.y==new_y){
                            if(H.team==OBJ_HORSE[i].team) return;

                            H.alive=false;
                            break;
                        }
                    }
                } 

                board1[pre_x][pre_y]=PIB::EMPTY;
                board[pre_x][pre_y]=Board::NONE;

                OBJ_HORSE[i].x=new_x;
                OBJ_HORSE[i].y=new_y;

                OBJ_HORSE[i].prePos={pre_x, pre_y};
                OBJ_HORSE[i].actualPos={new_x, new_y};

                if(OBJ_HORSE[i].team){
                    board[new_x][new_y]=Board::WHITE;
                    board1[new_x][new_y]=PIB::_HORSE;
                }else{
                    board[new_x][new_y]=Board::BLACK;
                    board1[new_x][new_y]=PIB::_HORSE;
                }
                
                SPRITE_HORSE[i].setPosition(sf::Vector2f(250+CELL*new_y, 150+CELL*new_x));
            }
        }

        possiblesMoving.clear();
    }

    void calculatedHorse(){
        int _row=newCoords.first;
        int _col=newCoords.second;

        for(auto& H: OBJ_HORSE){
            if(H.alive &&
               _row==H.actualPos.first && 
               _col==H.actualPos.second) H.inspect();
        }
    }

    void calculatedTower(){
        int _row=newCoords.first;
        int _col=newCoords.second;

        for(auto& T: OBJ_TOWER){
            if(T.alive &&
               _row==T.actualPos.first && 
               _col==T.actualPos.second) T.inspect();
        }
    }

    void calculated(){
        if(board1[pieceSelected.first][pieceSelected.second]==PIB::_HORSE) calculatedHorse();
        else if(board1[pieceSelected.first][pieceSelected.second]==PIB::_TOWER) calculatedTower();
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

        if(R.SelectedNewPosition()){
            R.actualization();
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
