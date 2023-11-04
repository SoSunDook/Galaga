//
// Created by SoSunDook on 25.09.2023.
//

#ifndef GALAGA_GAME_H
#define GALAGA_GAME_H
#include "Level.h"
#include "Menu.h"

class Game {
private:
    enum STATES {
        MENU,
        LEVEL
    };

    std::shared_ptr<std::filesystem::path> dir_path;
    std::shared_ptr<sf::Font> font;
    std::shared_ptr<std::map<std::string, std::shared_ptr<sf::Texture>>> textureManager;
    std::shared_ptr<std::map<std::string, std::shared_ptr<BezierPath>>> pathManager;

    sf::Clock clock;
    std::shared_ptr<sf::Time> deltaTime;

    std::shared_ptr<sf::RenderWindow> window;

    STATES currentState;

    std::unique_ptr<Menu> menu;
    std::unique_ptr<Level> level;

    void initDeltaTime();
    void initConstants();
    void initWindow();
    void initFont();
    void initTextures();
    void initPaths();
    void initMenu();
    void initLevel();

    void handleMenuState();
    void handleLevelState();
    void handleStates();

    void updateDeltaTime();
    void update();

    void render();
public:
    Game();
    ~Game() = default;

    void run();
};

#endif //GALAGA_GAME_H
