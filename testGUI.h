#ifndef TESTGUI_H
#define TESTGUI_H
#pragma once

#include <stdio.h>
#include<utility>

class Reversi;

class BoardGraph
{	
public:
	void drawPiece(int row, int col, int color);
	void showBoard();
	void closeBoard();
	void showText(const char* text);
	std::pair<int, int> waitInput();

};

class Game
{
	BoardGraph graph;
	int board[19][19];
	Reversi* reversi;
	void updateBoard(int row,int col,int color);
	int isGameOver(); //-1 not over  / 1 white win / 0 black win
public:
	Game();
	void AIGame(int humanColor);//human-AI combat
	void HumanGame(); //human-human combat
	~Game();
};

#endif

