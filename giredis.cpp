#include <cpp_redis/cpp_redis>

#include <signal.h>
#include <iostream>

volatile std::atomic_bool should_exit(false);
cpp_redis::redis_client client;

void sigint_handler(int) {
    std::cout << "disconnected (sigint handler)" << std::endl;
    client.disconnect();
}

void geoadd(std::string city, std::string latitude, std::string longitude, std::string area) {
    client.send({"GEOADD", city, latitude, longitude, area}, [] (cpp_redis::reply& reply) {
        std::cout << reply.as_string() << std::endl;
    });
}

void geodist(std::string city, std::string location_one, std::string location_two) {
    client.send({"GEODIST", city, location_one, location_two}, [] (cpp_redis::reply& reply) {
        std::cout << reply.as_string() << std::endl;
    });
}

void geohash(std::string city, std::string location_one) {
    client.send({"GEOHASH", city, location_one}, [] (cpp_redis::reply& reply) {
        std::cout << reply.as_string() << std::endl;
    });
}

int main(void) {
    client.set_disconnection_handler([] (cpp_redis::redis_client&) {
        std::cout << "client disconnected (disconnection handler)" << std::endl;
        should_exit = true;
    });

    client.connect();

    signal(SIGINT, &sigint_handler);
    while (not should_exit);

    return 0;
}