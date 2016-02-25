#include <cpp_redis/cpp_redis>

#include <csignal>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <regex>

volatile std::atomic_bool should_exit(false);
cpp_redis::redis_client client;

void sigint_handler(int) {
    std::cout << "disconnected (sigint handler)" << std::endl;
    client.disconnect();
}

struct GeoData {
    std::string coord1;
    std::string coord2;
    std::string name;
};

void geoadd(const std::string& city, GeoData& geo) {
    client.send({"GEOADD", city, geo.coord1, geo.coord2, geo.name}, [] (auto reply) {
        if (!reply.as_string().empty()) {
            std::cout << reply.as_string() << std::endl;
        }
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

GeoData ParseData(const std::string& str) {
    const auto regex = std::regex("^(\\-?\\d+(\\.\\d+)?),\\s*(\\-?\\d+(\\.\\d+)?),(.*)$");
    auto matches = std::smatch();   
    std::regex_match(str, matches, regex);    
    return { matches[1].str(), matches[3].str(), matches[5].str() };
}

void inputTestData(const std::string& fileName) {
    std::string buffer;
    std::ifstream file(fileName);
    
    if (!file.good()) return;

    std::getline(file, buffer);
    std::string key = buffer;

    while (std::getline(file, buffer)) {
        GeoData currentLineData = ParseData(buffer);
        geoadd(key, currentLineData);
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