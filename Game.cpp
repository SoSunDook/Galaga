//
// Created by SoSunDook on 25.09.2023.
//
#include "Game.h"

void Game::initWindow() {
    this->window = std::make_unique<sf::RenderWindow>(sf::VideoMode(720, 1080), "Galaga");
}

void Game::initTextures() {
    std::string path(this->dir_path.string());
    path.append("\\Data\\Textures");
    for (const auto & entry : std::filesystem::directory_iterator(path)) {
        auto entryPath = &entry.path();
        auto fileName = entryPath->stem().string();
        auto tmpTexture = std::make_shared<sf::Texture>();
        if (!tmpTexture->loadFromFile(entryPath->string())) {
            throw std::invalid_argument(fileName + "image can not be loaded");
        }
        this->textureManager[fileName] = tmpTexture;
    }
}

Game::Game() {
    this->dir_path = std::filesystem::current_path();
    this->initWindow();
    this->initTextures();
    this->player = std::make_unique<Player>(this->textureManager["galaga"]);
}

void Game::update() {

}

void Game::render() {
    this->window->clear();
    this->player->render(*window);
    this->window->display();
}

void Game::run() {
    while (this->window->isOpen()) {
        sf::Event event{};
        while (window->pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window->close();
            }
        }
        this->update();
        this->render();
    }
}

