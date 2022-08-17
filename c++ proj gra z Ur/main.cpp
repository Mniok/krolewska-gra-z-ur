#pragma once
#include <SFML/Graphics.hpp>
#include <iostream> //tylko dla wypisywania bledu przy probie ladowania czcionki
#include "Board.h"
#include "GlobalConstants.h"
#include <stdlib.h>>
#include <time.h>

int rollDice() {
    int sumOfDice = 0;
    for (int i = 0; i < 4; i++)
        sumOfDice += rand() % 2;    ///rzuca sie 4 4sciennymi kostkami z 2 bialymi i 2 czarnymi czubkami i zlicza biale czubki - to tak jak 4 rzuty moneta

    return sumOfDice;
}


int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 600), "Krolewska Gra z Ur - Marian Stopyra 164014 P3 - Projekt C++ 2022");

    sf::Font arial;
    if (!arial.loadFromFile("arial.ttf"))
        std::cout << "ERROR: failed to load arial.ttf\n";
    
    Board board(&window, &arial);
    srand(time(NULL));

    bool startGame = false;
    while (window.isOpen() && !startGame)   ///MENU
    {
        sf::Text menuTitle;         //wiem ¿e mo¿naby to zoptymalizowaæ przenosz¹c wiêkszoœæ przed pêtlê, ale zostawiam tutaj ¿eby kod by³ czytelniejszy i wszystko by³o razem
        menuTitle.setFont(arial);
        sf::Text menuGamemode1(menuTitle);
        sf::Text menuGamemode2(menuTitle);
        sf::Text menuGamemode3(menuTitle);
        sf::Text menuDoBotDelay(menuTitle);
        menuTitle.setString(L"Królewska gra z Ur");
        menuGamemode1.setString(L"Wciœnij 1, by zagraæ w trybie 2 graczy");
        menuGamemode2.setString(L"Wciœnij 2, by zagraæ przeciwko komputerowi");
        menuGamemode3.setString(L"Wciœnij 3, by obejrzeæ grê komputer vs komputer");
        if (board.applyBotDelay) {
            menuDoBotDelay.setString(L"Wciœnij 0, by wy³¹czyæ czekanie komputera przed wykonaniem ruchu (obecnie - w³¹czone)");
            menuDoBotDelay.setFillColor(sf::Color(220U, 220U, 220U));
        }
        else {
            menuDoBotDelay.setString(L"Wciœnij 0, by w³¹czyæ czekanie komputera przed wykonaniem ruchu (obecnie - wy³¹czone)");
            menuDoBotDelay.setFillColor(sf::Color(150U, 150U, 150U));
        }
        menuTitle.setCharacterSize(PAWN_SIZE);
        menuDoBotDelay.setScale(sf::Vector2f(0.5, 0.5));
        float avgWidth = (menuGamemode1.getLocalBounds().width + menuGamemode2.getLocalBounds().width + menuGamemode3.getLocalBounds().width) / 3;
        menuTitle.setPosition(sf::Vector2f(WINDOW_WIDTH/2 - menuTitle.getLocalBounds().width/2, WINDOW_HEIGHT/8));
        menuGamemode1.setPosition(sf::Vector2f(sf::Vector2f(WINDOW_WIDTH/2 - avgWidth/2, menuTitle.getPosition().y + menuTitle.getLocalBounds().height*2)));
        menuGamemode2.setPosition(sf::Vector2f(sf::Vector2f(WINDOW_WIDTH/2 - avgWidth/2, menuGamemode1.getPosition().y + menuGamemode1.getLocalBounds().height*2)));
        menuGamemode3.setPosition(sf::Vector2f(sf::Vector2f(WINDOW_WIDTH/2 - avgWidth/2, menuGamemode2.getPosition().y + menuGamemode2.getLocalBounds().height*2)));
        menuDoBotDelay.setPosition(sf::Vector2f(sf::Vector2f(WINDOW_WIDTH/2 - avgWidth/2, menuGamemode3.getPosition().y + menuGamemode3.getLocalBounds().height*2)));

        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::KeyReleased) {
                if (event.key.code == sf::Keyboard::Num1) { //PvP
                    startGame = true;
                }

                if (event.key.code == sf::Keyboard::Num2) { //PvE
                    board.player2.isBot = true;
                    board.botDelay = sf::milliseconds(BOT_DELAY_PLAYER_VS_BOT);
                    startGame = true;
                }

                if (event.key.code == sf::Keyboard::Num3) { //EvE
                    board.player1.isBot = true;
                    board.player2.isBot = true;
                    board.botDelay = sf::milliseconds(BOT_DELAY_BOT_VS_BOT);
                    startGame = true;
                }

                if (event.key.code == sf::Keyboard::Num0) { //toggle bot delay
                    board.applyBotDelay = !board.applyBotDelay;
                }
            }
        }

        window.clear();
        window.draw(menuTitle);
        window.draw(menuGamemode1);
        window.draw(menuGamemode2);
        window.draw(menuGamemode3);
        window.draw(menuDoBotDelay);
        window.display();
    }

    Player* whoseTurn = &board.player1;
    int moveByThisTurn = rollDice();     //o ile pol sie poruszyc
    bool keepTurn = true;    //gdy gracz konczy ture na rozetce //inicjalizowane na true zeby zaczynal ten sam gracz co inicjalizowany w whoseTurn
    bool LMBReleased = false;
    bool moveMade = false;      // czy w tym frame wykonany zostal ruch - przejscie do nastepnej tury
    bool gameEnded = false;

    while (window.isOpen() && !gameEnded)     ///MAIN GAME LOOP
    {
        ///handle turn order
        if (moveMade) {
            if (!keepTurn) {
                if (*whoseTurn == board.player1) whoseTurn = &board.player2;
                else whoseTurn = &board.player1;
            }
            moveByThisTurn = rollDice();
            moveMade = false;
        }

        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::MouseButtonReleased) {     // sprawdzanie czy w tym frame puszczony zostal lewy przycisk myszy
                if (event.key.code == sf::Mouse::Left) {
                    LMBReleased = true;
                }
            }
        }

        window.clear();

        board.drawGame(whoseTurn, moveByThisTurn);   //musi byc 1sze bo board inaczej wszystko zakrywa


        sf::Vector2i localMousePosition = sf::Mouse::getPosition(window); //pozycja wzglêdem okna
        sf::Vector2<float> convertMPos((float)localMousePosition.x, (float)localMousePosition.y);
        if (!whoseTurn->isBot) {
            board.processPlayerActions(whoseTurn, (float)localMousePosition.x, (float)localMousePosition.y, LMBReleased, moveByThisTurn, &keepTurn, &moveMade);
            window.display();   //narysowanie po przetworzeniu poniewaz jest podglad ruchu
        }
        else {
            window.display();   //narysowanie przed przetworzeniem zeby zupdateowac wskaznik rzutu i tury podczas gdy bot czeka
            board.processBotActions(whoseTurn, moveByThisTurn, &keepTurn, &moveMade);
        }
            
        LMBReleased = false; 

        gameEnded = whoseTurn->winCondition();
    }

    ///GAME OVER SCREEN
    if (window.isOpen() && gameEnded) {    //zeby nie probowac rysowac po zamknieciu okna
        window.clear();
        board.drawGame(whoseTurn, moveByThisTurn);

        sf::Text gameOverMesage;
        gameOverMesage.setFont(arial);
        sf::Text closeWindowPrompt(gameOverMesage);
        if (whoseTurn->pawnColor == sf::Color::Blue)
            gameOverMesage.setString(L"Wygra³ gracz niebieski!");
        else
            gameOverMesage.setString(L"Wygra³ gracz czerwony!");
        gameOverMesage.setFillColor(whoseTurn->pawnColor);
        gameOverMesage.setCharacterSize(PAWN_SIZE);
        gameOverMesage.setStyle(sf::Text::Bold);
        gameOverMesage.setPosition(WINDOW_WIDTH/2 - gameOverMesage.getLocalBounds().width/2, WINDOW_HEIGHT/6);
        window.draw(gameOverMesage);

        closeWindowPrompt.setString(L"Wciœnij enter lub escape by wyjœæ");
        closeWindowPrompt.setPosition(WINDOW_WIDTH/2 - closeWindowPrompt.getLocalBounds().width/2, 3*WINDOW_HEIGHT/4);
        window.draw(closeWindowPrompt);

        window.display();

        bool exitKeyPressed = false;
        sf::Event event;
        //oczekiwanie na wyjscie
        while (window.isOpen() && !exitKeyPressed) {
            while (window.pollEvent(event))
            {
                if (event.type == sf::Event::Closed)
                    window.close();

                if (event.type == sf::Event::KeyPressed) {
                    if (event.key.code == sf::Keyboard::Enter || event.key.code == sf::Keyboard::Escape) {
                        exitKeyPressed = true;
                    }
                }
            }
        }
    }

    return 0;
}
