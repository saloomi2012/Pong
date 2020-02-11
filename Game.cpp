//
// Created by Suliman Alsaid on 2/10/20.
//

#include "Game.h"
#include <cmath>
#include <cstdlib>
#include <ctime>

Game::Game(): win(sf::VideoMode(640, 480), "Pong") {
    const int length=100, radius=10;
    leftPlayer.setSize({10, length});
    rightPlayer.setSize({10, length});

    leftPlayer.setPosition(10, 480/2-(length/2));
    rightPlayer.setPosition(640-20, 480/2-(length/2));

    ball.setRadius(radius);
    ball.setPosition(640/2-radius, 480/2-radius);

    leftPlayer.setFillColor(sf::Color::White);
    rightPlayer.setFillColor(sf::Color::White);
    ball.setFillColor(sf::Color::White);

    beepSound.loadFromFile("beep.wav");
    beep.setBuffer(beepSound);

    srand(time(NULL));

    int xdir = rand() % 2;
    int ydir = rand() % 2;

    ballVelocity.x = (xdir == 1) ? 250 : -250;
    ballVelocity.y = (ydir == 1) ? 250 : -250;
    clock.restart();

    win.setFramerateLimit(60);
    loop();
}

void Game::loop() {

    while(true) {
        input();
        update();
        draw();
    }

}

void Game::input() {
    sf::Event e;
    while(win.pollEvent(e)) {
        switch (e.type) {
            case sf::Event::KeyPressed:
                if(paused) {
                    paused = false;
                    int xdir = rand() % 2;
                    int ydir = rand() % 2;

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
        leftPlayer.move(0, -250*clock.getElapsedTime().asSeconds());
    } else if(leftDown) {
        leftPlayer.move(0, 250*clock.getElapsedTime().asSeconds());
    }

    if(leftPlayer.getPosition().y < 0) {
        leftPlayer.setPosition(leftPlayer.getPosition().x, 0);
    } else if(leftPlayer.getPosition().y + leftPlayer.getSize().y > 480) {
        leftPlayer.setPosition(leftPlayer.getPosition().x, 480 - leftPlayer.getSize().y);
    }

    if(rightUp) {
        rightPlayer.move(0, -250*clock.getElapsedTime().asSeconds());
    } else if(rightDown) {
        rightPlayer.move(0, 250*clock.getElapsedTime().asSeconds());
    }

    if(rightPlayer.getPosition().y < 0) {
        rightPlayer.setPosition(rightPlayer.getPosition().x, 0);
    } else if(rightPlayer.getPosition().y + rightPlayer.getSize().y > 480) {
        rightPlayer.setPosition(rightPlayer.getPosition().x, 480 - rightPlayer.getSize().y);
    }

    if(ball.getPosition().x + 20 < 0 || ball.getPosition().x > 640) {
        ball.setPosition(640/2-10, 480/2-10);
        ballVelocity.x = 0;
        ballVelocity.y = 0;
        leftPlayer.setPosition(10, 480/2-(100/2));
        rightPlayer.setPosition(640-20, 480/2-(100/2));
        paused = true;
    }


    clock.restart();



}

void Game::draw() {
    win.clear(sf::Color::Black);
    win.draw(leftPlayer);
    win.draw(rightPlayer);
    win.draw(ball);
    win.display();

}
