#include "socket.h"

int socket_init(const char *ipaddr, const char *ipport)
{
    int serial_fd = -1;
    int ret = -1;
    struct sockaddr_in s_addr;
    memset(&s_addr,0,sizeof(struct sockaddr_in));

    //1. socket
    serial_fd = socket(AF_INET, SOCK_STREAM, 0);
    if(serial_fd == -1)
    {
        perror("socket");
        return -1;
    }

    s_addr.sin_family = AF_INET;
    s_addr.sin_port = htons(atoi(ipport));
    inet_aton(ipaddr,&s_addr.sin_addr);

    //2. bind
    ret = bind(serial_fd,(struct sockaddr *)&s_addr,sizeof(struct sockaddr_in));

    if (-1 == ret)
    {
        perror("bind");
        return -1;
    }

    //3. listen
    ret = listen(serial_fd,1); //只监听1个连接，排队扔垃圾
    if (-1 == ret)
    {
        perror("listen");
        return -1;
    }

    return serial_fd;
}