#include <cpp_redis/cpp_redis>

#include <csignal>
#include <iostream>

volatile std::atomic_bool should_exit(false);
cpp_redis::redis_client client;

void sigint_handler(int) {
    std::cout << "disconnected (sigint handler)" << std::endl;
    client.disconnect();
}

void geoadd(const std::string& city, const std::string& latitude, const std::string& longitude, const std::string& area) {
    client.send({"GEOADD", city, latitude, longitude, area}, [] (auto reply) {
        std::cout << reply.as_string() << std::endl;
    });
}

void geodist(const std::string& city, const std::string& location_one, const std::string& location_two) {
    client.send({"GEODIST", city, location_one, location_two}, [] (auto reply) {
        std::cout << reply.as_string() << std::endl;
    });
}

void geohash(const std::string& city, const std::string& location_one) {
    client.send({"GEOHASH", city, location_one}, [] (auto reply) {
        std::cout << reply.as_string() << std::endl;
    });
}

void inputTestData() {
    geoadd("nyc", "40.7362722", "-73.9783613", "apartment");
    geoadd("nyc", "40.7475228", "-73.945493", "lic market");
}

int main() {
    client.set_disconnection_handler([] (auto&) {
        std::cout << "client disconnected (disconnection handler)" << std::endl;
        should_exit = true;
    });

    client.connect();

    inputTestData();

    signal(SIGINT, &sigint_handler);
    while (not should_exit);

    return 0;
}