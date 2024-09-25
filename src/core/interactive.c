#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h> // For O_CREAT, O_EXCL
#include <semaphore.h>
#include "common.h"
#include "logger.h"

#define BUFFER_SIZE 256

// TODO 增加清理指令 清理命令行IO文件 防止错误写入数据导致不可用

void print_help()
{
    printf("Usage: Command file should contain lines with the following format:\n");
    printf("       <device_id> <hex_command_data>\n");
    printf("       Where <device_id> is a hexadecimal number (e.g., 0x01) and <hex_command_data> is a hex string (e.g., '01 23 45')\n");
    printf("Special commands:\n");
    printf("  /help        - Show this help message\n");
    printf("  /exit        - Exit interactive mode\n");
    printf("  /terminate   - Exit interactive mode (same as /exit)\n");
    printf("  /upgrade <file_path> - Upgrade firmware with the file located at <file_path>\n");
    printf("\n");
}

void interactive_mode_main()
{
    sem_t *sem = sem_open(SEMAPHORE_NAME, O_CREAT, 0644, 0);
    if (sem == SEM_FAILED)
    {
        log_error("Failed to create semaphore.");
        exit(EXIT_FAILURE);
    }

    FILE *input_file = fopen(COMMAND_FILE, "a+"); // 使用 "a+" 模式确保文件存在，并允许读取
    if (input_file == NULL)
    {
        log_error("Failed to open file for writing user commands.");
        exit(EXIT_FAILURE);
    }

    char command[BUFFER_SIZE];
    print_help(); // Print help when entering interactive mode
    while (1)
    {
        printf("Enter command: ");
        if (fgets(command, sizeof(command), stdin) != NULL)
        {
            // 去掉末尾的换行符
            command[strcspn(command, "\n")] = '\0';

            // 处理特殊命令
            if (strcmp(command, "/help") == 0)
            {
                print_help();
            }
            else if (strcmp(command, "/exit") == 0 || strcmp(command, "/terminate") == 0)
            {
                log_info("Exiting interactive mode.");
                break;
            }
            else
            {
                // 将用户输入写入文件
                fprintf(input_file, "%s\n", command);
                fflush(input_file);

                // 释放信号量，让另一个线程知道有命令写入
                sem_post(sem);
            }
        }
    }

    fclose(input_file);
    sem_close(sem); // Close the semaphore when done
}
