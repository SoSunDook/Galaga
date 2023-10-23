//
// Created by SoSunDook on 25.09.2023.
//

#include "Game.h"

void Game::initConstants() {
    this->clock = {};

    this->playerVelocity = 1.f;
    this->bulletsVelocity = 1.f;
    this->enemyVelocity = 0.6f;
    this->enemyRotationVelocity = 5.4f;

    this->playerShootCooldown = sf::milliseconds(350);
    this->enemyShootCooldown = sf::milliseconds(350);

    this->bulletsScale = 3.f;
    this->enemiesScale = 3.f;
    this->playersScale = 0.75f;

    this->maxCountZako = 20;
    this->maxCountGoei = 16;
    this->maxCountBoss = 4;

    this->currentCountZako = {};
    this->currentCountGoei = {};
    this->currentCountBoss = {};

    this->aliveCountZako = {};
    this->aliveCountGoei = {};
    this->aliveCountBoss = {};

    this->currentFlyInPriority = {};
    this->currentFlyInIndex = {};

    this->spawningFinished = {};
    this->spawningDelay = 0.13f;
    this->spawningTimer = 0.f;

    this->respawningDelay = 2.f;
    this->respawningTimer = 0.f;

    this->divingGoei = {};
    this->skipFirstGoei = false;
    this->goeiDiveDelay = 6.f;
    this->goeiDiveTimer = 0.f;

    this->firstDivingZako = {};
    this->secondDivingZako = {};
    this->zakoDiveDelay = 3.5f;
    this->zakoDiveTimer = 0.f;
    this->zakoDiveTimer = {};

    this->divingBoss = {};
    this->captureDive = {};
    this->skipFirstBoss = {};
    this->bossDiveDelay = 7.f;
    this->bossDiveTimer = 0.f;

    this->capturedPlayer = {};
    this->savedCapturedPlayer = {};
    this->savedCapturedPlayerIndex = {};
}

void Game::initDeltaTime() {
    this->deltaTime = std::make_shared<sf::Time>();
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

void Game::initFormationVectors() {
    this->formationZakos.resize(this->maxCountZako);
    this->formationGoeis.resize(this->maxCountGoei);
    this->formationBosses.resize(this->maxCountBoss);
}

void Game::initFormation() {
    this->formation = std::make_shared<Formation>(this->deltaTime);
}

void Game::initSpawningPatterns() {
    std::string spawningPatternsPath = this->dir_path.string() + "\\Data\\Texts\\level.xml";
    this->spawningPatterns.load_file(spawningPatternsPath.c_str());
}

void Game::initPaths() {
    this->pathManager = std::make_shared<std::map<std::string, std::shared_ptr<BezierPath>>>();
    int samples = 60;

    std::shared_ptr<BezierPath> new_path;
    std::shared_ptr<BezierPath> new_path_mirrored;
    sf::Vector2<float> p0, p1, p2, p3;
    BezierCurve new_curve;

    new_path = std::make_shared<BezierPath>();
    new_path_mirrored = std::make_shared<BezierPath>();
    p0 = {435, -75};
    p3 = {50, 335};
    p1 = {435, 285};
    p2 = {50, 125};
    new_curve = BezierCurve(p0, p1, p2, p3);
    new_path->addCurve(new_curve, samples);

    p0 = {720 - p0.x, p0.y};
    p1 = {720 - p1.x, p1.y};
    p2 = {720 - p2.x, p2.y};
    p3 = {720 - p3.x, p3.y};
    new_curve = BezierCurve(p0, p1, p2, p3);
    new_path_mirrored->addCurve(new_curve, samples);

    p0 = {50, 335};
    p3 = {285, 335};
    p1 = {50, 500};
    p2 = {285, 500};
    new_curve = BezierCurve(p0, p1, p2, p3);
    new_path->addCurve(new_curve, samples);
    new_path->makePath();
    pathManager->operator[]("flyInHook") = new_path;

    p0 = {720 - p0.x, p0.y};
    p1 = {720 - p1.x, p1.y};
    p2 = {720 - p2.x, p2.y};
    p3 = {720 - p3.x, p3.y};
    new_curve = BezierCurve(p0, p1, p2, p3);
    new_path_mirrored->addCurve(new_curve, samples);
    new_path_mirrored->makePath();
    pathManager->operator[]("flyInHookMirrored") = new_path_mirrored;

    new_path = std::make_shared<BezierPath>();
    new_path_mirrored = std::make_shared<BezierPath>();
    p0 = {-75, 600};
    p3 = {270, 410};
    p1 = {50, 600};
    p2 = {270, 600};
    new_curve = BezierCurve(p0, p1, p2, p3);
    new_path->addCurve(new_curve, samples);

    p0 = {720 - p0.x, p0.y};
    p1 = {720 - p1.x, p1.y};
    p2 = {720 - p2.x, p2.y};
    p3 = {720 - p3.x, p3.y};
    new_curve = BezierCurve(p0, p1, p2, p3);
    new_path_mirrored->addCurve(new_curve, samples);

    p0 = {270, 410};
    p3 = {25, 410};
    p1 = {270, 250};
    p2 = {25, 250};
    new_curve = BezierCurve(p0, p1, p2, p3);
    new_path->addCurve(new_curve, samples);

    p0 = {720 - p0.x, p0.y};
    p1 = {720 - p1.x, p1.y};
    p2 = {720 - p2.x, p2.y};
    p3 = {720 - p3.x, p3.y};
    new_curve = BezierCurve(p0, p1, p2, p3);
    new_path_mirrored->addCurve(new_curve, samples);

    p0 = {25, 410};
    p3 = {270, 410};
    p1 = {25, 570};
    p2 = {270, 570};
    new_curve = BezierCurve(p0, p1, p2, p3);
    new_path->addCurve(new_curve, samples);
    new_path->makePath();
    pathManager->operator[]("flyInCircle") = new_path;

    p0 = {720 - p0.x, p0.y};
    p1 = {720 - p1.x, p1.y};
    p2 = {720 - p2.x, p2.y};
    p3 = {720 - p3.x, p3.y};
    new_curve = BezierCurve(p0, p1, p2, p3);
    new_path_mirrored->addCurve(new_curve, samples);
    new_path_mirrored->makePath();
    pathManager->operator[]("flyInCircleMirrored") = new_path_mirrored;

    new_path = std::make_shared<BezierPath>();
    new_path_mirrored = std::make_shared<BezierPath>();
    p0 = {0, 0};
    p3 = {-60, 0};
    p1 = {0, -60};
    p2 = {-60, -60};

    new_curve = BezierCurve(p0, p1, p2, p3);
    new_path->addCurve(new_curve, samples);

    p0 = {-p0.x, p0.y};
    p1 = {-p1.x, p1.y};
    p2 = {-p2.x, p2.y};
    p3 = {-p3.x, p3.y};
    new_curve = BezierCurve(p0, p1, p2, p3);
    new_path_mirrored->addCurve(new_curve, samples);

    p0 = {-60, 0};
    p3 = {320, 275};
    p1 = {-60, 275};
    p2 = {320, 50};

    new_curve = BezierCurve(p0, p1, p2, p3);
    new_path->addCurve(new_curve, samples);

    p0 = {-p0.x, p0.y};
    p1 = {-p1.x, p1.y};
    p2 = {-p2.x, p2.y};
    p3 = {-p3.x, p3.y};
    new_curve = BezierCurve(p0, p1, p2, p3);
    new_path_mirrored->addCurve(new_curve, samples);

    p0 = {320, 275};
    p3 = {45, 275};
    p1 = {320, 500};
    p2 = {45, 500};

    new_curve = BezierCurve(p0, p1, p2, p3);
    new_path->addCurve(new_curve, samples);
    new_path->makePath();
    pathManager->operator[]("divezako") = new_path;

    p0 = {-p0.x, p0.y};
    p1 = {-p1.x, p1.y};
    p2 = {-p2.x, p2.y};
    p3 = {-p3.x, p3.y};
    new_curve = BezierCurve(p0, p1, p2, p3);
    new_path_mirrored->addCurve(new_curve, samples);
    new_path_mirrored->makePath();
    pathManager->operator[]("divezakoMirrored") = new_path_mirrored;

    new_path = std::make_shared<BezierPath>();
    new_path_mirrored = std::make_shared<BezierPath>();
    p0 = {0, 0};
    p3 = {-60, 0};
    p1 = {0, -60};
    p2 = {-60, -60};

    new_curve = BezierCurve(p0, p1, p2, p3);
    new_path->addCurve(new_curve, samples);

    p0 = {-p0.x, p0.y};
    p1 = {-p1.x, p1.y};
    p2 = {-p2.x, p2.y};
    p3 = {-p3.x, p3.y};
    new_curve = BezierCurve(p0, p1, p2, p3);
    new_path_mirrored->addCurve(new_curve, samples);

    p0 = {-60, 0};
    p3 = {180, 210};
    p1 = {-60, 175};
    p2 = {180, 80};

    new_curve = BezierCurve(p0, p1, p2, p3);
    new_path->addCurve(new_curve, samples);

    p0 = {-p0.x, p0.y};
    p1 = {-p1.x, p1.y};
    p2 = {-p2.x, p2.y};
    p3 = {-p3.x, p3.y};
    new_curve = BezierCurve(p0, p1, p2, p3);
    new_path_mirrored->addCurve(new_curve, samples);

    p0 = {180, 210};
    p3 = {100, 360};
    p1 = {180, 275};
    p2 = {100, 295};

    new_curve = BezierCurve(p0, p1, p2, p3);
    new_path->addCurve(new_curve, samples);

    p0 = {-p0.x, p0.y};
    p1 = {-p1.x, p1.y};
    p2 = {-p2.x, p2.y};
    p3 = {-p3.x, p3.y};
    new_curve = BezierCurve(p0, p1, p2, p3);
    new_path_mirrored->addCurve(new_curve, samples);

    p0 = {100, 360};
    p3 = {330, 540};
    p1 = {100, 490};
    p2 = {330, 410};

    new_curve = BezierCurve(p0, p1, p2, p3);
    new_path->addCurve(new_curve, samples);

    p0 = {-p0.x, p0.y};
    p1 = {-p1.x, p1.y};
    p2 = {-p2.x, p2.y};
    p3 = {-p3.x, p3.y};
    new_curve = BezierCurve(p0, p1, p2, p3);
    new_path_mirrored->addCurve(new_curve, samples);

    p0 = {330, 540};
    p3 = {230, 615};
    p1 = {330, 590};
    p2 = {230, 565};

    new_curve = BezierCurve(p0, p1, p2, p3);
    new_path->addCurve(new_curve, samples);
    new_path->makePath();
    pathManager->operator[]("divegoei") = new_path;

    p0 = {-p0.x, p0.y};
    p1 = {-p1.x, p1.y};
    p2 = {-p2.x, p2.y};
    p3 = {-p3.x, p3.y};
    new_curve = BezierCurve(p0, p1, p2, p3);
    new_path_mirrored->addCurve(new_curve, samples);
    new_path_mirrored->makePath();
    pathManager->operator[]("divegoeiMirrored") = new_path_mirrored;

    new_path = std::make_shared<BezierPath>();
    new_path_mirrored = std::make_shared<BezierPath>();
    p0 = {0, 0};
    p3 = {-60, 0};
    p1 = {0, -60};
    p2 = {-60, -60};

    new_curve = BezierCurve(p0, p1, p2, p3);
    new_path->addCurve(new_curve, samples);

    p0 = {-p0.x, p0.y};
    p1 = {-p1.x, p1.y};
    p2 = {-p2.x, p2.y};
    p3 = {-p3.x, p3.y};
    new_curve = BezierCurve(p0, p1, p2, p3);
    new_path_mirrored->addCurve(new_curve, samples);

    p0 = {-60, 0};
    p3 = {60, 300};
    p1 = {-60, 100};
    p2 = {60, 200};

    new_curve = BezierCurve(p0, p1, p2, p3);
    new_path->addCurve(new_curve, samples);

    p0 = {-p0.x, p0.y};
    p1 = {-p1.x, p1.y};
    p2 = {-p2.x, p2.y};
    p3 = {-p3.x, p3.y};
    new_curve = BezierCurve(p0, p1, p2, p3);
    new_path_mirrored->addCurve(new_curve, samples);

    p0 = {60, 300};
    p3 = {185, 300};
    p1 = {60, 385};
    p2 = {185, 385};

    new_curve = BezierCurve(p0, p1, p2, p3);
    new_path->addCurve(new_curve, samples);

    p0 = {-p0.x, p0.y};
    p1 = {-p1.x, p1.y};
    p2 = {-p2.x, p2.y};
    p3 = {-p3.x, p3.y};
    new_curve = BezierCurve(p0, p1, p2, p3);
    new_path_mirrored->addCurve(new_curve, samples);

    p0 = {185, 300};
    p3 = {60, 300};
    p1 = {185, 215};
    p2 = {60, 215};

    new_curve = BezierCurve(p0, p1, p2, p3);
    new_path->addCurve(new_curve, samples);

    p0 = {-p0.x, p0.y};
    p1 = {-p1.x, p1.y};
    p2 = {-p2.x, p2.y};
    p3 = {-p3.x, p3.y};
    new_curve = BezierCurve(p0, p1, p2, p3);
    new_path_mirrored->addCurve(new_curve, samples);

    p0 = {60, 300};
    p3 = {200,  670};
    p1 = {60, 380};
    p2 = {200, 590};

    new_curve = BezierCurve(p0, p1, p2, p3);
    new_path->addCurve(new_curve, samples);
    new_path->makePath();
    pathManager->operator[]("diveboss") = new_path;

    p0 = {-p0.x, p0.y};
    p1 = {-p1.x, p1.y};
    p2 = {-p2.x, p2.y};
    p3 = {-p3.x, p3.y};
    new_curve = BezierCurve(p0, p1, p2, p3);
    new_path_mirrored->addCurve(new_curve, samples);
    new_path_mirrored->makePath();
    pathManager->operator[]("divebossMirrored") = new_path_mirrored;

    new_path = std::make_shared<BezierPath>();
    new_path_mirrored = std::make_shared<BezierPath>();
    p0 = {0, 0};
    p3 = {-60, 0};
    p1 = {0, -60};
    p2 = {-60, -60};

    new_curve = BezierCurve(p0, p1, p2, p3);
    new_path->addCurve(new_curve, samples);

    p0 = {-p0.x, p0.y};
    p1 = {-p1.x, p1.y};
    p2 = {-p2.x, p2.y};
    p3 = {-p3.x, p3.y};
    new_curve = BezierCurve(p0, p1, p2, p3);
    new_path_mirrored->addCurve(new_curve, samples);

    p0 = {-60, 0};
    p3 = {-195,  403};
    p1 = {-60, 100};
    p2 = {-195, 303};

    new_curve = BezierCurve(p0, p1, p2, p3);
    new_path->addCurve(new_curve, samples);
    new_path->makePath();
    pathManager->operator[]("divebosscaptureleft") = new_path;

    p0 = {-p0.x, p0.y};
    p1 = {-p1.x, p1.y};
    p2 = {-p2.x, p2.y};
    p3 = {-p3.x, p3.y};
    new_curve = BezierCurve(p0, p1, p2, p3);
    new_path_mirrored->addCurve(new_curve, samples);
    new_path_mirrored->makePath();
    pathManager->operator[]("divebosscaptureleftMirrored") = new_path_mirrored;

    new_path = std::make_shared<BezierPath>();
    new_path_mirrored = std::make_shared<BezierPath>();
    p0 = {0, 0};
    p3 = {-60, 0};
    p1 = {0, -60};
    p2 = {-60, -60};

    new_curve = BezierCurve(p0, p1, p2, p3);
    new_path->addCurve(new_curve, samples);

    p0 = {-p0.x, p0.y};
    p1 = {-p1.x, p1.y};
    p2 = {-p2.x, p2.y};
    p3 = {-p3.x, p3.y};
    new_curve = BezierCurve(p0, p1, p2, p3);
    new_path_mirrored->addCurve(new_curve, samples);

    p0 = {-60, 0};
    p3 = {12,  403};
    p1 = {-60, 100};
    p2 = {12, 303};

    new_curve = BezierCurve(p0, p1, p2, p3);
    new_path->addCurve(new_curve, samples);
    new_path->makePath();
    pathManager->operator[]("divebosscapturemid") = new_path;

    p0 = {-p0.x, p0.y};
    p1 = {-p1.x, p1.y};
    p2 = {-p2.x, p2.y};
    p3 = {-p3.x, p3.y};
    new_curve = BezierCurve(p0, p1, p2, p3);
    new_path_mirrored->addCurve(new_curve, samples);
    new_path_mirrored->makePath();
    pathManager->operator[]("divebosscapturemidMirrored") = new_path_mirrored;

    new_path = std::make_shared<BezierPath>();
    new_path_mirrored = std::make_shared<BezierPath>();
    p0 = {0, 0};
    p3 = {-60, 0};
    p1 = {0, -60};
    p2 = {-60, -60};

    new_curve = BezierCurve(p0, p1, p2, p3);
    new_path->addCurve(new_curve, samples);

    p0 = {-p0.x, p0.y};
    p1 = {-p1.x, p1.y};
    p2 = {-p2.x, p2.y};
    p3 = {-p3.x, p3.y};
    new_curve = BezierCurve(p0, p1, p2, p3);
    new_path_mirrored->addCurve(new_curve, samples);

    p0 = {-60, 0};
    p3 = {219,  403};
    p1 = {-60, 100};
    p2 = {219, 303};

    new_curve = BezierCurve(p0, p1, p2, p3);
    new_path->addCurve(new_curve, samples);
    new_path->makePath();
    pathManager->operator[]("divebosscaptureright") = new_path;

    p0 = {-p0.x, p0.y};
    p1 = {-p1.x, p1.y};
    p2 = {-p2.x, p2.y};
    p3 = {-p3.x, p3.y};
    new_curve = BezierCurve(p0, p1, p2, p3);
    new_path_mirrored->addCurve(new_curve, samples);
    new_path_mirrored->makePath();
    pathManager->operator[]("divebosscapturerightMirrored") = new_path_mirrored;
}

void Game::initPlayer() {
    this->player = std::make_unique<Player>(this->deltaTime, this->textureManager["playerExplosion"], this->textureManager["galaga"],
                                            this->playerVelocity, this->playerShootCooldown, this->playersScale);
}

std::shared_ptr<PlayerBullet> Game::initNewPlBullet() {
    auto newBullet = std::make_shared<PlayerBullet>(this->textureManager["bulletPlayer"], this->bulletsScale, this->bulletsVelocity);
    auto pos_x = this->player->getGlobalBounds().getPosition().x + this->player->getOrigin().x * this->playersScale;
    auto pos_y = this->player->getGlobalBounds().getPosition().y - newBullet->getGlobalBounds().getSize().y / 2;
    newBullet->setPosition(pos_x, pos_y);
    return newBullet;
}

Game::Game() {
    this->dir_path = std::filesystem::current_path();
    this->initConstants();
    this->initDeltaTime();
    this->initSpawningPatterns();
    this->initWindow();
    this->initFormationVectors();
    this->initFormation();
    this->initTextures();
    this->initPaths();
    this->initPlayer();
}

void Game::updateDeltaTime() {
    *(this->deltaTime) = this->clock.restart();
}

void Game::updateInput() {
    if (this->player->getCurrentState() == Player::STATES::alive) {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
            this->player->move(-1.f, 0.f);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
            this->player->move(1.f, 0.f);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::N) && this->player->canAttack()) {
            auto newBullet = this->initNewPlBullet();
            auto tmp_bvl = -this->bulletsVelocity;
            auto tmp_vel = 0.f;
            newBullet->setDirection(tmp_vel, tmp_bvl);
            this->playerBullets.push_back(newBullet);
        }
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

bool Game::enemyFlyingIn() {
    for (auto & zako : this->formationZakos) {
        if (zako && zako->getCurrentState() == Enemy::STATES::flyIn) {
            return true;
        }
    }

    for (auto & goei : this->formationGoeis) {
        if (goei && goei->getCurrentState() == Enemy::STATES::flyIn) {
            return true;
        }
    }

    for (auto & boss : this->formationBosses) {
        if (boss && boss->getCurrentState() == Enemy::STATES::flyIn) {
            return true;
        }
    }

    return false;
}

void Game::handleSpawning() {
    this->spawningTimer += this->deltaTime->asSeconds();
    if (this->spawningTimer >= this->spawningDelay) {
        auto element = this->spawningPatterns.root().child("Level").first_child();
        bool spawned = false;
        bool priorityFound = false;

        while (element) {
            int priority = element.attribute("priority").as_int();
            std::string path = element.attribute("path").as_string();
            auto child = element.first_child();

            if (this->currentFlyInPriority == priority) {
                priorityFound = true;
                for (int i = 0; (i < this->currentFlyInIndex) && child; ++i) {
                    child = child.next_sibling();
                }
                if (child) {
                    std::string type = child.attribute("type").as_string();
                    int index = child.attribute("index").as_int();

                    if (type == "zako") {

                        auto new_enemy_zako = std::make_shared<Zako>(this->deltaTime, this->pathManager->operator[](path), this->formation,
                                                                     this->textureManager["explosion"], this->textureManager["zako"],
                                                                     this->enemyVelocity, this->enemyRotationVelocity, this->enemyShootCooldown, this->enemiesScale, index);
                        this->formationZakos.at(index) = new_enemy_zako;
                        this->currentCountZako++;
                        this->aliveCountZako++;

                    } else if (type == "goei") {

                        auto new_enemy_goei = std::make_shared<Goei>(this->deltaTime, this->pathManager->operator[](path), this->formation,
                                                                     this->textureManager["explosion"], this->textureManager["goei"],
                                                                     this->enemyVelocity, this->enemyRotationVelocity, this->enemyShootCooldown, this->enemiesScale, index);
                        this->formationGoeis.at(index) = new_enemy_goei;
                        this->currentCountGoei++;
                        this->aliveCountGoei++;

                    } else if (type == "boss") {

                        auto new_enemy_boss = std::make_shared<Boss>(this->deltaTime, this->pathManager->operator[](path), this->formation,
                                                                     this->textureManager["explosion"], this->textureManager["boss"], this->textureManager["boss2"], this->textureManager["beam"],
                                                                     this->enemyVelocity, this->enemyRotationVelocity, this->enemyShootCooldown, this->enemiesScale, index);
                        this->formationBosses.at(index) = new_enemy_boss;
                        this->currentCountBoss++;
                        this->aliveCountBoss++;

                    }

                    spawned = true;

                }
            }
            element = element.next_sibling();
        }
        if (!priorityFound) {
            this->spawningFinished = true;
        } else {
            if (!spawned) {
                if (!this->enemyFlyingIn()) {
                    this->currentFlyInPriority++;
                    this->currentFlyInIndex = 0;
                }
            } else {
                this->currentFlyInIndex++;
            }
        }
        this->spawningTimer = 0.f;
    }
}

void Game::handleFormation() {
    this->formation->update();

    if (!this->formation->isLocked()) {
        if (currentCountZako == maxCountZako && currentCountGoei == maxCountGoei && currentCountBoss == maxCountBoss) {
            if (!this->enemyFlyingIn()) {
                this->formation->lock();
            }
        }
    } else {
        if (this->player->getCurrentState() == Player::STATES::alive) {
            this->handleDiving();
        } else {
            if (this->firstDivingZako != nullptr && this->firstDivingZako->getCurrentState() != Enemy::STATES::dive) {
                this->firstDivingZako = {};
            }
            if (this->secondDivingZako != nullptr && this->secondDivingZako->getCurrentState() != Enemy::STATES::dive) {
                this->secondDivingZako = {};
            }
            if (this->divingGoei != nullptr && this->divingGoei->getCurrentState() != Enemy::STATES::dive) {
                this->divingGoei = {};
            }
            if (this->divingBoss != nullptr && this->divingBoss->getCurrentState() != Enemy::STATES::dive) {
                this->divingBoss = {};
            }

            if (!(this->firstDivingZako || this->secondDivingZako || this->divingGoei || this->divingBoss)) {
                if (this->player->getCurrentState() != Player::STATES::dead) {
                    this->respawningTimer += this->deltaTime->asSeconds();
                    if (this->respawningTimer >= this->respawningDelay) {
                        this->player->respawn();
                        this->respawningTimer = 0.f;
                    }
                }
            }
        }
    }
}

void Game::handleDiving() {
    if (this->divingGoei == nullptr) {
        this->goeiDiveTimer += this->deltaTime->asSeconds();
        if (this->goeiDiveTimer >= this->goeiDiveDelay) {
            bool skipped;
            if (this->aliveCountGoei <= 1) {
                skipped = true;
            } else {
                skipped = false;
            }
            for (int i = this->formationGoeis.size() - 1; i >= 0; --i) {
                if (this->formationGoeis[i]->getCurrentState() == Enemy::STATES::formation) {
                    if (!this->skipFirstGoei || (this->skipFirstGoei && skipped)) {
                        this->divingGoei = this->formationGoeis[i];
                        std::string path = "divegoei";
                        if (i % 2 == 1) {
                            path.append("Mirrored");
                        }
                        this->divingGoei->setPath(this->pathManager->operator[](path));
                        this->divingGoei->toDive();
                        this->skipFirstGoei = !this->skipFirstGoei;
                        break;
                    }
                    skipped = true;
                }
            }
            this->goeiDiveTimer = 0.f;
        }
    } else {
        if (this->divingGoei->getCurrentState() != Enemy::STATES::dive) {
            this->divingGoei = {};
        }
    }

    this->zakoDiveTimer += this->deltaTime->asSeconds();
    if (this->zakoDiveTimer >= this->zakoDiveDelay) {
        for (int i = this->formationZakos.size() - 1; i >= 0; --i) {
            if (this->formationZakos[i]->getCurrentState() == Enemy::STATES::formation) {
                if (this->firstDivingZako == nullptr) {
                    this->firstDivingZako = this->formationZakos[i];
                    std::string path = "divezako";
                    if (i % 2 == 1) {
                        path.append("Mirrored");
                    }
                    this->firstDivingZako->setPath(this->pathManager->operator[](path));
                    this->firstDivingZako->toDive();
                } else if (this->secondDivingZako == nullptr) {
                    this->secondDivingZako = this->formationZakos[i];
                    std::string path = "divezako";
                    if (i % 2 == 1) {
                        path.append("Mirrored");
                    }
                    this->secondDivingZako->setPath(this->pathManager->operator[](path));
                    this->secondDivingZako->toDive();
                }
                break;
            }
        }
        this->zakoDiveTimer = 0.f;
    }

    if (this->firstDivingZako != nullptr && this->firstDivingZako->getCurrentState() != Enemy::STATES::dive) {
        this->firstDivingZako = {};
    }
    if (this->secondDivingZako != nullptr && this->secondDivingZako->getCurrentState() != Enemy::STATES::dive) {
        this->secondDivingZako = {};
    }

    if (this->divingBoss == nullptr) {
        this->bossDiveTimer += this->deltaTime->asSeconds();
        if (this->bossDiveTimer >= this->bossDiveDelay) {
            bool skipped;
            if (this->aliveCountBoss <= 1) {
                skipped = true;
            } else {
                skipped = false;
            }
            for (int i = this->formationBosses.size() - 1; i >= 0; --i) {
                if (this->formationBosses[i]->getCurrentState() == Enemy::STATES::formation) {
                    if (!this->skipFirstBoss || (this->skipFirstBoss && skipped)) {
                        this->divingBoss = this->formationBosses[i];
                        if (this->captureDive) {

                            std::string path = "divebosscapture";

                            srand(time (nullptr));
                            auto choose = rand() % 3;
                            switch (choose) {
                                case 0:
                                    path.append("left");
                                    break;
                                case 1:
                                    path.append("mid");
                                    break;
                                case 2:
                                    path.append("right");
                                    break;
                                default:
                                    path.append("mid");
                                    break;
                            }

                            if (i % 2 == 1) {
                                path.append("Mirrored");
                            }

                            this->divingBoss->setPath(this->pathManager->operator[](path));
                            this->divingBoss->toDive(true);
                        } else {

                            std::string path = "diveboss";
                            if (i % 2 == 1) {
                                path.append("Mirrored");
                            }

                            this->divingBoss->setPath(this->pathManager->operator[](path));
                            this->divingBoss->toDive();
                            int index = this->divingBoss->getIndex();
                            int firstEscortIndex = (index % 2 == 0) ? (index * 2) : (index * 2 - 1);
                            int secondEscortIndex = firstEscortIndex + 4;

                            if (this->formationGoeis[firstEscortIndex]->getCurrentState() == Enemy::STATES::formation) {

                                std::string path = "diveboss";
                                if (i % 2 == 1) {
                                    path.append("Mirrored");
                                }

                                this->formationGoeis[firstEscortIndex]->setPath(this->pathManager->operator[](path));
                                this->formationGoeis[firstEscortIndex]->toDive(true);
                            }
                            if (this->formationGoeis[secondEscortIndex]->getCurrentState() == Enemy::STATES::formation) {

                                std::string path = "diveboss";
                                if (i % 2 == 1) {
                                    path.append("Mirrored");
                                }

                                this->formationGoeis[secondEscortIndex]->setPath(this->pathManager->operator[](path));
                                this->formationGoeis[secondEscortIndex]->toDive(true);
                            }
                        }
                        this->skipFirstBoss = !this->skipFirstBoss;
                        this->captureDive = !this->captureDive;
                        break;
                    }
                    skipped = true;
                }
            }
            this->bossDiveTimer = 0.f;
        }
    } else {
        if (this->divingBoss->getCurrentState() != Enemy::STATES::dive) {
            this->divingBoss = {};
        }
    }
}

void Game::handlePVE() {
    for (auto pl_bullet_iter = this->playerBullets.begin(); pl_bullet_iter != this->playerBullets.end(); ) {
        bool bullet_deleted = false;
        for (auto zako_iter = this->formationZakos.begin(); (zako_iter != this->formationZakos.end()) && (!bullet_deleted); ) {
            if (*zako_iter) {
                if (zako_iter->get()->getCurrentState() != Enemy::STATES::dead) {
                    if (zako_iter->get()->getGlobalBounds().intersects(pl_bullet_iter->get()->getGlobalBounds())) {
                        pl_bullet_iter = this->playerBullets.erase(pl_bullet_iter);
                        zako_iter->get()->hit();
                        if (zako_iter->get()->getCurrentState() == Enemy::STATES::dead) {
                            this->aliveCountZako--;
                        }
                        bullet_deleted = true;
                        break;
                    } else {
                        ++zako_iter;
                    }
                } else {
                    ++zako_iter;
                }
            } else {
                ++zako_iter;
            }
        }
        for (auto goei_iter = this->formationGoeis.begin(); (goei_iter != this->formationGoeis.end()) && (!bullet_deleted); ) {
            if (*goei_iter) {
                if (goei_iter->get()->getCurrentState() != Enemy::STATES::dead) {
                    if (goei_iter->get()->getGlobalBounds().intersects(pl_bullet_iter->get()->getGlobalBounds())) {
                        pl_bullet_iter = this->playerBullets.erase(pl_bullet_iter);
                        goei_iter->get()->hit();
                        if (goei_iter->get()->getCurrentState() == Enemy::STATES::dead) {
                            this->aliveCountGoei--;
                        }
                        bullet_deleted = true;
                        break;
                    } else {
                        ++goei_iter;
                    }
                } else {
                    ++goei_iter;
                }
            } else {
                ++goei_iter;
            }
        }
        for (auto boss_iter = this->formationBosses.begin(); (boss_iter != this->formationBosses.end()) && (!bullet_deleted); ) {
            if (*boss_iter) {
                if (boss_iter->get()->getCurrentState() != Enemy::STATES::dead) {
                    if (boss_iter->get()->getGlobalBounds().intersects(pl_bullet_iter->get()->getGlobalBounds())) {
                        pl_bullet_iter = this->playerBullets.erase(pl_bullet_iter);
                        boss_iter->get()->hit();
                        if (boss_iter->get()->getCurrentState() == Enemy::STATES::dead) {
                            this->aliveCountBoss--;
                        }
                        bullet_deleted = true;
                        break;
                    } else {
                        ++boss_iter;
                    }
                } else {
                    ++boss_iter;
                }
            } else {
                ++boss_iter;
            }
        }

        if (!bullet_deleted) {
            ++pl_bullet_iter;
        }
    }
}

void Game::handleEVP() {
    if (this->player->getCurrentState() == Player::STATES::alive) {
        for (auto zako_iter = this->formationZakos.begin(); (zako_iter != this->formationZakos.end()) && (this->player->getCurrentState() == Player::STATES::alive); ) {
            if (*zako_iter) {
                if (zako_iter->get()->getCurrentState() != Enemy::STATES::dead) {
                    if (zako_iter->get()->getGlobalBounds().intersects(this->player->getGlobalBounds())) {
                        this->player->toGetHit();
                        zako_iter->get()->die();
                        if (zako_iter->get()->getCurrentState() == Enemy::STATES::dead) {
                            this->aliveCountZako--;
                        }
                        break;
                    } else {
                        ++zako_iter;
                    }
                } else {
                    ++zako_iter;
                }
            } else {
                ++zako_iter;
            }
        }
        for (auto goei_iter = this->formationGoeis.begin(); (goei_iter != this->formationGoeis.end()) && (this->player->getCurrentState() == Player::STATES::alive); ) {
            if (*goei_iter) {
                if (goei_iter->get()->getCurrentState() != Enemy::STATES::dead) {
                    if (goei_iter->get()->getGlobalBounds().intersects(this->player->getGlobalBounds())) {
                        this->player->toGetHit();
                        goei_iter->get()->die();
                        if (goei_iter->get()->getCurrentState() == Enemy::STATES::dead) {
                            this->aliveCountGoei--;
                        }
                        break;
                    } else {
                        ++goei_iter;
                    }
                } else {
                    ++goei_iter;
                }
            } else {
                ++goei_iter;
            }
        }
        for (auto boss_iter = this->formationBosses.begin(); (boss_iter != this->formationBosses.end()) && (this->player->getCurrentState() == Player::STATES::alive); ) {
            if (*boss_iter) {
                if (boss_iter->get()->getCurrentState() != Enemy::STATES::dead) {
                    if (boss_iter->get()->getGlobalBounds().intersects(this->player->getGlobalBounds())) {
                        this->player->toGetHit();
                        boss_iter->get()->die();
                        if (boss_iter->get()->getCurrentState() == Enemy::STATES::dead) {
                            this->aliveCountBoss--;
                        }
                        break;
                    } else {
                        ++boss_iter;
                    }
                } else {
                    ++boss_iter;
                }
            } else {
                ++boss_iter;
            }
        }

    }
}

void Game::updateEnemies() {
    for (auto & zako : this->formationZakos) {
        if (zako) {
            zako->update();
        }
    }

    for (auto & goei : this->formationGoeis) {
        if (goei) {
            goei->update();
        }
    }

    for (auto & boss : this->formationBosses) {
        if (boss) {
            boss->update();
        }
    }
}

void Game::updateCombat() {
    this->handlePVE();
    this->handleEVP();
}

void Game::update() {
    this->updateDeltaTime();
    this->updateInput();
    this->updatePlayers();
    this->updateBullets();

    if (!this->spawningFinished) {
        this->handleSpawning();
    }
    this->handleFormation();

    this->updateEnemies();
    this->updateCombat();
}

void Game::render() {
    this->window->clear();

    this->player->render(*this->window);

    for (auto & playerBullet : this->playerBullets) {
        playerBullet->render(*this->window);
    }

    for (auto & zako : this->formationZakos) {
        if (zako) {
            zako->render(*this->window);
        }
    }

    for (auto & goei : this->formationGoeis) {
        if (goei) {
            goei->render(*this->window);
        }
    }

    for (auto & boss : this->formationBosses) {
        if (boss) {
            boss->render(*this->window);
        }
    }

    this->window->display();
}

void Game::run() {
    while (this->window->isOpen()) {
        sf::Event event{};
        while (this->window->pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                this->window->close();
            }
        }
        this->update();
        this->render();
    }
}

