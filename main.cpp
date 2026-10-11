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

    void Killed(bool attackerTeam){
        if(board1[new_x][new_y]==PIB::_ALFIL){
            for(auto& A: OBJ_ALFIL){
                if(!A.alive) continue;

                if(A.x==new_x && A.y==new_y){
                    if(A.team==attackerTeam) return;
                    A.alive=false;
                    break;
                }
            }
        }else if(board1[new_x][new_y]==PIB::_TOWER){
            for(auto& T: OBJ_TOWER){
                if(!T.alive) continue;
                        
                if(T.x==new_x && T.y==new_y){
                    if(T.team==attackerTeam) return;
                    T.alive=false;
                    break;
                }
            }
        }else if(board1[new_x][new_y]==PIB::_HORSE){
            for(auto& H: OBJ_HORSE){
                if(!H.alive) continue;

                if(H.x==new_x && H.y==new_y){
                    if(H.team==attackerTeam) return;

                    H.alive=false;
                    break;
                }
            }
        }else if(board1[new_x][new_y]==PIB::_KING){
            for(auto& K: OBJ_KING){
                if(!K.alive) continue;

                if(K.x==new_x && K.y==new_y){
                    if(K.team==attackerTeam) return;
                    K.alive=false;
                    break;
                }
            }
        }else if(board1[new_x][new_y]==PIB::_QUEEN){
            for(auto& Q: OBJ_QUEEN){
                if(!Q.alive) continue;

                if(Q.x==new_x && Q.y==new_y){
                    if(Q.team==attackerTeam) return;
                    Q.alive=false;
                    break;
                }
            }
        }
    }

    template<typename T, typename U, typename V>
    void actualizationGeneral(std::vector<T>& OBJ, std::vector<U>& SPRITE, V _piece){
        for(int i=0; i<OBJ.size(); i++){

            if(!OBJ[i].alive) continue;

            if(OBJ[i].x==pieceSelected.first &&
               OBJ[i].y==pieceSelected.second){

                Killed(OBJ[i].team);

                board1[pre_x][pre_y]=PIB::EMPTY;
                board[pre_x][pre_y]=Board::NONE;

                OBJ[i].x=new_x;
                OBJ[i].y=new_y;

                OBJ[i].prePos={pre_x, pre_y};
                OBJ[i].actualPos={new_x, new_y};

                if(OBJ[i].team){
                    board[new_x][new_y]=Board::WHITE;
                    board1[new_x][new_y]=_piece;
                }else{
                    board[new_x][new_y]=Board::BLACK;
                    board1[new_x][new_y]=_piece;
                }
                
                SPRITE[i].setPosition(sf::Vector2f(250+CELL*new_y, 150+CELL*new_x));
            }
        }

        possiblesMoving.clear();
    }

    void actualization(){
        if(board1[pieceSelected.first][pieceSelected.second]==PIB::_HORSE) actualizationGeneral(OBJ_HORSE, SPRITE_HORSE, PIB::_HORSE);
        else if(board1[pieceSelected.first][pieceSelected.second]==PIB::_TOWER) actualizationGeneral(OBJ_TOWER, SPRITE_TOWER, PIB::_TOWER);
        else if(board1[pieceSelected.first][pieceSelected.second]==PIB::_ALFIL) actualizationGeneral(OBJ_ALFIL, SPRITE_ALFIL, PIB::_ALFIL);
        else if(board1[pieceSelected.first][pieceSelected.second]==PIB::_KING) actualizationGeneral(OBJ_KING, SPRITE_KING, PIB::_KING);
        else if(board1[pieceSelected.first][pieceSelected.second]==PIB::_QUEEN) actualizationGeneral(OBJ_QUEEN, SPRITE_QUEEN, PIB::_QUEEN);
    }

    template<typename T>
    void calculatedGeneral(int& _row, int& _col,
                           const std::vector<T>& _PIECES_){
        for(auto& _X_: _PIECES_){
            if(_X_.alive &&
               _row==_X_.actualPos.first &&
               _col==_X_.actualPos.second) _X_.inspect();
        }
    }

    void calculated(){
        int _row=newCoords.first;
        int _col=newCoords.second;

        if(board1[pieceSelected.first][pieceSelected.second]==PIB::_HORSE) calculatedGeneral(_row, _col, OBJ_HORSE);
        else if(board1[pieceSelected.first][pieceSelected.second]==PIB::_TOWER) calculatedGeneral(_row, _col, OBJ_TOWER);
        else if(board1[pieceSelected.first][pieceSelected.second]==PIB::_ALFIL) calculatedGeneral(_row, _col, OBJ_ALFIL);
        else if(board1[pieceSelected.first][pieceSelected.second]==PIB::_KING) calculatedGeneral(_row, _col, OBJ_KING);
        else if(board1[pieceSelected.first][pieceSelected.second]==PIB::_QUEEN) calculatedGeneral(_row, _col, OBJ_QUEEN);
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

        RE.coloredFuture(window);

        window.display();
    }

    return 0;
}
