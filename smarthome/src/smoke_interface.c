#include <pthread.h>
#include <wiringPi.h>

#include "control.h"
#include "smoke_interface.h"
#include "msg_queue.h"
#include "global.h"

#define SMOKE_PIN 6
#define SMOKE_MODE INPUT

static int smoke_init(void)
{
    printf("%s|%s|%d\n",__FILE__, __func__, __LINE__);
    pinMode(SMOKE_PIN, SMOKE_MODE);

    return 0;
}

static void smoke_final(void)
{
    //do nothing;
}

static void* smoke_get(void *arg)
{
    int status = HIGH;
    int switch_status = 0;
    unsigned char buffer[6] = {0xAA, 0x55, 0x00, 0x00, 0x55, 0xAA};
    ssize_t byte_send = -1;
    mqd_t mqd = -1;
    ctrl_info_t *ctrl_info = NULL;

    if (NULL != arg)
	    ctrl_info = (ctrl_info_t *)arg;

	if(NULL != ctrl_info)
	{
		mqd = ctrl_info->mqd;
	}
	if ((mqd_t)-1 == mqd)
	{
		pthread_exit(0);
	}

	pthread_detach(pthread_self());
	printf("%s thread start\n", __func__);
    while (1)
    {
        status = digitalRead(SMOKE_PIN);
        if (LOW == status)
        {
            buffer[2] = 0x45;
            buffer[3] = 0x00;//测试值为0
            switch_status = 1;
            printf("%s|%s|%d:0x%x,0x%x,0x%x,0x%x,0x%x,0x%x\n",__FILE__,__func__,__LINE__,\
                            buffer[0],buffer[1],buffer[2],buffer[3],buffer[4],buffer[5]);
            byte_send = mq_send(mqd, buffer, 6, 0);
            if (-1 == byte_send)
            {
                continue;
            }
        }
        else if (HIGH == status && 1 == switch_status)
        {
            buffer[2] = 0x45;
            buffer[3] = 0x01;//状态变化
            switch_status = 0;
            printf("%s|%s|%d:0x%x,0x%x,0x%x,0x%x,0x%x,0x%x\n",__FILE__,__func__,__LINE__,\
                    buffer[0],buffer[1],buffer[2],buffer[3],buffer[4],buffer[5]);
            byte_send = mq_send(mqd, buffer, 6, 0);
            if (-1 == byte_send)
            {
                continue;
            }
        }
        sleep(5);
    }
    

	pthread_exit(0);
}

struct control smoke_control = {
    .control_name = "smoke",
    .init = smoke_init, 
    .final = smoke_final,
    .get = smoke_get,
    .set = NULL,
    .next = NULL
};

struct control *add_smoke_to_ctrl_list(struct control *phead)
{//头插法
    return add_interface_to_ctrl_list(phead, &smoke_control);
};