#include <sys/socket.h>
#include <netinet/in.h>
#include <poll.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>

int main()
{
    printf("HELLO WORLD SERVER \n\n\n\n");
    int socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    printf("socket fd %d\n", socket_fd);

    struct sockaddr_in address = {
        .sin_family = AF_INET,
        .sin_port = htons(5174),
        .sin_addr.s_addr = INADDR_ANY,
    };

    int result = bind(socket_fd, (struct sockaddr *)&address, sizeof(address));
    printf("bind server: %d\n", result);

    listen(socket_fd, 10);

    int client_fd = accept(socket_fd, NULL, NULL);

    // stdin - 0

    struct pollfd fds[2] =
        {
            {0, POLLIN, 0},
            {client_fd, POLLIN, 0}};

    // Timestamps for measuring polling speed
    struct timeval start_time, end_time;

    // Number of iterations
    int iterations = 0;

    // Record the start time
    gettimeofday(&start_time, NULL);

    for (;;)
    {
        char buffer[256] = {0};

        poll(fds, 2, 50000);

        if (fds[0].revents & POLLIN)
        {
            read(0, buffer, 255);
            send(client_fd, buffer, 255, 0);
        }
        else if (fds[1].revents & POLLIN)
        {
            if ((recv(client_fd, buffer, 255, 0)) == 0)
            {
                return 0;
            }
            printf("%s\n", buffer);
        }

        iterations++;

        // Record the end time after each iteration
        gettimeofday(&end_time, NULL);

        // Calculate the elapsed time in seconds
        double elapsed_time = (end_time.tv_sec - start_time.tv_sec) +
                              (end_time.tv_usec - start_time.tv_usec) / 1e6;

        // Print polling speed every second
        if (elapsed_time >= 1.0)
        {
            double polling_speed = iterations / elapsed_time;
            printf("Polling Speed: %.2f iterations per second\n", polling_speed);

            // Reset counters for the next measurement
            iterations = 0;
            gettimeofday(&start_time, NULL);
        }
    }

    return 0;
}
