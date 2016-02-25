#include <cpp_redis/cpp_redis>

#include <csignal>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

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

void inputTestData(const std::string& fileName) {
    std::string buffer;
    std::ifstream file(fileName);
    
    if (!file.good()) return;

    std::getline(file, buffer);
    std::string key = buffer;

    while (std::getline(file, buffer)) {
        std::stringstream ss(buffer);
        std::string item;

        std::vector<std::string> tokens;
        while (std::getline(ss, item, ',')) {
            tokens.push_back(item);
        }
        geoadd(key, tokens[0], tokens[1], tokens[2]);
    }
}

int main() {
    client.set_disconnection_handler([] (auto&) {
        std::cout << "client disconnected (disconnection handler)" << std::endl;
        should_exit = true;
    });

    client.connect();

    inputTestData("test_data/nyu.txt");

    signal(SIGINT, &sigint_handler);
    while (not should_exit);

    return 0;
}