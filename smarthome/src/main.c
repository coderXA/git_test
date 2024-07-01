#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <wiringPi.h>

#include "voice_interface.h"
#include "socket_interface.h"
#include "smoke_interface.h"
#include "receive_interface.h"
#include "msg_queue.h"
#include "control.h"
#include "global.h"

int main(int argc, char *argv[])
{
    pthread_t thread_id;
    struct control *control_phead = NULL;
    struct control *pointer = NULL;
    int node_num = 0;

    ctrl_info_t *ctrl_info = NULL;

    ctrl_info = (ctrl_info_t *)malloc(sizeof(ctrl_info_t));
    ctrl_info->ctrl_phead = NULL;
    ctrl_info->mqd = -1;

    if (-1 == wiringPiSetup())
    {
        return -1;
    }

    ctrl_info->mqd = msg_queue_create();

    if (ctrl_info->mqd == -1)
    {
        printf("%s|%s|%d,ctrl_info->mqd=%d\n",__FILE__, __func__,__LINE__,ctrl_info->mqd);
        return -1;
    }

    ctrl_info->ctrl_phead = add_voice_to_ctrl_list(ctrl_info->ctrl_phead);
    ctrl_info->ctrl_phead = add_tcpsocket_to_ctrl_list(ctrl_info->ctrl_phead);
    ctrl_info->ctrl_phead = add_smoke_to_ctrl_list(ctrl_info->ctrl_phead);
    ctrl_info->ctrl_phead = add_receive_to_ctrl_list(ctrl_info->ctrl_phead);

    pointer = ctrl_info->ctrl_phead;
    while (pointer != NULL)
    {
        if (NULL != pointer->init)
        {
            printf("%s|%s|%d:pointer->control_name=%s\n",__FILE__, __func__,__LINE__,pointer->control_name);
            pointer->init();
        }
        pointer = pointer->next;
        node_num ++;
    }

    pthread_t *tid = malloc(sizeof(int) * node_num);
    int i = 0;

    pointer = ctrl_info->ctrl_phead;
    for (int i = 0; i < node_num; i++) 
    {
        if (NULL != pointer->get)
        {        
            printf("%s|%s|%d:pointer->control_name=%s\n",__FILE__, __func__,__LINE__,pointer->control_name);
            pthread_create(&tid[i], NULL, (void *)pointer->get, (void *)ctrl_info);
        }
        pointer = pointer->next;
    }

    for (int i = 0; i < node_num; i++)
    {
        pthread_join(tid[i], NULL);
    }

    for (int i = 0; i < node_num; i++)
    {
        if (NULL != pointer->final)
            pointer->final();
        pointer = pointer->next;
    }

    msg_queue_final(ctrl_info->mqd);
    if (NULL != ctrl_info)
    {
        free(ctrl_info);
    }

    if (NULL != tid)
    {
        free(tid);
    }
    
    return 0;
}