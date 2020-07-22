#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main (void) {
    int sockfd, nbytes;
    char buf [BUFSIZ];
    char *mesg = "Hello, world";
    struct sockaddr_in servaddr;
    if ((sockfd = socket (AF_INET, SOCK_STREAM, 0)) < 0) {
        perror ("socket"); exit(1);
    }
    /* memoryをゼロで埋める. memsetは埋める値を指定できる */
    bzero (&servaddr, sizeof (servaddr));
    /* 昔は色々なネットがあったが今はinternetしか使わないのでAF_INET以外を指定することはあまりない */
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons (10000); /* echo port is 7 */
    /* IP address of localhost is 127.0.0.1 */
    if (inet_pton (AF_INET, "127.0.0.1", &servaddr.sin_addr) < 0) {
        perror ("inet_pton"); exit (1);
    }
    if (connect (sockfd, (struct sockaddr *)&servaddr, sizeof (servaddr))< 0) {
        perror ("connect"); exit (1);
    }
    /* nbytesの値のチェックは省略(一度に送信できない場合もある) */
    nbytes = write (sockfd, mesg, strlen (mesg) + 1);
    nbytes = read (sockfd, buf, sizeof (buf) - 1);
    buf [nbytes] = '\0'; /* 後ろにnull文字を追加 */
    puts (buf); /* 改行付き出力 */
    close (sockfd);
    return 0;
} 
