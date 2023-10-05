cmake_minimum_required(VERSION 3.26)
project(Galaga)

set(CMAKE_CXX_STANDARD 17)

set(SFML_STATIC_LIBRARIES TRUE)
set(SFML_DIR "C:/SFML/lib/cmake/SFML")
find_package(SFML 2.6.0 COMPONENTS system window graphics audio REQUIRED)

include_directories(C:/SFML/include)
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
        DynamicBezierPath.cpp)
target_link_libraries(Galaga sfml-system sfml-window sfml-graphics sfml-audio)
