#include <wiringPi.h>
#include "gdevice.h"

//头插法
#if 0
struct gdevice *add_device_to_gdevice_list(struct gdevice *pgdevhead, struct gdevice *gdev)
{
    if (NULL == pgdevhead)
    {
        pgdevhead = gdev;
    }
    else
    {
        gdev->next = pgdevhead;
        pgdevhead = gdev;
    }
    
    return pgdevhead;
}
#endif

struct gdevice *find_device_by_key(struct gdevice *pgdevhead, unsigned char key)
{
    struct gdevice *p = NULL;

    if (NULL == pgdevhead)
    {
        return NULL;
    }

    p = pgdevhead;

    while (p != NULL)
    {
        if (p->key == key)
        {
            return p;
        }
        p = p->next;
    }

    return NULL;
}

int set_gpio_gdevice_status(struct gdevice *pgdev)
{
    if (NULL == pgdev)
    {
        return -1;
    }

    if (-1 != pgdev->gpio_pin)
    {
        if (-1 != pgdev->gpio_mode)
        {
            pinMode(pgdev->gpio_pin, pgdev->gpio_mode);//配置引脚的输入输出模式
        }
        if (-1 != pgdev->gpio_status)
        {
            digitalWrite(pgdev->gpio_pin, pgdev->gpio_status);//配置引脚的输出状态
        }
    }
    return 0;
}
