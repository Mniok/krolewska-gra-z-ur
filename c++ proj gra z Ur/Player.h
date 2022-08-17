#pragma once
#include "Pawn.h"
#include <vector>
#include <SFML/Graphics.hpp>
#include "GlobalConstants.h"


class Player
{
private:	//pola
	sf::RenderWindow* ptrWindow;	//tylko potrzebne zeby podawac pionkom
	const sf::Font* ptrFont;		//po to zeby nie wczytywac z pliku 2x na frame a tylko raz na poczatku
	sf::Vector2i path[20];
	int inactivePawns = 7;	//kazdy gracz ma 7 pionkow
	std::vector<Pawn> activePawns;
public:
	int ID;
	bool isBot;
	sf::Color pawnColor;



private:	//metody
	Pawn* newActivePawn(int startingPosition);

	friend class Board;	//daje dostep do newActivePawn(...) z poziomu Board::processPlayerActions(...) i Board::processBotActions(...), ale nie z poziomu main()
public:
	void drawInactivePawnHUD(bool isLarger = false)const;
	void drawDiceRollHUD(int rolledToMove)const;
	bool winCondition()const { return inactivePawns <= 0 && activePawns.size() == 0; };		//¿eby inactivePawns i activePawns mog³y byæ prywatne  ¿eby nie da³o siê nic namieszaæ z poziomu maina 
	Player(sf::RenderWindow* ptrWindow, const sf::Font* ptrFont, int playerID, sf::Color pawnColor, bool isBot = false) :
		ptrWindow(ptrWindow), ptrFont(ptrFont), ID(playerID), isBot(isBot), pawnColor(pawnColor)
	{
		activePawns.reserve((inactivePawns + 1) * sizeof(Pawn));	//zeby nie realokowac pamieci przy push_back i nie resetowac wskaznikow do pionkow w Board::tileContens 
	};								//+1 naprawia bug ze znikajacymi pionkami gdy wszystkie 7 jest na planszy - te¿ realokacja wektora		//@edit: redundantne od wprowadzenia odœwierzania wskaŸników po wykonaniu ruchu

	friend bool operator==(Player Lvalue, Player Rvalue);
	friend bool operator!=(Player Lvalue, Player Rvalue);

};

