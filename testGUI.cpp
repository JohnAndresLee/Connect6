#include "testGUI.h"
#include"Reversi.h"
#include"easyX.h"
#include"evaluate.h"

struct visited
{
	bool line;
	bool vertical;
	bool diagonal;
	bool back_diagonal;
	visited() :line(false), vertical(false), diagonal(false), back_diagonal(false) {  }
};


void BoardGraph::drawPiece(int row, int col, int color)
{
	if (color == 1)
		setfillcolor(WHITE);
	else if (color == 0)
		setfillcolor(BLACK);
	solidcircle(15 + row * 30, 15 + col * 30, 10);
	
}

void BoardGraph::showBoard()
{
	initgraph(570, 620);
	setbkcolor(BROWN);
	cleardevice();
	setlinecolor(BLACK);
	for (int i = 0; i < 19; i++) {
		line(15, 15 + i * 30, 555, 15 + i * 30);
		line(15 + i * 30, 15, 15 + i * 30, 555);
	}
}

void BoardGraph::closeBoard()
{
	closegraph();
}

void BoardGraph::showText(const char* text)
{
	setfillcolor(BROWN);
	solidrectangle(0, 571, 570, 620);
	int iLength;
	TCHAR* t_text = (TCHAR*)malloc(200);
	iLength = MultiByteToWideChar(CP_ACP, 0, text, strlen(text) + 1, NULL, 0);
	MultiByteToWideChar(CP_ACP, 0, text, strlen(text) + 1, t_text, iLength);
	outtextxy(10, 580, t_text);
	free(t_text);
}

std::pair<int,int> BoardGraph::waitInput()
{
	MOUSEMSG m;
	int row, col;
	while (true)
	{
		m = GetMouseMsg();
		if(m.uMsg == WM_LBUTTONDOWN)
		{
			row = m.x / 30;
			col = m.y / 30;
			if (row >= 0 && row <= 18 && col >= 0 && col <= 18)
				break;
		}
	}
	return std::pair<int,int>(row,col);
}


Game::Game()
{
	reversi = new Reversi;
	for (int i = 0; i < 19; i++)
		for (int j = 0; j < 19; j++)
			board[i][j] = -1;//0表示没有棋子
}

void Game::updateBoard(int row, int col, int color)
{
	board[row][col] = color;
	graph.drawPiece(row, col, color);
}

int Game::isGameOver()
{
	visited visited[19][19];
	for (int i = 0; i < 19; i++)
		for (int j = 0; j < 19; j++)
		{
			int color = board[i][j];
			if (color == -1) continue;
			//line
			if (!visited[i][j].line) {
				visited[i][j].line = true;
				for (int k = 1; k < 6; k++)
				{
					if (i + k >= 19) break;
					if (board[i + k][j] != color) break;
					visited[i + k][j].line = true;
					if (k == 5) {
						switch (color) {
						case 0:
							graph.showText("***BLACK WIN! Chessboard will close in 10s.***");
							break;
						case 1:
							graph.showText("***WHITE WIN! Chessboard will close in 10s.***");
							break;
						}
						return color;
					}
				}
			}
			//vertical
			if (!visited[i][j].vertical) {
				visited[i][j].vertical = true;
				for (int k = 1; k < 6; k++)
				{
					if (j + k >= 19) break;
					if (board[i][j + k] != color) break;
					visited[i][j + k].vertical = true;
					if (k == 5) {
						switch (color) {
						case 0:
							graph.showText("***BLACK WIN! Chessboard will close in 10s.***");
							break;
						case 1:
							graph.showText("***WHITE WIN! Chessboard will close in 10s.***");
							break;
						}
						return color;
					}
				}
			}
			//diagonal
			if (!visited[i][j].diagonal) {
				visited[i][j].diagonal = true;
				for (int k = 1; k < 6; k++){
					if (i + k >= 19 || j + k >= 19) break;
					if (board[i + k][j + k] != color) break;
					visited[i + k][j + k].diagonal = true;
					if (k == 5) {
						switch (color) {
						case 0:
							graph.showText("***BLACK WIN! Chessboard will close in 10s.***");
							break;
						case 1:
							graph.showText("***WHITE WIN! Chessboard will close in 10s.***");
							break;
						}
						return color;
					}
				}
			}
			//back-diagonal
			if (!visited[i][j].back_diagonal) {
				visited[i][j].back_diagonal = true;
				for (int k = 1; k < 6; k++){
					if (i - k >= 19 || j + k >= 19) break;
					if (board[i - k][j + k] != color) break;
					visited[i - k][j + k].back_diagonal = true;
					if (k == 5) {
						switch (color) {
						case 0:
							graph.showText("***BLACK WIN! Chessboard will close in 10s.***");
							break;
						case 1:
							graph.showText("***WHITE WIN! Chessboard will close in 10s.***");
							break;
						}
						return color;
					}
				}
			}
		}
		return -1;
}


void Game::AIGame(int humanColor)
{
	pair<int, int> humanStep1, humanStep2;
	pair<int, int> AIStep1, AIStep2;
	graph.showBoard();
	updateBoard(9, 9, 1);

	chessnode chess;

	chess.baseScore = 0;
	for (int i = 0; i < 19; ++i)
	{
		for (int j = 0; j < 19; ++j)
			chess.position[i][j] = 0;
	}
	chess.position[9][9] = 7;
	chess.number = 0;

	if (humanColor == 1)
	{
		chess.type = 1;

		reversi->setOwnColor(0);
		while (1)
		{
			pair<pair<int, int>, pair<int, int>> AIStep = reversi->steptest(chess);
			AIStep1 = AIStep.first;
			AIStep2 = AIStep.second;
			while (board[AIStep1.first][AIStep1.second] != -1)
			{
				graph.showText("Invalid Step from AI. Generate a random step");
				int row = rand() % 19;
				int col = rand() % 19;
				AIStep1.first = row;
				AIStep1.second = col;
			}
			updateBoard(AIStep1.first, AIStep1.second, 0);
			if (isGameOver() != -1) break;
			while (board[AIStep2.first][AIStep2.second] != -1)
			{
				graph.showText("Invalid Step from AI. Generate a random step");
				int row = rand() % 19;
				int col = rand() % 19;
				AIStep2.first = row;
				AIStep2.second = col;
			}
			updateBoard(AIStep2.first, AIStep2.second, 0);
			if (isGameOver() != -1) break;

			reversi->handleMessagetest(chess, AIStep1.first, AIStep1.second, AIStep2.first, AIStep2.second, 0);

			humanStep1 = graph.waitInput();
			while (board[humanStep1.first][humanStep1.second] != -1)
			{
				humanStep1 = graph.waitInput();
			}
			graph.showText("");
			updateBoard(humanStep1.first, humanStep1.second, 1);
			if (isGameOver() != -1) break;
			humanStep2 = graph.waitInput();
			while (board[humanStep2.first][humanStep2.second] != -1)
			{
				humanStep2 = graph.waitInput();
			}
			updateBoard(humanStep2.first, humanStep2.second, 1);
			if (isGameOver() != -1) break;
			reversi->handleMessagetest(chess,humanStep1.first, humanStep1.second, humanStep2.first, humanStep2.second, 1);
		}
	}
	else if (humanColor == 0)
	{
		chess.type = 7;
		reversi->setOwnColor(1);
		while (1)
		{
			
			humanStep1 = graph.waitInput();
			while (board[humanStep1.first][humanStep1.second] != -1)
			{
				humanStep1 = graph.waitInput();
			}
			graph.showText("");
			updateBoard(humanStep1.first, humanStep1.second, 0);
			if (isGameOver() != -1) break;
			humanStep2 = graph.waitInput();
			while (board[humanStep2.first][humanStep2.second] != -1)
			{
				humanStep2 = graph.waitInput();
			}
			updateBoard(humanStep2.first, humanStep2.second, 0);
			if (isGameOver() != -1) break;
			////////////////
			reversi->handleMessagetest(chess,humanStep1.first, humanStep1.second, humanStep2.first, humanStep2.second,0);
			pair<pair<int, int>, pair<int, int>> AIStep = reversi->steptest(chess);
			AIStep1 = AIStep.first;
			AIStep2 = AIStep.second;
			while (board[AIStep1.first][AIStep1.second] != -1)
			{
				graph.showText("Invalid Step from AI. Generate a random step");
				int row = rand() % 19;
				int col = rand() % 19;
				AIStep1.first = row;
				AIStep1.second = col;
			}
			updateBoard(AIStep1.first, AIStep1.second, 1);
			if (isGameOver() != -1) break;
			while (board[AIStep2.first][AIStep2.second] != -1)
			{
				graph.showText("Invalid Step from AI. Generate a random step");
				int row = rand() % 19;
				int col = rand() % 19;
				AIStep2.first = row;
				AIStep2.second = col;
			}
			updateBoard(AIStep2.first, AIStep2.second, 1);
			if (isGameOver() != -1) break;
			reversi->handleMessagetest(chess, AIStep1.first, AIStep1.second, AIStep2.first, AIStep2.second, 1);

		}
	}
	Sleep(10000);
}

void Game::HumanGame()
{
	graph.showBoard();
	updateBoard(9, 9, 1);
	pair<int, int> humanStep;
	while (1)
	{
		graph.showText("BLACK turns");
		humanStep = graph.waitInput();
		while (board[humanStep.first][humanStep.second] != -1)
		{
			humanStep = graph.waitInput();
		}
		updateBoard(humanStep.first, humanStep.second, 0);
		if (isGameOver() != -1) break;
		humanStep = graph.waitInput();
		while (board[humanStep.first][humanStep.second] != -1)
		{
			humanStep = graph.waitInput();
		}
		updateBoard(humanStep.first, humanStep.second, 0);
		if (isGameOver() != -1) break;

		graph.showText("WHITE turns");
		humanStep = graph.waitInput();
		while (board[humanStep.first][humanStep.second] != -1)
		{
			humanStep = graph.waitInput();
		}
		updateBoard(humanStep.first, humanStep.second, 1);
		if (isGameOver() != -1) break;
		humanStep = graph.waitInput();
		while (board[humanStep.first][humanStep.second] != -1)
		{
			humanStep = graph.waitInput();
		}
		updateBoard(humanStep.first, humanStep.second, 1);
		if (isGameOver() != -1) break;
	}
	Sleep(10000);
}


Game::~Game()
{
	delete reversi;
}