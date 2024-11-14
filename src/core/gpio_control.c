#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include "gpio_control.h"
#include "logger.h"
#include "common.h"

int gpio_export(const char *gpio_pin)
{
    char path[50];
    snprintf(path, sizeof(path), "/sys/class/gpio/gpio%s", gpio_pin);

    // 检查 GPIO 是否已经被导出
    if (access(path, F_OK) == 0)
    {
        log_info("GPIO %s is already exported", gpio_pin);
        return 0; // 已经导出，无需再次导出
    }

    int fd = open("/sys/class/gpio/export", O_WRONLY);
    if (fd < 0)
    {
        log_error("Failed to open export for writing");
        return -1;
    }
    if (write(fd, gpio_pin, strlen(gpio_pin)) < 0)
    {
        log_error("Failed to export GPIO");
        close(fd);
        return -1;
    }
    close(fd);
    return 0;
}

int gpio_set_direction(const char *gpio_pin, const char *direction)
{
    char path[50];
    snprintf(path, sizeof(path), "/sys/class/gpio/gpio%s/direction", gpio_pin);

    int fd = open(path, O_WRONLY);
    if (fd < 0)
    {
        log_error("Failed to open gpio direction");
        return -1;
    }
    if (write(fd, direction, strlen(direction)) < 0)
    {
        log_error("Failed to set direction");
        close(fd);
        return -1;
    }
    close(fd);
    return 0;
}

int gpio_set_value(const char *gpio_pin, int value)
{
    char path[50];
    snprintf(path, sizeof(path), "/sys/class/gpio/gpio%s/value", gpio_pin);

    int fd = open(path, O_WRONLY);
    if (fd < 0)
    {
        log_error("Failed to open gpio value");
        return -1;
    }
    if (write(fd, value ? "1" : "0", 1) < 0)
    {
        log_error("Failed to write value");
        close(fd);
        return -1;
    }
    close(fd);
    return 0;
}

int gpio_unexport(const char *gpio_pin)
{
    int fd = open("/sys/class/gpio/unexport", O_WRONLY);

    if (fd < 0)
    {
        log_error("Failed to open unexport for writing");
        return -1;
    }
    if (write(fd, gpio_pin, strlen(gpio_pin)) < 0)
    {
        log_error("Failed to unexport GPIO");
        close(fd);
        return -1;
    }
    close(fd);
    return 0;
}

void gpio_init_high(const char *gpio_pin)
{
    log_info("Initializing GPIO %s to high...", gpio_pin);

    if (gpio_export(gpio_pin) != 0)
    {
        log_error("Error exporting GPIO %s. Skipping GPIO initialization.", gpio_pin);
        return; // 错误兼容处理：跳过GPIO设置
    }

    if (gpio_set_direction(gpio_pin, "out") != 0)
    {
        log_error("Error setting GPIO %s direction. Attempting to unexport GPIO.", gpio_pin);
        gpio_unexport(gpio_pin);
        return;
    }

    if (gpio_set_value(gpio_pin, 1) != 0)
    {
        log_error("Error setting GPIO %s value. Attempting to unexport GPIO.", gpio_pin);
        gpio_unexport(gpio_pin);
        return;
    }

    usleep(100000); // 保持100毫秒

    if (gpio_unexport(gpio_pin) != 0)
    {
        log_error("Error unexporting GPIO %s", gpio_pin);
    }

    log_info("GPIO %s initialized to high.", gpio_pin);
}

void gpio_cleanup(const char *gpio_pin)
{
    log_info("Cleaning up GPIO %s...", gpio_pin);

    if (gpio_export(gpio_pin) != 0)
    {
        log_error("Error exporting GPIO %s. Skipping GPIO cleanup.", gpio_pin);
        return; // 错误兼容处理：跳过GPIO清理
    }

    if (gpio_set_direction(gpio_pin, "out") != 0)
    {
        log_error("Error setting GPIO %s direction. Attempting to unexport GPIO.", gpio_pin);
        gpio_unexport(gpio_pin);
        return;
    }

    if (gpio_set_value(gpio_pin, 0) != 0)
    {
        log_error("Failed to set GPIO %s to low", gpio_pin);
    }

    usleep(100000); // 100 毫秒

    if (gpio_unexport(gpio_pin) < 0)
    {
        log_error("Failed to unexport GPIO %s", gpio_pin);
    }

    log_info("GPIO %s cleanup complete.", gpio_pin);
}

/**
 * @brief 通过 GPIO 控制 STM32 复位
 */
void reset_stm32()
{
    const char *gpio_pin = GPIO_PIN; // 假设 GPIO_PIN 用于复位信号

    log_info("STM32H750 is being reset via GPIO...");

    if (gpio_export(gpio_pin) != 0) // 使用 gpio_export
    {
        log_error("Error exporting GPIO %s", gpio_pin);
        return;
    }

    if (gpio_set_direction(gpio_pin, "out") != 0) // 使用 gpio_set_direction
    {
        log_error("Error setting GPIO %s direction", gpio_pin);
        gpio_unexport(gpio_pin);
        return;
    }

    if (gpio_set_value(gpio_pin, 0) != 0) // 使用 gpio_set_value，拉低 GPIO 引脚以触发复位
    {
        log_error("Error setting GPIO %s to low", gpio_pin);
        gpio_unexport(gpio_pin);
        return;
    }

    usleep(100000); // 保持 100 毫秒

    if (gpio_set_value(gpio_pin, 1) != 0) // 使用 gpio_set_value，拉高 GPIO 引脚以释放复位
    {
        log_error("Error setting GPIO %s to high", gpio_pin);
        gpio_unexport(gpio_pin);
        return;
    }

    if (gpio_unexport(gpio_pin) != 0)
    {
        log_error("Error unexporting GPIO %s", gpio_pin);
    }

    log_info("STM32H750 reset complete.");
}
