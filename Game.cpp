//
// Created by SoSunDook on 25.09.2023.
//

#include "Game.h"

void Game::initConstants() {
    this->playerVelocity = 1.f;
    this->bulletsVelocity = 1.f;
    this->enemyVelocity = 0.6f;

    this->playerShootCooldown = sf::milliseconds(350);
    this->enemyShootCooldown = sf::milliseconds(350);

    this->bulletsScale = 3.f;
    this->enemiesScale = 3.f;
}

void Game::initWindow() {
    this->window = std::make_unique<sf::RenderWindow>(sf::VideoMode(720, 720), "Galaga");
    this->window->setFramerateLimit(600);
}

void Game::initTextures() {
    std::string path(this->dir_path.string());
    path.append("\\Data\\Textures");
    for (const auto & entry : std::filesystem::directory_iterator(path)) {
        auto entryPath = &entry.path();
        auto fileName = entryPath->stem().string();
        auto tmpTexture = std::make_shared<sf::Texture>();
        if (!tmpTexture->loadFromFile(entryPath->string())) {
            throw std::invalid_argument(fileName + " image can not be loaded");
        }
        this->textureManager[fileName] = tmpTexture;
    }
}

void Game::initPaths() {
    auto new_path = std::make_shared<BezierPath>();
    sf::Vector2<float> p0(720, 720);
    sf::Vector2<float> p3(0, 0);
    sf::Vector2<float> p1(720, 0);
    sf::Vector2<float> p2(0, 720);
    auto new_curve = BezierCurve(p0, p1, p2, p3);
    int a = 60;
    new_path->addCurve(new_curve, a);
    new_path->makePath();
    pathManager["zakoDefault"] = new_path;
}

void Game::initPlayer() {
    this->player = std::make_unique<Player>(this->textureManager["galaga"], *window, this->playerVelocity, this->playerShootCooldown);
}

std::shared_ptr<PlayerBullet> Game::initNewPlBullet() {
    auto newBullet = std::make_shared<PlayerBullet>(this->textureManager["bulletPlayer"], this->bulletsScale, this->bulletsVelocity);
    auto pos_x = this->player->getGlobalBounds().getPosition().x + (this->player->getLocalBounds().getSize().x - (newBullet->getLocalBounds().getSize().x * this->bulletsScale)) / 2;
    auto pos_y = this->player->getGlobalBounds().getPosition().y - (newBullet->getLocalBounds().getSize().y * this->bulletsScale);
    newBullet->setPosition(pos_x, pos_y);
    return newBullet;
}

void Game::initEnemies() {
    auto new_enemy = std::make_shared<Zako>(this->textureManager["zako"], this->enemyVelocity, this->enemyShootCooldown, this->enemiesScale);
//    auto tmp = pathManager["zakoDefault"];
//    new_enemy->setPosition(tmp->getPath().at(0).x, tmp->getPath().at(0).y);
//    new_enemy->setPath(tmp);
    float x = 620.f;
    float y = 620.f;
    new_enemy->setPosition(x, y);
    this->enemies.push_back(new_enemy);
}

Game::Game() {
    this->dir_path = std::filesystem::current_path();
    this->initConstants();
    this->initWindow();
    this->initTextures();
    this->initPaths();
    this->initPlayer();
    this->initEnemies();
}

void Game::updateInput() {
    bool left = false;
    bool right = false;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
        left = true;
        this->player->move(-1.f, 0.f, *window);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
        right = true;
        this->player->move(1.f, 0.f, *window);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::N) && this->player->canAttack()) {
        auto newBullet = this->initNewPlBullet();
        auto tmp_bvl = -this->bulletsVelocity;
        if (left) {
            auto tmp_vel = -this->playerVelocity;
            newBullet->setDirection(tmp_vel, tmp_bvl);
        } else if (right) {
            newBullet->setDirection(this->playerVelocity, tmp_bvl);
        } else {
            auto tmp_vel = 0.f;
            newBullet->setDirection(tmp_vel, tmp_bvl);
        }
        this->playerBullets.push_back(newBullet);
    }
}

void Game::updateBullets() {
    for (auto iter = this->playerBullets.begin(); iter != this->playerBullets.end(); ) {
        iter->get()->update();
//        Bounds check
        if ((iter->get()->getGlobalBounds().getPosition().y <= (-(iter->get()->getLocalBounds().getSize().y * this->bulletsScale)))
        || (iter->get()->getGlobalBounds().getPosition().x <= (-(iter->get()->getLocalBounds().getSize().x * this->bulletsScale)))
        || (iter->get()->getGlobalBounds().getPosition().x >= static_cast<float>(this->window->getSize().x))) {
            iter = this->playerBullets.erase(iter);
        } else {
            ++iter;
        }
    }
}

void Game::updatePlayers() {
    this->player->update();
}

void Game::updateEnemies() {
    for (auto & enemy : this->enemies) {
        enemy->update();
    }
}

void Game::updateCombat() {
    for (auto enemy_iter = this->enemies.begin(); enemy_iter != this->enemies.end(); ) {
        bool enemy_deleted = false;
        for (auto pl_bullet_iter = this->playerBullets.begin(); (pl_bullet_iter != this->playerBullets.end()) && (!enemy_deleted); ) {
            if (enemy_iter->get()->getGlobalBounds().intersects(pl_bullet_iter->get()->getGlobalBounds())) {
                enemy_iter = this->enemies.erase(enemy_iter);
                pl_bullet_iter = this->playerBullets.erase(pl_bullet_iter);
                enemy_deleted = true;
            } else {
                ++pl_bullet_iter;
            }
        }
        if (!enemy_deleted) {
            ++enemy_iter;
        }
    }
}

void Game::update() {
    this->updateInput();
    this->updatePlayers();
    this->updateBullets();
    this->updateEnemies();
    this->updateCombat();
}

void Game::render() {
    this->window->clear();

    this->player->render(*window);

    for (auto & playerBullet : this->playerBullets) {
        playerBullet->render(*window);
    }

    for (auto & enemy : this->enemies) {
        enemy->render(*window);
    }

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

