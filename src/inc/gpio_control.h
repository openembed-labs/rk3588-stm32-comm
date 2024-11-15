#ifndef GPIO_CONTROL_H
#define GPIO_CONTROL_H

int gpio_export(const char *gpio_pin);
int gpio_set_direction(const char *gpio_pin, const char *direction);
int gpio_set_value(const char *gpio_pin, int value);
int gpio_unexport(const char *gpio_pin);
void gpio_init_high(const char *gpio_pin);
void gpio_cleanup(const char *gpio_pin);
void reset_stm32();

#endif // GPIO_CONTROL_H
