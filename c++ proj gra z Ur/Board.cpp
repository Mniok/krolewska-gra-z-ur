#include "Board.h"
#include <iostream> //tylko do wypisania errora

void Board::drawBoard()const {

    sf::RectangleShape tile(sf::Vector2<float>(BOARD_TILE_SIZE - 2*BOARD_TILE_OUTLINE_THICKNESS, BOARD_TILE_SIZE - 2* BOARD_TILE_OUTLINE_THICKNESS)); //uwzglednienie obramowki w rozmiary pola

    sf::Sprite rosetteSprite;
    rosetteSprite.setTexture(rosetteImg);

    float posX = BOARD_POSITION_X, posY = BOARD_POSITION_Y;
    
    tile.setFillColor(sf::Color((sf::Uint8)200U, (sf::Uint8)135U, (sf::Uint8)70U));   //jasnobrazowy
    tile.setOutlineColor(sf::Color((sf::Uint8)160U, (sf::Uint8)100U, (sf::Uint8)45U));
    tile.setOutlineThickness(BOARD_TILE_OUTLINE_THICKNESS);

    /// lewy prostokat 4x3      //rysuje od lewej do prawej i z gory na dol
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 3; j++) {
            tile.setPosition(sf::Vector2<float>(posX + i*BOARD_TILE_SIZE, posY + j*BOARD_TILE_SIZE));
            ptrWindow->draw(tile);

        }
    }

    /// srodkowy most
    tile.setPosition(sf::Vector2<float>(posX + 4*BOARD_TILE_SIZE, posY + BOARD_TILE_SIZE));
    ptrWindow->draw(tile);
    tile.setPosition(sf::Vector2<float>(posX + 5*BOARD_TILE_SIZE, posY + BOARD_TILE_SIZE));
    ptrWindow->draw(tile);

    /// prawy prostokat - 2x3
    for (int i = 6; i < 8; i++) {
        for (int j = 0; j < 3; j++) {
            tile.setPosition(sf::Vector2<float>(posX + i*BOARD_TILE_SIZE, posY + j*BOARD_TILE_SIZE));
            ptrWindow->draw(tile);

        }
    }

    /// rozetki
    rosetteSprite.setPosition(sf::Vector2<float>(posX + 0*BOARD_TILE_SIZE, posY + 0*BOARD_TILE_SIZE));
    ptrWindow->draw(rosetteSprite);
    rosetteSprite.setPosition(sf::Vector2<float>(posX + 0*BOARD_TILE_SIZE, posY + 2*BOARD_TILE_SIZE));
    ptrWindow->draw(rosetteSprite);
    rosetteSprite.setPosition(sf::Vector2<float>(posX + 3*BOARD_TILE_SIZE, posY + 1*BOARD_TILE_SIZE));
    ptrWindow->draw(rosetteSprite);
    rosetteSprite.setPosition(sf::Vector2<float>(posX + 6*BOARD_TILE_SIZE, posY + 0*BOARD_TILE_SIZE));
    ptrWindow->draw(rosetteSprite);
    rosetteSprite.setPosition(sf::Vector2<float>(posX + 6*BOARD_TILE_SIZE, posY + 2*BOARD_TILE_SIZE));
    ptrWindow->draw(rosetteSprite);

}

void Board::drawPawns(const Player& player)const {
    for (int i = 0, tile; i < player.activePawns.size(); i++) {
        tile = player.activePawns.at(i).tileOnPath;
        player.activePawns.at(i).drawPawn( player.path[tile] );
    }
}

void Board::drawGame(Player* whoseTurn, int moveBy)const {
    drawBoard();
    player1.drawInactivePawnHUD();
    player2.drawInactivePawnHUD();

    drawPawns(player1);
    drawPawns(player2);

    whoseTurn->drawDiceRollHUD(moveBy);

}

//check if curPawn can be moved to targetTile
bool Board::checkIfValidMove(Pawn &curPawn, int moveBy, sf::Vector2i targetTile)const {

    if (curPawn.tileOnPath + moveBy > 14) return false; //14 - finish
    if (tileContents[targetTile.x][targetTile.y] == nullptr) return true;   //wolne pole
    if (tileContents[targetTile.x][targetTile.y]->ownerID == curPawn.ownerID) return false; //blokada zbijania wlasnych pionkow
    if (isRosette[targetTile.x][targetTile.y]) return false;    //blokada zbijania przeciwnika na rozetce (wolna rozetka juz zalatwiona wolnym polem)
    return true; //zostanie zbity przeciwnik
}

//check if new pawn can be placed at targetTile
bool Board::checkIfValidMove(const int &curPlayerID, int moveBy, sf::Vector2i targetTile)const {

    if (tileContents[targetTile.x][targetTile.y] == nullptr) return true;   //wolne pole
    return false;
}

void Board::processPlayerActions(Player* whoseTurn, float mX, float mY, bool released, int moveBy, bool* landedOnRosette, bool* hasMoveBeenMade) {
    // 3 mo¿liwe akcje - pominiêcie ruchu, postawienie nowego pionka, i ruch pionkiem ju¿ znajduj¹cym siê na planszy

    //skip turn
    sf::Text skip;
    skip.setFont(*ptrFont);
    skip.setString(L"Pomiñ turê");  //wstring wspiera polskie znaki
    skip.setFillColor(sf::Color(80U, 80U, 80U));
    sf::Vector2f skipTextPos(WINDOW_WIDTH-HUD_MARGIN-skip.getLocalBounds().width, WINDOW_HEIGHT-HUD_MARGIN-skip.getLocalBounds().height);
    skip.setPosition(skipTextPos);
    if (mX > skip.getPosition().x && mX < skip.getPosition().x + skip.getLocalBounds().width \
        && mY > skip.getPosition().y && mY < skip.getPosition().y + skip.getLocalBounds().height) {
        skip.setFillColor(sf::Color(140U, 140U, 140U));
        if (released) {
            *hasMoveBeenMade = true;
            *landedOnRosette = false;
        }
    }
    ptrWindow->draw(skip);


    Player* playerPlacingNewPawn = nullptr;
    //check if mouse is over player1 hud (to place new player1 pawns)
    if (*whoseTurn == player1 && player1.inactivePawns > 0 && moveBy > 0 && mX > HUD_MARGIN && mX < HUD_MARGIN + PAWN_SIZE \
        && mY > HUD_MARGIN && mY < HUD_MARGIN + PAWN_SIZE)
            playerPlacingNewPawn = &player1;
    else//check if mouse is over player2 hud
        if (*whoseTurn == player2 && player2.inactivePawns > 0 && moveBy > 0 && mX > HUD_MARGIN && mX < HUD_MARGIN + PAWN_SIZE \
        && mY > WINDOW_HEIGHT - HUD_MARGIN - PAWN_SIZE && mY < WINDOW_HEIGHT - HUD_MARGIN)
            playerPlacingNewPawn = &player2;

    if(playerPlacingNewPawn != nullptr) {   //ktorys gracz stawia na planszy nowy pionek
        //draw powiekszony, draw przewidywany, handle click
        playerPlacingNewPawn->drawInactivePawnHUD(LARGER);
        sf::Vector2i targTile = playerPlacingNewPawn->path[moveBy - 1];
        bool valid = checkIfValidMove(playerPlacingNewPawn->ID, moveBy, targTile); //-1 bo 0-based - ruch o 1 do przodu (ze startu!) postawi pionek na polu 0

        Pawn pawnSilhouette(ptrWindow, playerPlacingNewPawn->ID, playerPlacingNewPawn->pawnColor);
        if (valid)                                                              //pokazywanie przewidywanej nastepnej pozycji
            pawnSilhouette.drawPawn(targTile, LARGER, TRANSPARENT);
        else
            pawnSilhouette.drawPawn(targTile, false, false, FORBIDDEN);

        //handle clicks - place new pawn on board
        if (valid && released) {
            Pawn* ptrNewPawn = playerPlacingNewPawn->newActivePawn(moveBy - 1);
            tileContents[targTile.x][targTile.y] = ptrNewPawn;
            *landedOnRosette = isRosette[targTile.x][targTile.y];
            *hasMoveBeenMade = true;
        }

        return;
    }   // - - - - -

    /*//check if mouse is over player2 hud
    if (*whoseTurn == player2 &&player2.inactivePawns>0 && moveBy > 0 && mX > HUD_MARGIN && mX < HUD_MARGIN + PAWN_SIZE \
        && mY > WINDOW_HEIGHT - HUD_MARGIN - PAWN_SIZE && mY < WINDOW_HEIGHT - HUD_MARGIN) {
        //draw powiekszony, draw przewidywany, handle click
        player2.drawInactivePawnHUD(LARGER);
        sf::Vector2i targTile = player2.path[moveBy - 1];
        bool valid = checkIfValidMove(player2.ID, moveBy, targTile); //-1 bo 0-based - ruch o 1 do przodu (ze startu!) postawi pionek na polu 0

        Pawn pawnSilhouette(ptrWindow, player2.ID, player2.pawnColor);
        if (valid)    //pokazywanie przewidywanej nastepnej pozycji
            pawnSilhouette.drawPawn(targTile, LARGER, TRANSPARENT);
        else
            pawnSilhouette.drawPawn(targTile, false, false, FORBIDDEN);

        //handle clicks - place new pawn on board
        if (valid && released) {
            Pawn* ptrNewPawn = player2.newActivePawn(moveBy - 1);
            tileContents[targTile.x][targTile.y] = ptrNewPawn;
            *landedOnRosette = isRosette[targTile.x][targTile.y];
            *hasMoveBeenMade = true;
        }

        return;
    }   // - - - - -
    */
    
    /// RUCH PIONKIEM JU¯ ZNAJDUJ¥CYM SIÊ NA PLANSZY
    //figure out which tile is hovered over (if any)
    int tileXcoord = (int)((mX-BOARD_POSITION_X+BOARD_TILE_OUTLINE_THICKNESS) / BOARD_TILE_SIZE +1) -1;     //(int)(..+1) -1 - bugfix zwi¹zany z konwersj¹ float na int, przez co przy krawedziach przyjmowa³o 2x wiêkszy obszar jako koordynat 0
    int tileYcoord = (int)((mY-BOARD_POSITION_Y+BOARD_TILE_OUTLINE_THICKNESS) / BOARD_TILE_SIZE +1) -1;     //    (najechanie nad lub po lewej od planszy rejestrowa³o siê jak najechanie na planszê)
    if (tileXcoord<0 || tileXcoord>7 || tileYcoord<0 || tileYcoord>2) 
        return; //out of bounds
    

    Pawn* hoveredTileContents = tileContents[tileXcoord][tileYcoord];   /// checking what's in the tile currently hovered over
    if (hoveredTileContents == nullptr)
        return; //empty tile

    int hoveredTileOwnerID = hoveredTileContents->ownerID;
    Player* hoveredTileOwner;

    if (hoveredTileOwnerID == player1.ID) hoveredTileOwner = &player1;
    else if (hoveredTileOwnerID == player2.ID) hoveredTileOwner = &player2;
    else return; //bugfix //@edit: ten sam bug jest ju¿ naprawiany przez odœwierzanie wskaŸników do wektora, ale zostawiam bo jest bardziej optymalne (odrzuca najechanie na puste pole przed wejœciem w checkValidMove)

    if (*hoveredTileOwner != *whoseTurn) return; //odrzucenie podgladu i mozliwozci ruchu dla nieswojej tury

    sf::Vector2i currentTile = sf::Vector2i(tileXcoord, tileYcoord);
    sf::Vector2i targetTile = hoveredTileOwner->path[hoveredTileContents->tileOnPath + moveBy];
    bool valid = checkIfValidMove(*hoveredTileContents, moveBy, targetTile);

    Pawn pawnSilhouette(ptrWindow, hoveredTileOwner->ID, hoveredTileOwner->pawnColor);
    if (valid)    //pokazywanie przewidywanej nastepnej pozycji
        pawnSilhouette.drawPawn(targetTile, LARGER, TRANSPARENT);
    else
        pawnSilhouette.drawPawn(targetTile, false, false, FORBIDDEN);

    if (moveBy < 1 && released) {      /// odrzucenie dopiero po narysowaniu podgladu
        *hasMoveBeenMade = true;
        valid = false;
    }

    //handle clicks - move pawn
    if (valid && released) {
        movePawn(currentTile, targetTile, hoveredTileContents->tileOnPath + moveBy);
        *hasMoveBeenMade = true;
        *landedOnRosette = isRosette[targetTile.x][targetTile.y];
    }


    refreshPointers(); //!!! bugfix
    return;

}   //////// koniec processPlayerActions --------------


void Board::processBotActions(Player* whoseTurn, int moveBy, bool* landedOnRosette, bool* hasMoveBeenMade) {
    Pawn* bestMove = nullptr;
    int bestMoveEval = 0, curMoveEval;
    *landedOnRosette = false;    //bugfix na rzadki bug - gdy bot stan¹³ na rozetce i nie móg³ wykonaæ ruchu to móg³ losowaæ a¿ mu siê uda, zatrzymuj¹c swoj¹ turê

    if (applyBotDelay) 
        sf::sleep(botDelay); //zeby dalo sie nadazyc za botem

    //przy wyrzuceniu 0
    if (moveBy < 1) {
        *hasMoveBeenMade = true;
        return;
    }

    //ocenia najlepszy ruch spoœród pionków znajduj¹chych siê na planszy
    for (std::vector<Pawn>::iterator iter = whoseTurn->activePawns.begin(); iter != whoseTurn->activePawns.end(); ++iter) {
        sf::Vector2i tileCoord = whoseTurn->path[iter->tileOnPath];
        sf::Vector2i targetTileCoord = whoseTurn->path[iter->tileOnPath + moveBy];

        //odrzucenie ruchow niedozwolonych
        if (!checkIfValidMove(*iter, moveBy, targetTileCoord)) {
            curMoveEval = 0; //tylko informacyjnie
            continue;
        }

        //podstawowa waga - ruchy dalej oceniane wyzej, ale nie z wielkim znaczeniem
        curMoveEval = 20 + iter->tileOnPath + moveBy;

        //wysokie ocenienie zbicia wrogiego pionka
        if (tileContents[targetTileCoord.x][targetTileCoord.y] != nullptr)    //moze to byc tylko zbijalny wrogi pionek - wszystko inne zostalo juz odrzucone
            curMoveEval *= 5;

        //wysokie ocenienie zejscia z planszy
        if (iter->tileOnPath + moveBy == FINISH_LINE)
            curMoveEval *= 4;

        //wysokie ocenienie wejscia na rozetke
        if (isRosette[targetTileCoord.x][targetTileCoord.y]) {
            
            if (targetTileCoord.x == 3 && targetTileCoord.y == 1)
                curMoveEval *= 20; //bardzo wysokie ocenienie zdobycia kontroli nad œrodkow¹ rozetk¹ - najwazniejsz¹ pozycj¹ w grze
            else
                curMoveEval *= 7;
        }
        
        //karanie za opuszczenie œrodkowej rozetki - lepiej zachowac nad ni¹ kontrolê
        if (tileCoord.x == 3 && tileCoord.y == 1 && isRosette[tileCoord.x][tileCoord.y])
            curMoveEval *= 0.6;

        if (curMoveEval > bestMoveEval) {
            bestMoveEval = curMoveEval;
            bestMove = &*iter;
        }
    }

    //ocena postawienia nowego pionka na planszê, i postawienie go jeœli jest to lepszy ruch
    if (whoseTurn->inactivePawns > 0) {
        sf::Vector2i targetTileCoord = whoseTurn->path[moveBy - 1];
        if (checkIfValidMove(whoseTurn->ID, moveBy, targetTileCoord)) {

            //podstawowa waga - wyzsza niz dla pionków ju¿ na planszy bo op³aca siê mieæ du¿o pionków
            curMoveEval = 30 + moveBy;

            //wysokie ocenienie wejscia na rozetke
            if (isRosette[targetTileCoord.x][targetTileCoord.y])
                    curMoveEval *= 7;

            if (curMoveEval > bestMoveEval) {
                Pawn* ptrNewPawn = whoseTurn->newActivePawn(moveBy - 1);
                tileContents[targetTileCoord.x][targetTileCoord.y] = ptrNewPawn;
                *landedOnRosette = isRosette[targetTileCoord.x][targetTileCoord.y];
                *hasMoveBeenMade = true;
                return;
            }
        }
    }

    ///wykonanie najwy¿ej ocenionego ruchu
    if (bestMoveEval > 0) {
        sf::Vector2i tileCoord = whoseTurn->path[bestMove->tileOnPath];
        sf::Vector2i targetTileCoord = whoseTurn->path[bestMove->tileOnPath + moveBy];
        movePawn(tileCoord, targetTileCoord, bestMove->tileOnPath + moveBy);
        *landedOnRosette = isRosette[targetTileCoord.x][targetTileCoord.y];
    }

    ///zakoñczenie - i pominiêcie tury jeœli nie da siê wykonaæ ¿adnego ruchu
    *hasMoveBeenMade = true;
    refreshPointers();
    return;
}   //////// koniec processBotActions ----------------



void Board::movePawn(sf::Vector2i fromTile, sf::Vector2i toTile, int newPositionOnPath) {
    Pawn* movedPawn = tileContents[fromTile.x][fromTile.y];
        if (movedPawn == nullptr) return;   //bugfix
    int movedPawnOwnerID = movedPawn->ownerID;
    Player *ptrMPOwner, *ptrOpponent;        //ptr na 2giego gracza jest potrzebny do usuwania jego pionkow

    if (movedPawnOwnerID == player1.ID) {
        ptrMPOwner = &player1;
        ptrOpponent = &player2;
    }
    else {
        ptrMPOwner = &player2;
        ptrOpponent = &player1;
    }

    //zejscie z planszy
    if (newPositionOnPath == FINISH_LINE) {
        //znalezienie i usuniecie pionka ktory schodzi z planszy
        for (int i = 0; i < ptrMPOwner->activePawns.size(); i++)
            if (*movedPawn == ptrMPOwner->activePawns.at(i)) {
                ptrMPOwner->activePawns.erase(ptrMPOwner->activePawns.begin() + i);
                tileContents[fromTile.x][fromTile.y] = nullptr;
                return; //pionek zszedl z planszy - koniec ruchu
            }
    }

    //zbijanie pionkow przeciwnika:
    if (tileContents[toTile.x][toTile.y] != nullptr) {  //na polu docelowym znajduje sie pionek - jako ze sprawdzono wczesniej czy ruch jest dozwolony, wiadomo ze jest to przeciwnik do skucia
        for (int i = 0; i < ptrOpponent->activePawns.size(); i++)
            if (*tileContents[toTile.x][toTile.y] == ptrOpponent->activePawns.at(i)) {
                ptrOpponent->activePawns.erase(ptrOpponent->activePawns.begin() + i);   //.begin() zeby przeksztalcic na iterator
                ptrOpponent->inactivePawns++;
                break;  //usunieto juz szukany pionek
            }   //nie trzeba resetowac wskaznika w tileContents bo zaraz bedzie podmieniony na nowy pionek
    }

    tileContents[fromTile.x][fromTile.y] = nullptr;     //pionek opuszcza to pole...
    tileContents[toTile.x][toTile.y] = movedPawn;       //...i wchodzi na to
    movedPawn->tileOnPath = newPositionOnPath;

}


void Board::refreshPointers() {
    for (int j = 0; j < 8; j++)
        for (int i = 0; i < 3; i++)
            tileContents[i][j] = nullptr;

    Player* plr = &player1;
    for (std::vector<Pawn>::iterator iter = plr->activePawns.begin(); iter != plr->activePawns.end(); ++iter) {
        sf::Vector2i tileCoord = plr->path[iter->tileOnPath];
        tileContents[tileCoord.x][tileCoord.y] = &*iter;        //przypisuje do pola wskaznik na pionek wskazywany przez iterator
    }

    plr = &player2;
    for (std::vector<Pawn>::iterator iter = plr->activePawns.begin(); iter != plr->activePawns.end(); ++iter) {
        sf::Vector2i tileCoord = plr->path[iter->tileOnPath];
        tileContents[tileCoord.x][tileCoord.y] = &*iter;        //przypisuje do pola wskaznik na pionek wskazywany przez iterator
    }
}


// ____________________ konstruktor

Board::Board(sf::RenderWindow *ptrWindow, sf::Font *ptrFont): ptrWindow(ptrWindow), ptrFont(ptrFont) {

    //koordynaty rozetek
    isRosette[0][0] = true;
    isRosette[0][2] = true;
    isRosette[3][1] = true; // rozetka na srodkowym pasie - najwazniejsze pole
    isRosette[6][0] = true;
    isRosette[6][2] = true;

    if (!rosetteImg.loadFromFile("rosette.png")) {
        std::cout << "ERROR: coudn't load rosette.png\n";
    }


    for (int i = 0, x = 3; i < 4; i++, x--) {               // 1szy segment trasy - przed wejsciem na pas srodkowy
        player1.path[i] = sf::Vector2i(x, 0);
        player2.path[i] = sf::Vector2i(x, 2);
    }

    for (int i = 4, x = 0; i < 4+8; i++, x++) {             // 2gi segment trasy - pas srodkowy
        player1.path[i] = sf::Vector2i(x, 1);
        player2.path[i] = sf::Vector2i(x, 1);
    }

    for (int i = 4+8, x = 7; i < 4+8+3; i++, x--) {         // 3ci segment trasy - przed meta i meta - path[14] to meta gdzie pionek schodzi z planszy
        player1.path[i] = sf::Vector2i(x, 0);
        player2.path[i] = sf::Vector2i(x, 2);
    }

    for (int i = 4 + 8 + 3, x = 4; i < 20; i++) {           // margines po koncu planszy na blokowanie wyskokow dalszych niz meta (i pokazywanie X ze probuje sie wyskoczyc poza mete! dlatego nie if(przewidywanaTrasa>15) !)
        player1.path[i] = sf::Vector2i(x, 0);
        player2.path[i] = sf::Vector2i(x, 2);
    }

}