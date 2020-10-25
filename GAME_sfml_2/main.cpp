#define _CRT_SECURE_NO_WARNINGS
#include<SFML/Graphics.hpp>
#include<SFML/Audio.hpp>
#include<iostream>
#include<stdio.h>
#include<time.h>
const float view_w = 640.0f, view_h = 360.0f;
const float normalSpeed = 2.5;
int foodtype[15], score = 0;
struct character 
{
	bool startjump = false, walk;
	float high = 0, x, y;
};
void resizeView(const sf::RenderWindow& window, sf::View& view)
{
	float viewRatio = float(window.getSize().x) / float(window.getSize().y);
	view.setSize(view_w * viewRatio, view_h);
}
void positionUpdate(character &c,sf::Sprite &sprite)
{
	c.x = sprite.getPosition().x;
	c.y = sprite.getPosition().y;
}
void randomicon(sf::Sprite &icon,int which)
{
	int base = rand() % 8;
	foodtype[which] = rand() % 45;
	icon.setTextureRect(sf::IntRect(foodtype[which] * 32, 0, 32, 32));
}
void randompositionIcon(sf::Sprite &icon)
{
	int whatrec = (rand() % 8) + 1;
	switch (whatrec)
	{
	case 1:
		icon.setPosition((rand() % 1241) + 10, 640);
		break;
	case 2:
		icon.setPosition((rand() % 159) + 10, 468);
		break;
	case 3:
		icon.setPosition((rand() % 219) + 350, 560-32);
		break;
	case 4:
		icon.setPosition((rand() % 319) + 100, 330-32);
		break;
	case 5:
		icon.setPosition((rand() % 119) + 600, 415-32);
		break;
	case 6:
		icon.setPosition((rand() % 119) + 850, 530-32);
		break;
	case 7:
		icon.setPosition((rand() % 119) + 975, 370-32);
		break;
	case 8:
		icon.setPosition((rand() % 479) + 550, 250-32);
		break;
	}
}
int main()
{
	srand(time(NULL));
	// Create the main window
	sf::RenderWindow window(sf::VideoMode(1280, 720), "GameMaiDaiNon", sf::Style::Close | sf::Style::Resize);
	window.setFramerateLimit(60);
	// Camera created
	sf::View cam(sf::FloatRect(0.f, 0.f, view_w, view_h));

	// Charactor Tex & Sprite
	sf::Texture playerTexture, dragonidle, nekotex, foodicon;
	if (!playerTexture.loadFromFile("picture/Originals/player.png"))
		std::cout << "Load player failed" << std::endl;
	if (!dragonidle.loadFromFile("picture/dragon/dragonidlewalkbgdelete.png"))
		std::cout << "Load dragonidlewalkbgdelete failed" << std::endl;
	if (!nekotex.loadFromFile("picture/nekowalkLRsmallsize.png"))
		std::cout << "Load nekowalkLR failed" << std::endl;
	if (!foodicon.loadFromFile("picture/iconz_resizeto32.png"))
		std::cout << "Load iconz failed" << std::endl;
	//.setSmooth(true);
	//foodicon.setSmooth(true);
	sf::Sprite playerSprite, dragonsprite, nekosprite, foodsprite[15];
	playerSprite.setTexture(playerTexture);
	dragonsprite.setTexture(dragonidle);
	nekosprite.setTexture(nekotex);
	float spriteSizeX = playerTexture.getSize().x / 3;
	float spriteSizeY = playerTexture.getSize().y / 4;
	float dragonsizex = dragonidle.getSize().x / 5;
	float dragonsizey = dragonidle.getSize().y / 4;
	float nekosizex = nekotex.getSize().x / 4;
	float nekosizey = nekotex.getSize().y / 2;
	float foodsize = foodicon.getSize().y;// size x == size y
	int animationFrame = 0, animationFrame5 = 0, animationFrame4 = 0, dragonway = 2, nekoway = 1;
	bool dragonright = false, nekoright = false;

	playerSprite.setPosition(510.0,640);
	playerSprite.setTextureRect(sf::IntRect( 0, 0, spriteSizeX, spriteSizeY));

	dragonsprite.setPosition(700, 485);

	nekosprite.setPosition(650, 225);

	character player,dragon;

	for (int i = 0; i <= 14; i++)
	{
		foodsprite[i].setTexture(foodicon);
		randomicon(foodsprite[i], i);
		randompositionIcon(foodsprite[i]);
	}

	//Map texture
	sf::Texture bg, re1, undergroundd;
	sf::Sprite bgSprite	,underground	,rec1	,rec2	,rec3	,rec4	,rec5	,rec6	,rec7	,rec8;
	if (!bg.loadFromFile("picture/Forest_BG_03.png"))
		std::cout << "Load background failed" << std::endl;
	bgSprite.setTexture(bg);
	bgSprite.setPosition(0,-207);
	if (!re1.loadFromFile("picture/Tiles/tile51.png"))
		std::cout << "Load re1 failed" << std::endl;
	if (!undergroundd.loadFromFile("picture/Tiles/tile11.png"))
		std::cout << "Load underground failed" << std::endl;
	re1.setRepeated(true);
	undergroundd.setRepeated(true);
	underground.setTexture(undergroundd);
	underground.setTextureRect(sf::IntRect(0,0,1280,150));
	underground.setPosition(0,720);
	rec1.setTexture(re1);
	rec1.setTextureRect(sf::IntRect(0,0,1280,48));
	rec1.setPosition(0, 672);
	rec2.setTexture(re1);
	rec2.setTextureRect(sf::IntRect(0, 0, 200, 48));
	rec2.setPosition(0, 500);
	rec3.setTexture(re1);
	rec3.setTextureRect(sf::IntRect(0, 0, 250, 48));
	rec3.setPosition(350, 560);
	rec4.setTexture(re1);
	rec4.setTextureRect(sf::IntRect(0, 0, 350, 48));
	rec4.setPosition(100, 330);
	rec5.setTexture(re1);
	rec5.setTextureRect(sf::IntRect(0, 0, 150, 48));
	rec5.setPosition(600, 415);
	rec6.setTexture(re1);
	rec6.setTextureRect(sf::IntRect(0, 0, 250, 48));
	rec6.setPosition(850, 530);
	rec7.setTexture(re1);
	rec7.setTextureRect(sf::IntRect(0, 0, 250, 48));
	rec7.setPosition(975, 370);
	rec8.setTexture(re1);
	rec8.setTextureRect(sf::IntRect(0, 0, 510, 48));
	rec8.setPosition(550, 250);

	// Load font
	sf::Font Kangfu;
	if(!Kangfu.loadFromFile("Font/Kangfu.ttf"))
		std::cout << "Load Kangfu failed" << std::endl;

	// Text
	sf::Text starttext;
	starttext.setFont(Kangfu);
	starttext.setString("START");
	starttext.setCharacterSize(108);
	starttext.setPosition(200.0f, 200.0f);
	starttext.setOutlineThickness(2);

	sf::Text highscoretext("HIGH SCORE", Kangfu, 108);
	highscoretext.setPosition(200, 247+30);
	highscoretext.setOutlineThickness(2);

	sf::Text exittext("EXIT", Kangfu, 108);
	exittext.setPosition(200, 294+60);
	exittext.setOutlineThickness(2);
	
	printf("%f", highscoretext.getGlobalBounds().height);

	sf::FloatRect startbox = starttext.getGlobalBounds();

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
	float totalTime = 0.0f;
	float totaltimeDRAGONway = 0;
	float totaltimeNEKOway = 1.5;

	sf::Clock clock;

	bool gameStarted = true;
	bool walk = false,noway = false;

	float grv = 0.0, veloX = normalSpeed,veloY=normalSpeed;

	/////////////////////////////////////////////////////////////// Start the game loop /////////////////////////////////////////////////////////////
	
	//cam.zoom(3);
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
			if (exittext.getGlobalBounds().contains(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y))
			{
				window.close();
			}
			//printf("%d\t%d\n", sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y);
		}

		if (gameStarted)//Menu Gui Button
		{
			if (starttext.getGlobalBounds().contains(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition().y-260+47))
			{
				starttext.setFillColor(sf::Color::Red);
				starttext.setOutlineColor(sf::Color::Cyan);
			}
			else if (highscoretext.getGlobalBounds().contains(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition().y - 260 + 47))
			{
				highscoretext.setFillColor(sf::Color::Red);
				highscoretext.setOutlineColor(sf::Color::Cyan);
			}
			else if (exittext.getGlobalBounds().contains(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition().y - 260 + 47))
			{
				exittext.setFillColor(sf::Color::Red);
				exittext.setOutlineColor(sf::Color::Cyan);
			}
			else
			{
				starttext.setFillColor(sf::Color::White);
				starttext.setOutlineColor(sf::Color::Black);
				highscoretext.setFillColor(sf::Color::White);
				highscoretext.setOutlineColor(sf::Color::Black);
				exittext.setFillColor(sf::Color::White);
				exittext.setOutlineColor(sf::Color::Black);
			}
		}

		//colision check food icon
		for (int k = 0; k <= 14; k++)
		{
			if (playerSprite.getGlobalBounds().intersects(foodsprite[k].getGlobalBounds()))
			{
				randomicon(foodsprite[k],k);
				randompositionIcon(foodsprite[k]);
				printf("%d\n", ++score);
			}
		}

		//Neko bot
		if (nekoway == 0)//		0-วิ่งขวา 
		{
			if (nekosprite.getPosition().x < 1060-nekosizex+10)
				nekosprite.move(1.5, 0);
			else
				nekoway = 1;
			nekosprite.setTextureRect(sf::IntRect(nekosizex * animationFrame4, nekosizey * 1, nekosizex, nekosizey));
			nekoright = false;
		}
		else if (nekoway == 1)//		1-วิ่งซ้าย 
		{
			if (nekosprite.getPosition().x > 540)
				nekosprite.move(-1.5, 0);
			else
				nekoway = 0;
			nekosprite.setTextureRect(sf::IntRect(nekosizex * animationFrame4, nekosizey * 0, nekosizex, nekosizey));
			nekoright = true;
		}
		//drogon bot
		if (dragonway == 0)//		0-วิ่งขวา 
		{
			if (dragonsprite.getPosition().x < 1070)
				dragonsprite.move(2, 0);
			else
				dragonway = 1;
			dragonsprite.setTextureRect(sf::IntRect(dragonsizex * animationFrame5, dragonsizey * 2, dragonsizex, dragonsizey));
			dragonright = false;
		}
		else if (dragonway == 1)//		1-วิ่งซ้าย 
		{
			if (dragonsprite.getPosition().x > -30)
				dragonsprite.move(-2, 0);
			else
				dragonway = 0;
			dragonsprite.setTextureRect(sf::IntRect(dragonsizex * animationFrame5, dragonsizey * 3, dragonsizex, dragonsizey));
			dragonright = true;
		}
		else if (dragonway == 2)//		2-idle
		{
			if (dragonright)//หันซ้าย
			{
				dragonsprite.setTextureRect(sf::IntRect(dragonsizex * animationFrame, dragonsizey * 1, dragonsizex, dragonsizey));
			}
			else//หัวขวา
			{
				dragonsprite.setTextureRect(sf::IntRect(dragonsizex * animationFrame, dragonsizey * 0, dragonsizex, dragonsizey));
			}
		}
		
		//new updated
		
		
		positionUpdate(player, playerSprite);
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
			if (playerSprite.getPosition().x < 188 && playerSprite.getPosition().y > 468 && veloY >= 0 && playerSprite.getPosition().y < 500) 
			{
				playerSprite.setPosition(playerSprite.getPosition().x, 468);
				player.startjump = false;
				grv = 0.f;
				noway = false;
			}//พื้น rec 2
			if (veloY >= 0 && playerSprite.getPosition().x > 327 && playerSprite.getPosition().x < 592 && playerSprite.getPosition().y < 560 && playerSprite.getPosition().y > 528) 
			{
				playerSprite.setPosition(playerSprite.getPosition().x, 528);
				player.startjump = false;
				grv = 0.f;
				noway = false;
			}//พื้น rec3
			if (veloY >= 0 && playerSprite.getPosition().x > 100-23 && playerSprite.getPosition().x < 450-8 && playerSprite.getPosition().y < 330 && playerSprite.getPosition().y > 330-32) 
			{
				playerSprite.setPosition(playerSprite.getPosition().x, 330 - 32);
				player.startjump = false;
				grv = 0.f;
				noway = false;
			}//พื้น rec4
			if (veloY >= 0 && playerSprite.getPosition().x > 600 - 23 && playerSprite.getPosition().x < 750 - 8 && playerSprite.getPosition().y < 415 && playerSprite.getPosition().y > 415 - 32) 
			{
				playerSprite.setPosition(playerSprite.getPosition().x, 415 - 32);
				player.startjump = false;
				grv = 0.f;
				noway = false;
			}//พื้น rec5
			if (veloY >= 0 && playerSprite.getPosition().x > 850 - 23 && playerSprite.getPosition().x < 1100 - 8 && playerSprite.getPosition().y < 530 && playerSprite.getPosition().y > 530 - 32) 
			{
				playerSprite.setPosition(playerSprite.getPosition().x, 530 - 32);
				player.startjump = false;
				grv = 0.f;
				noway = false;
			}//พื้น rec6
			if (veloY >= 0 && playerSprite.getPosition().x > 975 - 23 && playerSprite.getPosition().x < 1125 - 8 && playerSprite.getPosition().y < 370 && playerSprite.getPosition().y > 370 - 32) 
			{
				playerSprite.setPosition(playerSprite.getPosition().x, 370 - 32);
				player.startjump = false;
				grv = 0.f;
				noway = false;
			}//พื้น rec7
			if (veloY >= 0 && playerSprite.getPosition().x > 550 - 23 && playerSprite.getPosition().x < 1060 - 8 && playerSprite.getPosition().y < 250 && playerSprite.getPosition().y > 250 - 32) 
			{
				playerSprite.setPosition(playerSprite.getPosition().x, 250 - 32);
				player.startjump = false;
				grv = 0.f;
				noway = false;
			}//พื้น rec8
			// ^^ ADD rectangle statements HERE ^^ 
			if (playerSprite.getPosition().y >= 640 && veloY >= 0) 
			{
				playerSprite.setPosition(playerSprite.getPosition().x, 640);
				player.startjump = false;
				grv = 0;
			}//พื้นล่างสุด rec1
		}
		if (!walk && playerSprite.getPosition().x > 192 && playerSprite.getPosition().y == 468 && !noway) 
		{
			walk = true;
			player.startjump = true;
		}//GRAVITY rec2
		if (!walk && !noway && (playerSprite.getPosition().x < 350-24 || playerSprite.getPosition().x > 600-8) && playerSprite.getPosition().y == 528 ) 
		{
			walk = true;
			player.startjump = true;
		}//GRAVITY rec3
		if (!walk && !noway && (playerSprite.getPosition().x < 100 - 24 || playerSprite.getPosition().x > 450 - 8) && playerSprite.getPosition().y == 330-32) 
		{
			walk = true;
			player.startjump = true;
		}//GRAVITY rec4
		if (!walk && !noway && (playerSprite.getPosition().x < 600 - 24 || playerSprite.getPosition().x > 750 - 8) && playerSprite.getPosition().y == 415 - 32) 
		{
			walk = true;
			player.startjump = true;
		}//GRAVITY rec5
		if (!walk && !noway && (playerSprite.getPosition().x < 850 - 24 || playerSprite.getPosition().x > 1100 - 8) && playerSprite.getPosition().y == 530 - 32) 
		{
			walk = true;
			player.startjump = true;
		}//GRAVITY rec6
		if (!walk && !noway && (playerSprite.getPosition().x < 975 - 24 || playerSprite.getPosition().x > 1125 - 8) && playerSprite.getPosition().y == 370 - 32) 
		{
			walk = true;
			player.startjump = true;
		}//GRAVITY rec7
		if (!walk && !noway && (playerSprite.getPosition().x < 550 - 24 || playerSprite.getPosition().x > 1060 - 8) && playerSprite.getPosition().y == 250 - 32) 
		{
			walk = true;
			player.startjump = true;
		}//GRAVITY rec8
		// ^^ ADD rectangle statements HERE ^^ 

		veloX = normalSpeed;

		//animation updated
		totalTime += deltaTime;
		totaltimeDRAGONway += deltaTime;
		totaltimeNEKOway += deltaTime;

		if (totalTime >= switchTime)
		{
			totalTime -= switchTime;
			animationFrame++;
			animationFrame4++;
			animationFrame5++;
			if (animationFrame > 2)
				animationFrame = 0;
			if (animationFrame5 > 4)
				animationFrame5 = 0;
			if (animationFrame4 > 3)
				animationFrame4 = 0;
		}

		
		if (totaltimeDRAGONway >= 3)
		{
			dragonway = rand() % 3;
			totaltimeDRAGONway -= 3;
		}
		if (totaltimeNEKOway >= 3)
		{
			nekoway = rand() % 2;
			totaltimeNEKOway -= 3;
		}

		//cam updated
		if (playerSprite.getPosition().y < 528)
		{
			if (playerSprite.getPosition().x < 320)
				cam.setCenter(320 + playerTexture.getSize().x / 6, playerSprite.getPosition().y + playerTexture.getSize().y / 8);
			else if (playerSprite.getPosition().x > 944)
				cam.setCenter(944 + playerTexture.getSize().y / 8, playerSprite.getPosition().y + playerTexture.getSize().y / 8);
			else
				cam.setCenter(playerSprite.getPosition().x + playerTexture.getSize().x / 6, playerSprite.getPosition().y + playerTexture.getSize().y / 8);
		}
		else
		{
			if (playerSprite.getPosition().x < 320)
				cam.setCenter(320 + playerTexture.getSize().x / 6, 528 + 16);
			else if (playerSprite.getPosition().x > 944)
				cam.setCenter(944 + playerTexture.getSize().y / 8, 528 + 16);
			else
				cam.setCenter(playerSprite.getPosition().x + playerTexture.getSize().x / 6, 528 + 16);
		}

		// clear & draw display
		if (!gameStarted)
		{
			window.clear();
			window.draw(bgSprite);
			window.draw(underground);
			
			window.draw(rec1);
			window.draw(rec2);
			window.draw(rec3);
			window.draw(rec4);
			window.draw(rec5);
			window.draw(rec6);
			window.draw(rec7);
			window.draw(rec8);

			window.setView(cam);
			for (int t = 0; t <= 14; t++)
				window.draw(foodsprite[t]);
			window.draw(playerSprite);
			window.draw(dragonsprite);
			window.draw(nekosprite);
		}
		else
		{
			window.clear();
			window.draw(starttext);
			window.draw(highscoretext);
			window.draw(exittext);
		}
		window.display();
		//printf("%.1f %.1f\n", playerSprite.getPosition().x, playerSprite.getPosition().y);//player position
		//printf("%.1f %.1f\n", playerSprite.getPosition().x + playerTexture.getSize().x / 6, playerSprite.getPosition().y + playerTexture.getSize().y / 8);//cam center position
	}
	return 0;
}
//map
/*




				-----rec8------
	--rec4--    
			--rec5--	---rec7---
--rec2--			
		---rec3---	---rec6---
-------------------rec1------------

*/