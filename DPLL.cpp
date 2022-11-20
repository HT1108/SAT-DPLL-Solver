#include "header.h"

void DestroyCNF(ClauseNode *&CNFhead, VariableInfo *&varInfo) //删除CNF
{
    ClauseNode *p = CNFhead, *temp = NULL;
    VariableNode *var = NULL, *tempVar = NULL;
    extern int varNumTotal;
    while (CNFhead->nextClause) //删除子句节点
    {
        temp = CNFhead->nextClause;

        var = temp->firstVar;

        while (var) //删除变量节点
        {
            tempVar = var->nextVar;
            free(var);
            temp->firstVar = tempVar;
            var = temp->firstVar;
        }

        CNFhead->nextClause = temp->nextClause;
        free(temp);
    }
    free(CNFhead);

    InClause *v = NULL;
    InClause *vtemp = NULL;
    for (int i = 0; i <= varNumTotal; i++) //删除变量信息数组
    {
        v = varInfo[i].headY;
        vtemp = v->nextClause;
        while (v->nextClause)
        {
            v->nextClause = vtemp->nextClause;
            free(vtemp);
            vtemp = v->nextClause;
        }
        free(v);

        v = varInfo[i].headN;
        vtemp = v->nextClause;
        while (v->nextClause)
        {
            v->nextClause = vtemp->nextClause;
            free(vtemp);
            vtemp = v->nextClause;
        }
        free(v);
    }
    free(varInfo);
    varInfo = NULL;
    CNFhead = NULL;
}


void AddSingleClause(int val)
{
    if (!val)
    {
        return;
    }
    extern ClauseNode *CNFhead;
    extern OperationRecord stack[];
    extern VariableInfo *varInfo;
    extern int top, nowDepth, *frequency;

    ClauseNode *temp = CNFhead->nextClause;
    ClauseNode *newClause = NULL;
    newClause = (ClauseNode *)malloc(sizeof(ClauseNode));//新建子句节点
    newClause->flag = 1;
    newClause->varnum = 1;
    newClause->varnum = 1;
    newClause->nextClause = temp;
    CNFhead->nextClause = newClause;

    newClause->firstVar = (VariableNode *)malloc(sizeof(VariableNode));//新建变量节点
    newClause->firstVar->flag = 1;
    newClause->firstVar->nextVar = NULL;
    newClause->firstVar->val = val;

    InClause *inClause = (InClause *)malloc(sizeof(InClause));//在varInfo中记录变量出现的子句
    inClause->clause = newClause;
    if(val > 0)
    {
        inClause->nextClause = varInfo[val].headY->nextClause;
        varInfo[val].headY->nextClause = inClause;
    }
    else
    {
        inClause->nextClause = varInfo[-val].headN->nextClause;
        varInfo[-val].headN->nextClause = inClause;
    
    }
    frequency[abs(val)]++;
    stack[++top].op = ADD_SINGLECLAUSE;//记录操作以回溯
    stack[top].clause = newClause;
    stack[top].depth = nowDepth;
    stack[top].variable = NULL;
}

int IsSingleClause(ClauseNode *cl)
{
    if(cl->flag == 0)
    {
        return FALSE;
    }
    VariableNode *v = cl->firstVar;
    int num = 0;
    if (cl == NULL)
        return FALSE;
    if (cl->firstVar != NULL)
    {
        while(v)
        {
            if(v->flag)
            {
                num++;
            }
            v = v->nextVar;
        }
    }
    if(num == 1)
    {
        return TRUE;
    }
    return FALSE;
}

void DeleteClause(ClauseNode *cl)
{
    if (cl->flag == 0)
    {
        return;
    }
    extern OperationRecord stack[];
    extern int top, nowDepth, *frequency;
    VariableNode *v = cl->firstVar;
    while(v)
    {
        if (v->flag)
        {
            frequency[abs(v->val)]--;
        }
        v = v->nextVar;
    }
    cl->flag = 0;
    stack[++top].op = DEL_CLAUSE;
    stack[top].clause = cl;
    stack[top].variable = NULL;
    stack[top].depth = nowDepth;

}

int IsVarInClause(int val, ClauseNode *cl)
{
    if (cl == NULL)
    {
        return FALSE;
    }
    VariableNode *var = cl->firstVar;
    while (var)
    {
        if (var->val == val && var->flag)
        {
            return TRUE;
        }
        var = var->nextVar;
    }
    return FALSE;
}

int DeleteLiteral(ClauseNode *cl, int val) //val表示变量文字，可正可负
{
    if(cl == NULL || cl->flag == 0)
    {
        return ERROR;
    }
    extern OperationRecord stack[];
    extern VariableInfo *varInfo;
    extern int top, nowDepth, *frequency;
    VariableNode *v = cl->firstVar;
    while(IsVarInClause(val, cl))
    {
        cl->varnum--;
        v = cl->firstVar;
        while (v)
        {
            if (v->val == val && v->flag)
            {
                v->flag = 0;
                stack[++top].op = DEL_LITERAL;
                stack[top].variable = v;
                stack[top].clause = cl;
                stack[top].depth = nowDepth;

                frequency[abs(v->val)]--;
            }
            v = v->nextVar;
        }
    }
    return OK;
}


void DeleteVariable(int var)//var为变量编号，正值
{
    extern ClauseNode *CNFhead;
    extern VariableInfo *varInfo;
    InClause *p = NULL;
    switch(varInfo[var].val)
    {
        case TRUE:
            p = varInfo[var].headY->nextClause;
            while(p)
            {
                DeleteClause(p->clause);
                p = p->nextClause;
            }

            p = varInfo[var].headN->nextClause;
            while(p)
            {
                DeleteLiteral(p->clause, -var);
                p = p->nextClause;
            }
            break;

        case FALSE:
            p = varInfo[var].headN->nextClause;
            while (p)
            {
                DeleteClause(p->clause);
                p = p->nextClause;
            }

            p = varInfo[var].headY->nextClause;
            while (p)
            {
                DeleteLiteral(p->clause, var);
                p = p->nextClause;
            }
            break;
    }
}

void VariableAssign(int var, int val)
{
    extern VariableInfo *varInfo;
    extern OperationRecord stack[];
    extern int top, knownVar;
    extern int nowDepth;

    varInfo[var].val = val;
    knownVar++;
    stack[++top].op = VAR_ASSIGN;
    stack[top].var = var;
    stack[top].depth = nowDepth;
    stack[top].clause = NULL;
    stack[top].variable = NULL;
}

ClauseNode *FindSingleClause(ClauseNode *CNFhead)
{
    
    ClauseNode *p = CNFhead;
    while (p)
    {
        if (p->flag && IsSingleClause(p))
        {
            return p;
        }
        p = p->nextClause;
    }
    return NULL;
}



int IsEmptyClause(clauseNode *cl)//判断子句是否为空子句
{
    VariableNode *var = cl->firstVar;
    int varExist = 0;
    while(var)
    {
        if(var->flag)
        {
            varExist++;
        }
        var = var->nextVar;
    }
    if(varExist > 0)
    {
        return FALSE;
    }
    else
    {
        return TRUE;
    }
}

int FindEmptyClause()//查找CNF中是否存在空子句
{
    extern ClauseNode *CNFhead;
    ClauseNode *cl = CNFhead->nextClause;
    VariableNode *var = NULL;
    int numDeleted;
    while(cl)
    {
        if(IsEmptyClause(cl))
        {
            return TRUE;
        }
        cl = cl->nextClause;
    }
    return FALSE;
}

void RestoreCNF()       //撤销当前决策层对CNF的修改实现回溯
{
    extern ClauseNode *CNFhead;
    extern VariableInfo *varInfo;
    extern OperationRecord stack[];
    extern int nowDepth,top, knownVar;
    OperationRecord p;
    ClauseNode *cl = NULL, *temp = NULL;
    InClause *inClause = NULL, *tempC = NULL;
    p = stack[top];
    while(p.depth == nowDepth && top)
    {
        top--;
        switch (p.op)
        {
        case DEL_CLAUSE:
            p.clause->flag = 1;
            
            break;

        case DEL_LITERAL:
            p.clause->varnum++;
            p.variable->flag = 1;
            break;

        case VAR_ASSIGN:
            varInfo[p.var].val = UNKNOWN;
            knownVar--;
            break;

        case ADD_SINGLECLAUSE:
            cl = CNFhead;
            while(cl->nextClause != p.clause)
            {
                cl = cl->nextClause;
            }
            temp = p.clause;
            cl->nextClause = p.clause->nextClause;
            if (p.clause->firstVar->val > 0)
                inClause = varInfo[abs(p.clause->firstVar->val)].headY;
            else if (p.clause->firstVar->val < 0)
                inClause = varInfo[abs(p.clause->firstVar->val)].headN;
            
            while(inClause->nextClause->clause!= p.clause)
            {
                inClause = inClause->nextClause;
            }
            tempC = inClause->nextClause;
            inClause->nextClause = inClause->nextClause->nextClause;
            free(tempC);
            free(p.clause->firstVar);
            free(p.clause);

            break;
        }
        p = stack[top];
    }
    nowDepth--;
}

int ChooseMax()//统计并返回出现频率最高的变量
{
    extern ClauseNode *CNFhead;
    extern VariableInfo *varInfo;
    extern int varNumTotal;
    extern int *frequency;
    int max;
    for (int i = 0; i <= varNumTotal; i++)
    {
        if (frequency[i] > frequency[max] && varInfo[i].val == UNKNOWN)
        {
            max = i;
        }
    }
    return max;
}

int GetNearest()//选择第一个子句的第一个变量
{
    extern ClauseNode *CNFhead;
    extern VariableInfo *varInfo;
    ClauseNode *p = CNFhead->nextClause;
    VariableNode *v = p->firstVar;
    while (p)
    {
        if (p->flag)
        {
            v = p->firstVar;
            while (v)
            {
                if (v->flag && varInfo[abs(v->val)].val == UNKNOWN)
                {
                    return abs(v->val);
                }

                v = v->nextVar;
            }
        }
        p = p->nextClause;
    }
    return 0;
}



int IsCNFEmpty()
{
    extern ClauseNode *CNFhead;
    ClauseNode *p = CNFhead->nextClause;
    while(p)
    {
        if (p->flag)
        {
            return FALSE;
        }
        p = p->nextClause;
    }
    return TRUE;
}

int DPLL()//递归的DPLL算法
{
    extern ClauseNode *CNFhead;
    extern VariableInfo *varInfo;
    extern OperationRecord stack[];
    extern int nowDepth, top, varNumTotal, knownVar;
    extern int ans[];
    int temp = 0, var = 0;
    ClauseNode *singleClasue = NULL;
    VariableNode *v = NULL;
    singleClasue = FindSingleClause(CNFhead);
    while(singleClasue)
    {
        v = singleClasue->firstVar;
        while(v)
        {
            if (v->flag)
            {
                temp = v->val;
            }
            v = v->nextVar;
        }
        VariableAssign(abs(temp), temp > 0 ? TRUE : FALSE);
        DeleteClause(singleClasue);
        DeleteVariable(abs(temp));
        if (IsCNFEmpty())
        {
            if (knownVar == varNumTotal)
            {
                for (int i = 0; i <= varNumTotal; i++)
                {
                    ans[i] = varInfo[i].val;
                }
            }
            knownVar++;
            return TRUE;
        }
        if (FindEmptyClause())
        {
            
            return FALSE;
        }
        singleClasue = FindSingleClause(CNFhead);
    }

    var = GetNearest(); // VariableDecide()返回0时所有变量都被赋值

    nowDepth++;
    AddSingleClause(var);
    if (DPLL())
    {
        if (knownVar == varNumTotal)
        {
            for (int i = 0; i <= varNumTotal; i++)
            {
                ans[i] = varInfo[i].val;
            }
            knownVar++;
        }

        RestoreCNF();
        return TRUE;
    }
    else
    {
        RestoreCNF();
        nowDepth++;
        AddSingleClause(-var);
        if (DPLL())
        {
            if (knownVar == varNumTotal)
            {
                for (int i = 0; i <= varNumTotal; i++)
                {
                    ans[i] = varInfo[i].val;
                }
            }
            RestoreCNF();
            return TRUE;
        }
        else
        {
            RestoreCNF();
            return FALSE;
        }
    }
}





int NonRecursiveDPLL()//非递归DPLL算法
{
    extern ClauseNode *CNFhead;
    extern VariableInfo *varInfo;
    extern OperationRecord stack[];//记录操作的栈
    extern int nowDepth, top, varNumTotal, knownVar;
    extern int ans[];
    int temp = 0, var = 0, T = 0;//T:栈顶指针  var:当前选中赋值变量
    ClauseNode *singleClasue = NULL;
    VariableNode *v = NULL;
    int St[50000] = { 0 };//存储决策树节点的栈（模拟递归）
    

    singleClasue = FindSingleClause(CNFhead);//第一轮化简
    while (singleClasue)
    {
        v = singleClasue->firstVar;
        while (v)
        {
            if (v->flag)
            {
                temp = v->val;
            }
            v = v->nextVar;
        }
        VariableAssign(abs(temp), temp > 0 ? TRUE : FALSE);
        DeleteClause(singleClasue);
        DeleteVariable(abs(temp));
        if (IsCNFEmpty())
        {
            for (int i = 0; i <= varNumTotal; i++)
            {
                if (varInfo[i].val == -1)
                {
                    ans[i] = 1;
                }
                else
                {
                    ans[i] = varInfo[i].val;
                }
            }

            return TRUE;
        }
        if (FindEmptyClause())
        {
            return FALSE;
        }
        singleClasue = FindSingleClause(CNFhead);
    }

    var = GetNearest(); //选取第一个变量

    St[++T] = var;
    nowDepth++;
    AddSingleClause(var);

    while(1)
    {
        singleClasue = FindSingleClause(CNFhead);//搜索真值为TRUE的分支
        while (singleClasue)
        {
            v = singleClasue->firstVar;
            while (v)
            {
                if (v->flag)
                {
                    temp = v->val;
                }
                v = v->nextVar;
            }
            VariableAssign(abs(temp), temp > 0 ? TRUE : FALSE);
            DeleteClause(singleClasue);
            DeleteVariable(abs(temp));
            if (IsCNFEmpty())
            {

                for (int i = 0; i <= varNumTotal; i++)
                {
                    if (varInfo[i].val == -1)
                    {
                        ans[i] = 1;
                    }
                    else
                    {
                        ans[i] = varInfo[i].val;
                    }
                }
                    
                return TRUE;
            }
            if (FindEmptyClause())//不符合条件
            {
                break;
            }
            singleClasue = FindSingleClause(CNFhead);
        }

        if (FindEmptyClause())
        {
            while (varInfo[St[T]].val != UNKNOWN)
            {
                RestoreCNF();
            }

            temp = St[T--]; //不符合要求的猜测值出栈
            St[T + 1] = 0;

            nowDepth++;     //进入下一层
            AddSingleClause(-temp);
            singleClasue = FindSingleClause(CNFhead); //搜索真值为FALSE的分支
            while (singleClasue)
            {
                v = singleClasue->firstVar;
                while (v)//获取单子句中的变量值
                {
                    if (v->flag)
                    {
                        temp = v->val;
                    }
                    v = v->nextVar;
                }
                VariableAssign(abs(temp), temp > 0 ? TRUE : FALSE);
                DeleteClause(singleClasue);
                DeleteVariable(abs(temp));
                if (IsCNFEmpty())
                {
                    for (int i = 0; i <= varNumTotal; i++)
                    {
                        if (varInfo[i].val == -1)
                        {
                            ans[i] = 1;
                        }
                        else
                        {
                            ans[i] = varInfo[i].val;
                        }
                    }
                    return TRUE;
                }
                if (FindEmptyClause())
                {
                    if(T == 0)
                    {
                        return FALSE;
                    }
                    else
                    {
                        while(varInfo[St[T]].val != UNKNOWN)
                        {
                            RestoreCNF();
                        }
                        goto down;
                    }
                    
                }
                singleClasue = FindSingleClause(CNFhead);
            }

        }

        var = GetNearest(); //选取变量
        St[++T] = var;
        nowDepth++;
        AddSingleClause(var);
        continue;

        down:
            var = St[T--]; //不符合要求的猜测值出栈
            St[T + 1] = 0;
            nowDepth++; //进入下一层
            AddSingleClause(-var);
            if (T == 0)
            {
                return 0;
            }
    }

}
