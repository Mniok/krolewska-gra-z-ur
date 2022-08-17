#pragma once
#include <SFML/Graphics.hpp>
#include "GlobalConstants.h"


class Pawn
{
private:	//pola
	sf::RenderWindow* ptrWindow;
	int ownerID;
	sf::Color color;
	int tileOnPath = 0;


		//metody
	Pawn(sf::RenderWindow* ptrWindow, int playerID, sf::Color pawnColor) : ptrWindow(ptrWindow), ownerID(playerID), color(pawnColor) {};
	void drawPawn(float x, float y, bool larger = false, bool transparent = false)const; //relative to window
	void drawPawn(sf::Vector2i tile, bool larger = false, bool transparent = false, bool forbidden = false)const; //relative to board
	void drawForbidden(float x, float y)const;
	friend bool operator==(Pawn Lvalue, Pawn Rvalue);

	friend class Player;
	friend class Board;
};
