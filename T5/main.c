#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>

#define PATH_LENGTH 512

enum error_type
{ CORRECT, DIR_ER };

enum error_type ls_for_dir(char* path);
void ls(int count, char* paths[]);

int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        printf("You must enter one or more paths\n");
        return 0;
    }

    ls(argc - 1, argv + 1);

    return 0;
}

enum error_type ls_for_dir(char* path)
{
    DIR *directory = opendir(path);

    if (directory == NULL)
        return DIR_ER;

    struct dirent* file;

    while ((file = readdir(directory)) != NULL)
    {
        char full_path[PATH_LENGTH];
        snprintf(full_path, sizeof(full_path) / sizeof(char), "%s/%s", path, file->d_name);
        struct stat file_stat;
        if (stat(full_path, &file_stat) == 0)
        {
            printf("%40s -> %9s\n", file->d_name, S_ISDIR(file_stat.st_mode) ? "DIRECTORY" :
                                                                          S_ISREG(file_stat.st_mode) ? "FILE" :
																		  S_ISLNK(file_stat.st_mode) ? "LINK" : "UNKNOWN");
        } else
        {
            printf("Error while opening file\n");
        }
    }

    closedir(directory);
    return CORRECT;
}

void ls(int count, char* paths[])
{
    for (int i = 0; i < count; ++i)
    {
        printf("Directory \"%s\" contains:\n", paths[i]);
        if (ls_for_dir(paths[i]) == DIR_ER)
        {
            printf("Error while reading directory\n");
        }
        printf("\n");
    }
}