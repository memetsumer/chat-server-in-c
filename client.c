#include <sys/socket.h>
#include <netinet/in.h>
#include <poll.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>

int main()
{
    printf("HELLO WORLD CLIENT\n\n\n\n");
    int socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    fflush(stdout);                      // Explicitly flush the output buffer
    printf("socket fd %d\n", socket_fd); // Added '\n' and fflush

    struct sockaddr_in address = {
        .sin_family = AF_INET,
        .sin_port = htons(5174),
        INADDR_ANY,
    };

    int result = connect(socket_fd, &address, sizeof(address));
    printf("connect client: %d\n", result);

    // stdin - 0

    struct pollfd fds[2] =
        {
            {0,
             POLLIN,
             0},
            {socket_fd,
             POLLIN,
             0}};

    for (;;)
    {

        char buffer[256] = {0};

        poll(fds, 2, 50000);

        if (fds[0].revents & POLLIN)
        {
            read(0, buffer, 255);
            send(socket_fd, buffer, 255, 0);
        }
        else if (fds[1].revents & POLLIN)
        {
            if ((recv(socket_fd, buffer, 255, 0)) == 0)
            {
                return 0;
            }
            printf("%s\n", buffer);
        }
    }

    return 0;
}