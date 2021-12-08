#include "pch.h"
#include <iostream>
#include <sstream>
#include <SFML/Graphics.hpp>


int main()
{
    //Creating window with current desktop resolution
    sf::RenderWindow window(sf::VideoMode::getDesktopMode() , "Timber!", sf::Style::Close);
   
    //Creating a default view 
    sf::View view(sf::FloatRect(0,0,1920,1080));
    window.setView(view);

    //background
    sf::Texture textureBackground;
    textureBackground.loadFromFile("Graphics/background.png");
    sf::Sprite spriteBackground(textureBackground);
    spriteBackground.setPosition(0, 0);

    //tree
    sf::Texture textureTree;
    textureTree.loadFromFile("Graphics/tree.png");
    sf::Sprite spriteTree(textureTree);
    spriteTree.setPosition(810,0);
    sf::Vector2f treepos = spriteTree.getPosition();
    
    //bee
    sf::Texture textureBee;
    textureBee.loadFromFile("Graphics/bee.png");
    sf::Sprite spriteBee(textureBee);
    spriteBee.setPosition(0, 800);
    bool beeActive = false;
    float beeSpeed = 0.0f;

    //cloud
    sf::Texture textureCloud;
    textureCloud.loadFromFile("Graphics/cloud.png");
    sf::Sprite spriteCloud1(textureCloud), spriteCloud2(textureCloud), spriteCloud3(textureCloud);
    spriteCloud1.setPosition(0, 0);
    spriteCloud2.setPosition(0, 150);
    spriteCloud3.setPosition(0, 300);
    bool cloud1Active = false, cloud2Active = false, cloud3Active = false;
    float cloud1Speed = 0.0f, cloud2Speed = 0.0f, cloud3Speed = 0.0f;


    //Message and Score
    sf::Font font;
    font.loadFromFile("fonts/KOMIKAP_.ttf");
    int score = 0;
    sf::Text messageText;
    sf::Text scoreText;
    messageText.setFont(font);
    scoreText.setFont(font);
    messageText.setString("Press Enter to start!");
    scoreText.setString("Score = 0");
    messageText.setCharacterSize(75);
    scoreText.setCharacterSize(100);
    messageText.setFillColor(sf::Color::White);
    scoreText.setFillColor(sf::Color::White);

    sf::FloatRect textRect = messageText.getLocalBounds();
    messageText.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
    messageText.setPosition(1920 / 2.0f, 1080 / 2.0f);
    scoreText.setPosition(20, 20);
    

    sf::Clock clock;
    
    //Time Bar
    sf::RectangleShape timeBar;
    float timeBarStartWidth = 400;
    float timeBarheight = 80;
    timeBar.setSize(sf::Vector2f(timeBarStartWidth, timeBarheight));
    timeBar.setPosition((1920 / 2) - timeBarStartWidth / 2, 980);
    timeBar.setFillColor(sf::Color::Red);

    sf::Time gameTotalTime;
    float timeRemaining = 6.0f;
    float timeBarWidthPerSecond = timeBarStartWidth / timeRemaining;


    
    bool paused = true;

    //game loop
    while (window.isOpen())
    {
        

        sf::Event evnt;
        while (window.pollEvent(evnt))
        {
            if (evnt.type == evnt.Closed() || sf::Keyboard::isKeyPressed(sf::Keyboard::Delete))
                window.close();   

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return))
            {
                paused = false;

                score = 0;
                timeRemaining = 5;
            }
                
        }
        
        if (!paused)
        {
            //Holds time elapsed between each frame
            //Clock needs to be restarted for each frame
            sf::Time deltaTime = clock.restart();

            timeRemaining -= deltaTime.asSeconds();
            timeBar.setSize(sf::Vector2f(timeBarWidthPerSecond * timeRemaining, timeBarheight));

            if (timeRemaining <= 0.0f)
            {
                paused = true;
                messageText.setString("Out of Time!!");
                sf::FloatRect textRect = messageText.getLocalBounds();
                messageText.setOrigin(sf::Vector2f((textRect.left + textRect.width) / 2.0f, (textRect.top + textRect.height) / 2.0f));
                messageText.setPosition(sf::Vector2f(1920 / 2.0f, 1080 / 2.0f));
            }
            //Setup Bee
            if (!beeActive)
            {
                //Bee speed
                srand((int)time(0) * 10);
                beeSpeed = (rand() % 200) + 200; //199-399

                //Bee Height
                srand((int)time(0) * 10);
                float beeHeight = (rand() % 500) + 500; //499-999
                spriteBee.setPosition(2000, beeHeight);
                beeActive = true;
            }
            else
            {
                spriteBee.setPosition(spriteBee.getPosition().x - (beeSpeed * deltaTime.asSeconds()), spriteBee.getPosition().y);

                if (spriteBee.getPosition().x < -100)
                    beeActive = false;
            }

            //setup Cloud1
            if (!cloud1Active)
            {
                srand((int)time(0) * 10);
                cloud1Speed = (rand() % 200); //0-199

                srand((int)time(0) * 10);
                float cloudHeight = (rand() % 150);
                spriteCloud1.setPosition(-200, cloudHeight);
                cloud1Active = true;
            }
            else
            {
                spriteCloud1.setPosition(spriteCloud1.getPosition().x + (cloud1Speed * deltaTime.asSeconds()), spriteCloud1.getPosition().y);

                if (spriteCloud1.getPosition().x > 1930)
                    cloud1Active = false;
            }

            //setup Cloud2
            if (!cloud2Active)
            {
                srand((int)time(0) * 20);
                cloud2Speed = (rand() % 250);

                srand((int)time(0) * 20);
                float cloudHeight = (rand() % 300) - 150;
                spriteCloud2.setPosition(-200, cloudHeight);
                cloud2Active = true;
            }
            else
            {
                spriteCloud2.setPosition(spriteCloud2.getPosition().x + (cloud2Speed * deltaTime.asSeconds()), spriteCloud2.getPosition().y);

                if (spriteCloud2.getPosition().x > 1930)
                    cloud2Active = false;
            }

            //setup Cloud3
            if (!cloud3Active)
            {
                srand((int)time(0) * 30);
                cloud3Speed = (rand() % 200);

                srand((int)time(0) * 30);
                float cloudHeight = (rand() % 450) - 150;
                spriteCloud3.setPosition(-200, cloudHeight);
                cloud3Active = true;
            }
            else
            {
                spriteCloud3.setPosition(spriteCloud3.getPosition().x + (cloud3Speed * deltaTime.asSeconds()), spriteCloud3.getPosition().y);

                if (spriteCloud3.getPosition().x > 1930)
                    cloud3Active = false;
            }

            std::stringstream ss;
            ss << "Score =" << score;
            scoreText.setString(ss.str());
        }
        



        //double buffering
        window.clear(); //can hold colour
        window.draw(spriteBackground); //need to be a sprite object
        window.draw(spriteCloud1);
        window.draw(spriteCloud2);
        window.draw(spriteCloud3);
        window.draw(spriteTree);
        window.draw(spriteBee);
        window.draw(scoreText);
        window.draw(timeBar);
        if (paused)
            window.draw(messageText);
        //displays everything just drawn
        window.display(); 
    }

    return 0;
}

