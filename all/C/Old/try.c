#include <stdio.h>
#include <stdlib.h>

#define MAX_LINE_LENGTH 24

int main()
{
    FILE *fp = fopen("set.txt", "r");
    if (fp == NULL)
    {
        perror("Error opening file");
        return EXIT_FAILURE;
    }

    char line[MAX_LINE_LENGTH];
    if (fgets(line, MAX_LINE_LENGTH, fp) != NULL)
    {
        // 成功读取一行，现在line中包含了读取的内容（包括换行符，如果有的话）
        printf("Read from file: %s", line);
        int celllong = atoi(line);
        printf("to -> celllong :%d\n", celllong);
        int cellgroup[celllong];
        for (int i = 0; i < celllong; i++)
        {
            if (fgets(line, MAX_LINE_LENGTH, fp) != NULL)
            {
                cellgroup[i] = atoi(line);
            }
            else
            {
                break;
            }
        }
        printf("cellgroup:\n");
        for (int i = 0; i < celllong; i++)
        {
            printf("%d\n", cellgroup[i]);
        }
    }
    else
    {
        // 文件可能是空的，或者遇到了错误
        if (feof(fp))
        {
            printf("Reached end of file without reading any data.\n");
        }
        else
        {
            perror("Error reading from file");
        }
    }

    fclose(fp);
    return 0;
}