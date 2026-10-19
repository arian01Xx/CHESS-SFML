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
struct NEURAL_NETWORK{

};

struct TEXT_RENDERING{ //CARPETA UI
    sf::Font font;

    sf::Text initTextureLetters(const std::string & letters, int row, 
                                int col, int size){
        if(!font.openFromFile("UI/main.ttf"))
            throw std::runtime_error("ERROR DE CARGA DE LA FUENTE");

        sf::Text text(font);
        text.setString(letters);
        text.setCharacterSize(size);
        text.setFillColor(sf::Color(0,128,40));

        text.setPosition(sf::Vector2f(COL_TEXT+col, row));

        return text;
    }

    void buildTextBox(std::string& text, int row, int col, int size){
        sf::Text Title=initTextureLetters(text, row, col, size);
        textBoxes.push_back(Title);
    }

    void drawTextBox(sf::RenderWindow& window){
        for(auto& x: textBoxes){
            window.draw(x);
        }
    }

    void drawBoxOfText(sf::RenderWindow& window){
        for(auto& x: BoxForText){
            window.draw(x);
        }
    }

    void drawingBox(int row, int col){
        sf::RectangleShape marco(sf::Vector2f(ANCHURA, ALTURA)); //TA CAGAO ESTO, anchura, altura
        marco.setPosition(sf::Vector2f(COL_TEXT+col, row));

        marco.setFillColor(sf::Color::Transparent);
        marco.setOutlineColor(sf::Color::Green);
        marco.setOutlineThickness(0.8f);

        BoxForText.push_back(marco);
    }

    void minit(int i, int j){
        drawingBox(i, j);
        CB cb(i, j);
        CBs.push_back(cb);
        coordBoxes.push_back({i,j}); //FILA - COLUMNA
    }

    void CreationBoxes(){ //AQUI se crean tus botones donde quieras!!
        std::string textMain="NEURAL'S NETWORK";
        buildTextBox(textMain, 20, 0, 25); 

        std::string textMain1="G.D.INTELECTUAL";
        buildTextBox(textMain1, 70, 0, 25);

        std::string boxTicTacToe="TIC-TAC\n-TOE";
        buildTextBox(boxTicTacToe, 650, 0, 10);
        minit(650, 0);

        std::string boxChess="CHESS";
        buildTextBox(boxChess, 650, 160, 10);
        minit(650, 160);

        std::string boxSnake="SNAKE";
        buildTextBox(boxSnake, 650, 240, 10);
        minit(650, 240);

        std::string boxPong="PONG";
        buildTextBox(boxPong, 650, 330, 10);
        minit(650, 330);
    }
};

struct GAME{
    void Gaming(RULE& R){ //CHESS
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

    void GamingTTT(){

    }

    void GamingSnake(){

    }

    void GamingPong(){

    }

    bool anyGameElection(std::pair<int,int> coords){
        int row=coords.first;
        int col=coords.second;

        if(row>=CBs[0].x && 
           row<CBs[0].x+ALTURA &&
           col>=CBs[0].y+COL_TEXT && 
           col<CBs[0].y+COL_TEXT+ANCHURA){
            currentPlay=PLAY::TICTACTOE;
            return true;
        }else if(row>=CBs[1].x && 
                row<CBs[1].x+ALTURA &&
                col>=CBs[1].y+COL_TEXT && 
                col<CBs[1].y+COL_TEXT+ANCHURA){
            currentPlay=PLAY::CHESS;
            return true;
        }else if(row>=CBs[2].x && 
                row<CBs[2].x+ALTURA &&
                col>=CBs[2].y+COL_TEXT && 
                col<CBs[2].y+COL_TEXT+ANCHURA){
            currentPlay=PLAY::SNAKE;
            return true;
        }else if(row>=CBs[3].x && 
                row<CBs[3].x+ALTURA &&
                col>=CBs[3].y+COL_TEXT && 
                col<CBs[3].y+COL_TEXT+ANCHURA){
            currentPlay=PLAY::PONG;
            return true;
        }else return false;;

        return true;
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

    TXR.CreationBoxes();

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

                    if (G.anyGameElection({row, col})) continue;

                    newCoords=D.convertion(row, col);

                    if(!D.insideBoard(newCoords.first, newCoords.second)) continue;

                    if(currentPlay==PLAY::CHESS) G.Gaming(R); //RULE TAMBIEN SOLO TIENE CHESS
                    else if(currentPlay==PLAY::TICTACTOE) G.GamingTTT();
                    else if(currentPlay==PLAY::SNAKE) G.GamingSnake();
                    else if(currentPlay==PLAY::PONG) G.GamingPong();

                    //G.Gaming(R);
                }
            }
        }

        window.clear();

        //aqui necesito poner condicionales if para cambiar pantalla
        if(currentPlay==PLAY::CHESS){
            for(auto& x: Boxes){ window.draw(x.box); }
            for(int i=0; i<OBJ_KING.size(); i++){ if(OBJ_KING[i].alive) OBJ_KING[i].paint(window, SPRITE_KING[i]); }
            for(int i=0; i<OBJ_ALFIL.size(); i++){ if(OBJ_ALFIL[i].alive) OBJ_ALFIL[i].paint(window, SPRITE_ALFIL[i]); }
            for(int i=0; i<OBJ_TOWER.size(); i++){ if(OBJ_TOWER[i].alive) OBJ_TOWER[i].paint(window, SPRITE_TOWER[i]); }
            for(int i=0; i<OBJ_HORSE.size(); i++){ if(OBJ_HORSE[i].alive) OBJ_HORSE[i].paint(window, SPRITE_HORSE[i]); }
            for(int i=0; i<OBJ_QUEEN.size(); i++){ if(OBJ_QUEEN[i].alive) OBJ_QUEEN[i].paint(window, SPRITE_QUEEN[i]); }
            for(int i=0; i<OBJ_PAWN.size(); i++){ if(OBJ_PAWN[i].alive) OBJ_PAWN[i].paint(window, SPRITE_PAWN[i]); }
            RE.coloredFuture(window);
        }

        TXR.drawTextBox(window);
        TXR.drawBoxOfText(window);

        window.display();
    }

    return 0;
}
