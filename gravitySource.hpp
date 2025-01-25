#include <SFML/Graphics.hpp>
#include "Settings.hpp"

class GravitySource{

private:
    sf::Vector2f pos;
    float strength;
    sf::CircleShape s;

public:
    GravitySource(float pos_x, float pos_y, float strength) {
        pos.x = pos_x;
        pos.y = pos_y;
        this->strength = strength;

        s.setPosition(pos);
        s.setFillColor(sf::Color::White);
        s.setRadius(10);
    }

    void updateStrength(float strength) {
        this->strength = strength;
    }

    void render(sf::RenderWindow& wind) {
        wind.draw(s);
    }

    sf::Vector2f get_pos() const {
        return pos;
    }

    float get_strength() const {
        return strength;
    }

};

