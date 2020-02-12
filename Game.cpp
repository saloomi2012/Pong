//
// Created by Suliman Alsaid on 2/10/20.
//

#include "Game.h"
#include <cmath>
#include <string>

Game::Game(): win(sf::VideoMode(640, 480), "Pong"), dist(0, 1) {
    const int length=100, radius=10;
    leftPlayer.setSize({10, length});
    rightPlayer.setSize({10, length});

    leftPlayer.setPosition(10, 480.0f/2-(length/2.0f));
    rightPlayer.setPosition(640-20, 480.0f/2-(length/2.0f));

    ball.setRadius(radius);
    ball.setPosition(640.0f/2-radius, 480.0f/2-radius);

    leftPlayer.setFillColor(sf::Color::White);
    rightPlayer.setFillColor(sf::Color::White);
    ball.setFillColor(sf::Color::White);

    beepSound.loadFromFile("beep.wav");
    beep.setBuffer(beepSound);


    font.loadFromFile("munro-small.ttf");
    scores[0] = 0;
    scores[1] = 0;
    score.setFont(font);
    score.setPosition(100, 10);
    score.setCharacterSize(100);
    score.setFillColor(sf::Color::White);
    score.setStyle(sf::Text::Regular);
    std::string text = std::to_string(scores[0])+"\t\t\t"+ std::to_string(scores[1]);
    score.setString(text);

    std::random_device rd;
    mt.seed(rd);




    ballVelocity.x = 0;
    ballVelocity.y = 0;
    paused = true;
    clock.restart();

    win.setFramerateLimit(60);
    loop();
}

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"
void Game::loop() {

    while(true) {
        input();
        update();
        draw();
    }

}
#pragma clang diagnostic pop

void Game::input() {
    sf::Event e{};
    while(win.pollEvent(e)) {
        switch (e.type) {
            case sf::Event::KeyPressed:
                if(paused) {
                    paused = false;
                    int xdir = dist(mt);
                    int ydir = dist(mt);

                    ballVelocity.x = (xdir == 1) ? 250 : -250;
                    ballVelocity.y = (ydir == 1) ? 250 : -250;
                }
                switch(e.key.code) {
                    case sf::Keyboard::W:
                        leftUp = true;
                        break;
                    case sf::Keyboard::S:
                        leftDown = true;
                        break;
                    case sf::Keyboard::Up:
                        rightUp = true;
                        break;
                    case sf::Keyboard::Down:
                        rightDown = true;
                        break;
                }
                break;
            case sf::Event::KeyReleased:
                switch(e.key.code) {
                    case sf::Keyboard::W:
                        leftUp = false;
                        break;
                    case sf::Keyboard::S:
                        leftDown = false;
                        break;
                    case sf::Keyboard::Up:
                        rightUp = false;
                        break;
                    case sf::Keyboard::Down:
                        rightDown = false;
                        break;
                }
                break;
            case sf::Event::Closed:
                exit(0);
            default:
                break;
        }
    }

}

void Game::update() {
    if(ball.getPosition().x <= leftPlayer.getPosition().x+leftPlayer.getSize().x
    && ball.getPosition().x >= leftPlayer.getPosition().x
    && ball.getPosition().y <= leftPlayer.getPosition().y+leftPlayer.getSize().y
    && ball.getPosition().y >= leftPlayer.getPosition().y) {
        ballVelocity.x *= sin(3 * 3.14159 / 2);
        beep.play();
    } else if(ball.getPosition().x + 20 >= rightPlayer.getPosition().x
              && ball.getPosition().x + 20 <= rightPlayer.getPosition().x + rightPlayer.getSize().x
              && ball.getPosition().y <= rightPlayer.getPosition().y+rightPlayer.getSize().y
              && ball.getPosition().y >= rightPlayer.getPosition().y) {
        ballVelocity.x *= sin(3 * 3.14159 / 2);
        beep.play();

    } else if(ball.getPosition().y + 20 >= 480 || ball.getPosition().y <= 0) {
        ballVelocity.y *= sin(3 * 3.14159 / 2);
        beep.play();
    }

    ball.move(ballVelocity*clock.getElapsedTime().asSeconds());

    if(leftUp) {
        leftPlayer.move(0, -270*clock.getElapsedTime().asSeconds());
    } else if(leftDown) {
        leftPlayer.move(0, 270*clock.getElapsedTime().asSeconds());
    }

    if(leftPlayer.getPosition().y < 0) {
        leftPlayer.setPosition(leftPlayer.getPosition().x, 0);
    } else if(leftPlayer.getPosition().y + leftPlayer.getSize().y > 480) {
        leftPlayer.setPosition(leftPlayer.getPosition().x, 480 - leftPlayer.getSize().y);
    }

    if(rightUp) {
        rightPlayer.move(0, -270*clock.getElapsedTime().asSeconds());
    } else if(rightDown) {
        rightPlayer.move(0, 270*clock.getElapsedTime().asSeconds());
    }

    if(rightPlayer.getPosition().y < 0) {
        rightPlayer.setPosition(rightPlayer.getPosition().x, 0);
    } else if(rightPlayer.getPosition().y + rightPlayer.getSize().y > 480) {
        rightPlayer.setPosition(rightPlayer.getPosition().x, 480 - rightPlayer.getSize().y);
    }

    if(ball.getPosition().x + 20 < 0) {
        scores[1]++;
        ball.setPosition(640.0f/2-10, 480.0f/2-10);
        ballVelocity.x = 0;
        ballVelocity.y = 0;
        leftPlayer.setPosition(10, 480.0f/2-(100.0f/2));
        rightPlayer.setPosition(640-20, 480.0f/2-(100.0f/2));
        std::string text = std::to_string(scores[0])+"\t\t\t"+ std::to_string(scores[1]);
        score.setString(text);
        paused = true;
    } else if(ball.getPosition().x > 640) {
        scores[0]++;
        ball.setPosition(640.0f/2-10, 480.0f/2-10);
        ballVelocity.x = 0;
        ballVelocity.y = 0;
        leftPlayer.setPosition(10, 480.0f/2-(100.0f/2));
        rightPlayer.setPosition(640-20, 480.0f/2-(100.0f/2));
        std::string text = std::to_string(scores[0])+"\t\t\t"+ std::to_string(scores[1]);
        score.setString(text);
        paused = true;
    }


    clock.restart();



}

void Game::draw() {
    win.clear(sf::Color::Black);
    win.draw(leftPlayer);
    win.draw(rightPlayer);
    win.draw(ball);

    if(paused) {
        win.draw(score);
    }
    win.display();

}
