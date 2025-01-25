#include "gravitySource.hpp"


class Particle {
private:
    sf::Vector2f pos;
    sf::Vector2f vel;
    sf::CircleShape s;

    float max_speed = 15.0f;
    sf::Color Col;

    bool isDead = false;

public:
    Particle(float pos_x, float pos_y, float vel_x, float vel_y) {
        pos.x = pos_x;
        pos.y = pos_y;
        vel.x = vel_x;
        vel.y = vel_y;


        s.setPosition(pos);
        s.setFillColor(sf::Color::White);
        s.setRadius(3);
    }

    void render(sf::RenderWindow& wind) {
        s.setPosition(pos);
        wind.draw(s);
    }

    sf::Vector2f getPos() const {
        return pos;
    }

    sf::Color getCol() const {
        return Col;
    }

    bool getDeath() const {
        return isDead;
    }

    void Update_Color() {
        // Calculate the speed (magnitude of the velocity vector)
        float speed = sqrt(vel.x * vel.x + vel.y * vel.y);

        // Normalize speed to a range [0, 1] where 0 is slowest and 1 is the maximum speed
        float normalized_speed = (speed / max_speed);

        // Clamp the normalized speed to [0, 1] just in case it's outside that range
        normalized_speed = std::min(1.0f, std::max(0.0f, normalized_speed));

        // Map normalized speed to color: blue for slow, red for fast
        // Blue -> (0, 0, 255) at low speed, Red -> (255, 0, 0) at high speed
        int r = static_cast<int>(255 * normalized_speed);  // Red increases with speed
        int b = static_cast<int>(255 * (1.0f - normalized_speed));  // Blue decreases with speed
        int g = 0;  // Green stays at 0 for the gradient

        Col = sf::Color(r, g, b);
    }

    void outOfWindow() {
        if (pos.x<0 || pos.x>window_h || pos.y<0 || pos.y>window_w) isDead = true;
    }


    void update_physics(const GravitySource& s, float deltaTime) {


        float distance_x = s.get_pos().x - pos.x;
        float distance_y = s.get_pos().y - pos.y;

        float distance = sqrt((distance_x * distance_x) + (distance_y * distance_y));

        float inverse_distance = 1.0f / distance;

        float normalized_x = inverse_distance * distance_x;
        float normalized_y = inverse_distance * distance_y;

        float inverse_square_dropoff = inverse_distance * inverse_distance;

        float acceleration_x = normalized_x * s.get_strength() * deltaTime * inverse_square_dropoff;
        float acceleration_y = normalized_y * s.get_strength() * deltaTime * inverse_square_dropoff;


        vel.x += acceleration_x;
        vel.y += acceleration_y;

        float speed = sqrt(vel.x * vel.x + vel.y * vel.y);  // Compute total speed
        if (speed > max_speed) {
            vel.x = (vel.x / speed) * max_speed;  // Normalize and scale down
            vel.y = (vel.y / speed) * max_speed;
        }

        if (colision) {
            if (pos.x<0 || pos.x>window_h) vel.x = -vel.x;
            if (pos.y<0 || pos.y>window_w) vel.y = -vel.y;
        }

        pos.x += vel.x;
        pos.y += vel.y;
    }
};