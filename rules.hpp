#ifndef RULES_HPP
#define RULES_HPP

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
        else if(board1[new_x][new_y]==PIB::_PAWN){
            for(auto& PA: OBJ_PAWN){
                if(!PA.alive) continue;

                if(PA.x==new_x && PA.y==new_y){
                    if(PA.team==attackerTeam) return;
                    PA.alive=false;
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
                
                SPRITE[i].setPosition(sf::Vector2f(COLUMN_SCREEN+CELL*new_y, ROW_SCREEN+CELL*new_x));
            }
        }

        possiblesMoving.clear();
    }

    void actualizationFirstTime(){ //FUNCION EXCLUSIVA PARA PAWNS
        for(auto& PAWN: OBJ_PAWN){
            if(PAWN.x==new_x && PAWN.y==new_y) PAWN.firstTime++;
        }
    }

    void actualization(){
        if(board1[pieceSelected.first][pieceSelected.second]==PIB::_HORSE) actualizationGeneral(OBJ_HORSE, SPRITE_HORSE, PIB::_HORSE);
        else if(board1[pieceSelected.first][pieceSelected.second]==PIB::_TOWER) actualizationGeneral(OBJ_TOWER, SPRITE_TOWER, PIB::_TOWER);
        else if(board1[pieceSelected.first][pieceSelected.second]==PIB::_ALFIL) actualizationGeneral(OBJ_ALFIL, SPRITE_ALFIL, PIB::_ALFIL);
        else if(board1[pieceSelected.first][pieceSelected.second]==PIB::_KING) actualizationGeneral(OBJ_KING, SPRITE_KING, PIB::_KING);
        else if(board1[pieceSelected.first][pieceSelected.second]==PIB::_QUEEN) actualizationGeneral(OBJ_QUEEN, SPRITE_QUEEN, PIB::_QUEEN);
        else if(board1[pieceSelected.first][pieceSelected.second]==PIB::_PAWN){
            actualizationGeneral(OBJ_PAWN, SPRITE_PAWN, PIB::_PAWN);
            actualizationFirstTime();
        }
    }

    template<typename T>
    void calculatedGeneral(int& _row, int& _col,
                           std::vector<T>& _PIECES_){
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
        else if(board1[pieceSelected.first][pieceSelected.second]==PIB::_PAWN) calculatedGeneral(_row, _col, OBJ_PAWN);
    }
};

#endif
