//
// Created by SoSunDook on 04.11.2023.
//

#ifndef GALAGA_LABEL_H
#define GALAGA_LABEL_H
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <memory>
#include <string>
#include <iostream>

class Label {
private:
    sf::Text text;
    sf::Vector2<float> endPosition;

    void initText(const std::shared_ptr<sf::Font> & font,
                  const std::string & txt,
                  const sf::Color & color,
                  const unsigned int & size,
                  const sf::Vector2<float> & startPos);
    void initOrigin();
public:
    Label(const std::shared_ptr<sf::Font> & font,
          const std::string & txt,
          const sf::Color & color,
          const unsigned int & size,
          const sf::Vector2<float> & startPos,
          const sf::Vector2<float> & endPos);
    ~Label() = default;

    void move(const float & x, const float & y);
    void setEndPosition();
    void setPosition(sf::Vector2<float> & pos);

    void update(std::string & txt);

    void render(sf::RenderTarget & target);

    sf::Vector2<float> getPosition();
};


#endif //GALAGA_LABEL_H
