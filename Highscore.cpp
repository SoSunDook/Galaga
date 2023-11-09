//
// Created by SoSunDook on 08.11.2023.
//

#include "Highscore.h"

Highscore::Highscore(std::shared_ptr<std::filesystem::path> & dirPath) {
    this->dir_path = dirPath;
    this->highScorePath = this->dir_path->string() + "\\Data\\Texts\\highscore.txt";
    if (!std::filesystem::exists(this->highScorePath)) {
        throw std::invalid_argument("High score can not be loaded");
    }
}

int Highscore::read() {
    std::ifstream input(this->highScorePath, std::ios::in);
    int highScore;
    input >> highScore;
    return highScore;
}

void Highscore::write(int newHighscore) {
    std::ofstream output(this->highScorePath, std::ios::out);
    output << newHighscore;
}
