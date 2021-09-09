#include"evaluate.h"
#include<iostream>


using namespace std;

int scoreOfMyRoad[7] = { 0,3,200,250,300,200,1000000 };
int scoreOfEnemyRoad[7] = { 0, 3,240,380,6000,6000,1000000 };

#define MIN_VALUE -10000000
#define ROWS 19
#define COLS 19

movenode* evaluation::searchAGoodMove(chessnode*node) 
{
    evaluation evalution;
//    TranspositionTable.calculateInitHashKey(position);
 //   int baseScore = evalution.wholeevalute(node);
//    node->baseScore = baseScore;
    movenode* bestmove = new movenode;
    bestmove =generatestep(node);
    return bestmove;
  //  this.position = position;
 //   alphaBeta(maxDepth, type, -200000, 200000);
  //  return bestMove;
}

/*int alphabeta(int depth, chessnode* chess, int alpha, int beta)
{






}*/

movenode* evaluation::generatestep(chessnode* chess)
{
    int max = -1000000;
    movenode* bestmove = new movenode;
    evaluation* feva = new evaluation;
    for (int i = 0; i < 19; ++i)
    {
        for (int j = 0; j < 19; ++j)
        {
            if (chess->position[i][j] == 0)
            {
                movenode* move = new movenode;
                move->x1 = i;
                move->y1 = j;
                int temp = feva->evalute(chess, move);
                if (temp > max)
                {
                    bestmove = move;
                    bestmove->score = temp;
                    max = temp;
                }
                /*	if (!examine[i - 2][j - 2]&&chess->position[i - 2][j - 2] == 0)
                    {
                        movenode* move = new movenode;
                        move->x1 = i - 2;
                        move->y1 = j - 2;
                        int temp=feva->evalute(chess,move);
                        if (temp > max)
                        {
                            bestmove = move;
                            max = temp;
                        }
                        examine[i - 2][j - 2] = true;
                    }
                    chess->position[i - 1][j - 1];
                    chess->position[i + 1][j + 1];
                    chess->position[i + 2][j + 2];

                    chess->position[i][j + 1];
                    chess->position[i][j + 2];
                    chess->position[i][j - 1];
                    chess->position[i][j - 2];

                    chess->position[i+1][j];
                    chess->position[i+2][j];
                    chess->position[i-1][j];
                    chess->position[i-2][j];

                    chess->position[i - 1][j + 1];
                    chess->position[i - 2][j + 2];
                    chess->position[i + 1][j - 1];
                    chess->position[i + 2][j - 2];

                }
                else
                    continue;*/
            }
        }
    }
    return bestmove;
}

int evaluation::wholeevalute(chessnode*node) 
{
    int baseScore = MIN_VALUE;
    baseScore = calScoreall(node);
    return baseScore;
}

int evaluation::calScoreall(chessnode* node)
{
    int score = 0;
    int condition = 0;

       for (int i = 1; i < 7; i++)
       {
           node->numberOfEnemyRoad[i] = 0;
           node->numberOfMyRoad[i] = 0;
       }

    wholeanalysisHorizon(node);
    wholeanalysisVertical(node);
    wholeanalysisLeft(node);
    wholeanalysisRight(node);

    for (int i = 1; i < 7; i++) {
        score += (node->numberOfMyRoad[i] * scoreOfMyRoad[i]-node->numberOfEnemyRoad[i] *scoreOfEnemyRoad[i]);
    }

    return score;
}

int evaluation::calScore(chessnode* node, movenode* mnode)
{
    int score = 0;


    analysisHorizon(mnode,node);
    analysisVertical(mnode,node);
    analysisLeft(mnode,node);
    analysisRight(mnode,node);

    for (int i = 1; i < 7; i++) {
        score += (mnode->numberOfMyRoad[i] * scoreOfMyRoad[i] -
            mnode->numberOfEnemyRoad[i] *scoreOfEnemyRoad[i]);
    }

    return score;
}

void evaluation::makeMove(chessnode*node,movenode*move)
{
    node->position[move->x1][move->y1] = node->type;
    return;

}

void evaluation::unMakeMove(chessnode* node, movenode* move)
{
    node->position[move->x1][move->y1] =0;
    return;
}

int evaluation::evalute( chessnode* node, movenode*mnode)
{
    int fScore = 0;
    int bScore = 0;

    for (int i = 1; i < 7; i++)
    {
        mnode->numberOfEnemyRoad[i] = 0;
        mnode->numberOfMyRoad[i] = 0;
    }

    fScore = calScore(node,mnode);
    makeMove(node,mnode);
    for (int i = 1; i < 7; i++)
    {
        mnode->numberOfEnemyRoad[i] = 0;
        mnode->numberOfMyRoad[i] = 0;
    }
    bScore = calScore(node,mnode);
    unMakeMove(node,mnode);

    return bScore - fScore;
}

void evaluation::wholeanalysisVertical(chessnode*node)
{
    for (int i = 0; i <ROWS; i++) {
        for (int j = 0; j < COLS - 5; j++) {
            int number = node->position[i][j] + node->position[i][j + 1] + node->position[i][j + 2]
                + node->position[i][j + 3] + node->position[i][j + 4] + node->position[i][j + 5];
            if (number == 0 || (number > 6 && number % 7!= 0)) {
                continue;
            }
            if (number < 7) {
                if (node->type ==1) {
                    node->numberOfMyRoad[number]++;//对当下全局的状评价
                }
                else {
                    node->numberOfEnemyRoad[number]++;
                }

            }
            else {
                if (node->type ==1) {
                    node->numberOfEnemyRoad[number / 7]++;
                }
                else {
                    node->numberOfMyRoad[number / 7]++;
                }

            }
        }
    }
    return;
}

void evaluation::wholeanalysisHorizon(chessnode* node)
{
    for (int i = 0; i < ROWS-5; i++) {
        for (int j = 0; j < COLS; j++) {
            int number = node->position[i][j] + node->position[i+1][j] + node->position[i+2][j]
                + node->position[i+3][j] + node->position[i+4][j] + node->position[i+5][j];
            if (number == 0 || (number > 6 && number % 7 != 0)) {
                continue;
            }
            if (number < 7) {
                if (node->type == 1) {
                    node->numberOfMyRoad[number]++;//对当下全局的状评价
                }
                else {
                    node->numberOfEnemyRoad[number]++;
                }

            }
            else {
                if (node->type == 1) {
                    node->numberOfEnemyRoad[number / 7]++;
                }
                else {
                    node->numberOfMyRoad[number / 7]++;
                }

            }
        }
    }
    return;
}

void evaluation::wholeanalysisLeft(chessnode* node)
{
    for (int i = 0; i < ROWS - 5; i++) {
        for (int j = 0; j < COLS-5; j++) {
            int number = node->position[i][j] + node->position[i + 1][j+1] + node->position[i + 2][j+2]
                + node->position[i + 3][j+3] + node->position[i + 4][j+4] + node->position[i + 5][j+5];
            if (number == 0 || (number > 6 && number % 7 != 0)) {
                continue;
            }
            if (number < 7) {
                if (node->type == 1) {
                    node->numberOfMyRoad[number]++;//对当下全局的状评价
                }
                else {
                    node->numberOfEnemyRoad[number]++;
                }

            }
            else {
                if (node->type == 1) {
                    node->numberOfEnemyRoad[number / 7]++;
                }
                else {
                    node->numberOfMyRoad[number / 7]++;
                }

            }
        }
    }
    return;
}

void evaluation::wholeanalysisRight(chessnode* node)
{
    for (int i = 5; i < ROWS; i++) {
        for (int j = 0; j < COLS - 5; j++) {
            int number = node->position[i][j] + node->position[i - 1][j + 1] + node->position[i - 2][j + 2]
                + node->position[i - 3][j + 3] + node->position[i - 4][j + 4] + node->position[i - 5][j + 5];
            if (number == 0 || (number > 6 && number % 7 != 0)) {
                continue;
            }
            if (number < 7) {
                if (node->type == 1) {
                    node->numberOfMyRoad[number]++;//对当下全局的状评价
                }
                else {
                    node->numberOfEnemyRoad[number]++;
                }

            }
            else {
                if (node->type == 1) {
                    node->numberOfEnemyRoad[number / 7]++;
                }
                else {
                    node->numberOfMyRoad[number / 7]++;
                }

            }
        }
    }
    return;
}

void evaluation::analysisHorizon(movenode*mnode ,chessnode*node) {
    if (mnode!= NULL) {
        int x = mnode->x1;
        int y = mnode->y1;
        for (int i = x - 5 > 0 ? x - 5 : 0; i <= x && i + 5 < 19; i++) {
            int number = node->position[i][y] + node->position[i + 1][y] + node->position[i + 2][y]
                + node->position[i + 3][y] + node->position[i + 4][y] + node->position[i + 5][y];
            if (number == 0 || (number > 6 && number % 7 != 0)) {
                continue;
            }
            if (number < 7) {
                if (node->type == 1) {
                    mnode->numberOfMyRoad[number]++;
                }
                else {
                    mnode->numberOfEnemyRoad[number]++;
                }

            }
            else {
                if (node->type == 1) {
                    mnode->numberOfEnemyRoad[number/7]++;
                }
                else {
                    mnode->numberOfMyRoad[number/7]++;
                }

            }
        }

    }
 /*   if (mnode!=NULL) {
        int x = mnode->x2;
        int y =mnode->y2;
        for (int i = x - 5 > 0 ? x - 5 : 0; i <= x && i + 5 < 19; i++) {
            int xx=mnode->x1;
            if (i == xx || i + 1 == xx || i + 2 == xx || i + 3 == xx || i + 4 == xx || i + 5
                == xx) {//去重复 
                continue;
            }
            int number = node->position[i][y] + node->position[i + 1][y] + node->position[i + 2][y]
                + node->position[i + 3][y] + node->position[i + 4][y] + node->position[i + 5][y];
            if (number == 0 || (number > 6 && number % 7 != 0)) {
                continue;
            }
            if (number < 7) {
                if (node->type ==1)
                {
                    mnode->numberOfMyRoad[number]++;
                }
                else 
                {
                    mnode->numberOfEnemyRoad[number]++;
                }

            }
            else {
                if (node->type == 1)
                {
                   mnode->numberOfEnemyRoad[number / 7]++;
                }
                else 
                {
                    mnode->numberOfMyRoad[number/7]++;
                }

            }
        }
    }
    */
    return;
}

void evaluation::analysisVertical(movenode* mnode, chessnode* node) {
    if (mnode != NULL) {
        int x = mnode->x1;
        int y = mnode->y1;
        for (int i = y - 5 > 0 ? y - 5 : 0; i <= y && i + 5 < 19; i++) {
            int number = node->position[x][i] + node->position[x][i+1] + node->position[x][i+2]
                + node->position[x][i+3] + node->position[x][i+4] + node->position[x][i+5];
            if (number == 0 || (number > 6 && number % 7 != 0)) {
                continue;
            }
            if (number < 7) {
                if (node->type == 1) {
                    mnode->numberOfMyRoad[number]++;
                }
                else {
                    mnode->numberOfEnemyRoad[number]++;
                }

            }
            else {
                if (node->type == 1) {
                    mnode->numberOfEnemyRoad[number / 7]++;
                }
                else {
                    mnode->numberOfMyRoad[number / 7]++;
                }

            }
        }

    }
   /* if (mnode != NULL) {
        int x = mnode->x2;
        int y = mnode->y2;
        for (int i = y - 5 > 0 ? y - 5 : 0; i <= y && i + 5 < 19; i++) {
            int yy = mnode->y1;
            if (i == yy || i + 1 == yy || i + 2 == yy || i + 3 == yy || i + 4 == yy || i + 5
                == yy) {//去重复 
                continue;
            }
            int number = node->position[x][i] + node->position[x][i + 1] + node->position[x][i + 2]
                + node->position[x][i + 3] + node->position[x][i + 4] + node->position[x][i + 5];
            if (number == 0 || (number > 6 && number % 7 != 0)) {
                continue;
            }
            if (number < 7) {
                if (node->type == 1)
                {
                    mnode->numberOfMyRoad[number]++;
                }
                else
                {
                    mnode->numberOfEnemyRoad[number]++;
                }

            }
            else {
                if (node->type == 1)
                {
                    mnode->numberOfEnemyRoad[number / 7]++;
                }
                else
                {
                    mnode->numberOfMyRoad[number / 7]++;
                }

            }
        }
    }
    */
    return;
}

void evaluation::analysisLeft(movenode* mnode, chessnode* node) {
    if (mnode != NULL) {
        int x = mnode->x1;
        int y = mnode->y1;

        int left1 = 5, right1 = 5;//考虑左侧
        int left2 = 5, right2 = 5;
        int left = 0, right = 0;
        if (x < 5)
            left1 = x;
        if (y < 5)
            left2 = y;

        if (y > 13)
            right1 = 18 - y;
        if (x > 13)
            right2 = 18 - x;

        if (left1 < left2)
            left = left1;
        else
            left = left2;

        if (right1 < right2)
            right = right1;
        else
            right = right2;

        for (int i = -left; i <=right-5;++i) {
            int number = node->position[x+i][y+i] + node->position[x + i+1][y + i+1] + node->position[x + i+2][y + i+2]
                + node->position[x + i+3][y + i+3] + node->position[x + i+4][y + i+4] + node->position[x + i+5][y + i+5];
            if (number == 0 || (number > 6 && number % 7 != 0)) {
                continue;
            }
            if (number < 7) {
                if (node->type == 1) {
                    mnode->numberOfMyRoad[number]++;
                }
                else {
                    mnode->numberOfEnemyRoad[number]++;
                }

            }
            else {
                if (node->type == 1) {
                    mnode->numberOfEnemyRoad[number / 7]++;
                }
                else {
                    mnode->numberOfMyRoad[number / 7]++;
                }

            }
        }

    }
 /*   if (mnode != NULL) {
        int x = mnode->x2;
        int y = mnode->y2;
        for (int i = y - 5 > 0 ? y - 5 : 0; i <= y && i + 5 < 19; i++) {
            int yy = mnode->y1;
            if (i == yy || i + 1 == yy || i + 2 == yy || i + 3 == yy || i + 4 == yy || i + 5
                == yy) {//去重复 
                continue;
            }
            int number = node->position[x][i] + node->position[x][i + 1] + node->position[x][i + 2]
                + node->position[x][i + 3] + node->position[x][i + 4] + node->position[x][i + 5];
            if (number == 0 || (number > 6 && number % 7 != 0)) {
                continue;
            }
            if (number < 7) {
                if (node->type == 1)
                {
                    mnode->numberOfMyRoad[number]++;
                }
                else
                {
                    mnode->numberOfEnemyRoad[number]++;
                }

            }
            else {
                if (node->type == 1)
                {
                    mnode->numberOfEnemyRoad[number / 7]++;
                }
                else
                {
                    mnode->numberOfMyRoad[number / 7]++;
                }

            }
        }
    }
    */
    return;
}

void evaluation::analysisRight(movenode* mnode, chessnode* node) {
    if (mnode != NULL) {
        int x = mnode->x1;
        int y = mnode->y1;
         
        int left1 = 5, right1 = 5;//考虑左侧
        int left2 = 5, right2 = 5;
        int left = 0, right = 0;
        if (x > 13)
            left1 = 18-x;
        if (y < 5)
            left2 = y;
         
        if (y > 13)
            right1 = 18 - y;
        if (x < 5)
            right2 =x;
         
        if (left1 < left2)
            left = left1;
        else
            left = left2;

        if (right1 < right2)
            right = right1;
        else
            right = right2;

        for (int i = -left; i <= right - 5; ++i) {
            int number = node->position[x - i][y + i] + node->position[x - i - 1][y + i + 1] + node->position[x - i - 2][y + i + 2]
                + node->position[x - i - 3][y + i + 3] + node->position[x - i - 4][y + i + 4] + node->position[x - i - 5][y + i + 5];
            if (number == 0 || (number > 6 && number % 7 != 0)) {
                continue;
            }
            if (number < 7) {
                if (node->type == 1) {
                    mnode->numberOfMyRoad[number]++;
                }
                else {
                    mnode->numberOfEnemyRoad[number]++;
                }

            }
            else {
                if (node->type == 1) {
                    mnode->numberOfEnemyRoad[number / 7]++;
                }
                else {
                    mnode->numberOfMyRoad[number / 7]++;
                }

            }
        }

    }
 /*   if (mnode != NULL) {
        int x = mnode->x2;
        int y = mnode->y2;
        for (int i = y - 5 > 0 ? y - 5 : 0; i <= y && i + 5 < 19; i++) {
            int yy = mnode->y1;
            if (i == yy || i + 1 == yy || i + 2 == yy || i + 3 == yy || i + 4 == yy || i + 5
                == yy) {//去重复 
                continue;
            }
            int number = node->position[x][i] + node->position[x][i + 1] + node->position[x][i + 2]
                + node->position[x][i + 3] + node->position[x][i + 4] + node->position[x][i + 5];
            if (number == 0 || (number > 6 && number % 7 != 0)) {
                continue;
            }
            if (number < 7) {
                if (node->type == 1)
                {
                    mnode->numberOfMyRoad[number]++;
                }
                else
                {
                    mnode->numberOfEnemyRoad[number]++;
                }

            }
            else {
                if (node->type == 1)
                {
                    mnode->numberOfEnemyRoad[number / 7]++;
                }
                else
                {
                    mnode->numberOfMyRoad[number / 7]++;
                }

            }
        }
    }
    */
    return;
}