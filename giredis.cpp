#include <cpp_redis/cpp_redis>

#include <algorithm>
#include <iostream>
#include <cstring>
#include <stdio.h>
#include <string.h>
#include <vector>
#include <signal.h>
using namespace std;

volatile std::atomic_bool should_exit(false);
cpp_redis::redis_client client;

void
sigint_handler(int) {
    std::cout << "disconnected (sigint handler)" << std::endl;
    client.disconnect();
}

int main(int argc, char const *argv[])
{
    printf("X");
    return 0;
}