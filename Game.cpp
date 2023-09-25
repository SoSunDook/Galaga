//
// Created by SoSunDook on 25.09.2023.
//
#include "Game.h"
#include <memory>

void Game::initWindow() {
    this->window = std::make_unique<sf::RenderWindow>(sf::VideoMode(720, 1080), "Galaga");
}

void Game::initTextures() {

}

Game::Game() {
    this->initWindow();
    this->player = std::make_unique<Player>();
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

