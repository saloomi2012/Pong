//
// Created by Suliman Alsaid on 2/10/20.
//

#ifndef PONG_GAME_H
#define PONG_GAME_H

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>

class Game {
private:
    sf::RenderWindow win;
    sf::RectangleShape leftPlayer;
    sf::RectangleShape rightPlayer;
    sf::CircleShape ball;
    sf::SoundBuffer beepSound;
    sf::Sound beep;
    sf::Vector2f ballVelocity;
    sf::Clock clock;
    bool leftDown = false, rightDown = false, leftUp = false, rightUp = false;
    bool paused = false;

public:
    Game();


    void loop();

    void input();

    void update();

    void draw();
};


#endif //PONG_GAME_H
