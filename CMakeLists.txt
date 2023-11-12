cmake_minimum_required(VERSION 3.26)
project(Galaga)

set(CMAKE_CXX_STANDARD 20)

set(SFML_STATIC_LIBRARIES TRUE)
set(SFML_DIR ${CMAKE_PREFIX_PATH})
find_package(SFML 2.6.0 COMPONENTS graphics window audio system REQUIRED)

include_directories(include)
add_executable(Galaga main.cpp
        Game.cpp
        Player.cpp
        Bullet.cpp
        PlayerBullet.cpp
        Enemy.cpp
        Goei.cpp
        Zako.cpp
        Boss.cpp
        BezierCurve.cpp
        BezierPath.cpp
        Formation.cpp
        CaptureBeam.cpp
        pugixml.cpp
        CapturedPlayer.cpp
        EnemyBullet.cpp
        Level.cpp
        Menu.cpp
        Label.cpp
        UI.cpp
        Background.cpp
        Highscore.cpp)
target_link_libraries(Galaga sfml-graphics sfml-window sfml-audio sfml-system)
target_link_libraries(Galaga -static-libgcc -static-libstdc++ -static)