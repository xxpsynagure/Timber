#include "pch.h"
#include <iostream>
#include <sstream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

void updateBranches(int seed);
const int NUM_BRANCHES = 6;
sf::Sprite branches[NUM_BRANCHES];
//Where is the player or branch? (left/right)
enum class side {LEFT,RIGHT,NONE};
side branchPosition[NUM_BRANCHES];

int main()
{
    //Creating window with current desktop resolution
    sf::RenderWindow window(sf::VideoMode::getDesktopMode() , "Timber!", sf::Style::None);
   
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

    //Prepare 6 branches
    sf::Texture textureBranch;
    textureBranch.loadFromFile("Graphics/branch.png");
    for (int i = 0; i < NUM_BRANCHES; ++i)
    {
        branches[i].setTexture(textureBranch);
        branches[i].setPosition(-2000, -2000);
        //setting origin to sprite's centre to rotate later for change in side
        branches[i].setOrigin(220, 20);
    }
    
    //Player
    sf::Texture playerTexture;
    playerTexture.loadFromFile("Graphics/player.png");
    sf::Sprite playerSprite;
    playerSprite.setTexture(playerTexture);
    playerSprite.setPosition(580, 720);
    side playerSide = side::LEFT;

    //gravestone
    sf::Texture textureRIP;
    textureRIP.loadFromFile("Graphics/rip.png");
    sf::Sprite spriteRIP;
    spriteRIP.setTexture(textureRIP);
    spriteRIP.setPosition(600, 860);

    //axe
    sf::Texture textureAxe;
    textureAxe.loadFromFile("Graphics/axe.png");
    sf::Sprite spriteAxe;
    spriteAxe.setTexture(textureAxe);
    spriteAxe.setPosition(700, 830);

    const float AXE_POSITION_LEFT = 700;
    const float AXE_POSITION_RIGHT = 1075;

    //log
    sf::Texture textureLog;
    textureLog.loadFromFile("Graphics/log.png");
    sf::Sprite spriteLog;
    spriteLog.setTexture(textureLog);
    spriteLog.setPosition(810, 720);

    bool logActive = false;
    float logSpeedX = 1000;
    float logSpeedY = -1500;

    //control the player's input
    bool acceptInput = false;

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

    //sound
    sf::SoundBuffer chopBuffer;
    chopBuffer.loadFromFile("sound/wood_chop.wav");
    sf::Sound chop;
    chop.setBuffer(chopBuffer);

    sf::SoundBuffer deathBuffer;
    deathBuffer.loadFromFile("sound/sound_death.wav");
    sf::Sound death;
    death.setBuffer(deathBuffer);

    sf::SoundBuffer timeoutBuffer;
    timeoutBuffer.loadFromFile("sound/sound_out_of_time.wav");
    sf::Sound timeout;
    timeout.setBuffer(timeoutBuffer);


    //game loop
    while (window.isOpen())
    {
        

        sf::Event evnt;
        while (window.pollEvent(evnt))
        {
            if (evnt.type == evnt.Closed() || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
                window.close();

            if (evnt.type == sf::Event::KeyReleased && !paused)
            {
                acceptInput = true;

                spriteAxe.setPosition(2000, spriteAxe.getPosition().y);
            }
        }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return))
            {
                paused = false;
                
                //reset time and score
                score = 0;
                timeRemaining = 5;

                for (int i = 0; i < NUM_BRANCHES; ++i)
                {
                    branchPosition[i] = side::NONE;
                }
                spriteRIP.setPosition(675, 2000);//hidden
                playerSprite.setPosition(580, 720);
                acceptInput = true;

            }
             
            if (acceptInput)
            {
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
                {
                    playerSide = side::RIGHT;
                    score++;

                    timeRemaining += (2 / score) + .15;

                    spriteAxe.setPosition(AXE_POSITION_RIGHT, spriteAxe.getPosition().y);
                    playerSprite.setPosition(1200, 720);

                    updateBranches(score);

                    spriteLog.setPosition(810, 720);
                    logSpeedX = -5000;
                    logActive = true;

                    acceptInput = false;
                    //chop sound
                    chop.play();
                }

                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
                {
                    playerSide = side::LEFT;
                    score++;

                    timeRemaining += (2 / score) + .15;

                    spriteAxe.setPosition(AXE_POSITION_LEFT, spriteAxe.getPosition().y);
                    playerSprite.setPosition(580, 720);

                    updateBranches(score);

                    spriteLog.setPosition(810, 720);
                    logSpeedX = 5000;
                    logActive = true;

                    acceptInput = false;
                    //chop sound
                    chop.play();
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

                timeout.play();
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

            //Updating score text
            std::stringstream ss;
            ss << "Score =" << score;
            scoreText.setString(ss.str());

            //Updating branch sprites
            for (int i = 0; i < NUM_BRANCHES; ++i)
            {
                float height = i * 150;
                if (branchPosition[i] == side::LEFT)
                {
                    branches[i].setPosition(610, height);
                    branches[i].setRotation(180);
                }
                else if (branchPosition[i] == side::RIGHT)
                {
                    branches[i].setPosition(1330, height);
                    branches[i].setRotation(0);
                }
                else
                    branches[i].setPosition(3000, height);
                    //hiding the branch

            }

            if (logActive)
            {
                spriteLog.setPosition(spriteLog.getPosition().x + (logSpeedX * deltaTime.asSeconds()), spriteLog.getPosition().y + (logSpeedY * deltaTime.asSeconds()));

                if (spriteLog.getPosition().x < -100 || spriteLog.getPosition().x>2000)
                {
                    logActive = false;
                    //new log at next frame
                    spriteLog.setPosition(810, 720);
                }
            }

            //if player is squished
            if (branchPosition[5] == playerSide)
            {
                //death
                paused = true;
                acceptInput = false;
                spriteRIP.setPosition(525, 760);
                playerSprite.setPosition(2000, 660);//hiding

                messageText.setString("SQUISHED!!");
                sf::FloatRect textRect = messageText.getLocalBounds();
                messageText.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height/2.0f);
                messageText.setPosition(1920 / 2.0f, 1080 / 2.0f);

                //play death sound
                death.play();
            }
        }
        



        //double buffering
        window.clear(); //can hold colour
        window.draw(spriteBackground); //need to be a sprite object
        window.draw(spriteCloud1);
        window.draw(spriteCloud2);
        window.draw(spriteCloud3);
        for (int i = 0; i < NUM_BRANCHES; ++i)
            window.draw(branches[i]);
        window.draw(spriteTree);

        window.draw(playerSprite);
        window.draw(spriteAxe);
        window.draw(spriteLog);
        window.draw(spriteRIP);

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

void updateBranches(int seed)
{
    for (int i = NUM_BRANCHES - 1; i > 0; i--)
        branchPosition[i] = branchPosition[i - 1];

    //spawn a new branch at position 0
    srand((int)time(0) + seed);
    int r = (rand() % 5);

    switch (r)
    {
    case 0:
        branchPosition[0] = side::LEFT;
        break;
    case 1:
        branchPosition[0] = side::RIGHT;
        break;
    default:
        branchPosition[0] = side::NONE;

    }
}