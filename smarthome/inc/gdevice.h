#include <stdio.h>
#include <wiringPi.h>

#ifndef __GDEVICE_H
#define __GDEVICE_H

struct gdevice
{
    char dev_name[128]; //设备名称
    int key; //key值，用于匹配控制指令的值
    int gpio_pin; //控制的gpio引脚 6 7 8SS
    int gpio_mode; //输入输出模式 INPUT OUPUT
    int gpio_status; //高低电平状态 LOW HIGHs
    int check_face_status; //是否进行人脸检测状态
    int voice_set_status; //是否语音语音播报
    struct gdevice *next;
};

// struct gdevice *add_device_to_gdevice_list(struct gdevice *pgdevhead, struct gdevice *gdev);

struct gdevice *find_device_by_key(struct gdevice *pgdevhead, unsigned char key);

int set_gpio_gdevice_status(struct gdevice *pgdev);

#endif