#ifndef __SOCKET__H
#define __SOCKET__H
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h> /* See NOTES */
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <errno.h>
#define IPADDR "192.168.1.237"
#define IPPORT "8192"//不能用8080，该端口已经被摄像头占用

int socket_init(const char *ipadrr, const char *ipport);
#endif