#include "header.h"

int SATmain()
{
    extern ClauseNode *CNFhead;
    extern VariableInfo *varInfo;
    extern int varNumTotal;
    extern int ans[];
    clock_t start, end;
    int choice = 0;
    int result = -1;

    while(1)
    {
        system("cls");
        printf("                          SAT options                           \n");
        printf("----------------------------------------------------------------\n");
        printf("    1.ReadCNF               2.PrintCNF                3.DPLL    \n");
        printf("    4.NonRecursiveDPLL      5.Show Answer             0.exit    \n");
        printf("----------------------------------------------------------------\n");
        printf("choose operation:");
        scanf("%d", &choice);
        if(!choice)
        {
            break;
        }
        switch (choice)
        {
        case 1:
            CNFhead = NULL;
            varInfo = NULL;
            ReadCNF(CNFhead, varInfo, "data.cnf");
            if (CNFhead)
            {
                printf("Load CNF Successfully\n");
            }
            getchar();
            getchar();
            break;
        case 2:
            CNFcheck(CNFhead);
            getchar();
            getchar();
            break;
        case 3:
            start = clock();
            result = DPLL();
            end = clock();
            WriteAnswer(result, ans, start, end);
            printf("DPLL successfully\n");
            printf("time used: %.1lf ms", (double)(end - start) / CLOCKS_PER_SEC * 1000.0);
            getchar();
            getchar();
            break;
        case 4:
            start = clock();
            result = NonRecursiveDPLL();
            end = clock();
            WriteAnswer(result, ans, start, end);
            printf("DPLL successfully\n");
            printf("time used: %.1lf ms", (double)(end - start) / CLOCKS_PER_SEC * 1000.0);
            getchar();
            getchar();
            break;

        case 5:
            if (result == -1)
            {
                printf("ERROR:SAT Not Solved");
            }
            else if (result == 1)
            {
                printf("s %d\nv ", result);
                for (int i = 1; i <= varNumTotal; i++)
                {
                    if (ans[i] == 1)
                    {
                        printf("%d ", i);
                    }
                    else if (ans[i] == 0)
                    {
                        printf("%d ", -i);
                    }
                }
                printf("\nt %.1lf ms\n", (double)(end - start) / CLOCKS_PER_SEC * 1000.0);
            }
            else if (result == 0)
            {
                printf("s %d\nt %.1lf ms\n", result, (double)(end - start) / CLOCKS_PER_SEC * 1000.0);
            }
            getchar();
            getchar();
            break;
        }
        system("cls");
    }
    return 0;
}