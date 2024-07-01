#include <stdio.h>
#include "msg_queue.h"
#define QUEUE_NAME "/mq_queue"
 
mqd_t msg_queue_create(void)
{
    //创建消息队列
    mqd_t mqd = -1;//消息队列描述符

    struct mq_attr attr;
    attr.mq_flags = 0;
    attr.mq_maxmsg = 10;//最大消息数
    attr.mq_msgsize = 256;//每个消息最大大小
    attr.mq_curmsgs = 0;//当前消息数

    //                 队列名称      打开标志        权限   属性
    mqd = mq_open(QUEUE_NAME,  O_CREAT | O_RDWR, 0666, &attr);

    printf("%s| %s |%d: mqd = %d\n",__FILE__, __func__, __LINE__,mqd);

    return mqd;
}

void msg_queue_final(mqd_t mqd)
{
    if (-1 != mqd)
        mq_close(mqd);
    mq_unlink(QUEUE_NAME);
    mqd = -1;
}

int send_message(mqd_t mqd, void *msg, int msg_len)
{   
    int byte_send = -1;

    byte_send = mq_send(mqd, (char *)msg, msg_len, 0 );

    return byte_send;
}