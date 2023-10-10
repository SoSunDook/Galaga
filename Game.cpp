//
// Created by SoSunDook on 25.09.2023.
//

#include "Game.h"

void Game::initConstants() {
    this->playerVelocity = 1.f;
    this->bulletsVelocity = 1.f;
    this->enemyVelocity = 0.6f;
    this->enemyRotationVelocity = 3.f;

    this->playerShootCooldown = sf::milliseconds(350);
    this->enemyShootCooldown = sf::milliseconds(350);

    this->bulletsScale = 3.f;
    this->enemiesScale = 3.f;

    this->maxCountZako = 20;
    this->maxCountGoei = 16;
    this->maxCountBoss = 4;

    this->currentCountZako = {};
    this->currentCountGoei = {};
    this->currentCountBoss = {};
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

void Game::initFormation() {
    this->formation = std::make_shared<Formation>();
}

void Game::initPaths() {
    int samples = 60;

    std::shared_ptr<BezierPath> new_path;
    sf::Vector2<float> p0, p1, p2, p3;
    BezierCurve new_curve;

    new_path = std::make_shared<BezierPath>();
    p0 = {435, -75};
    p3 = {25, 335};
    p1 = {435, 205};
    p2 = {25, 205};
    new_curve = BezierCurve(p0, p1, p2, p3);
    new_path->addCurve(new_curve, samples);
    p0 = {25, 335};
    p3 = {285, 335};
    p1 = {25, 595};
    p2 = {285, 595};
    new_curve = BezierCurve(p0, p1, p2, p3);
    new_path->addCurve(new_curve, samples);
    new_path->makePath();
    pathManager["flyInHook"] = new_path;

    new_path = std::make_shared<BezierPath>();
    p0 = {-75, 600};
    p3 = {285, 410};
    p1 = {50, 600};
    p2 = {285, 600};
    new_curve = BezierCurve(p0, p1, p2, p3);
    new_path->addCurve(new_curve, samples);
    p0 = {285, 410};
    p3 = {25, 410};
    p1 = {285, 150};
    p2 = {25, 150};
    new_curve = BezierCurve(p0, p1, p2, p3);
    new_path->addCurve(new_curve, samples);
    p0 = {25, 410};
    p3 = {285, 410};
    p1 = {25, 670};
    p2 = {285, 670};
    new_curve = BezierCurve(p0, p1, p2, p3);
    new_path->addCurve(new_curve, samples);
    new_path->makePath();
    pathManager["flyInCircle"] = new_path;

// flyInCircleMirrored = {720 - point.x, point.y};
}

void Game::initPlayer() {
    this->player = std::make_unique<Player>(this->textureManager["galaga"], this->playerVelocity, this->playerShootCooldown);
}

std::shared_ptr<PlayerBullet> Game::initNewPlBullet() {
    auto newBullet = std::make_shared<PlayerBullet>(this->textureManager["bulletPlayer"], this->bulletsScale, this->bulletsVelocity);
    auto pos_x = this->player->getGlobalBounds().getPosition().x + this->player->getOrigin().x;
    auto pos_y = this->player->getGlobalBounds().getPosition().y - newBullet->getGlobalBounds().getSize().y / 2;
    newBullet->setPosition(pos_x, pos_y);
    return newBullet;
}

void Game::initEnemies() {
    for (int i = 0; i < this->maxCountZako; ++i) {
        auto new_enemy = std::make_shared<Zako>(this->formation, this->textureManager["zako"], this->enemyVelocity, this->enemyRotationVelocity, this->enemyShootCooldown, this->enemiesScale, i);
        auto tmp = pathManager["flyInHook"];
        new_enemy->setPosition(tmp->getPath().at(0).x, tmp->getPath().at(0).y);
        float rotation = 180;
        new_enemy->setRotation(rotation);
        new_enemy->setPath(tmp);
        this->enemies.push_back(new_enemy);
        this->currentCountZako++;
    }

    for (int i = 0; i < this->maxCountGoei; ++i) {
        auto new_enemy = std::make_shared<Goei>(this->formation, this->textureManager["goei"], this->enemyVelocity, this->enemyRotationVelocity, this->enemyShootCooldown, this->enemiesScale, i);
        auto tmp = pathManager["flyInHook"];
        new_enemy->setPosition(tmp->getPath().at(0).x, tmp->getPath().at(0).y);
        float rotation = 180;
        new_enemy->setRotation(rotation);
        new_enemy->setPath(tmp);
        this->enemies.push_back(new_enemy);
        this->currentCountGoei++;
    }

    for (int i = 0; i < this->maxCountBoss; ++i) {
        auto new_enemy = std::make_shared<Boss>(this->formation, this->textureManager["boss"], this->enemyVelocity, this->enemyRotationVelocity, this->enemyShootCooldown, this->enemiesScale, i);
        auto tmp = pathManager["flyInHook"];
        new_enemy->setPosition(tmp->getPath().at(0).x, tmp->getPath().at(0).y);
        float rotation = 180;
        new_enemy->setRotation(rotation);
        new_enemy->setPath(tmp);
        this->enemies.push_back(new_enemy);
        this->currentCountBoss++;
    }
}

Game::Game() {
    this->dir_path = std::filesystem::current_path();
    this->initConstants();
    this->initWindow();
    this->initFormation();
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
        this->player->move(-1.f, 0.f);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
        right = true;
        this->player->move(1.f, 0.f);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::N) && this->player->canAttack()) {
        auto newBullet = this->initNewPlBullet();
        auto tmp_bvl = -this->bulletsVelocity;
        if (left && right) {
            auto tmp_vel = 0.f;
            newBullet->setDirection(tmp_vel, tmp_bvl);
        } else if (left) {
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

void Game::updateFormation() {
    this->formation->update();

    if (currentCountZako == maxCountZako && currentCountGoei == maxCountGoei && currentCountBoss == maxCountBoss) {
        bool flyIn = false;

        for (auto & enemy : this->enemies) {
            if (enemy->getCurrentState() == Enemy::flyIn) {
                flyIn = true;
            }
        }
        if (!flyIn) {
            this->formation->lock();
        }
    }
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
    this->updateFormation();
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

