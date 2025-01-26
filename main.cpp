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
    std::vector<Particle> particles;
    particles.reserve(100000);


    sf::Clock frameClock;
    unsigned int fps = 0;

    sf::Font font("arial.ttf");
    sf::Text text(font);

  

    while (window.isOpen())
    {
       
        sf::Time frameTime = frameClock.restart();
        float deltaTime = frameTime.asSeconds();
        fps = 1 / deltaTime;


        //smother animations -- normally dont do that
        deltaTime = 0.5;
        if(fpsCap) window.setFramerateLimit(60);

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

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::C))
            {
                sources.clear();
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


        unsigned int counter = 0;
        // Update particle physics and colors
        for (int i = 0; i < sources.size(); i++) {
            sources[i].updateStrength(gravityStrength);
            for (int j = 0; j < particles.size(); j++)
            {
             
                if (!colision) particles[j].outOfWindow();

              if (!particles[j].getDeath()) {
                  counter++;
                  particles[j].update_physics(sources[i], deltaTime);
                  particles[j].Update_Color();
              }          
    
            }
           
        } 

        // Render gravity sources
        for (int i = 0; i < sources.size(); i++) {
            sources[i].render(window);
        }

        
        sf::VertexArray points(sf::PrimitiveType::Points);
        for (const auto& p : particles) {
            points.append(sf::Vertex(p.getPos(),p.getCol()));
        }
        window.draw(points);


       
        text.setString(" FPS: "+std::to_string(fps)
            +"\n Num Particles: " + std::to_string(num_particles)
            + "\n Particles: " + std::to_string(counter)
            + "\n Gravity Strength: " + std::to_string(gravityStrength)
            + "\n Collision: " + std::to_string(colision)
            + "\n FPScap: " + std::to_string(fpsCap)
        );

        window.draw(text);
        
        window.display();
    }
    return 0;
}
