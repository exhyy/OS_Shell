#include "utils.h"
#include "command.h"
#include <string.h>
#include <dirent.h>
#include <malloc.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <fcntl.h>

#define O_RW 0666

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

int is_dir(const char *path)
{
    if (path == NULL)
    {
        return 0;
    }
    if (opendir(path) == NULL)
    {
        return 0;
    }
    return 1;
}

void split_path(const char *file_path, char *dir_path, char *file_name)
{
    if(file_path == NULL)
    {
        return;
    }
    int path_length = strlen(file_path);
    int split_index = -1;
    for (int i = path_length - 1; i >= 0; i--)
    {
        if(file_path[i] == '/')
        {
            split_index = i;
            break;
        }
    }
    if (split_index != -1)
    {
        if (dir_path != NULL)
        {
            for (int i = 0; i < split_index; i++)
            {
                dir_path[i] = file_path[i];
            }
        }
        if(file_name != NULL)
        {
            for (int i = split_index + 1; i < path_length; i++)
            {
                file_name[i] = file_path[i];
            }
        }
    }
    else
    {
        if (dir_path != NULL)
            strcpy(dir_path, "");
        if (file_name != NULL)
            strcpy(file_name, file_path);
    }
}

void copy_file(const char *dst, const char *src)
{
    int fd_src, fd_dst, count;
    char buffer[2048];
    int mode = syscall(SYS_access, src, X_OK) == 0 ? 0777 : 0666; // 判断是否可执行
    fd_src = syscall(SYS_open, src, O_RDONLY);
    fd_dst = syscall(SYS_creat, dst, mode);
    
    while ((count = syscall(SYS_read, fd_src, buffer, sizeof(buffer))) != 0)
    {
        syscall(SYS_write, fd_dst, buffer, count);
    }
    syscall(SYS_close, fd_src);
    syscall(SYS_close, fd_dst);
}