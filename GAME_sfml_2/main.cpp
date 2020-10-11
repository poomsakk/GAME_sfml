#define _CRT_SECURE_NO_WARNINGS
#include<SFML/Graphics.hpp>
#include<SFML/Audio.hpp>
#include<iostream>
#include<stdio.h>
#include<time.h>
const float view_w = 640.0f, view_h = 360.0f;
const float normalSpeed = 2.5;
struct character 
{
	bool startjump = false, onAir = false;
	float high = 0;
	float left, top, right, bottom;
};
void resizeView(const sf::RenderWindow& window, sf::View& view)
{
	float viewRatio = float(window.getSize().x) / float(window.getSize().y);
	view.setSize(view_w * viewRatio, view_h);
}
void positionUpdate(character &c, float spritesizeX, float spritesizeY, sf::Sprite &sprite)
{
	c.left = sprite.getPosition().x;
	c.right = sprite.getPosition().x + spritesizeX;
	c.top = sprite.getPosition().y;
	c.bottom = sprite.getPosition().y + spritesizeY;
}
int main()
{
	// Create the main window
	sf::RenderWindow window(sf::VideoMode(1280, 720), "GameMaiDaiNon", sf::Style::Close | sf::Style::Resize);
	window.setFramerateLimit(60);
	// Camera created
	sf::View cam(sf::FloatRect(0.f, 0.f, view_w, view_h));

	// Charactor Tex & Sprite
	sf::Texture playerTexture;
	if (!playerTexture.loadFromFile("picture/Originals/player.png"))
		std::cout << "Load player failed" << std::endl;
	//playerTexture.setSmooth(true);
	sf::Sprite playerSprite;
	playerSprite.setTexture(playerTexture);
	float spriteSizeX = playerTexture.getSize().x / 3;
	float spriteSizeY = playerTexture.getSize().y / 4;
	int animationFrame = 0;

	playerSprite.setPosition(510.0,640);
	playerSprite.setTextureRect(sf::IntRect( 0, 0, spriteSizeX, spriteSizeY));

	character player,rec[5];

	//Map texture
	sf::Texture bg		,re1;
	sf::Sprite bgSprite	,rec1	,rec2;
	if (!bg.loadFromFile("picture/Forest_BG_03.png"))
		std::cout << "Load background failed" << std::endl;
	bgSprite.setTexture(bg);
	bgSprite.setPosition(0,-207);
	if (!re1.loadFromFile("picture/Tiles/tile51.png"))
		std::cout << "Load re1 failed" << std::endl;
	re1.setRepeated(true);
	rec1.setTexture(re1);
	rec1.setTextureRect(sf::IntRect(0,0,1280,48));
	rec1.setPosition(0, 672);
	rec2.setTexture(re1);
	rec2.setTextureRect(sf::IntRect(0, 0, 200, 48));
	rec2.setPosition(0, 500);

	// Load font
	sf::Font Kangfu;
	if(!Kangfu.loadFromFile("Font/Kangfu.ttf"))
		std::cout << "Load Kangfu failed" << std::endl;

	// Text
	sf::Text start;
	start.setFont(Kangfu);
	start.setString("START");
	start.setCharacterSize(108);
	start.setFillColor(sf::Color::Red);
	start.setPosition(448.0f, 250.0f);

	sf::FloatRect startbox = start.getGlobalBounds();

	// Load a music to play 
		// 1 JakTrongNi
		// 2 Sonic
	sf::Music JakTrongNi;
	if (!JakTrongNi.openFromFile("Music/Sonic.ogg"))
		std::cout << "Load JakTrongNi failed" << std::endl;

	// Play the music
	JakTrongNi.setLoop(true);
	JakTrongNi.setVolume(10.0f);
	JakTrongNi.play();
	bool p = true;

	//set up
	float deltaTime = 0.0f;
	float switchTime = 0.2f;
	float totalTime = 0.0f,ttt=0.0f;

	sf::Clock clock;

	bool gameStarted = true;
	bool walk = false,noway = false;

	float grv = 0.0, veloX = normalSpeed,veloY=normalSpeed;
	/////////////////////////////////////////////////////////////// Start the game loop /////////////////////////////////////////////////////////////
	
	cam.zoom(3);
	while (window.isOpen())
	{
		deltaTime = clock.restart().asSeconds();
		// Process events
		sf::Event event;
		while (window.pollEvent(event))
		{
			// "close requested" event: we close the window
			if (event.type == sf::Event::Closed)
				window.close();
			if (event.type == sf::Event::Resized)
				resizeView(window, cam);
		}
		// input

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) && !gameStarted )
		{
			if (playerSprite.getPosition().x < 1250) //เช็คขอบขวา
				playerSprite.move(veloX, 0.0f);
			playerSprite.setTextureRect(sf::IntRect(spriteSizeX * animationFrame, spriteSizeY * 2, spriteSizeX, spriteSizeY));
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) && !gameStarted)
		{
			if(playerSprite.getPosition().x > 12) //เช็คขอบ ซ้าย
				playerSprite.move(-veloX, 0.f);
			playerSprite.setTextureRect(sf::IntRect(spriteSizeX * animationFrame, spriteSizeY * 1, spriteSizeX, spriteSizeY));
		}
		//if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) && !gameStarted)
		//{
		//	playerSprite.move(0.f, -veloY);
		//	playerSprite.setTextureRect(sf::IntRect(spriteSizeX * animationFrame, spriteSizeY * 3, spriteSizeX, spriteSizeY));

		//}
		//else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) && !gameStarted)
		//{
		//	playerSprite.move(0.f, veloY);
		//	playerSprite.setTextureRect(sf::IntRect(spriteSizeX * animationFrame, spriteSizeY * 0, spriteSizeX, spriteSizeY));
		//}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && !gameStarted && !player.startjump)
		{
			player.startjump = true;
			noway = true;
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
		{
			window.close();
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::P))
		{
			switch (p)
			{
			case true:
				p = false;
				JakTrongNi.pause();
				break;
			case false:
				p = true;
				JakTrongNi.play();
				break;
			}
		}
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && gameStarted)
		{
			if (startbox.contains(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y))
			{
				gameStarted = false;
				//sf::Mouse::setPosition(sf::Vector2i(0, 0), window);
			}
			//printf("%d\t%d\n", sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y);
		}
		//new updated

		//jump updated
		if (player.startjump)
		{
			if (walk) 
			{
				grv = 10.0f;
				walk = false;
			}
			grv += 0.25;
			if (grv > 20.f) grv = 20.f;
			veloY = -10.f + grv;
			playerSprite.move(0, veloY);
			if (playerSprite.getPosition().x < 188 && playerSprite.getPosition().y >468 && veloY >= 0 && playerSprite.getPosition().y < 500) //พื้น rec 2
			{
				playerSprite.setPosition(playerSprite.getPosition().x, 468);
				player.startjump = false;
				grv = 0.f;
				noway = false;
			}
			
			//ADD rectangle statements HERE 


			if (playerSprite.getPosition().y >= 640 && veloY >= 0) //พื้นล่างสุด rec1
			{
				playerSprite.setPosition(playerSprite.getPosition().x, 640);
				player.startjump = false;
				grv = 0;
			}
		}
		if (!walk && playerSprite.getPosition().x > 192 && playerSprite.getPosition().y == 468 && !noway) //GRAVITY
			{
				walk = true;
				player.startjump = true;
			}

		veloX = normalSpeed;

		//animation updated
		totalTime += deltaTime;

		if (totalTime >= switchTime)
		{
			totalTime -= switchTime;
			animationFrame++;

			if (animationFrame >= 2)
			{
				animationFrame = 0;
			}
		}

		//position updated
		positionUpdate(player, spriteSizeX, spriteSizeY, playerSprite);

		//cam updated
		if (playerSprite.getPosition().x < 320)
			cam.setCenter(320 + playerTexture.getSize().x / 6, playerSprite.getPosition().y + playerTexture.getSize().y / 8);
		else if (playerSprite.getPosition().x > 944)
			cam.setCenter(944 + playerTexture.getSize().y / 8, playerSprite.getPosition().y + playerTexture.getSize().y / 8);
		else
			cam.setCenter(playerSprite.getPosition().x + playerTexture.getSize().x / 6, playerSprite.getPosition().y + playerTexture.getSize().y / 8);

		// clear & draw display
		if (!gameStarted)
		{
			window.clear();
			window.draw(bgSprite);
			
			window.draw(rec1);
			window.draw(rec2);

			window.setView(cam);
			window.draw(playerSprite);
		}
		else
		{
			window.clear();
			window.draw(start);
		}
		window.display();
		printf("%.1f %.1f\n", playerSprite.getPosition().x, playerSprite.getPosition().y);//player position
		//printf("%.1f %.1f\n", playerSprite.getPosition().x + playerTexture.getSize().x / 6, playerSprite.getPosition().y + playerTexture.getSize().y / 8);//cam center position
	}
	return 0;
}
//map
/*






--rec2--

-------------------rec1---------------------

*/