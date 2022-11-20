#include "header.h"

int GenerateRandomArray(int arr[])
{
    int rec[10];
    memset(arr, 0, 40);
    memset(rec, 0, 40);
    for(int a = 0; a < 50000000; a++);//这句用来拖时间
    srand((unsigned int)clock());
    int num = 0;
    for (int i = 1; i <= 9; i++)
    {
        num = rand() % 9 + 1;
        while(rec[num])
        {
            num = rand() % 9 + 1;
        }
        rec[num] = 1;
        arr[i] = num;
    }
    return 0;
}

int VarToSudoku(int var)
{
    int i = 0, j = 0, n = 0, m = 0, temp = abs(var);
    m = (temp - 1) / 729 + 1;
    temp = temp - (m - 1) * 729;
    i = (temp - 1) / 81 + 1;
    temp = temp - (i - 1) * 81;
    j = (temp - 1) / 9 + 1;
    n = temp - (j - 1) * 9;
    return var > 0 ? (m * 1000 + i * 100 + j * 10 + n) : -(m * 1000 + i * 100 + j * 10 + n);
}

int SudokuToVar(int sud)
{

    int i, j, n, m, temp= abs(sud), ans;
    m = temp / 1000;
    temp -= m * 1000;
    i = temp / 100;
    temp -= i * 100;
    j = temp / 10;
    n = temp - j * 10;
    ans = (m - 1) * 729 + (i - 1) * 81 + (j - 1) * 9 + n;
    return sud > 0 ? ans : -ans;
}

int SudokuConvert(const int a[][10], const int b[][10])//数独规约:将数独棋盘转化为CNF保存在文件中
{
    int m, i, j, n, k, tmp1, tmp2, bi, bj, clauseNum = 0;
    int box[4] = {0, 0, 3, 6};
    FILE *fp = NULL, *temp = NULL;
    fp = fopen("temp.cnf", "w");
    
    //1111 1112 1113 ...1119 0
    for (m = 1; m <= 2; m++) //每个格子填入至少一个数�?
    {
        for (i = 1; i <= 9; i++)
        {
            for (j = 1; j <= 9; j++)
            {
                for (n = 1; n <= 9; n++)
                {
                    tmp1 = m * 1000 + i * 100 + j * 10 + n;
                    fprintf(fp, "%d ", SudokuToVar(tmp1));
                    
                }
                fprintf(fp, "0\n");
                clauseNum++;
            }
        }
    }
    //每个格子只填一个数�?
    for (m = 1; m <= 2; m++)
    {
        for (i = 1; i <= 9; i++)
        {
            for (j = 1; j <= 9; j++)
            {
                for (n = 1; n <= 8; n++)//数字1
                {
                    for (k = n + 1; k <= 9; k++)//数字2
                    {
                        tmp1 = m * 1000 + i * 100 + j * 10 + n;
                        tmp2 = m * 1000 + i * 100 + j * 10 + k;
                        fprintf(fp, "%d ", SudokuToVar(-tmp1));
                        fprintf(fp, "%d ", SudokuToVar(-tmp2));
                        clauseNum++;
                        fprintf(fp, "0\n");
                    }
                    
                }
            }
        }
    }

    //第i行有数字n
    for (n = 1; n <= 9; n++)
    {
        for (m = 1; m <= 2; m++)
        {
            for (i = 1; i <= 9; i++)
            {
                for (j = 1; j <= 9; j++)
                {
                    tmp1 = m * 1000 + i * 100 + j * 10 + n;
                    fprintf(fp, "%d ", SudokuToVar(tmp1));
                    
                }
                fprintf(fp, "0\n");
                clauseNum++;
            }
        }
    }
    //行内去重
    for (n = 1; n <= 9; n++)
    {
        for (m = 1; m <= 2; m++)
        {
            for (i = 1; i <= 9; i++)
            {
                for (j = 1; j <= 8; j++)
                {
                    for (k = j + 1; k <= 9; k++)
                    {
                        tmp1 = m * 1000 + i * 100 + j * 10 + n;
                        tmp2 = m * 1000 + i * 100 + k * 10 + n;
                        fprintf(fp, "%d ", SudokuToVar(-tmp1));
                        fprintf(fp, "%d ", SudokuToVar(-tmp2));
                        clauseNum++;
                        fprintf(fp, "0\n");
                    }
                }
            }
        }   
    }

    //第j列有数字n
    for (n = 1; n <= 9; n++)
    {
        for (m = 1; m <= 2; m++)
        {
            for (j = 1; j <= 9; j++)
            {
                for (i = 1; i <= 9; i++)
                {
                    tmp1 = m * 1000 + i * 100 + j * 10 + n;
                    fprintf(fp, "%d ", SudokuToVar(tmp1));
                    
                }
                fprintf(fp, "0\n");
                clauseNum++;
            }
        }
    }

    //列内去重
    for (n = 1; n <= 9; n++)
    {
        for (m = 1; m <= 2; m++)
        {
            for (j = 1; j <= 9; j++)
            {
                for (i = 1; i <= 8; i++)
                {
                    for (k = i + 1; k <= 9; k++)
                    {
                        tmp1 = m * 1000 + i * 100 + j * 10 + n;
                        tmp2 = m * 1000 + k * 100 + j * 10 + n;
                        fprintf(fp, "%d ", SudokuToVar(-tmp1));
                        fprintf(fp, "%d ", SudokuToVar(-tmp2));
                        clauseNum++;
                        fprintf(fp, "0\n");
                    }
                }
            }
        }
    }

    //宫格约束 �?(bi, bj)宫有数字n
    for (m = 1; m <= 2; m++)
    {
        for (n = 1; n <= 9; n++)
        {
            for (bi = 1; bi <= 3; bi++)
            {
                for (bj = 1; bj <= 3; bj++)
                {
                    for (i = 1; i <= 3; i++)
                    {
                        for (j = 1; j <= 3; j++)
                        {
                            tmp1 = m * 1000 + (box[bi] + i) * 100 + (box[bj] + j) * 10 + n;
                            fprintf(fp, "%d ", SudokuToVar(tmp1));
                            
                        }
                    }
                    fprintf(fp, "0\n");
                    clauseNum++;
                }
            }
        }
    }

    //宫格去重
    int i2, j2;
    for (m = 1; m <= 2; m++)
    {
        for (n = 1; n <= 9; n++)
        {
            for (bi = 1; bi <= 3; bi++)
            {
                for (bj = 1; bj <= 3; bj++)
                {
                    for (i = 1; i <= 3; i++)
                    {
                        for (j = 1; j <= 3; j++)
                        {
                            if (j < 3)
                            {
                                i2 = i;
                                j2 = j + 1;
                            }
                            else if(j == 3)
                            {
                                i2 = i + 1;
                                j2 = 1;
                            }
                            while(i2 <= 3)
                            {
                                while (j2 <= 3)
                                {
                                    tmp1 = m * 1000 + (box[bi] + i) * 100 + (box[bj] + j) * 10 + n;
                                    tmp2 = m * 1000 + (box[bi] + i2) * 100 + (box[bj] + j2) * 10 + n;
                                    fprintf(fp, "%d ", SudokuToVar(-tmp1));
                                    fprintf(fp, "%d ", SudokuToVar(-tmp2));
                                    clauseNum++;
                                    fprintf(fp, "0\n");
                                    j2++;
                                }
                                i2++;
                            }
                        }
                    }
                }
            }
        }
    }

    //公共�?
    
    for (i = 1; i <= 3; i++)
    {
        for (j = 1; j <= 3; j++)
        {
            for (n = 1; n <= 9; n++)
            {
                tmp1 = 1 * 1000 + (box[3] + i) * 100 + (box[3] + j) * 10 + n;
                tmp2 = 2 * 1000 + (box[1] + i) * 100 + (box[1] + j) * 10 + n;
                fprintf(fp, "%d ", SudokuToVar(tmp1));
                fprintf(fp, "%d ", SudokuToVar(-tmp2));
                fprintf(fp, "0\n");
                clauseNum++;
                fprintf(fp, "%d ", SudokuToVar(-tmp1));
                fprintf(fp, "%d ", SudokuToVar(tmp2));
                fprintf(fp, "0\n");
                clauseNum++;
            }
        }
    }
    
    //数独已知格子
    for(i = 1; i <= 9; i++)
    {
        for (j = 1; j <= 9; j++)
        {
            if (a[i][j])
            {
                tmp1 = 1000 + i * 100 + j * 10 + a[i][j];
                fprintf(fp, "%d ", SudokuToVar(tmp1));
                fprintf(fp, "0\n");
                clauseNum++;
            }
            if (b[i][j] && (i > 3 || j > 3))
            {
                tmp1 = 2000 + i * 100 + j * 10 + b[i][j];
                fprintf(fp, "%d ", SudokuToVar(tmp1));
                fprintf(fp, "0\n");
                clauseNum++;
            }
        }
    }
    fclose(fp);

    fp = fopen("Sudoku.cnf", "w");
    temp = fopen("temp.cnf", "r");
    fprintf(fp, "p cnf %d %d\n", 1458, clauseNum);

    char *result = NULL;
    char line[500];
    result = fgets(line, 500, temp);
    while (result)
    {
        fprintf(fp, "%s", line);
        result = fgets(line, 500, temp);
    }
    fclose(fp);
    fclose(temp);

    return 0;
}




int ReadAnswer(int a[][10], int b[][10])
{
    FILE *fp = NULL;
    fp = fopen("ans.res", "r");
    char c;
    fscanf(fp, "%c", &c);
    while(c != 'v')
    {
        fscanf(fp, "%c", &c);
    }
    int var = 0, sud = 0, m = 0, i = 0, j = 0, n = 0;
    for (int i = 0; i < 1458; i++)
    {
        fscanf(fp, "%d", &var);
        if (var > 0)
        {
            sud = VarToSudoku(var);
            n = sud % 10;
            j = sud / 10 % 10;
            i = sud / 100 % 10;
            m = sud / 1000;
            if (m == 1)
            {
                a[i][j] = n;
            }
            else if (m == 2)
            {
                b[i][j] = n;
            }
        }
    }
    return 0;
}



int SudokuGenerate(int a[][10], int b[][10])
{

    int box[4] = {0, 0, 3, 6};
    int arr[10];
    int i, j, k, n;
    memset(a, 0, 400);
    memset(b, 0, 400);
    for (n = 1; n <= 3; n++)//随机填充
    {
        
        GenerateRandomArray(arr);
        for (i = 1; i <= 3; i++)
        {
            for (j = 1; j <= 3; j++)
            {
                a[box[n] + i][box[n] + j] = arr[(i - 1) * 3 + j];
            }
        }
    }
    for (n = 2; n <= 3; n++)
    {
        
        GenerateRandomArray(arr);
        for (i = 1; i <= 3; i++)
        {
            for (j = 1; j <= 3; j++)
            {
                b[box[n] + i][box[n] + j] = arr[(i - 1) * 3 + j];
            }
        }
    }
     
    return 0;
}





void DigHole(int a[][10], int b[][10], int holeNum)
{

    int i = 0, j = 0, m = 0, digged = 0, initNum, testNum;
    int testResult = 0, isDiggable = 0;
    extern ClauseNode *CNFhead;
    extern VariableInfo *varInfo;
    clock_t begin, end;
    begin = clock();
       
    while(digged <= holeNum / 2)//左上数独挖洞
    {
        srand((unsigned int )clock());
        i = rand() % 9 + 1;
        j = rand() % 9 + 1;
        while(a[i][j] == 0)
        {
            i = rand() % 9 + 1;
            j = rand() % 9 + 1;
        }

        initNum = a[i][j];
        isDiggable = TRUE;
        for (testNum = 1; testNum <= 9; testNum++)
        {
            if (initNum != testNum)
            {
                a[i][j] = testNum;


                SudokuConvert(a, b);

                ReadCNF(CNFhead, varInfo, "Sudoku.cnf");

                testResult = NonRecursiveDPLL();
                DestroyCNF(CNFhead, varInfo);
                if (testResult == 1)
                {
                    isDiggable = FALSE;
                    a[i][j] = initNum;
                    break;
                }
            }
        }

        if (isDiggable && digged <= holeNum / 2)
        {
            a[i][j] = 0;
            digged++;
            cout << digged - 1 << '/' << holeNum << endl;
        }
        else
        {
            a[i][j] = initNum;
        }
            
    }

    for (i = 1; i <= 3; i++)//公共部分同步
    {
        for (j = 1; j <= 3; j++)
        {
            b[i][j] = a[6 + i][6 + j];
        }
    }

    while (digged <= holeNum)//右下数独挖洞
    {
        srand((unsigned int)clock());
        i = rand() % 9 + 1;
        j = rand() % 9 + 1;
        while (b[i][j] == 0 || (i <= 3 && j <= 3))
        {
            i = rand() % 9 + 1;
            j = rand() % 9 + 1;
        }

        initNum = b[i][j];
        isDiggable = TRUE;
        for (testNum = 1; testNum <= 9; testNum++)
        {
            if (initNum != testNum)
            {
                b[i][j] = testNum;
                SudokuConvert(a, b);

                ReadCNF(CNFhead, varInfo, "Sudoku.cnf");

                testResult = NonRecursiveDPLL();
                DestroyCNF(CNFhead, varInfo);
                if (testResult == 1)
                {
                    isDiggable = FALSE;
                    b[i][j] = initNum;
                    break;
                }
            }
        }

        if (isDiggable && digged <= holeNum)
        {
            
            b[i][j] = 0;
            digged++;
            cout << digged - 1 << '/' << holeNum << endl;
        }
        else
        {
            b[i][j] = initNum;
        }
    }
    end = clock();
    SudokuConvert(a, b);
    printf("%d ms\n", (int)((end - begin)));
    system("pause");
}


int check(int a[][10], int b[][10], int temp)
{
    int i, j, n, m, k, r, c;
    int rec[10], box[4] = {0, 0, 3, 6};
    memset(rec, 0, 40);
    m = temp / 1000;
    temp -= m * 1000;
    i = temp / 100;
    temp -= i * 100;
    j = temp / 10;
    n = temp - j * 10;
    if (n == 0)
    {
        if (m == 1 && i >= 7 && j >= 7)
        {
            a[i][j] = 0;
            b[i - 6][j - 6] = 0;
        }
        else if (m == 2 && i <= 3 && j <= 3)
        {
            a[i + 6][j + 6] = 0;
            b[i][j] = 0;
        }
        else if (m == 1 && (i <= 6 || j <= 6))
        {
            a[i][j] = 0;
        }
        else if (m == 2 && (i >= 4 || j >= 4))
        {
            b[i][j] = 0;
        }
        return TRUE;
    }
    int bi = (i - 1) / 3 + 1, bj = (j - 1) / 3 + 1;

    if (m == 1 && i >= 7 && j >= 7)
    {
        memset(rec, 0, 40);
        for(k = 1; k <= 9; k++)//行检验a
        {
            if(a[i][k])
                rec[a[i][k]] = 1;

        }
        if (rec[n])
        {
            return ROW;
        }

        memset(rec, 0, 40);
        for (k = 1; k <= 9; k++) //列检验a
        {
            if (a[k][j])
                rec[a[k][j]] = 1;
        }
        if (rec[n])
        {
            return COL;
        }

        memset(rec, 0, 40);//宫格检验a
        for(r = 1; r <= 3; r++)
        {
            for(c= 1; c <= 3; c++)
            {
                if (a[box[3] + r][box[3] + c])
                    rec[a[box[3] + r][box[3] + c]] = 1;
            }
        }
        if (rec[n])
        {
            return BLK;
        }

        memset(rec, 0, 40);
        for (k = 1; k <= 9; k++) //行检验b
        {
            if (b[i - 6][k])
                rec[b[i - 6][k]] = 1;
        }
        if (rec[n])
        {
            return ROW;
        }

        memset(rec, 0, 40);
        for (k = 1; k <= 9; k++) //列检验b
        {
            if (b[k][j - 6])
                rec[b[k][j - 6]] = 1;
        }
        if (rec[n])
        {
            return COL;
        }

        memset(rec, 0, 40);
        for (r = 1; r <= 3; r++)//宫格检验b
        {
            for (c = 1; c <= 3; c++)
            {
                if (b[r][c])
                    rec[b[r][c]] = 1;
            }
        }
        if (rec[n])
        {
            return BLK;
        }
    }

    else if (m == 2 && i <= 3 && j <= 3)
    {
        memset(rec, 0, 40);
        for (k = 1; k <= 9; k++) //行检验a
        {
            if (a[i + 6][k])
                rec[a[i + 6][k]] = 1;
        }
        if (rec[n])
        {
            return ROW;
        }

        memset(rec, 0, 40);
        for (k = 1; k <= 9; k++) //列检验a
        {
            if (a[k][j + 6])
                rec[a[k][j + 6]] = 1;
        }
        if (rec[n])
        {
            return COL;
        }

        memset(rec, 0, 40); //宫格检验a
        for (r = 1; r <= 3; r++)
        {
            for (c = 1; c <= 3; c++)
            {
                if (a[box[3] + r][box[3] + c])
                    rec[a[box[3] + r][box[3] + c]] = 1;
            }
        }
        if (rec[n])
        {
            return BLK;
        }

        memset(rec, 0, 40);
        for (k = 1; k <= 9; k++) //行检验b
        {
            if (b[i][k])
                rec[b[i][k]] = 1;
        }
        if (rec[n])
        {
            return ROW;
        }

        memset(rec, 0, 40);
        for (k = 1; k <= 9; k++) //列检验b
        {
            if (b[k][j])
                rec[b[k][j]] = 1;
        }
        if (rec[n])
        {
            return COL;
        }

        memset(rec, 0, 40);
        for (r = 1; r <= 3; r++) //宫格检验b
        {
            for (c = 1; c <= 3; c++)
            {
                if (b[r][c])
                    rec[b[r][c]] = 1;
            }
        }
        if (rec[n])
        {
            return BLK;
        }
    }
    else if (m == 1 && (i <= 6 || j <= 6))
    {
        memset(rec, 0, 40);
        for (k = 1; k <= 9; k++) //行检验a
        {
            if (a[i][k])
                rec[a[i][k]] = 1;
        }
        if (rec[n])
        {
            return ROW;
        }

        memset(rec, 0, 40);
        for (k = 1; k <= 9; k++) //列检验a
        {
            if (a[k][j])
                rec[a[k][j]] = 1;
        }
        if (rec[n])
        {
            return COL;
        }

        memset(rec, 0, 40); //宫格检验a
        for (r = 1; r <= 3; r++)
        {
            for (c = 1; c <= 3; c++)
            {
                if (a[box[bi] + r][box[bj] + c])
                    rec[a[box[bi] + r][box[bj] + c]] = 1;
            }
        }
        if (rec[n])
        {
            return BLK;
        }
    }

    else if (m == 2 && (i >=4 || j >= 4))
    {
        memset(rec, 0, 40);
        for (k = 1; k <= 9; k++) //行检验b
        {
            if (b[i][k])
                rec[b[i][k]] = 1;
        }
        if (rec[n])
        {
            return ROW;
        }

        memset(rec, 0, 40);
        for (k = 1; k <= 9; k++) //列检验b
        {
            if (b[k][j])
                rec[b[k][j]] = 1;
        }
        if (rec[n])
        {
            return COL;
        }

        memset(rec, 0, 40);
        for (r = 1; r <= 3; r++) //宫格检验b
        {
            for (c = 1; c <= 3; c++)
            {
                if (b[box[bi] + r][box[bj] + c])
                    rec[b[box[bi] + r][box[bj] + c]] = 1;
            }
        }
        if (rec[n])
        {
            return BLK;
        }
    }
    return TRUE;
}

void PrintAnswer(int a[][10], int b[][10])
{
    system("cls");
    printf("+-----+-----+-----+\n");
    printf("|%d %d %d|%d %d %d|%d %d %d|\n", a[1][1], a[1][2], a[1][3], a[1][4], a[1][5], a[1][6], a[1][7], a[1][8], a[1][9]);
    printf("|%d %d %d|%d %d %d|%d %d %d|\n", a[2][1], a[2][2], a[2][3], a[2][4], a[2][5], a[2][6], a[2][7], a[2][8], a[2][9]);
    printf("|%d %d %d|%d %d %d|%d %d %d|\n", a[3][1], a[3][2], a[3][3], a[3][4], a[3][5], a[3][6], a[3][7], a[3][8], a[3][9]);
    printf("+-----+-----+-----+\n");
    printf("|%d %d %d|%d %d %d|%d %d %d|\n", a[4][1], a[4][2], a[4][3], a[4][4], a[4][5], a[4][6], a[4][7], a[4][8], a[4][9]);
    printf("|%d %d %d|%d %d %d|%d %d %d|\n", a[5][1], a[5][2], a[5][3], a[5][4], a[5][5], a[5][6], a[5][7], a[5][8], a[5][9]);
    printf("|%d %d %d|%d %d %d|%d %d %d|\n", a[6][1], a[6][2], a[6][3], a[6][4], a[6][5], a[6][6], a[6][7], a[6][8], a[6][9]);
    printf("+-----+-----+-----+-----+-----+\n");
    printf("|%d %d %d|%d %d %d|%d %d %d|%d %d %d|%d %d %d|\n", a[7][1], a[7][2], a[7][3], a[7][4], a[7][5], a[7][6], a[7][7], a[7][8], a[7][9], b[1][4], b[1][5], b[1][6], b[1][7], b[1][8], b[1][9]);
    printf("|%d %d %d|%d %d %d|%d %d %d|%d %d %d|%d %d %d|\n", a[8][1], a[8][2], a[8][3], a[8][4], a[8][5], a[8][6], a[8][7], a[8][8], a[8][9], b[2][4], b[2][5], b[2][6], b[2][7], b[2][8], b[2][9]);
    printf("|%d %d %d|%d %d %d|%d %d %d|%d %d %d|%d %d %d|\n", a[9][1], a[9][2], a[9][3], a[9][4], a[9][5], a[9][6], a[9][7], a[9][8], a[9][9], b[3][4], b[3][5], b[3][6], b[3][7], b[3][8], b[3][9]);
    printf("+-----+-----+-----+-----+-----+\n");
    printf("            |%d %d %d|%d %d %d|%d %d %d|\n", b[4][1], b[4][2], b[4][3], b[4][4], b[4][5], b[4][6], b[4][7], b[4][8], b[4][9]);
    printf("            |%d %d %d|%d %d %d|%d %d %d|\n", b[5][1], b[5][2], b[5][3], b[5][4], b[5][5], b[5][6], b[5][7], b[5][8], b[5][9]);
    printf("            |%d %d %d|%d %d %d|%d %d %d|\n", b[6][1], b[6][2], b[6][3], b[6][4], b[6][5], b[6][6], b[6][7], b[6][8], b[6][9]);
    printf("            +-----+-----+-----+\n");
    printf("            |%d %d %d|%d %d %d|%d %d %d|\n", b[7][1], b[7][2], b[7][3], b[7][4], b[7][5], b[7][6], b[7][7], b[7][8], b[7][9]);
    printf("            |%d %d %d|%d %d %d|%d %d %d|\n", b[8][1], b[8][2], b[8][3], b[8][4], b[8][5], b[8][6], b[8][7], b[8][8], b[8][9]);
    printf("            |%d %d %d|%d %d %d|%d %d %d|\n", b[9][1], b[9][2], b[9][3], b[9][4], b[9][5], b[9][6], b[9][7], b[9][8], b[9][9]);
    printf("            +-----+-----+-----+\n");
    system("pause");
}

void Play(int a[][10], int b[][10], int ansA[][10], int ansB[][10])
{
    char command[50];
    int m, i, j, n, isComplete = 1, result = 0;
    int initA[10][10], initB[10][10];
    for(i = 1; i <=9; i++)
    {
        for(j = 1; j <= 9; j++)
        {
            initA[i][j] = a[i][j];
            initB[i][j] = b[i][j];
        }
    }
    while(1)
    {
        isComplete = 1;
        system("cls");
        printf("+-----+-----+-----+\n");
        printf("|%d %d %d|%d %d %d|%d %d %d|\n", a[1][1], a[1][2], a[1][3], a[1][4], a[1][5], a[1][6], a[1][7], a[1][8], a[1][9]);
        printf("|%d %d %d|%d %d %d|%d %d %d|\n", a[2][1], a[2][2], a[2][3], a[2][4], a[2][5], a[2][6], a[2][7], a[2][8], a[2][9]);
        printf("|%d %d %d|%d %d %d|%d %d %d|\n", a[3][1], a[3][2], a[3][3], a[3][4], a[3][5], a[3][6], a[3][7], a[3][8], a[3][9]);
        printf("+-----+-----+-----+\n");
        printf("|%d %d %d|%d %d %d|%d %d %d|\n", a[4][1], a[4][2], a[4][3], a[4][4], a[4][5], a[4][6], a[4][7], a[4][8], a[4][9]);
        printf("|%d %d %d|%d %d %d|%d %d %d|\n", a[5][1], a[5][2], a[5][3], a[5][4], a[5][5], a[5][6], a[5][7], a[5][8], a[5][9]);
        printf("|%d %d %d|%d %d %d|%d %d %d|\n", a[6][1], a[6][2], a[6][3], a[6][4], a[6][5], a[6][6], a[6][7], a[6][8], a[6][9]);
        printf("+-----+-----+-----+-----+-----+\n");
        printf("|%d %d %d|%d %d %d|%d %d %d|%d %d %d|%d %d %d|\n", a[7][1], a[7][2], a[7][3], a[7][4], a[7][5], a[7][6], a[7][7], a[7][8], a[7][9], b[1][4], b[1][5], b[1][6], b[1][7], b[1][8], b[1][9]);
        printf("|%d %d %d|%d %d %d|%d %d %d|%d %d %d|%d %d %d|\n", a[8][1], a[8][2], a[8][3], a[8][4], a[8][5], a[8][6], a[8][7], a[8][8], a[8][9], b[2][4], b[2][5], b[2][6], b[2][7], b[2][8], b[2][9]);
        printf("|%d %d %d|%d %d %d|%d %d %d|%d %d %d|%d %d %d|\n", a[9][1], a[9][2], a[9][3], a[9][4], a[9][5], a[9][6], a[9][7], a[9][8], a[9][9], b[3][4], b[3][5], b[3][6], b[3][7], b[3][8], b[3][9]);
        printf("+-----+-----+-----+-----+-----+\n");
        printf("            |%d %d %d|%d %d %d|%d %d %d|\n", b[4][1], b[4][2], b[4][3], b[4][4], b[4][5], b[4][6], b[4][7], b[4][8], b[4][9]);
        printf("            |%d %d %d|%d %d %d|%d %d %d|\n", b[5][1], b[5][2], b[5][3], b[5][4], b[5][5], b[5][6], b[5][7], b[5][8], b[5][9]);
        printf("            |%d %d %d|%d %d %d|%d %d %d|\n", b[6][1], b[6][2], b[6][3], b[6][4], b[6][5], b[6][6], b[6][7], b[6][8], b[6][9]);
        printf("            +-----+-----+-----+\n");
        printf("            |%d %d %d|%d %d %d|%d %d %d|\n", b[7][1], b[7][2], b[7][3], b[7][4], b[7][5], b[7][6], b[7][7], b[7][8], b[7][9]);
        printf("            |%d %d %d|%d %d %d|%d %d %d|\n", b[8][1], b[8][2], b[8][3], b[8][4], b[8][5], b[8][6], b[8][7], b[8][8], b[8][9]);
        printf("            |%d %d %d|%d %d %d|%d %d %d|\n", b[9][1], b[9][2], b[9][3], b[9][4], b[9][5], b[9][6], b[9][7], b[9][8], b[9][9]);
        printf("            +-----+-----+-----+\n");
        printf("input your operation:\n");
        scanf("%s", command);
        if (command[0] == 'q')
        {
            return;
        }
        else if (command[0] == 'a')
        {
            PrintAnswer(ansA, ansB);
        }
        else if (command[0] == 'r')
        {
            for (i = 1; i <= 9; i++)
            {
                for (j = 1; j <= 9; j++)
                {
                    a[i][j] = initA[i][j];
                    b[i][j] = initB[i][j];
                }
            }
        }
        else if (command[0] >= '1' && command[0] <= '9')
        {
            m = command[0] - '0';
            i = command[1] - '0';
            j = command[2] - '0';
            n = command[3] - '0';
            result = check(a, b, m * 1000 + i * 100 + j * 10 + n);
            if (result != TRUE)
            {
                switch(result)
                {
                    case ROW:
                        printf("Same Number in Row %d\n", i);
                        break;
                    case COL:
                        printf("Same Number in Colomn %d\n", j);
                        break;
                    case BLK:
                        printf("Same Number in Block %d, %d\n", (i - 1)/ 3 + 1, (j - 1) / 3 + 1);
                }
                printf("Invalid Assignment\n");
                getchar();
                getchar();
            }
            else 
            {
                if (m == 1 && i >= 7 && j >= 7)
                {
                    if (initA[i][j] == 0 && initB[i - 6][j - 6] == 0)
                    {
                        a[i][j] = n;
                        b[i - 6][j - 6] = n;
                    }
                    else
                    {
                        printf("Invalid Assignment\n");
                        getchar();
                        getchar();
                    }
                }
                else if (m == 2 && i <= 3 && j <= 3)
                {
                    if (initA[i + 6][j + 6] == 0 && initB[i][j] == 0)
                    {
                        a[i + 6][j + 6] = n;
                        b[i][j] = n;
                    }
                    else
                    {
                        printf("Invalid Assignment\n");
                        getchar();
                        getchar();
                    }
                }
                else if (m == 1 && (i <= 6 || j <= 6))
                {
                    if (initA[i][j] == 0)
                        a[i][j] = n;
                    else
                    {
                        printf("Invalid Assignment\n");
                        getchar();
                        getchar();
                    }
                }
                else if (m == 2 && (i >= 4 || j >= 4))
                {
                    if (initB[i][j] == 0)
                        b[i][j] = n;
                    else
                    {
                        printf("Invalid Assignment\n");
                        getchar();
                        getchar();
                    }
                }
            }
            
        }

        for(i = 1; i <= 9; i++)
        {
            for(j = 1; j <= 9; j++)
            {
                if (a[i][j] == 0 || b[i][j] == 0)
                {
                    isComplete = 0;
                }
            }
        }
        if (isComplete == 1)
        {
            break;
        }
    }
    printf("Sudoku Finish\n");
    getchar();
    getchar();
}

int SudokuMain()
{
    extern ClauseNode *CNFhead;
    extern VariableInfo *varInfo;
    extern int ans[];
    int a[10][10], b[10][10], result, i, j, holeNum = 100;
    char filename[100];
    clock_t start, end;
    SudokuGenerate(a, b);//随机填充宫格
    SudokuConvert(a, b);//数独转化为CNF
    strcpy(filename, "Sudoku.cnf");
    ReadCNF(CNFhead, varInfo, filename);

    start = clock();
    result = NonRecursiveDPLL();//对填充后数独求解
    end = clock();
    WriteAnswer(result, ans, start, end);
    DestroyCNF(CNFhead, varInfo);

    ReadAnswer(a, b);//已生成数独终�?

    int atemp[10][10], btemp[10][10];

    for (i = 0; i <= 9; i++)//复制终盘挖洞
    {
        for (j = 0; j <= 9; j++)
        {
            atemp[i][j] = a[i][j];
            btemp[i][j] = b[i][j];
        }
    }
    printf("number of holes:");
    scanf("%d", &holeNum);
    DigHole(atemp, btemp, holeNum);//随机挖洞
    Play(atemp, btemp, a, b);
    return 0;
}