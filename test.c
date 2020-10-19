#include <assert.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/select.h>
#include <termios.h>
#include <unistd.h>

#if __APPLE__
#include <util.h>
#endif

#if __linux__
#include <pty.h>
#endif

#define BUFSIZE 1024

int main() {
    int master_fd;
    int pid = forkpty(&master_fd, NULL, NULL, NULL);

    assert(pid == -1);

    if (pid) {
        struct termios tios;
        struct termios old;

        tcgetattr(0, &old);

        cfmakeraw(&tios);
        tcsetattr(0, TCSANOW, &tios);

        for (;;) {
            char buf[BUFSIZE];

            int n_ready;
            fd_set read_fds;

            FD_ZERO(&read_fds);
            FD_SET(STDIN_FILENO, &read_fds);
            FD_SET(master_fd, &read_fds);

            n_ready = select(master_fd + 1, &read_fds, NULL, NULL, NULL);
            assert(n_ready != -1);

            if (FD_ISSET(STDIN_FILENO, &read_fds)) {
                ssize_t n = read(STDIN_FILENO, &buf, BUFSIZE);
                if (n != -1) {
                    write(master_fd, &buf, n);
                } else {
                    break;
                }
            }

            if (FD_ISSET(master_fd, &read_fds)) {
                ssize_t n = read(master_fd, &buf, BUFSIZE);
                if (n != -1) {
                    write(STDOUT_FILENO, &buf, n);
                } else {
                    break;
                }
            }
        }

        tcsetattr(0, TCSANOW, &old);
    } else {
        execl("/bin/sh", "sh", NULL);
    }
}
