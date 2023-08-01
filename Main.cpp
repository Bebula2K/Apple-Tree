// Apple Tree game. Started 1st August, 2023
#include <iostream>
#include <thread>
#include <chrono>
#include <ctime>
#include "CommandHandler.h"
#include "values.h"

std::string getCurrentTime() {
    auto now = std::chrono::system_clock::now();
    std::time_t time = std::chrono::system_clock::to_time_t(now);
    struct std::tm timeInfo;
    localtime_r(&time, &timeInfo);
    char buffer[9];
    strftime(buffer, sizeof(buffer), "%T", &timeInfo);
    return std::string(buffer);
}

int main() {
    srand(static_cast<unsigned int>(time(0))); // Initialize random seed

    CommandExecutor commandExecutor;
    system("clear");
    std::cout << "Type 'help' for a list of commands..." << std::endl;

    // Load the saved tree size before the main loop
    commandExecutor.loadVariables();

    while (true) {
        std::string currentTime = commandExecutor.getCurrentTime();
        std::cout << "[" << currentTime << "] > ";

        std::string userInput;
        std::getline(std::cin, userInput);
        if (!userInput.empty()) {
            commandExecutor.executeCommand(userInput);

            if (userInput == "water") {
                commandExecutor.saveVariables();
            }
        } else {
            std::cout << "Invalid Command" << std::endl;
        }
    }

    return 0;
}