#ifndef UTILS_H_
#define UTILS_H_

void init_shell();
int str_is_int(const char *str);
int cmp_strings(const void *_a, const void *_b);
char **list_dir(const char *path);
int print_file_names(const char *path, int show_hidden, int print_path);

#endif