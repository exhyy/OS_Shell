#include "utils.h"
#include "command.h"
#include <string.h>
#include <dirent.h>
#include <malloc.h>
#include <stdlib.h>

void init_shell()
{
    fprintf(stdout, "\n");
    fprintf(stdout, "__  __                           _____ __         ____\n");
    fprintf(stdout, "\\ \\/ /_  ____  ______ _____     / ___// /_  ___  / / /\n");
    fprintf(stdout, " \\  / / / / / / / __ `/ __ \\    \\__ \\/ __ \\/ _ \\/ / / \n");
    fprintf(stdout, " / / /_/ / /_/ / /_/ / /_/ /   ___/ / / / /  __/ / /  \n");
    fprintf(stdout, "/_/\\__,_/\\__, /\\__,_/\\____/   /____/_/ /_/\\___/_/_/   \n");
    fprintf(stdout, "        /____/                                        \n");
    fprintf(stdout, "\n");
}

int str_is_int(const char *str)
{
    const char *p = str;
    if(*p == '-')
        p++;
    while(*p != '\0')
    {
        if (*p < '0' || *p > '9')
            return 0;
        p++;
    }
    return 1;
}

int cmp_strings(const void *_a, const void *_b)
{
    char **a = (char **)_a;
    char **b = (char **)_b;
    return strcmp(*a, *b);
}

// 输入文件夹路径，返回该路径下所有文件（夹）的文件名
// 返回的指针需要手动free
char **list_dir(const char *path)
{
    int num_files = 0;
    int max_num_files = 16;
    // 多留一个位置存放NULL，作为指针数组结束的标志
    char **file_names = (char **)malloc((max_num_files + 1) * sizeof(char *));
    file_names[0] = NULL;
    DIR *dir = opendir(path);
    if(!dir)
    {
        return NULL;
    }
    struct dirent *entry = NULL;
    while ((entry = readdir(dir)) != 0)
    {
        if (num_files == max_num_files)
        {
            char **old = file_names;
            max_num_files *= 2;
            file_names = (char **)malloc((max_num_files + 1) * sizeof(char *));
            memcpy(file_names, old, num_files * sizeof(char *));
            free(old);
        }
        file_names[num_files] = entry->d_name;
        file_names[num_files + 1] = NULL; // 结束标志
        num_files++;
    }
    qsort(file_names, num_files, sizeof(file_names[0]), cmp_strings);
    closedir(dir);
    return file_names;
}

int print_file_names(const char *path, int show_hidden, int print_path)
{
    char **file_names = list_dir(path);
    if (file_names == NULL)
    {
        return 0;
    }
    if (print_path)
    {
        fprintf(stdout, "%s:\n", path);
    }
    for (char **p = file_names; *p != NULL; p++)
    {
        if ((*p)[0] != '.' || show_hidden)
        {
            fprintf(stdout, "%s\n", *p);
        }
    }
    free(file_names);
    return 1;
}