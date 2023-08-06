#include <SFML/Graphics.hpp>
#include <iostream>
#include <chrono> // Für std::chrono::milliseconds
#include <thread> // Für std::this_thread::sleep_for
#include <time.h>
#include <stdlib.h>
#include <vector>
using namespace sf;

// rect rect collision detection helper function
bool collides(float x1, float y1, float w1, float h1, float x2, float y2, float w2, float h2) {
	if (x1 + w1 >= x2 && x1 <= x2 + w2 && y1 + h1 >= y2 && y1 <= y2 + h2) {
		return true;
	}
	return false;
}

int main(){ 
    srand(time(NULL));
 
    RenderWindow window(VideoMode(800,600), "FlappyBird" , Style::Titlebar | Style::Close);
    Texture map;
    Texture flappy;
    Texture pipe;
    Texture gameover;
    gameover.loadFromFile("images/gameover.png");
    pipe.loadFromFile("images/pipe.png");
    map.loadFromFile("images/map.png");
    flappy.loadFromFile("images/flappy1.png");
    int counter = 0;
    Sprite background(map);
    Sprite flap(flappy);
    flap.setScale(1.5,1.5);
    Sprite pip(pipe);
    Sprite gameOV(gameover);
    gameOV.setPosition(250,200);
    gameOV.setScale(1.5,1.5);
 
    pip.setPosition(850,400);
 
    sf::Vector2u windowSize = window.getSize();
    sf::Vector2u textureSize = map.getSize();

    float xScale = (float)windowSize.x / textureSize.x;
    float yScale = (float)windowSize.y / textureSize.y;
 
    flap.setPosition(windowSize.x/4,windowSize.y/4);
    std::vector<sf::Sprite> pipes;  // Erstellen Sie ein std::vector für Ihre Röhren

    background.setScale(xScale,yScale);
    Event space;

    Text text;
    Font font;
    font.loadFromFile("images/arial.ttf");
    text.setFont(font);
    text.setCharacterSize(24); // in Pixel, nicht Punkte!
    text.setFillColor(sf::Color::Red);
    text.setPosition(25, 25); // Position in der oberen linken Ecke




    int flapv = 0;
    int gameframes = 0;

     // Start the game loop
    while (window.isOpen())
    {
            
		// update flappy
		float fx = flap.getPosition().x;
		float fy = flap.getPosition().y;
		float fw = 34 * flap.getScale().x;
		float fh = 24 * flap.getScale().y;
            //generate pipes
            if(gameframes % 100 == 0){
    			int r = rand() % 275 + 75;
			int gap = 150;

			// lower pipe
			Sprite pipeL;
			pipeL.setTexture(pipe);
			pipeL.setPosition(1000, r + gap);
			pipeL.setScale(2, 2);

			// upper pipe
			Sprite pipeU;
			pipeU.setTexture(pipe);
			pipeU.setPosition(1000, r);
			pipeU.setScale(2, -2);

			// push to the array
			pipes.push_back(pipeL);
			pipes.push_back(pipeU);
            }

        // Process events
        sf::Event event;
        while (window.pollEvent(event))
        {
            // Close window: exit
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::MouseButtonPressed)
            {
            if (event.mouseButton.button == sf::Mouse::Left)
             {
                flap.move(0,-75);

             }
            }
        }
   
 
    window.draw(background);
    window.draw(flap);
    window.draw(pip);
    window.draw(text);
 
 
 
    
    flap.move(0,5);
 
 

    std::this_thread::sleep_for(std::chrono::milliseconds(10)); // Geändert

 		// draw pipes
for (auto itr = pipes.begin(); itr != pipes.end(); itr++) {
    window.draw(*itr);
}



 // collision detection
for (auto itr = pipes.begin(); itr != pipes.end(); itr++) {

				float px, py, pw, ph;

				if ((*itr).getScale().y > 0) {
					px = (*itr).getPosition().x;
					py = (*itr).getPosition().y;
					pw = 52 * (*itr).getScale().x;
					ph = 320 * (*itr).getScale().y;
				} else {
					pw = 52 * (*itr).getScale().x;
					ph = -320 * (*itr).getScale().y;
					px = (*itr).getPosition().x;
					py = (*itr).getPosition().y - ph;
				}

				if (collides(fx, fy, fw, fh, px, py, pw, ph)) {
				                    std::cout << "Game Over, Counter: " << counter;
        window.draw(gameOV);
                std::this_thread::sleep_for(std::chrono::milliseconds(50)); // Geändert

        window.display();
        std::this_thread::sleep_for(std::chrono::milliseconds(3000)); // Geändert
        window.close();
                }
			}


//move the pipes
for (auto& pip : pipes) 
{
 
    pip.move(-5, 0); 
}
//erase pipes who are out of screen
pipes.erase(std::remove_if(pipes.begin(), pipes.end(),
    [](const sf::Sprite & pip) {
         return pip.getPosition().x < -100; }),
    pipes.end());



    window.display();
        
        
    if(fx > windowSize.x || fy > windowSize.y){
				                    std::cout << "Game Over, Counter: " << counter;
        window.draw(gameOV);
                std::this_thread::sleep_for(std::chrono::milliseconds(50)); // Geändert

        window.display();
        std::this_thread::sleep_for(std::chrono::milliseconds(3000)); // Geändert
        window.close();
    }
            for(auto& pip : pipes){
    if(fx == pip.getPosition().x){
        window.clear();
        counter++;
        text.setString("Counter: " + std::to_string(counter));
    }
    }
    gameframes++;
 
    }
    return 0;
}