#pragma once
#include"evaluate.h"
#include<iostream>

using namespace std;

struct movenode
{
	int num;
	int x1;
	int x2;
	int y1;
	int y2;
	int numberOfMyRoad[7];
	int numberOfEnemyRoad[7];
	int score;
	movenode* next;
};

struct chessnode
{
	int number;
	int position[19][19];//0表示没有棋子，1表示黑棋，7表示白棋
	int baseScore;
	int type;//落子人的棋色
	int numberOfEnemyRoad[7];
	int	numberOfMyRoad[7];
	movenode next1[6];
};

class evaluation
{

private:
	void unMakeMove(chessnode* node, movenode* move);
	void makeMove(chessnode* node, movenode* move);
public:
	int calScoreall(chessnode* node);
	int calScore(chessnode* node, movenode* mnode);
	int wholeevalute(chessnode* node);
	int evalute(chessnode* node, movenode* mnode);
	void wholeanalysisVertical(chessnode* node);
	void wholeanalysisHorizon(chessnode* node);
	void wholeanalysisLeft(chessnode* node);
	void wholeanalysisRight(chessnode* node);
	void analysisHorizon(movenode* mnode, chessnode* node);
	void analysisVertical(movenode* mnode, chessnode* node);
	void analysisLeft(movenode* mnode, chessnode* node);
	void analysisRight(movenode* mnode, chessnode* node);
	movenode* searchAGoodMove(chessnode* node);
	movenode* generatestep(chessnode* chess);
};

