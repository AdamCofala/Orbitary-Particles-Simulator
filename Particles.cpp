#include <SFML/Graphics.hpp>
#include <cmath>
#include <vector>
#include <iostream>
#include "Settings.hpp"
#include <string>
class GravitySource {
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

    sf::Vector2f get_pos() const{
        return pos;
    }

    float get_strength() const{
        return strength;
    }

};

class Particle {
private:
    sf::Vector2f pos;
    sf::Vector2f vel;
    sf::CircleShape s;

    float max_speed = 15.0f;

public:
    Particle(float pos_x, float pos_y, float vel_x, float vel_y) {
        pos.x = pos_x;
        pos.y = pos_y;
        vel.x = vel_x;
        vel.y = vel_y;


        s.setPosition(pos);
        s.setFillColor(sf::Color::White);
        s.setRadius(4);
    }

    void render(sf::RenderWindow& wind) {
        s.setPosition(pos);
        wind.draw(s);
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

        // Set the color based on speed
        s.setFillColor(sf::Color(r, g, b));
    }

    bool wyjebalo() {
        if (pos.x<0 || pos.x>window_h || pos.y<0 || pos.y>window_w) return true;
        return false;
    }


    void update_physics(const GravitySource &s, float deltaTime) {


        float distance_x = s.get_pos().x - pos.x;
        float distance_y = s.get_pos().y - pos.y;

        float distance = sqrt((distance_x * distance_x) + (distance_y * distance_y));

        float inverse_distance = 1.0f / distance;

        float normalized_x = inverse_distance * distance_x;
        float normalized_y = inverse_distance * distance_y;

        float inverse_square_dropoff = inverse_distance* inverse_distance;

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


int main()
{
    sf::RenderWindow window(sf::VideoMode({ window_h, window_w }), "Orbital Gravity Simulation");

   
    std::vector<GravitySource> sources;
    //sources.push_back(GravitySource(278, 450, gravityStrength));
    //sources.push_back(GravitySource(666, 450, gravityStrength));

    std::vector<Particle> particles;
    // Outside the game loop
    sf::Clock frameClock;
    unsigned int fps = 0;
    std::stringstream debugStream;  // For formatting the debug text

    sf::Font font("arial.ttf");
    sf::Text text(font);
    

    while (window.isOpen())
    {
        // Inside the game loop
        sf::Time frameTime = frameClock.restart();
        float deltaTime = frameTime.asSeconds();
        fps = 1 / deltaTime;

        // Event handling
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();

            if (const auto* mouseButtonPressed = event->getIf<sf::Event::MouseButtonPressed>())
            {
                if (mouseButtonPressed->button == sf::Mouse::Button::Right)
                {
                    sf::Vector2i mousepos = sf::Mouse::getPosition();
                    sf::Vector2i winpos = window.getPosition();

                    std::cout << "MOUSE X : " << (mousepos.x - winpos.x) << " MOUSE Y : " << (mousepos.y - winpos.y) << std::endl;

                    for (int i = 0; i < num_particles; i++) {
                        particles.push_back(Particle((float)(mousepos.x - winpos.x)-10, (float)(mousepos.y - winpos.y)-30, 0.2 + (0.2 / num_particles) * i, 0.2 + (0.2 / num_particles) * i));
                    }
                }

                if (mouseButtonPressed->button == sf::Mouse::Button::Left)
                {
                    sf::Vector2i mousepos = sf::Mouse::getPosition();
                    sf::Vector2i winpos = window.getPosition();

                    std::cout << "MOUSE X : " << (mousepos.x - winpos.x) << " MOUSE Y : " << (mousepos.y - winpos.y) << std::endl;

                  
                    sources.push_back(GravitySource((float)(mousepos.x - winpos.x)-20, (float)(mousepos.y - winpos.y)-40 ,gravityStrength));
                    
                }

            }

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left))
            {
                particles.clear();
            }

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right))
            {
                colision = colision - 1;
            }

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
            {
                num_particles += 100;
            }

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
            {
                num_particles -= 100;
            }

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Q))
            {
                gravityStrength += 1000;
            }

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::E))
            {
                gravityStrength -= 1000;
            }
        }

        window.clear();

        // Update particle physics and colors
        for (int i = 0; i < sources.size(); i++) {
            sources[i].updateStrength(gravityStrength);
            for (int j = 0; j < particles.size(); j++)
            {
                if (particles[j].wyjebalo() && !colision) {
                    if (particles.begin() + j < particles.end()) particles.erase(particles.begin() + j);
                }
                else {
                    particles[j].update_physics(sources[i], deltaTime);
                    particles[j].Update_Color();
                }
            }
        }

        // Render gravity sources
        for (int i = 0; i < sources.size(); i++) {
            sources[i].render(window);
        }

        // Render particles
        for (int j = 0; j < particles.size(); j++)
        {
            particles[j].render(window);
        }

       
        text.setString(" FPS: "+std::to_string(fps)
            +"\n Num Particles: " + std::to_string(num_particles)
            + "\n Particles: " + std::to_string(particles.size())
            + "\n Gravity Strength: " + std::to_string(gravityStrength)
        );
        window.draw(text);
        
        window.display();
    }

    return 0;
}
