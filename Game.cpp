//
// Created by SoSunDook on 25.09.2023.
//

#include "Game.h"

void Game::initConstants() {
    this->clock = {};
    this->currentState = STATES::MENU;
}

void Game::initVectors() {
    this->texturesVector = {"beam",
                            "bg_back",
                            "bg_front",
                            "black",
                            "boss",
                            "boss2",
                            "bulletPlayer",
                            "enemyBullet",
                            "explosion",
                            "galaga",
                            "galagaLogo",
                            "galagaRed",
                            "goei",
                            "obla",
                            "playerExplosion",
                            "zako"};
    this->soundsVector = {"bossHit1",
                          "bossHit2",
                          "capturedHit",
                          "enemyDive",
                          "fighterCaptured",
                          "fighterRescued",
                          "goeiHit",
                          "playerHit",
                          "playerShoot",
                          "stageFlag",
                          "startMusic",
                          "tractorBeamShoot",
                          "zakoHit"};
}

void Game::initDeltaTime() {
    this->deltaTime = std::make_shared<sf::Time>();
}

void Game::initWindow() {
    this->window = std::make_shared<sf::RenderWindow>(sf::VideoMode(900, 720), "Galaga");
    this->window->setFramerateLimit(600);
}

void Game::initIcon() {
    sf::Image ico;
    std::filesystem::path path = *(this->dir_path) / "Data" / "galagaicon.png";
    if (!ico.loadFromFile(path.string())) {
        throw std::invalid_argument("Icon can not be loaded");
    }
    this->window->setIcon(ico.getSize().x, ico.getSize().y, ico.getPixelsPtr());
}

void Game::initFont() {
    this->font = std::make_shared<sf::Font>();
    std::filesystem::path path = *(this->dir_path) / "Data" / "Fonts" / "Emulogic-zrEw.ttf";
    if (!this->font->loadFromFile(path.string())) {
        throw std::invalid_argument("Font can not be loaded");
    }
    this->font->setSmooth(false);
}

void Game::initTextures() {
    this->textureManager = std::make_shared<std::map<std::string, std::shared_ptr<sf::Texture>>>();
    std::filesystem::path path = *(this->dir_path) / "Data" / "Textures";
    for (const auto & entry : std::filesystem::directory_iterator(path)) {
        auto entryPath = &entry.path();
        auto fileName = entryPath->stem().string();
        auto tmpTexture = std::make_shared<sf::Texture>();
        if (!tmpTexture->loadFromFile(entryPath->string())) {
            throw std::invalid_argument(fileName + " image can not be loaded");
        }
        this->textureManager->operator[](fileName) = tmpTexture;
    }
    for (const auto & name : this->texturesVector) {
        if (!this->textureManager->contains(name)) {
            throw std::invalid_argument(name + " image can not be loaded");
        }
    }
}

void Game::initSounds() {
    this->soundManager = std::make_shared<std::map<std::string, std::shared_ptr<sf::SoundBuffer>>>();
    std::filesystem::path path = *(this->dir_path) / "Data" / "Sound";
    for (const auto & entry : std::filesystem::directory_iterator(path)) {
        auto entryPath = &entry.path();
        auto fileName = entryPath->stem().string();
        auto tmpBuffer = std::make_shared<sf::SoundBuffer>();
        if (!tmpBuffer->loadFromFile(entryPath->string())) {
            throw std::invalid_argument(fileName + " sound can not be loaded");
        }
        this->soundManager->operator[](fileName) = tmpBuffer;
    }
    for (const auto & name : this->soundsVector) {
        if (!this->soundManager->contains(name)) {
            throw std::invalid_argument(name + " image can not be loaded");
        }
    }
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
    p3 = {220,  780};
    p1 = {60, 380};
    p2 = {220, 700};

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

void Game::initHighScore() {
    this->highScoreObj = std::make_shared<Highscore>(this->dir_path);
}

void Game::initMenu() {
    this->menu = std::make_unique<Menu>(this->dir_path,
                                        this->textureManager,
                                        this->deltaTime,
                                        this->window,
                                        this->font,
                                        this->highScoreObj);
}

void Game::initLevel() {
    this->level = std::make_unique<Level>(this->dir_path,
                                          this->textureManager,
                                          this->soundManager,
                                          this->pathManager,
                                          this->deltaTime,
                                          this->window,
                                          this->font,
                                          this->highScoreObj);
}

Game::Game() {
    this->dir_path = std::make_shared<std::filesystem::path>(std::filesystem::current_path());
    this->initConstants();
    this->initVectors();
    this->initDeltaTime();
    this->initWindow();
    this->initIcon();
    this->initFont();
    this->initTextures();
    this->initSounds();
    this->initPaths();
    this->initHighScore();
    this->initMenu();
    this->initLevel();
}

void Game::handleMenuState() {
    this->menu->update();
    if (this->menu->getCurrentState() == Menu::STATES::onePlayer) {
        this->currentState = STATES::LEVEL;
        this->level->fullReset();
        this->level->setPlayers(false);
    } else if (this->menu->getCurrentState() == Menu::STATES::twoPlayers) {
        this->currentState = STATES::LEVEL;
        this->level->fullReset();
        this->level->setPlayers(true);
    }
}

void Game::handleLevelState() {
    this->level->update();
    if (this->level->getCurrentState() == Level::STATES::gameOver && this->level->getPlayers()) {
        this->currentState = STATES::LEVEL;
        this->level->fullReset(true);
    } else if (this->level->getCurrentState() == Level::STATES::gameOver) {
        this->currentState = STATES::MENU;
        this->menu->reset();
        this->level->fullReset();
    }
}

void Game::handleStates() {
    switch (this->currentState) {
        case STATES::MENU:
            this->handleMenuState();
            break;
        case STATES::LEVEL:
            this->handleLevelState();
            break;
    }
}

void Game::updateDeltaTime() {
    *(this->deltaTime) = this->clock.restart();
}

void Game::update() {
    this->updateDeltaTime();
    this->handleStates();
}

void Game::render() {
    this->window->clear();

    switch (this->currentState) {
        case STATES::MENU:
            this->menu->render();
            break;
        case STATES::LEVEL:
            this->level->render();
            break;
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

