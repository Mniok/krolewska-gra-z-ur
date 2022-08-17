#include "Pawn.h"

// absolute position relative to window - used by Player::drawInactivePawnHUD() (and Pawn::drawPawn())
void Pawn::drawPawn(float x, float y, bool larger, bool transparent)const {
	sf::CircleShape shape;

	if (larger) {
		shape.setRadius((PAWN_SIZE + BOARD_TILE_OUTLINE_THICKNESS) / 2); //PAWN_SIZE is diameter, CircleShape constructor takes radius as argument
		shape.setPosition(x-BOARD_TILE_OUTLINE_THICKNESS/2, y-BOARD_TILE_OUTLINE_THICKNESS/2);
	}
	else {
		shape.setRadius(PAWN_SIZE / 2);
		shape.setPosition(x, y);
	}

	if (transparent) {
		sf::Color cTransp(color);
		cTransp.a = sf::Uint8(100U);	//a - alpha
		shape.setFillColor(cTransp);
	}
	else
		shape.setFillColor(color);

	ptrWindow->draw(shape);
}

// position relative to board
void Pawn::drawPawn(sf::Vector2i tile, bool larger, bool transparent, bool forbidden)const {
	float newAbsoluteXpos = BOARD_POSITION_X + tile.x * BOARD_TILE_SIZE + BOARD_TILE_OUTLINE_THICKNESS;
	float newAbsoluteYpos = BOARD_POSITION_Y + tile.y * BOARD_TILE_SIZE + BOARD_TILE_OUTLINE_THICKNESS;

	if (forbidden) drawForbidden(newAbsoluteXpos, newAbsoluteYpos);
	else drawPawn(newAbsoluteXpos,newAbsoluteYpos,larger, transparent);
}

void Pawn::drawForbidden(float x, float y)const {
	sf::RectangleShape line(sf::Vector2f(PAWN_SIZE, BOARD_TILE_OUTLINE_THICKNESS));
	line.setOrigin(PAWN_SIZE / 2, BOARD_TILE_OUTLINE_THICKNESS / 2);	//zeby obracac wzgledem srodka
	line.setPosition(x + PAWN_SIZE / 2, y + PAWN_SIZE / 2);
	line.setFillColor(sf::Color(150, 150, 150));

	line.setRotation(45.f);
	ptrWindow->draw(line);

	line.setRotation(-45.f);
	ptrWindow->draw(line);

}

bool operator==(Pawn Lvalue, Pawn Rvalue) {
	if (Lvalue.tileOnPath == Rvalue.tileOnPath && Lvalue.ownerID == Rvalue.ownerID) //nie moga byc 2 pionki na tym samym polu, wiec jesli sa to to ten sam pionek
		return true;
	else return false;
};