#pragma once

#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>

void SetNonBlockOrDie(int sockfd)
{
    int fl = fcntl(sockfd, F_GETFL);
    if (fl < 0)
        exit(4);
    fcntl(sockfd, F_SETFL, fl | O_NONBLOCK);
}