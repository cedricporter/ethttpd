/**
 * @file   main.c
 * @author Hua Liang[Stupid ET]
 *
 * @brief
 *
 *
 */

#include "ethttpd.h"

int main(int argc, char **argv)
{
    int sockfd, n;
    char recvline[MAXLINE + 1];
    struct sockaddr_in servaddr;

    if (argc != 2)
    {
        printf("usage: a.out ip");
        exit(-1);
    }

    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("sokcet error");
        exit(-1);
    }


    bzero(&servaddr, sizeof(servaddr));

    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(8787);
    if (inet_pton(AF_INET, argv[1], &servaddr.sin_addr) <= 0)
    {
        printf("inet_pton error");
        exit(-1);
    }

    if (connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
    {
        printf("connect error");
        exit(-1);
    }

    while ((n = read(sockfd, recvline, MAXLINE)) > 0)
    {
        recvline[n] = 0;
        if (fputs(recvline, stdout) == EOF)
        {
            printf("end");

            exit(-1);
        }
    }

    if (n < 0)
    {
        printf("read error");
        exit(-1);
    }


    return 0;
}
