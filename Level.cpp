//
// Created by SoSunDook on 04.11.2023.
//

#include "Level.h"

Level::Level(std::shared_ptr<std::filesystem::path> & dirPath,
            std::shared_ptr<std::map<std::string, std::shared_ptr<sf::Texture>>> & textures,
            std::shared_ptr<std::map<std::string, std::shared_ptr<BezierPath>>> & pathManager,
            std::shared_ptr<sf::Time> & timer,
            std::shared_ptr<sf::RenderWindow> & window,
            std::shared_ptr<sf::Font> & font,
            std::shared_ptr<Highscore> & highScoreObj) {
    this->dir_path = dirPath;
    this->textureManager = textures;
    this->pathManager = pathManager;
    this->deltaTime = timer;
    this->window = window;
    this->font = font;
    this->highScoreObj = highScoreObj;
    this->initConstants();
    this->initScoreStage();
    this->initSpawningPatterns();
    this->initFormationVectors();
    this->initFormation();
    this->initPlayer();
    this->initMidLabel();
    this->initUI();
    this->initBackground();
}

void Level::initConstants() {
    this->playerVelocity = 1.f;
    this->playerBulletsVelocity = 1.f;
    this->enemyVelocity = 0.6f;
    this->enemyBulletsVelocity = 0.8f;
    this->enemyRotationVelocity = 5.4f;

    this->playerShootCooldown = sf::milliseconds(350);
    this->enemyShootCooldown = sf::milliseconds(3000);

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

    this->showMidLabel = {};

    this->levelStartStartDelay = 1.5f;
    this->levelStartStartTimer = {};

    this->levelStartStageDelay = 1.5f;
    this->levelStartStageTimer = {};

    this->levelStartReadyDelay = 1.5f;
    this->levelStartReadyTimer = {};

    this->gameOverDelay = 2.5f;
    this->gameOverTimer = {};

    this->currentState = STATES::playing;

    this->ordinarySize = 25;
}

void Level::initScoreStage() {
    this->currentScore = std::make_shared<int>();
    this->currentStage = std::make_shared<int>(1);
}

void Level::initFormationVectors() {
    this->formationZakos.resize(this->maxCountZako);
    this->formationGoeis.resize(this->maxCountGoei);
    this->formationBosses.resize(this->maxCountBoss);
}

void Level::initFormation() {
    this->formation = std::make_shared<Formation>(this->deltaTime);
}

void Level::initSpawningPatterns() {
    std::string spawningPatternsPath = this->dir_path->string() + "\\Data\\Texts\\level.xml";
    this->spawningPatterns.load_file(spawningPatternsPath.c_str());
}

void Level::initPlayer() {
    this->player = std::make_shared<Player>(this->deltaTime, this->textureManager->operator[]("playerExplosion"), this->textureManager->operator[]("galaga"),
                                            this->playerVelocity, this->playerShootCooldown, this->playersScale);
}

void Level::initMidLabel() {
    this->midLabel = std::make_unique<Label>(this->font,
                                            "START",
                                            sf::Color::Red,
                                            this->ordinarySize,
                                            sf::Vector2<float>(360.f, 360.f),
                                            sf::Vector2<float>(360.f, 360.f));
}

void Level::initUI() {
    this->ui = std::make_unique<UI>(this->dir_path,
                                    this->textureManager,
                                    this->deltaTime,
                                    this->window,
                                    this->font,
                                    this->highScoreObj,
                                    this->player->getHealth(),
                                    this->currentScore,
                                    this->currentStage);
}

void Level::initBackground() {
    this->background = std::make_unique<Background>(this->textureManager,
                                                    this->deltaTime,
                                                    sf::Vector2<float>(360.f, 360.f),
                                                    1.f);
}

std::shared_ptr<PlayerBullet> Level::initNewPlBullet() {
    auto newBullet = std::make_shared<PlayerBullet>(this->textureManager->operator[]("bulletPlayer"), this->bulletsScale, this->playerBulletsVelocity);
    return newBullet;
}

std::shared_ptr<EnemyBullet> Level::initNewEnBullet() {
    auto newBullet = std::make_shared<EnemyBullet>(this->textureManager->operator[]("enemyBullet"), this->bulletsScale, this->enemyBulletsVelocity);
    return newBullet;
}

void Level::updateInput() {
    if (this->player->getCurrentState() == Player::STATES::alive) {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
            this->player->toGetHit();
        }
        bool canMove;
        bool canShoot = true;
        if (this->capturedPlayer) {
            if (this->capturedPlayer->getCurrentState() != Enemy::STATES::dead && this->capturedPlayer->getCapturedState() == CapturedPlayer::CAPTURED_STATES::bossShotWhileDiving) {
                sf::Vector2<float> lockPoint = {this->player->getStartPos().x - this->player->getOrigin().x * this->playersScale, this->player->getStartPos().y};
                sf::Vector2<float> currentPlayerPos = {this->player->getGlobalBoundsMain().getPosition() + this->player->getOrigin() * this->playersScale};
                auto difference = currentPlayerPos.x - lockPoint.x;
                if ((difference > 0 ? difference : -difference) < 5) {
                    this->capturedPlayer->playerLocked = true;
                }
            }
            if (this->capturedPlayer->playerLocked) {
                canMove = false;
            } else {
                canMove = true;
            }
            if (this->capturedPlayer->getCurrentState() != Enemy::STATES::dead && this->capturedPlayer->getCapturedState() == CapturedPlayer::CAPTURED_STATES::bossShotWhileDiving) {
                canShoot = false;
            }
        } else {
            canMove = true;
        }
        if (canMove) {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
                this->player->move(-1.f, 0.f);
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
                this->player->move(1.f, 0.f);
            }
            if (canShoot) {
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::N) && this->player->canAttack()) {
                    auto newBullet = this->initNewPlBullet();
                    auto tmp_bvl = -this->playerBulletsVelocity;
                    auto tmp_vel = 0.f;
                    newBullet->setDirection(tmp_vel, tmp_bvl);
                    auto pos_x = this->player->getGlobalBoundsMain().getPosition().x +
                                 this->player->getOrigin().x * this->playersScale;
                    auto pos_y = this->player->getGlobalBoundsMain().getPosition().y -
                                 newBullet->getGlobalBounds().getSize().y / 2;
                    newBullet->setPosition(pos_x, pos_y);
                    this->playerBullets.push_back(newBullet);
                    if (this->player->getDoubledPlayer() &&
                        this->player->getCurrentDoubledState() != Player::STATES::dead) {
                        newBullet = this->initNewPlBullet();
                        tmp_bvl = -this->playerBulletsVelocity;
                        tmp_vel = 0.f;
                        newBullet->setDirection(tmp_vel, tmp_bvl);
                        pos_x = this->player->getGlobalBoundsDoubled().getPosition().x +
                                this->player->getOrigin().x * this->playersScale;
                        pos_y = this->player->getGlobalBoundsDoubled().getPosition().y -
                                newBullet->getGlobalBounds().getSize().y / 2;
                        newBullet->setPosition(pos_x, pos_y);
                        this->playerBullets.push_back(newBullet);
                    }
                }
            }
        }
    }
}

void Level::updateBullets() {
    for (auto iter = this->playerBullets.begin(); iter != this->playerBullets.end(); ) {
        iter->get()->update();
//        Bounds check
        if ((iter->get()->getGlobalBounds().getPosition().y <= (-(iter->get()->getLocalBounds().getSize().y * this->bulletsScale)))
        || (iter->get()->getGlobalBounds().getPosition().x <= (-(iter->get()->getLocalBounds().getSize().x * this->bulletsScale)))
        || (iter->get()->getGlobalBounds().getPosition().x >= 720.f)) {
            iter = this->playerBullets.erase(iter);
        } else {
            ++iter;
        }
    }

    for (auto iter = this->enemyBullets.begin(); iter != this->enemyBullets.end(); ) {
        iter->get()->update();
//        Bounds check
        if ((iter->get()->getGlobalBounds().getPosition().y >= 720.f)
        || (iter->get()->getGlobalBounds().getPosition().x <= (-(iter->get()->getLocalBounds().getSize().x * this->bulletsScale)))
        || (iter->get()->getGlobalBounds().getPosition().x >= 720.f)) {
            iter = this->enemyBullets.erase(iter);
        } else {
            ++iter;
        }
    }
}

void Level::updatePlayers() {
    this->player->update();
    if (this->capturedPlayer) {
        if (this->capturedPlayer->playerDoubled) {
            this->player->toDouble();
            this->capturedPlayer->playerDoubled = false;
        }
    }
}

bool Level::enemyFlyingIn() {
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

void Level::handleSpawning() {
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
                                                                     this->textureManager->operator[]("explosion"), this->textureManager->operator[]("zako"),
                                                                     this->enemyVelocity, this->enemyRotationVelocity, this->enemyShootCooldown, this->enemiesScale, index);
                        this->formationZakos.at(index) = new_enemy_zako;
                        this->currentCountZako++;
                        this->aliveCountZako++;

                    } else if (type == "goei") {

                        auto new_enemy_goei = std::make_shared<Goei>(this->deltaTime, this->pathManager->operator[](path), this->formation,
                                                                     this->textureManager->operator[]("explosion"), this->textureManager->operator[]("goei"),
                                                                     this->enemyVelocity, this->enemyRotationVelocity, this->enemyShootCooldown, this->enemiesScale, index);
                        this->formationGoeis.at(index) = new_enemy_goei;
                        this->currentCountGoei++;
                        this->aliveCountGoei++;

                    } else if (type == "boss") {

                        auto new_enemy_boss = std::make_shared<Boss>(this->deltaTime, this->pathManager->operator[](path), this->formation,
                                                                     this->textureManager->operator[]("explosion"), this->textureManager->operator[]("boss"), this->textureManager->operator[]("boss2"), this->textureManager->operator[]("beam"),
                                                                     this->enemyVelocity, this->enemyRotationVelocity, this->enemyShootCooldown, this->enemiesScale, index);
                        this->formationBosses.at(index) = new_enemy_boss;
                        this->currentCountBoss++;
                        this->aliveCountBoss++;

                    } else if (type == "capturedPlayer") {
                        if (this->savedCapturedPlayer) {
                            if (this->formationBosses[this->savedCapturedPlayerIndex]->getCurrentState() != Enemy::STATES::dead) {
                                this->capturedPlayer = std::make_shared<CapturedPlayer>(this->deltaTime, this->pathManager->operator[](path), this->formation, this->formationBosses[this->savedCapturedPlayerIndex], this->player,
                                                                     this->textureManager->operator[]("explosion"), this->textureManager->operator[]("galaga"), this->textureManager->operator[]("galagaRed"),
                                                                     this->enemyVelocity, this->enemyRotationVelocity, this->enemyShootCooldown, this->playersScale, this->savedCapturedPlayerIndex);
                                this->capturedPlayer->setCapturedState(CapturedPlayer::CAPTURED_STATES::normal);
                                this->capturedPlayer->setState(Enemy::flyIn);
                                this->capturedPlayer->initSpriteSaved();
                            }
                            this->savedCapturedPlayer = {};
                            this->savedCapturedPlayerIndex = {};
                        }
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

void Level::handleFormation() {
    this->formation->update();

    if (!this->formation->isLocked()) {
        if (currentCountZako == maxCountZako && currentCountGoei == maxCountGoei && currentCountBoss == maxCountBoss) {
            if (!this->enemyFlyingIn()) {
                this->formation->lock();
            }
        }
    } else {
        if (this->player->getCurrentState() == Player::STATES::alive && (!(this->capturedPlayer && this->capturedPlayer->getCurrentState() != Enemy::STATES::dead && this->capturedPlayer->getCapturedState() == CapturedPlayer::CAPTURED_STATES::bossShotWhileDiving))) {
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

            if (this->player->getCurrentState() != Player::STATES::alive) {
                if (!(this->firstDivingZako || this->secondDivingZako || this->divingGoei || this->divingBoss)) {
                    if (this->player->getCurrentState() != Player::STATES::dead) {
                        if ("READY" != this->midLabel->getText()) {
                            this->midLabel->update("READY");
                        }
                        this->respawningTimer += this->deltaTime->asSeconds();
                        if (this->respawningTimer >= this->respawningDelay) {
                            this->player->respawn();
                            this->respawningTimer = 0.f;
                            this->showMidLabel = false;
                        } else {
                            this->showMidLabel = true;
                        }
                    }
                }
                if (this->capturedPlayer && this->capturedPlayer->playerRespawnUnDoubled) {
                    this->player->respawn(false);
                    this->capturedPlayer->playerRespawnUnDoubled = false;
                }
            }
        }
    }
}

void Level::handleDiving() {
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
            if (this->capturedPlayer && this->capturedPlayer->getCurrentState() == Enemy::STATES::formation
            && (this->formationBosses[this->capturedPlayer->getIndex()]->getCurrentState() == Enemy::STATES::formation)) {
                this->divingBoss = this->formationBosses[this->capturedPlayer->getIndex()];

                std::string path = "diveboss";
                if (this->divingBoss->getIndex() % 2 == 1) {
                    path.append("Mirrored");
                }

                this->divingBoss->setPath(this->pathManager->operator[](path));
                this->divingBoss->toDive();
                this->capturedPlayer->setPath(this->pathManager->operator[](path));
                this->capturedPlayer->toDive();

                int index = this->divingBoss->getIndex();
                int firstEscortIndex = (index % 2 == 0) ? (index * 2) : (index * 2 - 1);
                int secondEscortIndex = firstEscortIndex + 4;

                if (this->formationGoeis[firstEscortIndex]->getCurrentState() == Enemy::STATES::formation) {

                    std::string path = "diveboss";
                    if (firstEscortIndex % 2 == 1) {
                        path.append("Mirrored");
                    }

                    this->formationGoeis[firstEscortIndex]->setPath(this->pathManager->operator[](path));
                    this->formationGoeis[firstEscortIndex]->toDive(true);
                }
                if (this->formationGoeis[secondEscortIndex]->getCurrentState() == Enemy::STATES::formation) {

                    std::string path = "diveboss";
                    if (secondEscortIndex % 2 == 1) {
                        path.append("Mirrored");
                    }

                    this->formationGoeis[secondEscortIndex]->setPath(this->pathManager->operator[](path));
                    this->formationGoeis[secondEscortIndex]->toDive(true);
                }
            } else {
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

                                srand(time(nullptr));
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

                                    this->formationGoeis[firstEscortIndex]->setPath(
                                            this->pathManager->operator[](path));
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
                            if (this->capturedPlayer) {
                                this->captureDive = false;
                            } else {
                                this->captureDive = !this->captureDive;
                            }
                            break;
                        }
                        skipped = true;
                    }
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

void Level::handleAllDied() {
    if (this->aliveCountZako == 0 && this->aliveCountGoei == 0 && this->aliveCountBoss == 0) {
        if (!this->capturedPlayer) {
            this->reset();
            *(this->currentStage) += 1;
        } else {
            if (this->capturedPlayer->getCurrentState() == Enemy::STATES::dead && !this->capturedPlayer->playerRespawnUnDoubled) {
                this->reset();
                *(this->currentStage) += 1;
                if (this->capturedPlayer) {
                    if (!this->player->getDoubledPlayer()) {
                        this->capturedPlayer = {};
                    }
                }
            }
        }
    }
}

void Level::handleGameOver() {
    if (*(this->player->getHealth()) == 0) {
        if (this->capturedPlayer) {
            if (this->capturedPlayer->getCurrentState() == Enemy::STATES::dead) {
                if (this->capturedPlayer->getCapturedState() == CapturedPlayer::CAPTURED_STATES::bossShotInFormation ||
                    this->capturedPlayer->getCapturedState() == CapturedPlayer::CAPTURED_STATES::bossShotWhileDiving) {
                    if (this->gameOverTimer >= this->gameOverDelay) {
                        this->showMidLabel = {};
                        this->currentState = STATES::gameOver;
                    } else {
                        if ("GAME OVER " + std::to_string(*(this->currentScore)) != this->midLabel->getText()) {
                            this->midLabel->update("GAME OVER " + std::to_string(*(this->currentScore)));
                        }
                        this->gameOverTimer += this->deltaTime->asSeconds();
                        this->showMidLabel = true;
                    }
                }
            } else {
                if (this->capturedPlayer->getCapturedState() != CapturedPlayer::CAPTURED_STATES::bossShotInFormation &&
                    this->capturedPlayer->getCapturedState() != CapturedPlayer::CAPTURED_STATES::bossShotWhileDiving) {
                    if (this->gameOverTimer >= this->gameOverDelay) {
                        this->showMidLabel = {};
                        this->currentState = STATES::gameOver;
                    } else {
                        if ("GAME OVER " + std::to_string(*(this->currentScore)) != this->midLabel->getText()) {
                            this->midLabel->update("GAME OVER " + std::to_string(*(this->currentScore)));
                        }
                        this->gameOverTimer += this->deltaTime->asSeconds();
                        this->showMidLabel = true;
                    }
                }
            }
        } else {
            if (this->gameOverTimer >= this->gameOverDelay) {
                this->showMidLabel = {};
                this->currentState = STATES::gameOver;
            } else {
                if ("GAME OVER " + std::to_string(*(this->currentScore)) != this->midLabel->getText()) {
                    this->midLabel->update("GAME OVER " + std::to_string(*(this->currentScore)));
                }
                this->gameOverTimer += this->deltaTime->asSeconds();
                this->showMidLabel = true;
            }
        }
    }
}

void Level::fullReset() {
    this->reset();

    this->player->reset();

    this->ui->reset();

    for (auto & zako : this->formationZakos) {
        if (zako) {
            zako = {};
        }
    }

    for (auto & goei : this->formationGoeis) {
        if (goei) {
            goei = {};
        }
    }

    for (auto & boss : this->formationBosses) {
        if (boss) {
            boss = {};
        }
    }

    this->playerBullets = {};

    this->enemyBullets = {};

    *(this->currentScore) = 0;
    *(this->currentStage) = 1;

    this->spawningTimer = 0.f;

    this->respawningTimer = 0.f;

    this->divingGoei = {};
    this->skipFirstGoei = false;
    this->goeiDiveTimer = 0.f;

    this->firstDivingZako = {};
    this->secondDivingZako = {};
    this->zakoDiveTimer = 0.f;
    this->zakoDiveTimer = {};

    this->divingBoss = {};
    this->captureDive = {};
    this->skipFirstBoss = {};
    this->bossDiveTimer = 0.f;

    this->capturedPlayer = {};
    this->savedCapturedPlayer = {};
    this->savedCapturedPlayerIndex = {};

    this->showMidLabel = {};

    this->levelStartStartTimer = {};

    this->gameOverTimer = {};

    this->currentState = STATES::playing;
}

void Level::reset() {
    this->formation->reset();

    this->currentCountZako = {};
    this->currentCountGoei = {};
    this->currentCountBoss = {};

    this->aliveCountZako = {};
    this->aliveCountGoei = {};
    this->aliveCountBoss = {};

    this->currentFlyInPriority = {};
    this->currentFlyInIndex = {};

    this->levelStartStageTimer = {};

    this->levelStartReadyTimer = {};

    this->spawningFinished = {};
}

void Level::handlePVE() {
    for (auto pl_bullet_iter = this->playerBullets.begin(); pl_bullet_iter != this->playerBullets.end(); ) {
        bool bullet_deleted = false;
        for (auto zako_iter = this->formationZakos.begin(); (zako_iter != this->formationZakos.end()) && (!bullet_deleted); ) {
            if (*zako_iter) {
                if (zako_iter->get()->getCurrentState() != Enemy::STATES::dead) {
                    if (zako_iter->get()->getGlobalBounds().intersects(pl_bullet_iter->get()->getGlobalBounds())) {
                        pl_bullet_iter = this->playerBullets.erase(pl_bullet_iter);
                        zako_iter->get()->hit();
                        if (zako_iter->get()->getCurrentState() == Enemy::STATES::dead) {
                            *(this->currentScore) += zako_iter->get()->getWorthPoints();
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
                            *(this->currentScore) += goei_iter->get()->getWorthPoints();
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
                        Enemy::STATES prev_state = boss_iter->get()->getCurrentState();
                        boss_iter->get()->hit();
                        if (boss_iter->get()->getCurrentState() == Enemy::STATES::dead) {
                            if (this->capturedPlayer && this->capturedPlayer->getIndex() == boss_iter->get()->getIndex()) {
                                CapturedPlayer::CAPTURED_STATES state;
                                if (prev_state == Enemy::STATES::formation) {
                                    std::string path = "diveboss";
                                    if (this->capturedPlayer->getIndex() % 2 == 1) {
                                        path.append("Mirrored");
                                    }
                                    this->capturedPlayer->setPath(this->pathManager->operator[](path));
                                    this->capturedPlayer->toDive();
                                    state = CapturedPlayer::CAPTURED_STATES::bossShotInFormation;
                                } else if (prev_state == Enemy::STATES::dive || prev_state == Enemy::STATES::flyIn) {
                                    state = CapturedPlayer::CAPTURED_STATES::bossShotWhileDiving;
                                }
                                this->capturedPlayer->setCapturedState(state);
                            }
                            *(this->currentScore) += boss_iter->get()->getWorthPoints();
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
            if (this->capturedPlayer) {
                if (this->capturedPlayer->getCurrentState() != Enemy::STATES::dead) {
                    if (this->capturedPlayer->getGlobalBounds().intersects(pl_bullet_iter->get()->getGlobalBounds())) {
                        pl_bullet_iter = this->playerBullets.erase(pl_bullet_iter);
                        this->capturedPlayer->hit();
                        this->savedCapturedPlayer = {};
                        this->savedCapturedPlayerIndex = {};
                        *(this->currentScore) += this->capturedPlayer->getWorthPoints();
                        bullet_deleted = true;
                    }
                }
            }
        }

        if (!bullet_deleted) {
            ++pl_bullet_iter;
        }
    }
}

void Level::handleEVP() {
    if (this->player->getCurrentState() == Player::STATES::alive) {
        for (auto zako_iter = this->formationZakos.begin(); (zako_iter != this->formationZakos.end()) && (this->player->getCurrentState() == Player::STATES::alive); ) {
            if (*zako_iter) {
                if (zako_iter->get()->getCurrentState() != Enemy::STATES::dead) {
                    if (zako_iter->get()->getCurrentState() == Enemy::STATES::dive && zako_iter->get()->canAttack()) {
                        auto newBullet = this->initNewEnBullet();

                        sf::Vector2<float> pos = zako_iter->get()->getGlobalBounds().getPosition() + zako_iter->get()->getOrigin() * this->enemiesScale;
                        sf::Vector2<float> distance = (this->player->getGlobalBoundsMain().getPosition() + this->player->getOrigin() * this->playersScale) - pos;

                        float tmp_bvl = this->enemyBulletsVelocity;
                        float tmp_vel;

                        float module = distance.x >= 0 ? distance.x : -distance.x;

                        if (module >= 100) {
                            tmp_vel = 0.25f;
                        } else if (module >= 50) {
                            tmp_vel = 0.15f;
                        } else if (module >= 20) {
                            tmp_vel = 0.05f;
                        } else {
                            tmp_vel = {};
                        }

                        if (distance.x < 0) {
                            tmp_vel = -tmp_vel;
                        }

                        newBullet->setDirection(tmp_vel, tmp_bvl);

                        auto pos_x = pos.x;
                        auto pos_y = pos.y + zako_iter->get()->getOrigin().y * this->enemiesScale + newBullet->getOrigin().y;
                        newBullet->setPosition(pos_x, pos_y);
                        newBullet->setRotation(180);
                        this->enemyBullets.push_back(newBullet);
                    }

                    if (zako_iter->get()->getGlobalBounds().intersects(this->player->getGlobalBoundsMain())) {
                        this->player->toGetHit();
                        zako_iter->get()->die();
                        if (zako_iter->get()->getCurrentState() == Enemy::STATES::dead) {
                            *(this->currentScore) += zako_iter->get()->getWorthPoints();
                            this->aliveCountZako--;
                        }
                        break;
                    } else if (this->player->getDoubledPlayer() && this->player->getCurrentDoubledState() != Player::STATES::dead && zako_iter->get()->getGlobalBounds().intersects(this->player->getGlobalBoundsDoubled())) {
                        this->player->toGetHit(true);
                        zako_iter->get()->die();
                        if (zako_iter->get()->getCurrentState() == Enemy::STATES::dead) {
                            *(this->currentScore) += zako_iter->get()->getWorthPoints();
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
                    if (goei_iter->get()->getCurrentState() == Enemy::STATES::dive && goei_iter->get()->canAttack()) {
                        auto newBullet = this->initNewEnBullet();

                        sf::Vector2<float> pos = goei_iter->get()->getGlobalBounds().getPosition() + goei_iter->get()->getOrigin() * this->enemiesScale;
                        sf::Vector2<float> distance = (this->player->getGlobalBoundsMain().getPosition() + this->player->getOrigin() * this->playersScale) - pos;

                        float tmp_bvl = this->enemyBulletsVelocity;
                        float tmp_vel;

                        float module = distance.x >= 0 ? distance.x : -distance.x;

                        if (module >= 100) {
                            tmp_vel = 0.25f;
                        } else if (module >= 50) {
                            tmp_vel = 0.15f;
                        } else if (module >= 20) {
                            tmp_vel = 0.05f;
                        } else {
                            tmp_vel = {};
                        }

                        if (distance.x < 0) {
                            tmp_vel = -tmp_vel;
                        }

                        newBullet->setDirection(tmp_vel, tmp_bvl);

                        auto pos_x = pos.x;
                        auto pos_y = pos.y + goei_iter->get()->getOrigin().y * this->enemiesScale + newBullet->getOrigin().y;
                        newBullet->setPosition(pos_x, pos_y);
                        newBullet->setRotation(180);
                        this->enemyBullets.push_back(newBullet);
                    }

                    if (goei_iter->get()->getGlobalBounds().intersects(this->player->getGlobalBoundsMain())) {
                        this->player->toGetHit();
                        goei_iter->get()->die();
                        if (goei_iter->get()->getCurrentState() == Enemy::STATES::dead) {
                            *(this->currentScore) += goei_iter->get()->getWorthPoints();
                            this->aliveCountGoei--;
                        }
                        break;
                    } else if (this->player->getDoubledPlayer() && this->player->getCurrentDoubledState() != Player::STATES::dead && goei_iter->get()->getGlobalBounds().intersects(this->player->getGlobalBoundsDoubled())) {
                        this->player->toGetHit(true);
                        goei_iter->get()->die();
                        if (goei_iter->get()->getCurrentState() == Enemy::STATES::dead) {
                            *(this->currentScore) += goei_iter->get()->getWorthPoints();
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
                    if (boss_iter->get()->getCurrentState() == Enemy::STATES::dive && boss_iter->get()->canAttack()) {
                        auto newBullet = this->initNewEnBullet();

                        sf::Vector2<float> pos = boss_iter->get()->getGlobalBounds().getPosition() + boss_iter->get()->getOrigin() * this->enemiesScale;
                        sf::Vector2<float> distance = (this->player->getGlobalBoundsMain().getPosition() + this->player->getOrigin() * this->playersScale) - pos;

                        float tmp_bvl = this->enemyBulletsVelocity;
                        float tmp_vel;

                        float module = distance.x >= 0 ? distance.x : -distance.x;

                        if (module >= 100) {
                            tmp_vel = 0.25f;
                        } else if (module >= 50) {
                            tmp_vel = 0.15f;
                        } else if (module >= 20) {
                            tmp_vel = 0.05f;
                        } else {
                            tmp_vel = {};
                        }

                        if (distance.x < 0) {
                            tmp_vel = -tmp_vel;
                        }

                        newBullet->setDirection(tmp_vel, tmp_bvl);

                        auto pos_x = pos.x;
                        auto pos_y = pos.y + boss_iter->get()->getOrigin().y * this->enemiesScale + newBullet->getOrigin().y;
                        newBullet->setPosition(pos_x, pos_y);
                        newBullet->setRotation(180);
                        this->enemyBullets.push_back(newBullet);
                    }

                    if (boss_iter->get()->getGlobalBounds().intersects(this->player->getGlobalBoundsMain())) {
                        this->player->toGetHit();
                        boss_iter->get()->die();
                        if (boss_iter->get()->getCurrentState() == Enemy::STATES::dead) {
                            if (this->capturedPlayer && this->capturedPlayer->getIndex() == boss_iter->get()->getIndex()) {
                                CapturedPlayer::CAPTURED_STATES state = CapturedPlayer::CAPTURED_STATES::bossShotWhileDiving;
                                this->capturedPlayer->setCapturedState(state);
                            }
                            *(this->currentScore) += boss_iter->get()->getWorthPoints();
                            this->aliveCountBoss--;
                        }
                        break;
                    } else if (this->player->getDoubledPlayer() && this->player->getCurrentDoubledState() != Player::STATES::dead && boss_iter->get()->getGlobalBounds().intersects(this->player->getGlobalBoundsDoubled())) {
                        this->player->toGetHit(true);
                        boss_iter->get()->die();
                        if (boss_iter->get()->getCurrentState() == Enemy::STATES::dead) {
                            *(this->currentScore) += boss_iter->get()->getWorthPoints();
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

        if (this->divingBoss && this->divingBoss->getCurrentState() == Enemy::STATES::dive) {
            if (this->divingBoss->getCapturing()) {
                if (this->divingBoss->getCaptureBeam().getGlobalBounds().intersects(this->player->getGlobalBoundsMain())) {
                    std::shared_ptr<BezierPath> tmp = {};
                    this->capturedPlayer = std::make_shared<CapturedPlayer>(this->deltaTime, tmp, this->formation, this->divingBoss, this->player,
                                                                     this->textureManager->operator[]("explosion"), this->textureManager->operator[]("galaga"), this->textureManager->operator[]("galagaRed"),
                                                                     this->enemyVelocity, this->enemyRotationVelocity, this->enemyShootCooldown, this->playersScale, this->divingBoss->getIndex());
                    auto playerPosition = this->player->getGlobalBoundsMain().getPosition() + this->player->getOrigin() * this->playersScale;
                    this->capturedPlayer->setPosition(playerPosition.x, playerPosition.y);
                    this->player->toGetCaptured();
                    this->divingBoss->captured = true;
                }
            }
        }

        if (this->capturedPlayer) {
            if (this->capturedPlayer->getCurrentState() != Enemy::STATES::dead && this->capturedPlayer->getCapturedState() != CapturedPlayer::CAPTURED_STATES::bossShotWhileDiving) {
                if ((this->capturedPlayer->getCurrentState() == Enemy::STATES::dive ||
                this->capturedPlayer->getCapturedState() == CapturedPlayer::CAPTURED_STATES::bossShotInFormation) && this->capturedPlayer->canAttack()) {
                    auto newBullet = this->initNewEnBullet();

                    sf::Vector2<float> pos = this->capturedPlayer->getGlobalBounds().getPosition() + this->capturedPlayer->getOrigin() * this->playersScale;
                    sf::Vector2<float> distance = (this->player->getGlobalBoundsMain().getPosition() + this->player->getOrigin() * this->playersScale) - pos;

                    float tmp_bvl = this->enemyBulletsVelocity;
                    float tmp_vel;

                    float module = distance.x >= 0 ? distance.x : -distance.x;

                    if (module >= 100) {
                        tmp_vel = 0.25f;
                    } else if (module >= 50) {
                        tmp_vel = 0.15f;
                    } else if (module >= 20) {
                        tmp_vel = 0.05f;
                    } else {
                        tmp_vel = {};
                    }

                    if (distance.x < 0) {
                        tmp_vel = -tmp_vel;
                    }

                    newBullet->setDirection(tmp_vel, tmp_bvl);

                    auto pos_x = pos.x;
                    auto pos_y = pos.y + this->capturedPlayer->getOrigin().y * this->playersScale + newBullet->getOrigin().y;
                    newBullet->setPosition(pos_x, pos_y);
                    newBullet->setRotation(180);
                    this->enemyBullets.push_back(newBullet);
                }

                if (this->capturedPlayer->getGlobalBounds().intersects(this->player->getGlobalBoundsMain())) {
                    this->capturedPlayer->die();
                    *(this->currentScore) += this->capturedPlayer->getWorthPoints();
                    this->savedCapturedPlayer = {};
                    this->savedCapturedPlayerIndex = {};
                    this->player->toGetHit();
                }
            }
        }

        for (auto en_bullet_iter = this->enemyBullets.begin(); en_bullet_iter != this->enemyBullets.end(); ) {
            bool bullet_deleted = false;
            if (this->player->getCurrentState() == Player::STATES::alive) {
                if (this->player->getGlobalBoundsMain().intersects(en_bullet_iter->get()->getGlobalBounds())) {
                    en_bullet_iter = this->enemyBullets.erase(en_bullet_iter);
                    this->player->toGetHit();
                    bullet_deleted = true;
                } else if (this->player->getDoubledPlayer() && this->player->getCurrentDoubledState() != Player::STATES::dead && en_bullet_iter->get()->getGlobalBounds().intersects(this->player->getGlobalBoundsDoubled())) {
                    en_bullet_iter = this->enemyBullets.erase(en_bullet_iter);
                    this->player->toGetHit(true);
                    bullet_deleted = true;
                }
            }
            if (!bullet_deleted) {
                ++en_bullet_iter;
            }
        }
    }
}

void Level::updateEnemies() {
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

    if (this->capturedPlayer) {
        this->capturedPlayer->update();
        if (this->capturedPlayer->savedNextLevel) {
            this->savedCapturedPlayer = true;
            this->savedCapturedPlayerIndex = this->capturedPlayer->getIndex();
            this->capturedPlayer->savedNextLevel = false;
        }
        if (this->capturedPlayer->getCapturedState() == CapturedPlayer::CAPTURED_STATES::onlyCaptured ||
        this->capturedPlayer->getCapturedState() == CapturedPlayer::CAPTURED_STATES::onlyCapturedWithBoss) {
            if ("FIGHTER CAPTURED" != this->midLabel->getText()) {
                this->midLabel->update("FIGHTER CAPTURED");
            }
        }
    }
}

void Level::updateCombat() {
    this->handlePVE();
    this->handleEVP();
}

Level::STATES & Level::getCurrentState() {
    return this->currentState;
}

void Level::update() {
    this->background->update();

    this->updateInput();
    this->updatePlayers();
    this->updateBullets();

    if (!this->spawningFinished) {
        if (this->levelStartStartTimer >= this->levelStartStartDelay) {
            if (this->levelStartStageTimer >= this->levelStartStageDelay) {
                if (this->levelStartReadyTimer >= this->levelStartReadyDelay) {
                    this->showMidLabel = {};
                    this->handleSpawning();
                } else {
                    if ("READY" != this->midLabel->getText()) {
                        this->midLabel->update("READY");
                    }
                    this->levelStartReadyTimer += this->deltaTime->asSeconds();
                    this->showMidLabel = true;
                }
            } else {
                if ("STAGE " + std::to_string(*(this->currentStage)) != this->midLabel->getText()) {
                    this->midLabel->update("STAGE " + std::to_string(*(this->currentStage)));
                }
                this->levelStartStageTimer += this->deltaTime->asSeconds();
                this->showMidLabel = true;
            }
        } else {
            if ("START" != this->midLabel->getText()) {
                this->midLabel->update("START");
            }
            this->levelStartStartTimer += this->deltaTime->asSeconds();
            this->showMidLabel = true;
        }
    } else {
        this->handleAllDied();
    }
    this->handleFormation();
    this->handleGameOver();

    this->updateEnemies();
    this->updateCombat();

    this->ui->update();
}

void Level::render() {
    this->background->render(*this->window);

    this->player->render(*this->window);

    for (auto & playerBullet : this->playerBullets) {
        playerBullet->render(*this->window);
    }

    for (auto & enemyBullet : this->enemyBullets) {
        enemyBullet->render(*this->window);
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

    if (this->capturedPlayer) {
        this->capturedPlayer->render(*this->window);
    }

    this->ui->render();

    if (this->showMidLabel || (this->capturedPlayer &&
    (this->capturedPlayer->getCapturedState() == CapturedPlayer::CAPTURED_STATES::onlyCaptured ||
    this->capturedPlayer->getCapturedState() == CapturedPlayer::CAPTURED_STATES::onlyCapturedWithBoss))) {
        this->midLabel->render(*this->window);
    }
}