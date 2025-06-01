#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <pthread.h>

#define SOCKET_PATH "/tmp/david.sock"

Display *display;
Window window;
int screen;
GC gc;

void *socket_listener(void *arg) {
    int sockfd;
    struct sockaddr_un addr;

    sockfd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("socket");
        return NULL;
    }

    unlink(SOCKET_PATH);
    memset(&addr, 0, sizeof(struct sockaddr_un));
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, SOCKET_PATH, sizeof(addr.sun_path) - 1);

    if (bind(sockfd, (struct sockaddr*)&addr, sizeof(addr)) == -1) {
        perror("bind");
        close(sockfd);
        return NULL;
    }

    listen(sockfd, 5);
    printf("David Display Manager is listening on %s\n", SOCKET_PATH);

    char buffer[256];
    while (1) {
        int client = accept(sockfd, NULL, NULL);
        if (client == -1) continue;

        memset(buffer, 0, sizeof(buffer));
        read(client, buffer, sizeof(buffer));
        printf("Received command: %s\n", buffer);

        // Simple command handler
        if (strncmp(buffer, "exit", 4) == 0) {
            printf("Exiting...\n");
            XDestroyWindow(display, window);
            XCloseDisplay(display);
            exit(0);
        }

        close(client);
    }

    return NULL;
}

int main() {
    display = XOpenDisplay(NULL);
    if (display == NULL) {
        fprintf(stderr, "Unable to open X display.\n");
        return 1;
    }

    screen = DefaultScreen(display);
    int width = DisplayWidth(display, screen);
    int height = DisplayHeight(display, screen);

    window = XCreateSimpleWindow(display, RootWindow(display, screen),
                                 0, 0, width, height, 0,
                                 BlackPixel(display, screen), WhitePixel(display, screen));

    // Make the window fullscreen
    Atom wm_state = XInternAtom(display, "_NET_WM_STATE", False);
    Atom fullscreen = XInternAtom(display, "_NET_WM_STATE_FULLSCREEN", False);
    XChangeProperty(display, window, wm_state, XA_ATOM, 32,
                    PropModeReplace, (unsigned char *)&fullscreen, 1);

    XMapWindow(display, window);
    XFlush(display);

    // Start the socket listener in a thread
    pthread_t thread_id;
    pthread_create(&thread_id, NULL, socket_listener, NULL);

    // Keep the window running
    XEvent event;
    while (1) {
        XNextEvent(display, &event);
        // Currently does nothing with events
    }

    return 0;
}
