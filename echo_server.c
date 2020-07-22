#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <strings.h>
#include <sys/socket.h>
#include <netinet/in.h>

int main (void) {
    int listenfd, connfd, nbytes;
    char buf [BUFSIZ];
    struct sockaddr_in servaddr;
    if ((listenfd = socket (AF_INET, SOCK_STREAM, 0)) < 0) {
        perror ("socket"); exit (1);
    }
    bzero (&servaddr, sizeof (servaddr));
    servaddr.sin_family = AF_INET;
    /* host to network short (portは16byte) */
    servaddr.sin_port = htons (10000); /* echo port 7 is reserved  */
    /* host to network long (addressは32byte) */
    servaddr.sin_addr.s_addr = htonl (INADDR_ANY); 
    if (bind (listenfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0) {
        perror ("bind"); exit (1);
    }
    if (listen (listenfd, 5) < 0) {
        perror ("listen"); exit (1);
    }
    for (;;) {
        if ((connfd = accept (listenfd, (struct sockaddr*)NULL, NULL)) < 0) {
            perror ("accept"); exit (1);
        }
        while ((nbytes = read (connfd, buf, sizeof (buf))) > 0)
            write (connfd, buf, nbytes);
        close (connfd);
    }    
    return 0;
} 
