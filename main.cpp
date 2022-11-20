#include "header.h"

int main()
{
    int choice = 0;
    while(1)
    {
        system("cls");
        printf("        SAT and Dual Sukoku Solver       \n");
        printf("-----------------------------------------\n");
        printf("     1.SAT    2.Sudoku    0.exit         \n");
        printf("-----------------------------------------\n");
        printf("choose operation:");
        scanf("%d", &choice);
        if(!choice)
        {
            break;
        }
        switch (choice)
        {
        case 1:
            SATmain();
            getchar();
            getchar();
            break;
        case 2:
            SudokuMain();
            getchar();
            getchar();
            break;
            
        }
        system("cls");
    }
    system("pause");
    return 0;
}