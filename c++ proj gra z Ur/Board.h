#pragma once
#include "Pawn.h"
#include "Player.h"
#include <vector>
#include <SFML/Graphics.hpp>
#include "GlobalConstants.h"

class Board
{
private:	//pola
	sf::RenderWindow* ptrWindow;
	sf::Font* ptrFont;
	sf::Texture rosetteImg;		//zeby nie czytac z pliku w kazdym framie	//inicjailzowane w konstruktorze
	Pawn* tileContents[8][3]{ nullptr };
	bool isRosette[8][3]{ false };
public:
	Player player1{ ptrWindow, ptrFont, 1, sf::Color::Blue };
	Player player2{ ptrWindow, ptrFont, 2, sf::Color::Red };
	bool applyBotDelay = true;	//mozliwosc wylaczenia czekania na poczatku tury bota
	sf::Time botDelay;



private:	//metody
	void refreshPointers();	//bugfix bo std::vector realokuje pamiec i przy dluzszych grach pionki znikaly ale dalej byly na planszy
	void drawBoard()const;
	void drawPawns(const Player& player)const;

	///check if existing pawn can be moved
	bool checkIfValidMove(Pawn& curPawn, int moveBy, sf::Vector2i targetTile)const;		//i moveBy i targetTile sa potrzebne ze wzgledu na rozne sciezki pomiedzy graczami
	///check if new pawn can be placed
	bool checkIfValidMove(const int& curPlayer, int moveBy, sf::Vector2i targetTile)const;
	void movePawn(sf::Vector2i fromTile, sf::Vector2i toTile, int newPositionOnPath);

public:
	void drawGame(Player* whoseTurn, int moveBy)const;
	void processPlayerActions(Player* whoseTurn, float mX, float mY, bool released, int moveBy, bool* landedOnRosette, bool* hasMoveBeenMade);
	void processBotActions(Player* whoseTurn, int moveBy, bool* landedOnRosette, bool* hasMoveBeenMade);

	Board(sf::RenderWindow* window, sf::Font* ptrFont); // inicjalizuje sciezki i rozmieszcza rozetki (w isRosette, w drawBoard nie zmienia polozenia dla wydajnosci zeby nie sprawdzac if(isRosette[i][j] 24 razy w kazdym frame)

};

