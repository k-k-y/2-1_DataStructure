#include <stdio.h>
#include <stdlib.h>
#pragma warning(disable : 4996)

void draw_maze(char* plus, int column, int low);

int main(void) {
    int i, j;
    char temp;
    char* plus;
    int count = 0;
    int column, row;

    FILE* fp;
    fp = fopen("maze1.txt", "r");
    if (fp == NULL) {
        printf("���� ���⿡ �����߽��ϴ�.");
        return 1;
    }
    fscanf(fp, "%d", &column);
    fscanf(fp, "%d", &row);
    fgetc(fp);
    for (i = 0; i < (2 * (column + 1) + 1) * 2 * row; i++) {
        temp = fgetc(fp);
        if (temp == '+')
            count++;
    }
    fclose(fp);

    fp = fopen("maze1.txt", "r");
    if (fp == NULL) {
        printf("���� ���⿡ �����߽��ϴ�.");
        return 1;
    }
    fscanf(fp, "%d", &column);
    fscanf(fp, "%d", &row);
    fgetc(fp);
    fgetc(fp);
    fgetc(fp);
    plus = malloc(sizeof(char) * (count - 2 + row));

    count = 0;
    for (i = 0; i < 2 * (column - 1); i++) {
        temp = fgetc(fp);
        if (temp == '+') {
            plus[count] = i / 2 + 1;
            count++;
        }
    }
    fgetc(fp);
    fgetc(fp);
    fgetc(fp);
    plus[count] = 'x'; // x�� �� �ٲ� ǥ��
    count++;
    fseek(fp, 2 * (column + 1) + 1, SEEK_CUR);

    for (j = 1; j < row; j++) {
        for (i = 0; i < 2 * (column + 1) + 1; i++)
        {
            temp = fgetc(fp);
            if (temp == '+') // + ���
            {
                temp = fgetc(fp);
                if (temp == '-') {
                    fseek(fp, 2 * (column + 1) - 1, SEEK_CUR);
                    temp = fgetc(fp);
                    if (temp == '|') {
                        plus[count] = 'A' + i / 2; // - | �Ѵ� -> �빮�� ���ĺ�
                        count++;
                    }
                    else {
                        plus[count] = (i / 2) * (-1); // - �� -> ����
                        count++;
                    }
                    fseek(fp, (-2) * (column + 1) - 1, SEEK_CUR);
                }
                else {
                    fseek(fp, 2 * (column + 1) - 1, SEEK_CUR);
                    temp = fgetc(fp);
                    if (temp == '|') {
                        plus[count] = 'a' + i / 2; // | �� -> �ҹ��� ���ĺ�
                        count++;
                    }
                    else {
                        plus[count] = i / 2; // �ƹ��͵� ���� -> ���
                        count++;
                    }
                    fseek(fp, (-2) * (column + 1) - 1, SEEK_CUR);
                }
            }
        }
        plus[count] = 'x';
        count++;
        fseek(fp, 2 * (column + 1) + 1,
            SEEK_CUR); // ¦�� ��° ���� ��ŵ(��¥�� + �ȳ���)
    }

    for (i = 0; i < count; i++) // x��ǥ �����س����� ����ϴ� �Լ�
    {
        if (-10 <= plus[i] && plus[i] <= 10) {
            printf("%d %d\n", i, plus[i]);
        }
        else
            printf("%d %c\n", i, plus[i]);
    }

    fclose(fp);

    draw_maze(plus, column, row);

    free(plus);

    return 0;
}

void draw_maze(char* plus, int column, int row)
{
    int i, j;
    int count = 0;
    int temp;
    int arr[41][41] = { 0, };

    arr[0][0] = '+'; // �𼭸� 4�� ����
    arr[0][2 * column] = '+';
    arr[2 * row][0] = '+';
    arr[2 * row][2 * column] = '+';

    for (i = 1; i < 2 * row; i++) // �޺� ������ ����
    {
        arr[i][0] = '|';
        arr[i][2 * column] = '|';
    }

    for (i = 0; plus[i] != '\0'; i++)
    {
        if (plus[i] >= 0 && plus[i] <= 21) // + ����� ��
        {
            temp = plus[i];
            arr[count][2 * temp] = '+';
            arr[count][2 * temp + 1] = ' ';
            arr[count + 1][2 * temp] = ' ';
            arr[count + 1][2 * temp + 1] = ' ';
        }
        else if (plus[i] >= 'A' && plus[i] <= 'W') // + - | �빮�� ���ĺ��� ��
        {
            temp = plus[i] - 'A';
            arr[count][2 * temp] = '+';
            arr[count][2 * temp + 1] = '-';
            arr[count + 1][2 * temp] = '|';
            arr[count + 1][2 * temp + 1] = ' ';
        }
        else if (plus[i] >= -21 && plus[i] <= -1) // + - ������ ��
        {
            temp = plus[i] * (-1);
            arr[count][2 * temp] = '+';
            arr[count][2 * temp + 1] = '-';
            arr[count + 1][2 * temp] = ' ';
            arr[count + 1][2 * temp + 1] = ' ';
        }
        else if (plus[i] >= 'a' && plus[i] <= 'w') // + | �ҹ��� ���ĺ��� ��
        {
            temp = plus[i] - 'a';
            arr[count][2 * temp] = '+';
            arr[count][2 * temp + 1] = ' ';
            arr[count + 1][2 * temp] = '|';
            arr[count + 1][2 * temp + 1] = ' ';
        }
        else // �� �ٲ�
        {
            count += 2;
        }
    }

    for (i = 1; i < 2 * column; i++) // ���� �Ʒ��� ����
    {
        if (arr[0][i] != '+')
        {
            arr[0][i] = '-';
        }
        else arr[1][i] = '|';
        arr[2 * row][i] = '-';
    }

    for (i = 0; i < 2 * row + 1; i++) // �� ������ ���� �ִ�
    {
        for (j = 0; j < 2 * column + 1; j++)
        {
            if (arr[i][j] == 0 && (arr[i][j - 1] == '|' || arr[i][j + 1] == '|' || arr[i - 1][j] == '-' || arr[i + 1][j] == '-'))
            {
                arr[i][j] = ' ';
            }
        }
    }

    for (i = 0; i < 2 * row + 1; i++) // �� ������ ���� �ִ�.
    {
        for (j = 0; j < 2 * column + 1; j++)
        {
            if (arr[i][j] == 0)
            {
                if (arr[i][j - 1] == '-') arr[i][j] = '-';
                else if (arr[i - 1][j] == '|') arr[i][j] = '|';
                else arr[i][j] = ' ';
            }
        }
    }

    for (i = 0; i < 2 * column + 1; i++) // ������ 2�� �׸���
    {
        if (arr[2 * row - 2][i] == '|')
        {
            arr[2 * row - 1][i] = '|';
        }
        if (arr[2 * row - 1][i] == '|')
        {
            arr[2 * row][i] = '+';
        }
    }

    for (i = 0; i < 2 * row + 1; i++)
    {
        for (j = 0; j < 2 * column + 1; j++)
        {
            if (arr[i][j] != 0) printf("%c", arr[i][j]);
            else printf("%d", arr[i][j]);
        }
        printf("\n");
    }
}
