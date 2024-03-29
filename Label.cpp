//
// Created by SoSunDook on 04.11.2023.
//

#include "Label.h"

Label::Label(const std::shared_ptr<sf::Font> & font,
              const std::string & txt,
              const sf::Color & color,
              const unsigned int & size,
              const sf::Vector2<float> & startPos,
              const sf::Vector2<float> & endPos) {
    this->endPosition = endPos;
    this->initText(font, txt, color, size, startPos);
    this->initOrigin();
}

void Label::initText(const std::shared_ptr<sf::Font> & font,
                      const std::string & txt,
                      const sf::Color & color,
                      const unsigned int & size,
                      const sf::Vector2<float> & startPos) {
    this->text.setFont(*font);
    this->text.setString(txt);
    this->text.setFillColor(color);
    this->text.setCharacterSize(size);
    this->text.setPosition(startPos);
}

void Label::initOrigin() {
    this->text.setOrigin(this->text.getLocalBounds().getSize() / 2.f);
}

void Label::move(const float & x, const float & y) {
    this->text.move({x, y});
}

void Label::setEndPosition() {
    this->text.setPosition(this->endPosition);
}

void Label::setPosition(sf::Vector2<float> & pos) {
    this->text.setPosition(pos);
}

void Label::update(const std::string & txt) {
    this->text.setString(txt);
    this->initOrigin();
}

void Label::render(sf::RenderTarget & target) {
    target.draw(this->text);
}

std::string Label::getText() {
    return this->text.getString();
}

sf::Vector2<float> Label::getPosition() {
    return this->text.getPosition();
}
