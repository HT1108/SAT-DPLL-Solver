#include "header.h"

void ReadCNF(ClauseNode *&CNFhead, VariableInfo * &varInfo, const char * filename)
{
    extern int varNumTotal;    //变量总数
    extern int clauseNumTotal; //子句总数
    extern int *frequency;
    
    CNFhead = (ClauseNode *)malloc(sizeof(ClauseNode)); //建立CNF头节点
    CNFhead->firstVar = NULL;
    CNFhead->nextClause = NULL;
    CNFhead->varnum = 0;
    CNFhead->flag = 1;

    FILE *fp = NULL;
    char ch = 0;
    int var = -1;

    fp = fopen(filename, "r");

    ch = fgetc(fp);
    while (ch != 'p')
    {
        ch = fgetc(fp);
    }
    fscanf(fp, " cnf%c", &ch);
    fscanf(fp, "%d%d", &varNumTotal, &clauseNumTotal); //读取变量数和子句数

    frequency = (int *)malloc(sizeof(int) * (varNumTotal + 1));
    memset(frequency, 0, sizeof(int) * (varNumTotal + 1));

    varInfo = (VariableInfo *)malloc(sizeof(VariableInfo) * (varNumTotal + 1));
    for (int i = 0; i <= varNumTotal; i++) //变量信息数组初始化
    {
        varInfo[i].headY = (struct inClause *)malloc(sizeof(struct inClause));
        varInfo[i].headY->clause = NULL;
        varInfo[i].headY->nextClause = NULL;
        varInfo[i].headN = (struct inClause *)malloc(sizeof(struct inClause));
        varInfo[i].headN->clause = NULL;
        varInfo[i].headN->nextClause = NULL;
        varInfo[i].val = UNKNOWN;
    }

    ClauseNode *newClause = NULL; //创建子句节点
    ClauseNode *Clausetemp = CNFhead;
    VariableNode *newVar = NULL; //创建变量节点
    VariableNode *Vartemp = NULL;
    struct inClause *temp, *p = NULL;

    for (int i = 0; i < clauseNumTotal; i++)
    {

        newClause = (ClauseNode *)malloc(sizeof(ClauseNode));
        Clausetemp->nextClause = newClause;
        Clausetemp = newClause;
        newClause->firstVar = (VariableNode *)malloc(sizeof(VariableNode)); //创建首个变量节点
        Vartemp = newClause->firstVar;
        Vartemp->flag = 1;      //删除标记设为1
        newClause->nextClause = NULL;
        newClause->varnum = 0;
        newClause->flag = 1;  //删除标记设为1
        fscanf(fp, "%d", &var); //读取子句变量

        frequency[abs(var)]++;

        while (var != 0)
        {
            newClause->varnum++;
            if (newClause->varnum == 1)
            {
                newClause->firstVar->val = var;
                newClause->firstVar->nextVar = NULL;
                if (var > 0) //在VarInfo数组记录var变量出现的子句
                {
                    temp = varInfo[var].headY;
                    while (temp->nextClause)
                    {
                        temp = temp->nextClause;
                    }
                    p = (struct inClause *)malloc(sizeof(struct inClause));
                    p->clause = newClause;
                    p->nextClause = NULL;
                    temp->nextClause = p;
                }
                else if (var < 0)
                {
                    temp = varInfo[-var].headN;
                    while (temp->nextClause)
                    {
                        temp = temp->nextClause;
                    }
                    p = (struct inClause *)malloc(sizeof(struct inClause));
                    p->clause = newClause;
                    p->nextClause = NULL;
                    temp->nextClause = p;
                }
            }
            else
            {
                newVar = (VariableNode *)malloc(sizeof(VariableNode));
                newVar->nextVar = NULL;
                newVar->val = var;
                newVar->flag = 1; //删除标记设为1
                Vartemp->nextVar = newVar;
                Vartemp = newVar;
                if (var > 0) //在VarInfo数组记录var变量出现的子句
                {
                    temp = varInfo[var].headY;
                    while (temp->nextClause)
                    {
                        temp = temp->nextClause;
                    }
                    p = (struct inClause *)malloc(sizeof(struct inClause));
                    p->clause = newClause;
                    p->nextClause = NULL;
                    temp->nextClause = p;
                }
                else if (var < 0)
                {
                    temp = varInfo[-var].headN;
                    while (temp->nextClause)
                    {
                        temp = temp->nextClause;
                    }
                    p = (struct inClause *)malloc(sizeof(struct inClause));
                    p->clause = newClause;
                    p->nextClause = NULL;
                    temp->nextClause = p;
                }
            }
            fscanf(fp, "%d", &var);
            if (var)
            {
                frequency[abs(var)]++;
            }
            
        }
    }

    fclose(fp);
}

/*将读取内容输出，检查CNF文件读取是否正确*/
void CNFcheck(ClauseNode *CNFhead)
{
    ClauseNode *p = CNFhead->nextClause;
    VariableNode *v = p->firstVar;
    while (p)
    {
        if(p->flag)
        {
            v = p->firstVar;
            while (v)
            {
                if (v->flag)
                {
                    cout << v->val << ' ';
                }

                v = v->nextVar;
            }
            cout << '0';
            cout << endl;
        }
        p = p->nextClause;
    }
}

void WriteAnswer(int result, int ans[], clock_t start, clock_t end)
{
    //extern char ans[];
    extern int varNumTotal;
    
    FILE *fp = NULL;
    fp = fopen("ans.res", "w");
    if (!result)
    {
        fprintf(fp, "s 0");
        fprintf(fp, "\nt %.1lf", (double)(end - start) / CLOCKS_PER_SEC * 1000.0);
        return;
    }
    fprintf(fp, "s 1\nv ");

    for (int i = 1; i <= varNumTotal; i++)
    {
        if (ans[i] == 1)
        {
            fprintf(fp, "%d ", i);
        }
        else if (ans[i] == 0)
        {
            fprintf(fp, "%d ", -i);
        }
    }
    fprintf(fp, "\nt %.1lf", (double)(end - start) / CLOCKS_PER_SEC * 1000.0);
    fclose(fp);
}