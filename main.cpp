#include <SFML/Window/Event.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <utility>
#include <cassert>
#include <vector>

#include "domain.hpp"
#include "global.hpp"

/*
 __FUNCTION__
 __LINE__ estos dos para imprimir
 */

struct PIECE{
    int x, y; //fila - columna
    bool team;
    sf::Texture* image;
    
    PIECE(int _x, int _y, sf::Texture& _image, bool _team): x(_x), y(_y), 
                                                           image(&_image), team(_team) {}

    sf::Sprite init(DOMAIN& D){
        return D.scale(*image,y,x); //SFML procesa en -> columna - fila
    }

    void paint(sf::RenderWindow& window, sf::Sprite figure){
        window.draw(figure);
    }
};

struct HORSE: PIECE{
    std::pair<int,int> prePos; //row - col
    std::pair<int,int> actualPos; //row - col puedo usar newCoords normalmente
    
    std::vector<int> inspect_row={2,1,-1,-2,-2,-1,1,2};
    std::vector<int> inspect_col={-1,-2,-2,-1,1,2,2,1};

    HORSE(int& _row, int& _col, sf::Texture& image, bool _team): 
        PIECE(_row, _col, image,_team){
        actualPos.first=_row;
        actualPos.second=_col;
    }

    void inspect(){

        PRINT<<"--------------------"<<END;
        PRINT<<"POSSIBLES MOVING: "<<END;

        for(int i=0; i<8; i++){
            int _x=newCoords.first+inspect_row[i];
            int _y=newCoords.second+inspect_col[i];
            
            if(_x>=0 && _x<8 && _y>=0 && _y<8 &&
               board[_x][_y]==Board::NONE){

                PRINT<<"Coordenada vacia numero: "<<i<<END;
                PRINT<<"_x: "<<_x<<" - _y: "<<_y<<END;
                possiblesMoving.push_back({_x,_y});
            }else continue;
        }
    }
};

void initTextureHorse(){
    sf::Texture HorseW;
    if(!HorseW.loadFromFile("piezas/horseW.png")){
        std::cerr<<"FAIL OPEN HORSE WHITE"<<std::endl;
    }

    TEXTURE_HORSE.push_back(HorseW);

    sf::Texture HorseW1;
    if(!HorseW1.loadFromFile("piezas/horseW.png")){
        std::cerr<<"FAIL OPEN HORSE WHITE"<<std::endl;
    }

    TEXTURE_HORSE.push_back(HorseW1);
}

void initSpriteHorse(DOMAIN& D){
    int rowHW=7, colHW=1;
    HORSE HW(rowHW, colHW, TEXTURE_HORSE[0], true);
    board[rowHW][colHW]=Board::WHITE;
    sf::Sprite HorseWhite=HW.init(D);

    OBJ_HORSE.push_back(HW);
    SPRITE_HORSE.push_back(HorseWhite);

    int colHW1=6;
    HORSE HW1(rowHW, colHW1, TEXTURE_HORSE[1], true);
    board[rowHW][colHW1]=Board::WHITE;
    sf::Sprite HorseWhite1=HW1.init(D);

    OBJ_HORSE.push_back(HW1);
    SPRITE_HORSE.push_back(HorseWhite1);
}

struct RULE{
    int pre_x, pre_y; //se usa newCoords vector de coordenadas convertidas
    int new_x, new_y;

    bool click(){
        if(board[newCoords.first][newCoords.second]==Board::WHITE ||
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
        //actualiza la posicion de la pieza
        for(int i=0; i<OBJ_HORSE.size(); i++){
            if(OBJ_HORSE[i].x==pieceSelected.first &&
               OBJ_HORSE[i].y==pieceSelected.second){

                OBJ_HORSE[i].x=new_x;
                OBJ_HORSE[i].y=new_y;

                OBJ_HORSE[i].prePos={pre_x, pre_y};
                OBJ_HORSE[i].actualPos={new_x, new_y};

                //aqui hay que tener cuidado para cuando se agreguen los negros
                board[new_x][new_y]=Board::WHITE;
                board[OBJ_HORSE[i].prePos.first][OBJ_HORSE[i].prePos.second]=Board::NONE;

                SPRITE_HORSE[i].setPosition(sf::Vector2f(250+CELL*new_y, 150+CELL*new_x));
            }
        } 

        possiblesMoving.clear();
    }

    void calculated(){
        int _row=newCoords.first;
        int _col=newCoords.second;

        for(auto& H: OBJ_HORSE){
            if(_row==H.actualPos.first && 
               _col==H.actualPos.second) H.inspect();
        }
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

    initTextureHorse();
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

                    //RULE
                    if(possiblesMoving.empty()){
                        if(R.click()){
                            pieceSelected=newCoords;
                            R.calculated(); //se te da la libertad de usar pieceSelected o newCoords
                        }
                    }else{
                        if(R.SelectedNewPosition()) R.actualization();
                    }
                }
            }
        }

        window.clear();

        for(auto& x: Boxes){ window.draw(x.box); }
        for(int i=0; i<OBJ_HORSE.size(); i++){
            OBJ_HORSE[i].paint(window, SPRITE_HORSE[i]);
        }

        //HW.paint(window,HorseWhite);
        //HW1.paint(window, HorseWhite1);
        RE.coloredFuture(window);

        window.display();
    }

    return 0;
}
