#if 0
struct control
{
    char control_name[128]; //监听模块名称
    int (*init)(void); //初始化函数
    void (*final)(void);//结束释放函数
    void *(*get)(void *arg);//监听函数，如语音监听
    void *(*set)(void *arg); //设置函数，如语音播报
    struct control *next;
};
#endif

#include <pthread.h>
#include "voice_interface.h"
#include "uartTool.h"
#include "msg_queue.h"
#include "global.h"

static int serial_fd = -1;

static int voice_init(void)
{
    serial_fd = myserialOpen(SERIAL_DEV, BAUD);
    printf("%s|%s|%d:serial_fd=%d\n",__FILE__,__func__,__LINE__,serial_fd);
    return serial_fd;
}

static void voice_final(void)
{
    if(serial_fd != -1)
    {
        close(serial_fd);
        serial_fd = -1;
    }
}

//接收语音指令
static void *voice_get(void *arg)// mqd应该来自于arg传参
{
    unsigned char buffer[6] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    int len = 0;
    mqd_t mqd = -1;
    ctrl_info_t *ctrl_info = NULL;
    if (NULL != arg)
        ctrl_info = (ctrl_info_t *)arg;
    
    if (-1 == serial_fd)
    {   
        serial_fd = voice_init();
        if (-1 == serial_fd)
        {
            pthread_exit(0);
        }
    }

    if(NULL != ctrl_info)
    {
        mqd = ctrl_info->mqd;
    }   

    if (-1 == mqd)
    {
        pthread_exit(0);
    }

    pthread_detach(pthread_self());

    printf("%s thread start\n", __func__);
    while(1)
    {
        len = serialGetstring(serial_fd, buffer);   
        // printf("%s|%s|%d:0x%x,0x%x,0x%x,0x%x,0x%x,0x%x\n",__FILE__,__func__,__LINE__,\
        //                 buffer[0],buffer[1],buffer[2],buffer[3],buffer[4],buffer[5]);
        // printf("%s|%s|%d:len=%d\n",__FILE__,__func__,__LINE__,len);
        if (len > 0)
        {
            if(buffer[0] == 0xAA && buffer[1] == 0x55 
            && buffer[4] == 0x55 && buffer[5] == 0xAA)
            {
                printf("%s|%s|%d:send 0x%x,0x%x,0x%x,0x%x,0x%x,0x%x\n",__FILE__,__func__,__LINE__,\
                                    buffer[0],buffer[1],buffer[2],buffer[3],buffer[4],buffer[5]);
                send_message(mqd, buffer, len);//注意，不要用strlen去计算实际长度
            }
            memset(buffer, 0, sizeof(buffer));
        }
    }
    pthread_exit(0);
}

//语音播报
static void *voice_set(void *arg)
{
    pthread_detach(pthread_self());
    unsigned char *buffer = (unsigned char *)arg;

    if (-1 == serial_fd)
    {   
        serial_fd = voice_init();
        if (-1 == serial_fd)
        {
            pthread_exit(0);
        }
    }

    if (NULL != buffer)
    {
        serialSendstring(serial_fd, buffer, 6);
    }

    pthread_exit(0);
}

struct control voice_control = {
    .control_name = "voice",
    .init = voice_init, 
    .final = voice_final,
    .get = voice_get,
    .set = voice_set,
    .next = NULL
};

struct control *add_voice_to_ctrl_list(struct control *phead)
{//头插法
   return add_interface_to_ctrl_list(phead, &voice_control);
}

