#include "Player.h"

void Player::drawInactivePawnHUD(bool isLarger)const {
	Pawn pawnSilhouette(ptrWindow, ID, pawnColor);

	sf::Text dispNumOfPawns;
	dispNumOfPawns.setFont(*ptrFont);
	dispNumOfPawns.setString(std::to_string(inactivePawns));
	dispNumOfPawns.setFillColor(pawnColor);
	dispNumOfPawns.setOutlineColor(sf::Color::White);
	
	if (ID == 1) {		/// player1 hud in top left corner, player2 hud in bottom left corner
		pawnSilhouette.drawPawn(HUD_MARGIN, HUD_MARGIN, isLarger);
		dispNumOfPawns.setPosition(HUD_MARGIN + PAWN_SIZE + BOARD_TILE_OUTLINE_THICKNESS,
								   HUD_MARGIN + PAWN_SIZE/2 - dispNumOfPawns.getCharacterSize()/2);
	}
	else {
		pawnSilhouette.drawPawn(HUD_MARGIN, WINDOW_HEIGHT - HUD_MARGIN - PAWN_SIZE, isLarger);
		dispNumOfPawns.setPosition(HUD_MARGIN + PAWN_SIZE + BOARD_TILE_OUTLINE_THICKNESS,
								   WINDOW_HEIGHT - HUD_MARGIN - PAWN_SIZE/2 - dispNumOfPawns.getCharacterSize()/2);
	}

	ptrWindow->draw(dispNumOfPawns);

}

void Player::drawDiceRollHUD(int rolledToMove)const {
	sf::Text dispRoll;
	dispRoll.setString(std::to_string(rolledToMove));
	dispRoll.setFont(*ptrFont);
	dispRoll.setCharacterSize(PAWN_SIZE);
	dispRoll.setPosition(sf::Vector2f(WINDOW_WIDTH - HUD_MARGIN - dispRoll.getCharacterSize(), HUD_MARGIN));
	dispRoll.setOutlineColor(pawnColor);
	dispRoll.setOutlineThickness(BOARD_TILE_OUTLINE_THICKNESS / 3);
	ptrWindow->draw(dispRoll);
}



Pawn* Player::newActivePawn(int startingPosition) {
	if (inactivePawns > 0) {
		activePawns.push_back(Pawn(ptrWindow, ID, pawnColor));
		activePawns.back().tileOnPath = startingPosition;
		inactivePawns--;
	}

	//nie da sie skuc przeciwnika ze startu wiec nie trzeba tu wykrywania
	
	return &activePawns.back();	//nowo dodany pionek - znajduje sie na koncu wektora
}


bool operator==(Player Lvalue, Player Rvalue) {
	if (Lvalue.ID == Rvalue.ID)
		return true;
	else return false;
};

bool operator!=(Player Lvalue, Player Rvalue) {
	return !(Lvalue == Rvalue);
};