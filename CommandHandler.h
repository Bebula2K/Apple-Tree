#ifndef COMMAND_HANDLER_H
#define COMMAND_HANDLER_H

#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <cctype>
#include <ctime>
#include <chrono>
#include "values.h"

class CommandExecutor {
private:

    void increaseCooldownIfNeeded() {
        if (TreeSize >= 5.0F) {
            int increaseAmount = static_cast<int>(TreeSize) / 5; // Increase cooldown by 1 second every 5Ft
            cooldown = 5 + increaseAmount;
        }
    }

    void updateGrowthFactor() {
        growthFactor = 0.1F + (static_cast<int>(TreeSize) / 3) * 0.1F;
        if (TreeSize >= 5.0F) {
            int appleIncrease = (static_cast<int>(TreeSize) - 4) / 10 * 2;
            Apples += appleIncrease;
        }
    }

    void checkFertilizerUsage() {
        if (FertilizerUses >= 20) {
            growthFactor -= 0.3F;
            std::cout << "Your fertilizer has run out! Tree growth has decreased." << std::endl;
        }
    }

    void resetVariables() {
        TreeSize = 0.0F;
        Apples = 0;
        Coins = 0;
        FertilizerUses = 0;
        cooldown = 5;
        growthFactor = 0.1F;
        saveVariables(); // Save the reset variables to the file
    }

public:

    float TreeSize = 0.0F;
    int Apples = 0;
    int Coins = 0;
    int FertilizerUses = 0;
    std::chrono::steady_clock::time_point lastWaterTime;
    int cooldown = 5;
    float growthFactor = 0.1F;

        std::string getSaveFilePath() const {
        // You can modify the path to suit your requirements
        return "save.txt";
    }

    void loadVariables() {
        std::ifstream saveFile(getSaveFilePath());
        if (saveFile) {
            saveFile >> TreeSize >> Apples >> Coins >> FertilizerUses;
            saveFile.close();
        }
    }

    void saveVariables() {
        std::ofstream saveFile(getSaveFilePath());
        if (saveFile) {
            saveFile << TreeSize << "\n" << Apples << "\n" << Coins << "\n" << FertilizerUses;
            saveFile.close();
        }
    }

    void resetCommand() {
        std::string currentTime = getCurrentTime();
        resetVariables();
        std::cout << "[" << currentTime << "] All variables have been reset to their initial values." << std::endl;
    }


        std::string getCurrentTime() const {
        auto now = std::chrono::system_clock::now();
        std::time_t time = std::chrono::system_clock::to_time_t(now);
        struct std::tm timeInfo;
        localtime_r(&time, &timeInfo);
        char buffer[9];
        strftime(buffer, sizeof(buffer), "%T", &timeInfo);
        return std::string(buffer);
    }
    void executeCommand(const std::string& command) {
        std::string gameCommand = command;
        std::transform(gameCommand.begin(), gameCommand.end(), gameCommand.begin(), [](unsigned char c) {
            return std::tolower(c);
        });
        if (gameCommand == "exit") {
            exit(0);
        } else if (gameCommand == "help") {
            std::cout << "----Commands----" << std::endl;
            std::cout << "[+] Help" << std::endl;
            std::cout << "[+] Water" << std::endl;
            std::cout << "[+] Info" << std::endl;
            std::cout << "[+] Wallet" << std::endl;
            std::cout << "[+] Shop" << std::endl;
            std::cout << "[+] Collect" << std::endl;
            std::cout << "[+] Reset" << std::endl;
            std::cout << "[+] Exit" << std::endl;
            std::cout << "----------------" << std::endl;
        } else if (gameCommand == "water") {
            std::string currentTime = getCurrentTime();
            std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
            std::chrono::duration<float> timeElapsed = now - lastWaterTime;
            if (timeElapsed.count() >= cooldown) {
                lastWaterTime = now;
                float chance = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
                if (chance <= 0.33) {
                    TreeSize += growthFactor;
                    std::cout << "[" << currentTime << "] You watered the tree! Current tree size: " << TreeSize << "Ft" << std::endl;
                    updateGrowthFactor();
                } else {
                    std::cout << "[" << currentTime << "] The tree didn't grow this time. Current tree size: " << TreeSize << "Ft" << std::endl;
                }
                increaseCooldownIfNeeded();
            } else {
                std::cout << "[" << currentTime << "] You can water the tree again in " << (cooldown - static_cast<int>(timeElapsed.count())) << " seconds." << std::endl;
            }
        } else if (gameCommand == "collect") {
            std::string currentTime = getCurrentTime();
            std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
            std::chrono::duration<float> timeElapsed = now - lastWaterTime;
            if (timeElapsed.count() >= 120) {
                lastWaterTime = now;
                if (Apples > 0) {
                    int collectedApples = rand() % 3 + 1;
                    Apples -= collectedApples;
                    Coins += collectedApples;
                    std::cout << "[" << currentTime << "] You collected " << collectedApples << " apple(s) from the tree." << std::endl;
                } else {
                    std::cout << "[" << currentTime << "] There are no apples on the tree to collect." << std::endl;
                }
            } else {
                std::cout << "[" << currentTime << "] You can collect apples again in " << (120 - static_cast<int>(timeElapsed.count())) << " seconds." << std::endl;
            }
        } else if (gameCommand == "sell") {
            std::string currentTime = getCurrentTime();
            std::cout << "[" << currentTime << "] You have " << Coins << " coin(s) from selling " << Coins << " apple(s)." << std::endl;
        } else if (gameCommand == "wallet") {
            std::string currentTime = getCurrentTime();
            std::cout << "[" << currentTime << "] You currently have " << Coins << " coin(s) in your wallet." << std::endl;
        } else if (gameCommand == "shop") {
            std::string currentTime = getCurrentTime();
            if (Coins >= 10) {
                Coins -= 10;
                FertilizerUses = 0;
                growthFactor += 0.3F;
                std::cout << "[" << currentTime << "] You bought a fertilizer! Your tree growth has increased." << std::endl;
            } else {
                std::cout << "[" << currentTime << "] Not enough coins to buy a fertilizer." << std::endl;
            }
        } else if (gameCommand == "info") {
            std::cout << "[+]----------------------------------------------------------------------------------------[+]" << std::endl;
            std::cout << "|Apple tree is a game about growing an apple tree (duh).                                     | " << std::endl;
            std::cout << "|For every 3Ft of tree you grow, your growth increases by 0.2Ft.                             | " << std::endl;
            std::cout << "|To get coins, you must grow your tree to 5Ft. This unlocks the 'collect' command.           | " << std::endl;
            std::cout << "|Every 2 minutes the 'collect' command is active and when activated, collects 1-3 apples.    | " << std::endl;
            std::cout << "|Each apple sells for 1 coin using the 'sell' command.                                       | " << std::endl;
            std::cout << "|You gain more apples the more your tree grows!                                              | " << std::endl;
            std::cout << "|Use the 'shop' command to buy fertilizer. Each fertilizer has 20 uses and costs 10 coins.   | " << std::endl;
            std::cout << "|Fertilizing your tree increases its growth by 0.3Ft.                                        | " << std::endl;
            std::cout << "[+]----------------------------------------------------------------------------------------[+]" << std::endl;
        } else if(gameCommand == "reset"){
            resetVariables();
        } else if(gameCommand == "clear"){
            system("clear");
        }  else {
            std::cout << "Unrecognized command! Try again." << std::endl;
        }
    }
};

#endif
