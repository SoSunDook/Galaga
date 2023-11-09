//
// Created by SoSunDook on 08.11.2023.
//

#ifndef GALAGA_HIGHSCORE_H
#define GALAGA_HIGHSCORE_H
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <memory>
#include <string>
#include <filesystem>
#include <iostream>
#include <fstream>

class Highscore {
private:
    std::shared_ptr<std::filesystem::path> dir_path;
    std::string highScorePath;
public:
    explicit Highscore(std::shared_ptr<std::filesystem::path> & dirPath);
    ~Highscore() = default;

    int read();

    void write(int newHighscore);
};


#endif //GALAGA_HIGHSCORE_H
