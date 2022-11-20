#ifndef GLOBAL
#define GLOBAL
#define TRUE 1
#define FALSE 0
#define ERROR 1
#define OK 0
#define UNKNOWN -1
#define MAX_SIZE 10000000
#define ROW -1
#define COL -2
#define BLK -3
#define SAT 1
#define SUDOKU 2
#define CHECK 3

using namespace std;


enum Operation          //表示操作种类
{
    DEL_CLAUSE,         //删除子句
    DEL_LITERAL,        //删除文字
    VAR_ASSIGN,         //变量赋值
    ADD_SINGLECLAUSE    //添加单子句
};

typedef struct variableNode //变量节点
{
    int val;
    int flag;                //标记变量节点是否被删除：0 删除  1 未删除
    struct variableNode *nextVar;
} VariableNode;

typedef struct clauseNode
{
    int varnum;                    //子句的变量数
    int flag;
    struct variableNode *firstVar; //当前子句首个变量
    struct clauseNode *nextClause; //下一子句
} ClauseNode;

typedef struct inClause
{
    ClauseNode *clause;
    struct inClause *nextClause;
} InClause;

typedef struct variableInfo
{
    int val;                //变量真值
    struct inClause *headY; //出现该变量正文字的子句
    struct inClause *headN; //出现该变量负文字的子句
} VariableInfo;

typedef struct operationRecord
{
    enum Operation op;              //操作的种类：
    struct clauseNode *clause;      //子句地址
    struct variableNode *variable;  //变量地址
    int var;                        //赋值的变量
    int depth;                      //决策树深度
}OperationRecord;


#include <conio.h>
#include <stdio.h>
#include <iostream>
#include <cstdlib>
#include <cmath>
#include <string.h>
#include <time.h>

#include "DPLL.cpp"
#include "CNFparser.cpp"
#include "SATsolver.cpp"

#include "Sudoku.cpp"


OperationRecord stack[MAX_SIZE];
int top = 0;
int nowDepth = 1;//当前决策层深度
int varNumTotal;
int clauseNumTotal;
int knownVar = 0;
int *frequency;
int ans[50000];
ClauseNode *CNFhead;
VariableInfo *varInfo;

#endif