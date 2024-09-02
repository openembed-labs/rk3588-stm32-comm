#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <syslog.h>
#include "interactive.h"
#include "logger.h"
#include "common.h"

void interactive_mode_main()
{
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
        }
    }

    fclose(input_file);
}
