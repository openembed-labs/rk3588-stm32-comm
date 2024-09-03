#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h> // For O_CREAT, O_EXCL
#include <semaphore.h>
#include "common.h"
#include "logger.h"

void interactive_mode_main()
{
    // 打开已存在的命名信号量
    sem_t *sem = sem_open(SEMAPHORE_NAME, 0);
    if (sem == SEM_FAILED)
    {
        log_error("Failed to open semaphore in interactive_mode_main.");
        exit(EXIT_FAILURE);
    }

    FILE *input_file = fopen(COMMAND_FILE, "a+"); // 使用 "a+" 模式确保文件存在，并允许读取
    if (input_file == NULL)
    {
        log_error("Failed to open file for writing user commands.");
        exit(EXIT_FAILURE);
    }

    char command[256];
    while (1)
    {
        printf("Enter command: ");
        if (fgets(command, sizeof(command), stdin) != NULL)
        {
            // 去掉末尾的换行符
            command[strcspn(command, "\n")] = '\0';

            // 检查是否输入了用于退出交互模式的特殊命令
            if (strcmp(command, "/exit") == 0 || strcmp(command, "/terminate") == 0)
            {
                log_info("Exiting interactive mode.");
                break;
            }

            // 将用户输入写入文件
            fprintf(input_file, "%s\n", command);
            fflush(input_file);

            // 释放信号量，让另一个线程知道有命令写入
            sem_post(sem);
        }
    }

    fclose(input_file);
    sem_close(sem); // Close the semaphore when done
}
