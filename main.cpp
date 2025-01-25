#include <SFML/Graphics.hpp>
#include <cmath>
#include <vector>
#include <iostream>
#include <string>

#include "particle.hpp"
#include "Settings.cpp"



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

      deltaTime = 0.5;
      window.setFramerateLimit(60);

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
                        particles.push_back(Particle((float)(mousepos.x - winpos.x)-10, (float)(mousepos.y - winpos.y)-30, 2, 0.2 + (0.2 / num_particles) * i));
                    }
                }

                if (mouseButtonPressed->button == sf::Mouse::Button::Left)
                {
                    sf::Vector2i mousepos = sf::Mouse::getPosition();
                    sf::Vector2i winpos = window.getPosition();

                    std::cout << "MOUSE X : " << (mousepos.x - winpos.x) - 20 << " MOUSE Y : " << (mousepos.y - winpos.y) - 40 << std::endl;

                  
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

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
            {
                num_particles += 100;
            }

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
            {
                num_particles -= 100;
            }

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::E))
            {
                gravityStrength += 1000;
            }

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Q))
            {
                gravityStrength -= 1000;
            }


            //nice simulatin
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
            {

                sources.clear();
                sf::Vector2i mousepos;
                sf::Vector2i winpos = window.getPosition();
                int fixed = 50;

                sources.push_back(GravitySource(313-20+fixed, 491-40, gravityStrength));
              
                sources.push_back(GravitySource(594-20 + fixed, 495-40, gravityStrength));

                for (int i = 0; i < num_particles; i++) {
                    particles.push_back(Particle(328-21 + fixed, 275-40, 2, 0.2 + (0.2 / num_particles) * i));
                }
            }

        }

        window.clear();

        // Update particle physics and colors
        for (int i = 0; i < sources.size(); i++) {
            sources[i].updateStrength(gravityStrength);
            for (int j = 0; j < particles.size(); j++)
            {
                if (particles[j].outOfWindow() && !colision) {
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
            + "\n Collision: " + std::to_string(colision)
        );
        window.draw(text);
        
        window.display();
    }

    return 0;
}
