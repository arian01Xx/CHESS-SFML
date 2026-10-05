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
};


#endif
