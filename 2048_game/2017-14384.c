#include<stdio.h>
#include<time.h>
#include<conio.h>
#include<windows.h>

#define size 5
#define timelimit 300000.0f

void make_puzzle(int** pi);
int display_puzzle(int** pi);
void input_puzzle(int** pi);
int check_puzzle(int** pi);

int combo = 0;
int move = 0;
float sttime = 0.f;

int main()
{
    char c[100];
    int** pi;
    pi = (int**)malloc(size * sizeof(int*));
    float starttime = 0.f;
    float endtime = 0.f;
    int menu;

    do {
        printf("1. Game Start\n");
        printf("2. How to\n");
        printf("3. Ranking\n");
        printf("4. Exit\n\n");

        printf("choose the menu: ");
        scanf("%d", &menu);
        getchar();
        printf("\n");

        if ((menu != 1) && (menu != 2) && (menu != 3) && (menu != 4))
        {
            printf("잘못된 입력입니다.\n");
            continue;
        }


        if (menu == 1)
        {
            starttime = GetTickCount();
            sttime = starttime;
            make_puzzle(pi);
            char* c;

            while (1)
            {
                display_puzzle(pi);

                if (display_puzzle(pi) == 1)
                {
                    c = "FAIL(time over)";
                    printf("\n\n%s\n\n", c);
                    break;
                }

                if (check_puzzle(pi) == 1)
                {
                    c = "SUCCESS";
                    printf("\n\n%s\n\n", c);
                    break;
                }
                else if (check_puzzle(pi) == 2)
                {
                    c = "FAIL(can not move anymore)";
                    printf("\n\n%s\n\n", c);
                    break;
                }
                input_puzzle(pi);
            }
            endtime = GetTickCount();
            char name[255] = { ' ' };
            printf("input name: ");
            scanf(" %s", &name);
            printf("\n");
            char blank[255] = { ' ' };

            FILE* pFile;
            pFile = fopen("myfile.txt", "a");
            fputs(name, pFile);
            fputs(blank, pFile);

            fwrite(c, sizeof(char), sizeof(c), pFile);
            fputs(blank, pFile);

            char movecount[255] = "move: ";

            fputs(movecount, pFile);
            char movestring[255] = { 0 };
            sprintf(movestring, "%d", move);
            fputs(movestring, pFile);
            fputs(blank, pFile);


            char combocount[255] = "combo: ";

            fputs(combocount, pFile);
            char combostring[255] = { ' ' };
            sprintf(combostring, "%d", combo);
            fputs(combostring, pFile);
            fputs(blank, pFile);

            char time[255] = "game time(ms): ";

            fputs(time, pFile);
            char array[255] = { ' ' };
            if (endtime - starttime > timelimit)
            {
                sprintf(array, "%f", timelimit);
            }
            else sprintf(array, "%f", endtime - starttime);
            
            char* ptr7 = array;
            fwrite(ptr7, sizeof(char), sizeof(ptr7), pFile);
            fputs(blank, pFile);

            char newline[255] = { '\n' };
            fputs(newline, pFile);

            combo = 0;
            move = 0;
            fclose(pFile);
        }

        else if (menu == 2)
        {
            printf("시작할 때 사용자가 선택한 사이즈의 판 위에 2개의 2(또는 4)가 있는데 키보드 방향키나 터치스크린으로 드래그하면 블록이 전부 그 방향으로 이동하면서 같은 숫자가 있을 경우 합쳐지며, 빈 자리 중 한 칸에 랜덤하게 2 또는 4가 나온다.\n");
            printf("이를 반복해서 2로부터 2048을 만들면 게임을 클리어한다. 만약 2048을 만들기 전 이동할 수 없는 경우, 즉, %d칸이 꽉 차있으면서 인접한 두 칸이 같지 않을 때, 게임 오버이다.\n", size * size);
            printf("\n");
            continue;
        }

        else if (menu == 3)
        {
            char buffer[100] = { ' ' };
            FILE* pFile;
            pFile = fopen("myfile.txt", "r");


            while (fgets(buffer, sizeof(buffer), pFile))
            {
                fputs(buffer, stdout);
                printf("\n");
            }
            fclose(pFile);
            continue;
        }

        else if (menu == 4)
        {
            exit(1);
        }
    } while (1);

    return 0;
}

void make_puzzle(int** pi)
{
    int m = 0;
    int num = 0;

    for (int i = 0; i < size; i++)
    {
        pi[i] = (int*)malloc(size * sizeof(int));
    }

    int L[size * size];


    srand((int)time(NULL));
    for (int i = 0; i < size * size; i++)
    {
        L[i] = (rand() % 5);

        if (num == 2)
        {
            L[i] = 0;
        }

        if (L[i] == 1 || L[i] == 3)
        {
            L[i] = 0;
        }


        if (L[i] == 2 || L[i] == 4)
        {
            num++;
        }

    }

    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            pi[i][j] = L[m];
            m++;
        }
    }
}


int display_puzzle(int** pi)
{
    system("cls");
    float temptime = GetTickCount() - sttime;
    float remainingtime = timelimit - temptime;
    printf("%f ms left\n\n", remainingtime);
    if (remainingtime < 0) return 1;

    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            if (pi[i][j] == 0) printf("*\t");
            else printf("%d\t", pi[i][j]);
        }
        printf("\n");
    }
    printf("\ncombo: %d", combo);

    return 0;
}

void input_puzzle(int** pi)
{
    int dir;
    int i, j, r;

    while (1)
    {
        dir = _getch();

        if ((dir != 119) && (dir != 97) && (dir != 115) && (dir != 100))
        {
            printf("잘못된 입력입니다. w,a,s,d를 눌러 게임을 진행하세요.\n");
            continue;
        }
        if (dir == 119)//상
        {
            for (j = 0; j < size; j++)
            {
                for (i = 1; i < size; i++)
                {
                    for (r = i; r > 0; r--)
                    {
                        if (pi[r][j] == 0 || pi[r][j] > 10000) break;
                        if (pi[r - 1][j] != 0 && pi[r - 1][j] != pi[r][j]) break;
                        if (pi[r - 1][j] == 0 && pi[r][j] != 0)
                        {
                            pi[r - 1][j] = pi[r][j];
                            move++;
                        }
                        else if (pi[r][j] == pi[r - 1][j])
                        {
                            pi[r - 1][j] *= 2;
                            pi[r - 1][j] += 10000;
                            combo++;
                            move++;
                        }
                        pi[r][j] = 0;
                    }
                }

            }
            break;
        }

        else if (dir == 115)//하
        {
            for (j = 0; j < size; j++)
            {
                for (i = size - 2; i >= 0; i--)
                {
                    for (r = i; r < size - 1; r++)
                    {
                        if (pi[r][j] == 0 || pi[r][j] > 10000) break;
                        if (pi[r + 1][j] != 0 && pi[r + 1][j] != pi[r][j]) break;
                        if (pi[r + 1][j] == 0 && pi[r][j] != 0)
                        {
                            pi[r + 1][j] = pi[r][j];
                            move++;
                        }
                        else if (pi[r][j] == pi[r + 1][j])
                        {
                            pi[r + 1][j] *= 2;
                            pi[r + 1][j] += 10000;
                            combo++;
                            move++;
                        }
                        pi[r][j] = 0;
                    }
                }

            }
            break;
        }

        else if (dir == 97)//좌
        {
            for (i = 0; i < size; i++)
            {
                for (j = 1; j < size; j++)
                {
                    for (r = j; r > 0; r--)
                    {
                        if (pi[i][r] == 0 || pi[i][r] > 10000) break;
                        if (pi[i][r - 1] != 0 && pi[i][r - 1] != pi[i][r]) break;

                        if (pi[i][r - 1] == 0 && pi[i][r] != 0)
                        {
                            pi[i][r - 1] = pi[i][r];
                            move++;
                        }
                        else if (pi[i][r] == pi[i][r - 1])
                        {
                            pi[i][r - 1] *= 2;
                            pi[i][r - 1] += 10000;
                            combo++;
                            move++;
                        }
                        pi[i][r] = 0;

                    }
                }
            }
            break;
        }

        else if (dir == 100)//우
        {
            for (i = 0; i < size; i++)
            {
                for (j = size - 2; j >= 0; j--)
                {
                    for (r = j; r < size - 1; r++)
                    {
                        if (pi[i][r] == 0 || pi[i][r] > 10000) break;
                        if (pi[i][r + 1] != 0 && pi[i][r + 1] != pi[i][r]) break;
                        if (pi[i][r + 1] == 0 && pi[i][r] != 0)
                        {
                            pi[i][r + 1] = pi[i][r];
                            move++;
                        }
                        else if (pi[i][r] == pi[i][r + 1])
                        {
                            pi[i][r + 1] *= 2;
                            pi[i][r + 1] += 10000;
                            combo++;
                            move++;
                        }
                        pi[i][r] = 0;

                    }
                }
            }
            break;
        }

    }

    for (i = 0; i < size; i++)
    {
        for (j = 0; j < size; j++)
        {
            if (pi[i][j] > 10000) pi[i][j] -= 10000;
        }
    }

    int cnt = 0;
    srand((int)time(NULL));
    while (1)
    {
        int m = rand() % size;
        int n = rand() % size;
        int k;

        if (pi[m][n] == 0)
        {
            do
            {
                k = (rand() % 5) + 1;
            } while (k % 2 != 0);

            pi[m][n] += k;
            cnt = 0;
            break;
        }

        else if (pi[m][n] != 0)
        {
            cnt++;
        }

        if (cnt == size * size) break;

    }
}

int check_puzzle(int** pi)
{
    int count = 0;
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            if (pi[i][j] == 0) count++;
            if (pi[i][j] == 2048) return 1;
        }
    }

    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size - 1; j++)
        {
            if (pi[i][j] == pi[i][j + 1]) count++;
        }
    }

    for (int j = 0; j < size; j++)
    {
        for (int i = 0; i < size - 1; i++)
        {
            if (pi[i][j] == pi[i + 1][j]) count++;
        }
    }

    if (count == 0) return 2;

    return 0;
}