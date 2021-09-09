#define _CRT_SECURE_NO_WARNINGS
#include "Reversi.h"
#include<iostream>
#include<fstream>
#include<iomanip>
#include"evaluate.h"

using namespace std;

int roundnum;
int stepnum;

#define random(x) (rand()%x)
#define ROWS 19
#define COLS 19
#define ROUNDS 3

chessnode chess;

Reversi::Reversi(){
    client_socket = ClientSocket();
    oppositeColor = ownColor = -1;
}

Reversi::~Reversi(){}
void Reversi::setOwnColor(int color)
{
	if (color != 0 || color != 1)
		return;
	ownColor = color;
	oppositeColor = 1 - color;
}
;

/*
 send id and password to server by socket
 rtn != 0 represents socket transfer error
 */
void Reversi::authorize(const char *id , const char *pass)
{
    client_socket.connectServer();
    std::cout << "Authorize " << id << std::endl;
    char msgBuf[BUFSIZE];
    memset(msgBuf , 0 , BUFSIZE);
    msgBuf[0] = 'A';
    memcpy(&msgBuf[1] , id , 9);
    memcpy(&msgBuf[10] , pass , 6);
    int rtn = client_socket.sendMsg(msgBuf);
    if (rtn != 0) printf("Authorized Failed!\n");
}

// 用户id输入，服务器上需要有对应的账号密码：对应文件 players-0.txt
void Reversi::gameStart()
{
    char id[12] = {0}, passwd[10] = {0};
	strcpy(id, ID);
	strcpy(passwd, PASSWORD);
    authorize(id, passwd);
    
    printf("Game Start!\n");
    
    for (int round = 0 ; round < ROUNDS ; round++){
		chess.baseScore = 0;
		for (int i = 0; i < 19; ++i)
		{
			for (int j = 0; j < 19; ++j)
				chess.position[i][j] = 0;
		}
		chess.position[9][9] = 7;
		chess.number=0;
		roundnum = round;
        roundStart(round);
        oneRound();
        roundOver(round);
    }
    gameOver();
    client_socket.close();
}

void Reversi::gameOver()
{
    printf("Game Over!\n");
}

//发一次消息，走哪一步，等两个消息，1.自己的步数行不行 2.对面走了哪一步 
void Reversi::roundStart(int round)
{
    printf("Round %d Ready Start!\n" , round);
    
    // first time receive msg from server
    int rtn = client_socket.recvMsg();
    if (rtn != 0) return ;
    if(strlen(client_socket.getRecvMsg()) < 2)
        printf("Authorize Failed!\n");
    else
        printf("Round start received msg %s\n", client_socket.getRecvMsg());
    switch (client_socket.getRecvMsg()[1]) {
            // this client : black chessman
        case 'B':
            ownColor = 0;
            oppositeColor = 1;
            rtn = client_socket.sendMsg("BB");
            printf("Send BB -> rtn: %d\n", rtn);

			chess.type = 1;

            if (rtn != 0) return ;
            break;
        case 'W':
            ownColor = 1;
            oppositeColor = 0;
            rtn = client_socket.sendMsg("BW");
            printf("Send BW -> rtn: %d\n", rtn);

			chess.type = 7;

            if (rtn != 0) return ;
            break;
        default:
            printf("Authorized Failed!\n");
            break;
    }
}

void Reversi::oneRound()
{
    int STEP = 1;
    switch (ownColor) {
        case 0:
            while (STEP < 10000) {
                
				pair<pair<int, int>, pair<int, int>> chess = step();                        // take action, send message
                
                // lazi only excute after server's message confirm  in observe function
                generateOneStepMessage(chess.first.first, chess.first.second, chess.second.first, chess.second.second);
                
                
				if (observe() >= 1)
				{
					saveChessBoard();
					break;     // receive RET Code

				}
				if (observe() >= 1)
				{
					saveChessBoard();
					break;    // see white move
				}
                saveChessBoard();
				STEP++;      
            }
            printf("One Round End\n");
            break;
        case 1:
            while (STEP < 10000) {
                
				if (observe() >= 1)
				{
					saveChessBoard();
					break;    // see black move
				}
                
				pair<pair<int, int>, pair<int, int>> chess = step();                        // take action, send message
                // lazi only excute after server's message confirm  in observe function
                generateOneStepMessage(chess.first.first,chess.first.second,chess.second.first, chess.second.second);
                
                
				if (observe() >= 1)
				{
					saveChessBoard();
					break;     // receive RET Code
				}
                 saveChessBoard();
                STEP++;
            }
            printf("One Round End\n");
            break;
            
        default:
            break;
    }
}

void Reversi::roundOver(int round)
{
    printf("Round %d Over!\n", round);
    // reset initializer

    ownColor = oppositeColor = -1;
}

int Reversi::observe()
{
	int rtn = 0;
	int recvrtn = client_socket.recvMsg();
	if (recvrtn != 0) return 1;
	printf("receive msg %s\n", client_socket.getRecvMsg());
	switch (client_socket.getRecvMsg()[0]) {
	case 'R':
	{
		switch (client_socket.getRecvMsg()[1]) {
		case 'Y':   // valid step
			switch (client_socket.getRecvMsg()[2]) {
			case 'P':   // update chessboard
			{
				int desRow1 = (client_socket.getRecvMsg()[3] - '0') * 10 + client_socket.getRecvMsg()[4] - '0';
				int desCol1 = (client_socket.getRecvMsg()[5] - '0') * 10 + client_socket.getRecvMsg()[6] - '0';
				int desRow2 = (client_socket.getRecvMsg()[7] - '0') * 10 + client_socket.getRecvMsg()[8] - '0';
				int desCol2 = (client_socket.getRecvMsg()[9] - '0') * 10 + client_socket.getRecvMsg()[10] - '0';
				int color = (client_socket.getRecvMsg()[11] - '0');
				//你应该在这里处理desRow和desCol，推荐使用函数
				handleMessage(desRow1, desCol1, desRow2, desCol2, color);

				printf("a valid step of : (%d %d) (%d %d)\n", desRow1, desCol1, desRow2, desCol2);
				break;
			}
			case 'N':   // R0N: enemy wrong step
			{
				//
				printf("a true judgement of no step\n");
				break;
			}
			}

			break;
		case 'W':
			// invalid step
			switch (client_socket.getRecvMsg()[2]) {
			case 'P': {
				int desRow1 = (client_socket.getRecvMsg()[3] - '0') * 10 + client_socket.getRecvMsg()[4] - '0';
				int desCol1 = (client_socket.getRecvMsg()[5] - '0') * 10 + client_socket.getRecvMsg()[6] - '0';
				int desRow2 = (client_socket.getRecvMsg()[7] - '0') * 10 + client_socket.getRecvMsg()[8] - '0';
				int desCol2 = (client_socket.getRecvMsg()[9] - '0') * 10 + client_socket.getRecvMsg()[10] - '0';
				int color = (client_socket.getRecvMsg()[11] - '0');
				printf("Invalid step , server random a true step of : (%d %d) (%d %d)\n", desRow1, desCol1, desRow2, desCol2);
				//你应该在这里处理desRow和desCol，推荐使用函数
				handleMessage(desRow1, desCol1, desRow2, desCol2, color);
				break;
			}
			case 'N': {
				printf("a wrong judgement of no step\n");
				break;
			}
			default:
				break;
			}
			break;
		case '1':

			printf("Error -1: Msg format error!\n");
			rtn = -1;
			break;
		case '2':

			printf("Error -2: Corrdinate error!\n");
			rtn = -2;
			break;
		case '4':

			printf("Error -4: Invalid step!\n");
			rtn = -4;
			break;
		default:

			printf("Error -5: Other error!\n");
			rtn = -5;
			break;
		}
		break;
	}
	case 'E':
	{
		switch (client_socket.getRecvMsg()[1]) {
		case '0':
			// game over
			rtn = 2;
			break;
		case '1':
			// round over
			rtn = 1;
		default:
			break;
		}
		break;
	}
	default:
		break;
	}
	return rtn;
}

void Reversi::generateOneStepMessage(int row1, int col1, int row2, int col2)
{
	char msg[BUFSIZE];
	memset(msg, 0, sizeof(msg));

	//put row and col in the message
	msg[0] = 'S';
	msg[1] = 'P';
	msg[2] = '0' + row1 / 10;
	msg[3] = '0' + row1 % 10;
	msg[4] = '0' + col1 / 10;
	msg[5] = '0' + col1 % 10;
	msg[6] = '0' + row2 / 10;
	msg[7] = '0' + row2 % 10;
	msg[8] = '0' + col2 / 10;
	msg[9] = '0' + col2 % 10;
	msg[10] = '\0';

	//print
	printf("generate one step at possition (%2d,%2d,%2d,%2d) : %s\n", row1, col1, row2, col2, msg);


	client_socket.sendMsg(msg);
}

/*-------------------------last three function--------------------------------
* step : find a good position to lazi your chess.
* saveChessBoard : save the chess board now.
* handleMessage: handle the message from server.
*/

pair<pair<int, int>, pair<int, int>> Reversi::step()
{
	movenode* p1 = new movenode;
	movenode* p2 = new movenode;
	//TODO:Generate a valid step here
	if (chess.number == 0)
	{
		for (int i = 0; i <= 1; ++i)
		{
			if (chess.position[8][8] == 0)
			{
				movenode* p = new movenode;
				p->x1 = 8;
				p->y1 = 8;
				if (i == 0)
					p1 = p;
				else
					p2 = p;
				chess.position[8][8] = chess.type;
			}
			else if (chess.position[8][10] == 0)
			{
				movenode* p = new movenode;
				p->x1 = 8;
				p->y1 = 10;
				if (i == 0)
					p1 = p;
				else
					p2 = p;
				chess.position[8][10] = chess.type;
			}
			else if (chess.position[10][8] == 0)
			{
				movenode* p = new movenode;
				p->x1 = 10;
				p->y1 = 8;
				if (i == 0)
					p1 = p;
				else
					p2 = p;
				chess.position[10][8] = chess.type;
			}
			else if (chess.position[10][10] == 0)
			{
				movenode* p = new movenode;
				p->x1 = 10;
				p->y1 = 10;
				if (i == 0)
					p1 = p;
				else
					p2 = p;
				chess.position[10][10] = chess.type;
			}
		}
		chess.number++;
	}
	else
	{
		evaluation* feva = new evaluation;
		p1=feva->searchAGoodMove(&chess);
		chess.position[p1->x1][p1->y1] = chess.type;
		p2 = feva->searchAGoodMove(&chess);
		chess.position[p2->x1][p2->y1] = chess.type;
	}
	pair<int, int> step1 = make_pair(p1->x1, p1->y1);
	pair<int, int> step2 = make_pair(p2->x1, p2->y1);
	return make_pair(step1, step2);
}

pair<pair<int, int>, pair<int, int>> Reversi::steptest(chessnode&chess)
{
	movenode* p1 = new movenode;
	movenode* p2 = new movenode;
	//TODO:Generate a valid step here
	if (chess.number == 0)
	{
		for (int i = 0; i <= 1; ++i)
		{
			if (chess.position[8][8] == 0)
			{
				movenode* p = new movenode;
				p->x1 = 8;
				p->y1 = 8;
				if (i == 0)
					p1 = p;
				else
					p2 = p;
				chess.position[8][8] = chess.type;
			}
			else if (chess.position[8][10] == 0)
			{
				movenode* p = new movenode;
				p->x1 = 8;
				p->y1 = 10;
				if (i == 0)
					p1 = p;
				else
					p2 = p;
				chess.position[8][10] = chess.type;
			}
			else if (chess.position[10][8] == 0)
			{
				movenode* p = new movenode;
				p->x1 = 10;
				p->y1 = 8;
				if (i == 0)
					p1 = p;
				else
					p2 = p;
				chess.position[10][8] = chess.type;
			}
			else if (chess.position[10][10] == 0)
			{
				movenode* p = new movenode;
				p->x1 = 10;
				p->y1 = 10;
				if (i == 0)
					p1 = p;
				else
					p2 = p;
				chess.position[10][10] = chess.type;
			}
		}
		chess.number++;
	}
	else
	{
		evaluation* feva = new evaluation;
		p1 = feva->searchAGoodMove(&chess);
		if (p1->x1 >= 0 && p1->x1 <= 19)
		{
			chess.position[p1->x1][p1->y1] = chess.type;
			p2 = feva->searchAGoodMove(&chess);
			if (p2->x1 >= 0 && p2->x1 <= 19)
				chess.position[p2->x1][p2->y1] = chess.type;
			else
			{
				p2->x1 = -1;
				p2->y1 = -1;
			}
		}
		else
		{
			p1->x1 = -1;
			p1->y1 = -1;
			p2->x1 = -1;
			p2->y1 = -1;
		}
	}
	pair<int, int> step1 = make_pair(p1->x1, p1->y1);
	pair<int, int> step2 = make_pair(p2->x1, p2->y1);
	return make_pair(step1, step2);
}

void Reversi::saveChessBoard()
{
	if (roundnum == 0&&stepnum==2)
	{
		ofstream myfile("saveChessBoard.txt");
		if (!myfile.is_open())
		{
			cout << "Open Failed!" << endl;
			return;
		}
		myfile << "Round" << roundnum << ":" << endl;
		myfile << "--------------------------------------" << endl;
		for (int i = 0; i < 19; ++i)
		{
			for (int j = 0; j < 19; ++j)
			{
				if (chess.position[i][j] == 1)
				{
					myfile << left << setw(2) << "|B|";
				}
				else if (chess.position[i][j] == 7)
				{
					myfile << left << setw(2) << "|P|";
				}
				else
					myfile << left << setw(3) << "|  |";
			}
			myfile << "|" << endl;
		}
		myfile << "--------------------------------------" << endl;
		myfile.close();
	}
	else
	{
		ofstream myfile("saveChessBoard.txt", ios::app);
		if (!myfile.is_open())
		{
			cout << "Open Failed!" << endl;
			return;
		}
		myfile << "Round" << roundnum << ":" << endl;
		myfile << "--------------------------------------" << endl;
		for (int i = 0; i < 19; ++i)
		{
			for (int j = 0; j < 19; ++j)
			{
				if (chess.position[i][j] == 1)
				{
					myfile << left << setw(2) << "|B|";
				}
				else if (chess.position[i][j] == 7)
				{
					myfile << left << setw(2) << "|P|";
				}
				else
					myfile << left << setw(3) << "|  |";
			}
			myfile << "|" << endl;
		}
		myfile << "--------------------------------------" << endl;
		myfile.close();
	}
	return;
}

void Reversi::handleMessage(int row1, int col1, int row2, int col2, int color) {
	if (color == 0)
	{
		chess.position[row1][col1] = 1;
		chess.position[row2][col2] = 1;
	}
	else
	{
		chess.position[row1][col1] = 7;
		chess.position[row2][col2] = 7;
	}
	stepnum++;
	return;
}


void Reversi::handleMessagetest(chessnode& chess,int row1, int col1, int row2, int col2, int color) {
	if (color == 0)
	{
		chess.position[row1][col1] = 1;
		chess.position[row2][col2] = 1;
	}
	else
	{
		chess.position[row1][col1] = 7;
		chess.position[row2][col2] = 7;
	}
	stepnum++;
	return;
}
