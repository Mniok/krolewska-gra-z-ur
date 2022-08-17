#pragma once
///uzyte w main.cpp, Board.h, Player.h, i Pawn.h (czyli wszedzie)

#define WINDOW_WIDTH 800.f
#define WINDOW_HEIGHT 600.f

#define BOARD_TILE_SIZE 80.f
#define BOARD_POSITION_X (WINDOW_WIDTH/2 - 4*BOARD_TILE_SIZE)
#define BOARD_POSITION_Y (WINDOW_HEIGHT/2 - 1.5*BOARD_TILE_SIZE)
#define BOARD_TILE_OUTLINE_THICKNESS (BOARD_TILE_SIZE / 10)

#define PAWN_SIZE (BOARD_TILE_SIZE - 4*BOARD_TILE_OUTLINE_THICKNESS)
//nawiasy na makrach bo dzialy sie dziwne rzeczy z dzialaniami - PAWN_SIZE/2 dzielilo przez 2 BOARD_TILE_OUTLINE_THICKNESS przez co calosc (okolo) mnozylo przez 2 zamiast podzielic

//odleglosc od granic okna
#define HUD_MARGIN 20.f

#define LARGER true
#define TRANSPARENT true
#define FORBIDDEN true

#define FINISH_LINE 14

//czas w milisekundach zanim bot wykona ruch - zeby gracz mogl nadazyc za tym co sie dzieje
#define BOT_DELAY_PLAYER_VS_BOT 400
#define BOT_DELAY_BOT_VS_BOT 200
//bot vs bot jest szybsze bo boty duzo sie nawzajem skuwaja i walki czasami dlugo trwaja