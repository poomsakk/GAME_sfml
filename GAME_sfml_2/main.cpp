#define _CRT_SECURE_NO_WARNINGS
#include <SFML/Graphics.hpp>
#include <stdio.h>
#include <iostream>
#include <time.h>
#include <fstream>
#include <algorithm>
#include <vector>
#include <SFML/Audio.hpp>
#include <math.h>
// LENGTH OF VECTOR: |V| = sqrt(V.x^2 + V.y^2)
// NORMALIZE VECTOR: U = V / |V|
#define idle 1
#define walk 2
#define hurt 4
#define attack 3
#define death 5
const float PI = 3.14159265;
const float view_h = 900, view_w = 1600;
sf::Sprite lizardsprite[20], hearthSprite[30], coinSprite[30], starSprite[30];
int HowManylizardInPlattform[9], lizardWherePlatAreU[20], lizardhp[20];//Item buff type 0 - No,1.Max Health,2.Hp regen+,3.Speed boost,4.Damge+,5.Mana regen+,6.Def+
int buffOnground = 0;
bool lizardfall[20] = { false }, islizardDie[20] = { true }, lizardDeadAnimationStart[20] = { false };
bool hearthOnScreen[30], coinOnScreen[30], starOnScreen[30];
std::vector<std::pair<std::string, int>> vec;
class Bullet
{
public:
	sf::Sprite sprite;
	sf::Vector2f currVelocity;
	int Type = 0;



	Bullet(sf::Texture& texture, int type)
		:currVelocity(0, 0)
	{
		this->sprite.setTexture(texture);
		this->Type = type;
	}

	void updateFrame(float deltatime)
	{
		if (Type == 2)
		{
			totaltime += deltatime;
			if (totaltime > 0.2)
			{
				totaltime -= 0.2;
				animationFrame++;
				if (animationFrame == 22)
					animationFrame = 0;
			}
			this->sprite.setTextureRect(sf::IntRect(50 * animationFrame, 0, 50, 50));
		}
		else if (Type == 3)
		{
			totaltime += deltatime;
			if (totaltime > 0.1)
			{
				totaltime -= 0.1;
				animationFrame++;
				if (animationFrame == 6)
					animationFrame = 0;
			}
			this->sprite.setTextureRect(sf::IntRect(234 * animationFrame, 0, 234, 90));
		}
		else if (Type == 4)
		{
			animationFrame++;
			if (animationFrame == 60)
				animationFrame = 0;
			this->sprite.setTextureRect(sf::IntRect(100 * animationFrame, 0, 100, 100));
		}
	}

private:
	float totaltime = 0;
	int animationFrame = 0;
};
//type 1 = mage
//type 2 = jelly 8x   290*274  Jelly.png
//type 3 = dragon  9x  170*155  dragonballz.png
//type 4 = batgreen 6x 84*51     batgreen.png
//type 5 = Duke 6x 170*125      Duke.png
class Enermy
{
public:
	sf::Sprite sprite;
	int Type = 0;
	int HP;
	float TimeAtk = 0;
	float totalTimeATK = 0;
	sf::Vector2f currentVelocity;

	sf::RectangleShape Redbg;
	sf::RectangleShape HpGreen;

	Enermy(sf::Texture& texture, int type)
	{
		sprite.setTexture(texture);
		Type = type;
		if (type == 1)
		{
			HP = 100;
			Redbg.setSize(sf::Vector2f(100, 5));
			Redbg.setFillColor(sf::Color::Red);
			HpGreen.setFillColor(sf::Color::Green);
			HpGreen.setSize(sf::Vector2f(100, 5));
		}
		else if (type == 2)
		{
			HP = 400;
			Redbg.setSize(sf::Vector2f(150, 5));
			Redbg.setFillColor(sf::Color::Red);
			HpGreen.setFillColor(sf::Color::Green);
			HpGreen.setSize(sf::Vector2f(150, 5));
		}
		else if (type == 3)
		{
			HP = 200;
			Redbg.setSize(sf::Vector2f(100, 5));
			Redbg.setFillColor(sf::Color::Red);
			HpGreen.setFillColor(sf::Color::Green);
			HpGreen.setSize(sf::Vector2f(100, 5));
		}
		else if (type == 4)
		{
			HP = 10;
			Redbg.setSize(sf::Vector2f(40, 5));
			Redbg.setFillColor(sf::Color::Red);
			HpGreen.setFillColor(sf::Color::Green);
			HpGreen.setSize(sf::Vector2f(40, 5));
		}
		else if (type == 5)
		{
			HP = 2000;
			Redbg.setSize(sf::Vector2f(200, 5));
			Redbg.setFillColor(sf::Color::Red);
			HpGreen.setFillColor(sf::Color::Green);
			HpGreen.setSize(sf::Vector2f(200, 5));
		}
	}

	void updateFrame(float deltatime, sf::Vector2f camPos)
	{
		totalTimeATK += deltatime;
		if (Type == 1)
		{
			totaltimeChangeFrame += deltatime;
			if (totaltimeChangeFrame > 0.1)
			{
				totaltimeChangeFrame -= 0.1;
				animationFrame++;
				if (animationFrame == 8)
					animationFrame = 0;
			}
			if (currentVelocity.x > 0)
				this->sprite.setTextureRect(sf::IntRect(122 * animationFrame, 0, 122, 110));
			else
				this->sprite.setTextureRect(sf::IntRect(122 * animationFrame + 122, 0, -122, 110));
			HpGreen.setSize(sf::Vector2f(HP, 5));
			//ออกขอบ
			if (sprite.getPosition().x < 0)
			{
				currentVelocity.x = 2 + (rand() % 30) / 10.0f;
			}
			else if (sprite.getPosition().x > 3840)
			{
				currentVelocity.x = -2 - (rand() % 30) / 10.0f;
			}
			else if (sprite.getPosition().y > 1900)
			{
				currentVelocity.y = -2 - (rand() % 30) / 10.0f;
			}
			else if (sprite.getPosition().y < 1000)
			{
				currentVelocity.y = 2 + (rand() % 30) / 10.0f;
			}
			else if (rand() % 14 < 2)//อยากนอน
			{
				if (sprite.getPosition().x > camPos.x + 800)
					currentVelocity.x = 10 - (rand() % 110 + 50) / 10.0f;
				else if (sprite.getPosition().x < camPos.x - 800)
					currentVelocity.x = 10 - (rand() % 110 + 50) / 10.0f;
			}
		}
		else if (Type == 2)
		{
			totaltimeChangeFrame += deltatime;
			if (totaltimeChangeFrame > 0.1)
			{
				totaltimeChangeFrame -= 0.1;
				animationFrame++;
				if (animationFrame == 8)
					animationFrame = 0;
			}
			if (currentVelocity.x > 0)
				this->sprite.setTextureRect(sf::IntRect(290 * animationFrame, 0, 290, 274));
			else
				this->sprite.setTextureRect(sf::IntRect(290 * animationFrame + 290, 0, -290, 274));
			HpGreen.setSize(sf::Vector2f(HP * 150 / 400.0, 5));
			//ออกขอบ
			if (sprite.getPosition().x < 0)
			{
				currentVelocity.x = 1 + (rand() % 20) / 10.0f;
			}
			else if (sprite.getPosition().x > 3840)
			{
				currentVelocity.x = -1 - (rand() % 20) / 10.0f;
			}
			else if (sprite.getPosition().y > 1900)
			{
				currentVelocity.y = -1 - (rand() % 20) / 10.0f;
			}
			else if (sprite.getPosition().y < 1000)
			{
				currentVelocity.y = 1 + (rand() % 30) / 10.0f;
			}
			else if (rand() % 14 < 2)//อยากนอน
			{
				if (sprite.getPosition().x > camPos.x + 800)
					currentVelocity.x = 10 - (rand() % 100 + 30) / 10.0f;
				else if (sprite.getPosition().x < camPos.x - 800)
					currentVelocity.x = 10 - (rand() % 100 + 30) / 10.0f;
			}
		}
		else if (Type == 3)
		{
			totaltimeChangeFrame += deltatime;
			if (totaltimeChangeFrame > 0.1)
			{
				totaltimeChangeFrame -= 0.1;
				animationFrame++;
				if (animationFrame == 9)
					animationFrame = 0;
			}
			if (currentVelocity.x < 0)
				this->sprite.setTextureRect(sf::IntRect(170 * animationFrame, 0, 170, 155));
			else
				this->sprite.setTextureRect(sf::IntRect(170 * animationFrame + 170, 0, -170, 155));
			HpGreen.setSize(sf::Vector2f(HP * 100 / 200.0, 5));
			if (sprite.getPosition().x < 0)
			{
				currentVelocity.x = 3 + (rand() % 20) / 10.0f;
			}
			else if (sprite.getPosition().x > 3840)
			{
				currentVelocity.x = -3 - (rand() % 20) / 10.0f;
			}
		}
		else if (Type == 4)//BAT
		{
			totaltimeChangeFrame += deltatime;
			if (totaltimeChangeFrame > 0.05)
			{
				totaltimeChangeFrame -= 0.05;
				animationFrame++;
				if (animationFrame == 6)
					animationFrame = 0;
			}
			if (currentVelocity.x < 0)
				this->sprite.setTextureRect(sf::IntRect(84 * animationFrame, 0, 84, 51));
			else
				this->sprite.setTextureRect(sf::IntRect(84 * animationFrame + 84, 0, -84, 51));
			HpGreen.setSize(sf::Vector2f(HP * 40 / 10.0, 5));
		}
		else if (Type == 5)//Boss type 5 = Duke 6x 170*125      Duke.png
		{
			totaltimeChangeFrame += deltatime;
			if (totaltimeChangeFrame > 0.15)
			{
				totaltimeChangeFrame -= 0.15;
				animationFrame++;
				if (animationFrame == 6)
					animationFrame = 0;
			}
			if (HP > 1000)
			{
				if (currentVelocity.x < 0)
					this->sprite.setTextureRect(sf::IntRect(170 * animationFrame, 0, 170, 125));
				else
					this->sprite.setTextureRect(sf::IntRect(170 * animationFrame + 170, 0, -170, 125));
			}
			else
			{
				if (currentVelocity.x < 0)
					this->sprite.setTextureRect(sf::IntRect(170 * animationFrame, 125, 170, 125));
				else
					this->sprite.setTextureRect(sf::IntRect(170 * animationFrame + 170, 125, -170, 125));
			}
			HpGreen.setSize(sf::Vector2f(HP / 10.0, 5));
		}
	}
private:
	float totaltimeChangeFrame = 0;
	int animationFrame = 0;
};
//Explosion1 354x343 * 7
class Explosion
{
public:
	sf::Sprite sprite;
	float totalTiemFrame = 0;
	float switchTimeFrame = 0;
	int animationFrame = 0;
	int Type = 0;

	Explosion(sf::Texture &tex, int type)
	{
		sprite.setTexture(tex);
		Type = type;
		if (Type == 1)
			switchTimeFrame = 0.1;
	}

	void update(float deltatime)
	{
		totalTiemFrame += deltatime;
		if (Type == 1)
		{
			if (totalTiemFrame > switchTimeFrame)
			{
				totalTiemFrame -= switchTimeFrame;
				animationFrame++;
			}
		}
		sprite.setTextureRect(sf::IntRect(354 * animationFrame, 0, 354, 343));
	}
};
float rotate(sf::Vector2f base, sf::Vector2f target)
{
	float dx = base.x - target.x;
	float dy = base.y - target.y;
	float rotation = (atan2(dy, dx)) * 180 / PI;
	return rotation + 180;
}
bool sortbysecdesc(const std::pair<std::string, int>& a, const std::pair<std::string, int>& b)
{
	return a.second > b.second;
}
void randomlizard(sf::RectangleShape& lizard, int lizardNumber)
{
	int whatPlatform = (rand() % 8) + 1;
	lizardhp[lizardNumber] = 100;
	if (HowManylizardInPlattform[whatPlatform] >= 2 && whatPlatform != 1)
	{
		return randomlizard(lizard, lizardNumber);
	}
	else
	{
		HowManylizardInPlattform[whatPlatform]++;
		lizardWherePlatAreU[lizardNumber] = whatPlatform;
		switch (whatPlatform)
		{
		case 1:
			lizard.setPosition(rand() % 3840 - 60, 0);
			break;
		case 2:
			lizard.setPosition(rand() % (500 - 60), 0);
			break;
		case 3:
			lizard.setPosition(rand() % (700 - 60) + 900, 0);
			break;
		case 4:
			lizard.setPosition(rand() % (300 - 60) + 1900, 0);
			break;
		case 5:
			lizard.setPosition(rand() % (450 - 60) + 2500, 0);
			break;
		case 6:
			lizard.setPosition(rand() % (450 - 60) + 450, 0);
			break;
		case 7:
			lizard.setPosition(rand() % (1000 - 60) + 1500, 0);
			break;
		case 8:
			lizard.setPosition(rand() % (700 - 60) + 2900, 0);
			break;
		}
		lizardfall[lizardNumber] = true;
	}
}
void lizarddrop(sf::Vector2f position, int dropRateHearth, int dropRateCoin, int DropRateStar)
{
	int hp = rand() % 100;
	int co = rand() % 100;
	if (hp < dropRateHearth)
	{
		for (int i = 0; i < 30; i++)
		{
			if (!hearthOnScreen[i])
			{
				hearthSprite[i].setPosition(position.x - 15, position.y + 10);
				hearthOnScreen[i] = true;
				break;
			}
		}
	}
	if (co < dropRateCoin)
	{
		for (int i = 0; i < 30; i++)
		{
			if (!coinOnScreen[i])
			{
				coinSprite[i].setPosition(position.x + 15, position.y + 10);
				coinOnScreen[i] = true;
				break;
			}
		}
	}
}
void MenuBgMove(sf::Sprite& bg, float speed)
{
	bg.move(speed / 100.0, 0);
	if (bg.getPosition().x < -1600 || bg.getPosition().x > 1600)
	{
		bg.setPosition(0, -900);
	}
}
int main()
{
	srand(time(NULL));
	sf::RenderWindow window(sf::VideoMode(1600, 900), "Test");//3840*2160
	window.setFramerateLimit(60);
	sf::View cam(sf::FloatRect(0.f, .0f, view_w, view_h));
	///////////////////////////////////////////////////////////  ตัวละคร   ///////////////////////////////////////////////////////////////
	sf::Texture wiztex;
	if (!wiztex.loadFromFile("picture/wizzardAll.png"))
		printf("Load wizard failed");
	float wizsizex = wiztex.getSize().x / 8, wizesizey = wiztex.getSize().y / 16, playerrecsizex = 68, playerrecsizey = 90;
	sf::Sprite wizsprite(wiztex, sf::IntRect(0, 0, wizsizex, wizesizey));
	sf::RectangleShape playerrec(sf::Vector2f(68, 90));//playerrec  68 x 90
	sf::RectangleShape playerfootrec(sf::Vector2f(68, 1));
	playerrec.setFillColor(sf::Color::Transparent);
	playerrec.setOutlineColor(sf::Color::Green);
	playerrec.setOutlineThickness(2);
	playerrec.setPosition(-100, 1800 - 90);
	playerfootrec.setPosition(playerrec.getPosition().x, playerrec.getPosition().y + 89);
	sf::Texture magetex;
	if (!magetex.loadFromFile("picture/mage-2-122x110.png"))
		printf("Load mage-2-122x110 fail");
	sf::Sprite magesprite(magetex);
	magesprite.setScale(3, 3);
	sf::Texture lizardtex;
	if (!lizardtex.loadFromFile("picture/LizardLR256x256V2.png"))
		printf("Load LizardLR256x256V2 fail");
	//sf::Sprite lizardsprite[10]; <- Global
	sf::RectangleShape lizardrec[20];
	sf::RectangleShape lizardfootrect[20];
	sf::RectangleShape lizardHpBar[20]; //HP BAR
	sf::RectangleShape lizardHitBox[20];
	sf::RectangleShape lizardAttack[20];
	//Bar setting
	for (int f = 0; f < 20; f++)
	{
		lizardsprite[f].setTexture(lizardtex);
		lizardrec[f].setSize(sf::Vector2f(68, 90));
		lizardfootrect[f].setSize(sf::Vector2f(68, 1));
		lizardfootrect[f].setPosition(lizardrec[f].getPosition().x, lizardrec[f].getPosition().y + 89);

		lizardrec[f].setPosition(570, 650);
		lizardrec[f].setFillColor(sf::Color::Transparent);
		lizardrec[f].setOutlineColor(sf::Color::Cyan);
		lizardrec[f].setOutlineThickness(1);

		lizardHpBar[f].setSize(sf::Vector2f(68, 5));//เปลี่ยนในลูปตลอดอยู่แล้ว
		lizardHpBar[f].setFillColor(sf::Color::Green);

		lizardHitBox[f].setSize(sf::Vector2f(500, 20));
		lizardHitBox[f].setFillColor(sf::Color(0, 0, 0, 0));
		lizardHitBox[f].setOutlineColor(sf::Color::Red);
		lizardHitBox[f].setOutlineThickness(1);

		lizardAttack[f].setSize(sf::Vector2f(102, 90));
		lizardAttack[f].setFillColor(sf::Color(0, 0, 0, 0));
		lizardAttack[f].setOutlineColor(sf::Color::Yellow);
		lizardAttack[f].setOutlineThickness(1);

		lizardsprite[f].setTextureRect(sf::IntRect(256 * 0, 0, 256, 256));
		lizardhp[f] = 100;
	}
	int lizardstatus[20] = { 1 };// 1=idle  2=walk  3=hurt 4=attack 5=death
	bool lizardLR[20] = { false };
	float lizardVeloX[20] = { 2 };
	float totalTimeToChange[20] = { 0 };
	float switchTimeChange[20];
	bool lizardDonTee[20] = { false };
	bool lizardSlow[20] = { false };
	int timeToNoSlow[20];
	for (int i = 0; i < 20; i++)
	{
		switchTimeChange[i] = rand() % 300 / 100.0 + 1.5;
		lizardstatus[i] = (rand() % 2) + 1;
		//randomlizard(lizardrec[i], i);
		//islizardDie[i] = true;
		lizardVeloX[i] = 2;
	}
	///////////////////////////////////////////////////////////  ตัวละคร   ///////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////  ฉาก     ///////////////////////////////////////////////////////////////
	sf::Texture bgtex, groundtex, undergroundtex;
	if (!bgtex.loadFromFile("picture/Forest_BG_03.png"))
		printf("Load wizard failed\n");
	if (!groundtex.loadFromFile("picture/Tiles/tile51.png"))
		printf("Load tile51 failed\n");
	if (!undergroundtex.loadFromFile("picture/Tiles/tile11.png"))
		printf("Load tile11 failed\n");
	bgtex.setSmooth(true);
	groundtex.setRepeated(true);
	sf::Sprite bgsprite(bgtex);
	bgsprite.setScale(2, 2);
	sf::Sprite groundsprite(groundtex);
	sf::Sprite undergroundsprite(undergroundtex);
	sf::Sprite rec2(groundtex);
	sf::Sprite rec3(groundtex);
	sf::Sprite rec4(groundtex);
	sf::Sprite rec5(groundtex);
	sf::Sprite rec6(groundtex);
	sf::Sprite rec7(groundtex);
	sf::Sprite rec8(groundtex);

	groundsprite.setPosition(0, 1800);
	groundsprite.setTextureRect(sf::IntRect(0, 0, 1920 * 2, groundtex.getSize().y));
	undergroundsprite.setPosition(0, 1800 + 48);
	undergroundsprite.setTextureRect(sf::IntRect(0, 0, 1920 * 2, 2160 - 1848));
	rec2.setPosition(0, 1550);
	rec2.setTextureRect(sf::IntRect(0, 0, 500, groundtex.getSize().y));
	rec3.setPosition(900, 1600);
	rec3.setTextureRect(sf::IntRect(0, 0, 700, groundtex.getSize().y));
	rec4.setPosition(1900, 1535);
	rec4.setTextureRect(sf::IntRect(0, 0, 300, groundtex.getSize().y));
	rec5.setPosition(2500, 1610);
	rec5.setTextureRect(sf::IntRect(0, 0, 450, groundtex.getSize().y));
	rec6.setPosition(450, 1300);
	rec6.setTextureRect(sf::IntRect(0, 0, 700, groundtex.getSize().y));
	rec7.setPosition(1500, 1320);
	rec7.setTextureRect(sf::IntRect(0, 0, 1000, groundtex.getSize().y));
	rec8.setPosition(2900, 1350);
	rec8.setTextureRect(sf::IntRect(0, 0, 700, groundtex.getSize().y));

	///////////////////////////////////////////////////////////  ฉาก     ///////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////  Object    ///////////////////////////////////////////////////////////////
	//EFFECT
	sf::Texture firetex, watertex;
	if (!firetex.loadFromFile("picture/Fireball2.png"))//"picture/FireRL.png",animationframe60
		printf("Load Fireball2 failed\n");
	if (!watertex.loadFromFile("picture/water-effect.png"))
		printf("Load water-effect failed\n");
	sf::Sprite watersprite(watertex);
	sf::Sprite firesprite[10];
	for (int p = 0; p < 10; p++)
	{
		firesprite[p].setTexture(firetex);
		firesprite[p].setScale(.8, .8);
		firesprite[p].setPosition(0, 2000);
	}
	sf::Vector2f firevector[10];
	bool firebool[10] = { false };
	bool fireLR[10];
	bool firsttime[10] = { false };
	float fireVeloX = 8 * 2;
	int damageSkill1 = 40, damageSkill2 = 25;
	sf::RectangleShape waterreccheck(sf::Vector2f(206.92, 91));//เส้นสี่เหลี่ยมเช็คชนของสกิล 1
	waterreccheck.setPosition(sf::Vector2f(3000, 3000));
	sf::RectangleShape HpBar(sf::Vector2f(400, 20));
	sf::RectangleShape ManaBar(sf::Vector2f(500, 20));
	sf::RectangleShape bgHpBar(sf::Vector2f(500, 20));
	sf::RectangleShape bgManaBar(sf::Vector2f(500, 20));
	HpBar.setFillColor(sf::Color::Green);
	ManaBar.setFillColor(sf::Color::Blue);
	bgHpBar.setOutlineColor(sf::Color::Black);
	bgManaBar.setOutlineColor(sf::Color::Black);
	bgHpBar.setOutlineThickness(2);
	bgManaBar.setOutlineThickness(2);
	bgHpBar.setFillColor(sf::Color(255, 255, 255, 150));
	bgManaBar.setFillColor(sf::Color(255, 255, 255, 150));
	//buff
	sf::Texture bufftex;
	if (!bufftex.loadFromFile("picture/buff32x32.png"))
		printf("Load buff32x32 failed\n");
	for (int i = 0; i < 30; i++)
	{
		hearthSprite[i].setTexture(bufftex);
		coinSprite[i].setTexture(bufftex);
		starSprite[i].setTexture(bufftex);
	}
	sf::Texture albus;
	if (!albus.loadFromFile("picture/albus.png"))
		printf("Load albus failed\n");

	sf::Sprite albusSprite;
	albusSprite.setTexture(albus);
	albusSprite.setScale(sf::Vector2f(0.2, 0.2));
	///////////////////////////////////////////////////////  Object    ///////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////  TEXT & Font    ///////////////////////////////////////////////////////////////
	sf::Font zonebrownfont;
	if (!zonebrownfont.loadFromFile("Font/game_over.ttf"))//Zone Brown.ttf
		printf("Load Zone Brown font Failed\n");
	sf::Text timeLefttext("TIME LEFT : ", zonebrownfont, 96);//48
	sf::Text scoretext("SCORE : ", zonebrownfont, 96);//48
	scoretext.setOutlineThickness(2);
	scoretext.setOutlineColor(sf::Color::Black);
	timeLefttext.setOutlineThickness(2);
	timeLefttext.setOutlineColor(sf::Color::Black);
	sf::Text hptext("", zonebrownfont, 32);//16
	sf::Text manatext("", zonebrownfont, 32);//16
	hptext.setFillColor(sf::Color::White);
	hptext.setOutlineThickness(1);
	hptext.setOutlineColor(sf::Color::Black);
	manatext.setFillColor(sf::Color::White);
	manatext.setOutlineColor(sf::Color::Black);
	manatext.setOutlineThickness(1);
	sf::Text cointext("", zonebrownfont, 96);
	cointext.setOutlineThickness(2);
	cointext.setOutlineColor(sf::Color::Black);
	sf::Text NameInGame("", zonebrownfont, 96);
	NameInGame.setOutlineThickness(2);
	///////////////////////////////////////////////////////  TEXT & Font    ///////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////  Main menu   ///////////////////////////////////////////////////////////////
	sf::Texture menuSkytex, menuRock1tex, menuRock2tex, menuClound1tex, menuClound2tex, menuClound3tex, menuClound4tex;
	if (!menuSkytex.loadFromFile("picture/MenuLayerBg/sky.png"))
		printf("Load Zone sky Failed\n");
	if (!menuRock1tex.loadFromFile("picture/MenuLayerBg/rocks_1.png"))
		printf("Load Zone rocks_1 Failed\n");
	if (!menuRock2tex.loadFromFile("picture/MenuLayerBg/rocks_2.png"))
		printf("Load Zone rocks_2 Failed\n");
	if (!menuClound1tex.loadFromFile("picture/MenuLayerBg/clouds_1.png"))
		printf("Load Zone clouds_1 Failed\n");
	if (!menuClound2tex.loadFromFile("picture/MenuLayerBg/clouds_2.png"))
		printf("Load Zone clouds_1 Failed\n");
	if (!menuClound3tex.loadFromFile("picture/MenuLayerBg/clouds_3.png"))
		printf("Load Zone clouds_1 Failed\n");
	if (!menuClound4tex.loadFromFile("picture/MenuLayerBg/clouds_4.png"))
		printf("Load Zone clouds_1 Failed\n");
	menuSkytex.setRepeated(true);
	menuRock1tex.setRepeated(true);
	menuRock2tex.setRepeated(true);
	menuClound1tex.setRepeated(true);
	menuClound2tex.setRepeated(true);
	menuClound3tex.setRepeated(true);
	menuClound4tex.setRepeated(true);
	sf::Sprite menuSky(menuSkytex, sf::IntRect(0, 0, 3200, 900));
	sf::Sprite menuRock1(menuRock1tex, sf::IntRect(0, 0, 3200, 900));
	sf::Sprite menuRock2(menuRock2tex, sf::IntRect(0, 0, 3200, 900));
	sf::Sprite menuClound1(menuClound1tex, sf::IntRect(0, 0, 3200, 900));
	sf::Sprite menuClound2(menuClound2tex, sf::IntRect(0, 0, 3200, 900));
	sf::Sprite menuClound3(menuClound3tex, sf::IntRect(0, 0, 3200, 900));
	sf::Sprite menuClound4(menuClound4tex, sf::IntRect(0, 0, 3200, 900));
	menuSky.setPosition(0, -900);
	menuRock1.setPosition(0, -900);
	menuRock2.setPosition(0, -900);
	menuClound1.setPosition(0, -900);
	menuClound2.setPosition(0, -900);
	menuClound3.setPosition(0, -900);
	menuClound4.setPosition(0, -900);
	sf::Text gameNametext("KING OF MAGICIAN", zonebrownfont, 300);
	sf::Text starttext("START", zonebrownfont, 120);
	sf::Text leaderboardtext("LEADERBOARD", zonebrownfont, 120);
	sf::Text howplaytext("HOW TO PLAY", zonebrownfont, 120);
	sf::Text exittext("EXIT", zonebrownfont, 120);
	gameNametext.setOutlineThickness(7);
	starttext.setOutlineThickness(1);
	leaderboardtext.setOutlineThickness(1);
	howplaytext.setOutlineThickness(1);
	exittext.setOutlineThickness(1);
	gameNametext.setPosition(800 - gameNametext.getGlobalBounds().width / 2, -950);
	starttext.setPosition(800 - starttext.getGlobalBounds().width / 2, -600);
	howplaytext.setPosition(800 - leaderboardtext.getGlobalBounds().width / 2, -500);
	leaderboardtext.setPosition(800 - howplaytext.getGlobalBounds().width / 2, -400);
	exittext.setPosition(800 - exittext.getGlobalBounds().width / 2, -300);
	sf::Text highscoretext("LEADERBOARD", zonebrownfont, 240);
	highscoretext.setOutlineThickness(2);
	sf::Text text[12];
	for (int s = 0; s < 12; s++)
	{
		text[s].setFont(zonebrownfont);
		text[s].setCharacterSize(120);
		text[s].setOutlineThickness(1);
	}
	highscoretext.setPosition(800 - highscoretext.getGlobalBounds().width / 2, -950);
	for (int i = 0; i < 12; i += 2)
	{
		text[i].setPosition(400, -650 + i * 30);
		text[i + 1].setPosition(1000, -650 + i * 30);
	}
	std::string str[12];
	text[0].setString("NAME");
	text[1].setString("SCORE");
	text[0].setFillColor(sf::Color(211, 211, 211));
	text[1].setFillColor(sf::Color(211, 211, 211));
	text[0].setPosition(400, -680);
	text[1].setPosition(1000, -680);
	sf::Text backtext("BACK", zonebrownfont, 120);
	backtext.setOutlineThickness(1);
	backtext.setPosition(800 - backtext.getGlobalBounds().width / 2, -250);
	sf::Text EnterNametext("Enter name : ", zonebrownfont, 120);
	EnterNametext.setPosition(400, -600);
	EnterNametext.setOutlineThickness(1);
	sf::Text enter1_13C("Enter 1 - 13 characters", zonebrownfont, 120);
	enter1_13C.setOutlineThickness(1);
	enter1_13C.setPosition(800 - enter1_13C.getGlobalBounds().width / 2, -430);
	sf::Text DevNametext("63010766      POOMSAK KAEWSEE", zonebrownfont, 70);
	DevNametext.setPosition(1100, -100);
	DevNametext.setOutlineThickness(1);
	sf::RectangleShape skillshapeBg1, skillshapeBg2, skillshapeBg3;
	skillshapeBg1.setSize(sf::Vector2f(70, 70));
	skillshapeBg2.setSize(sf::Vector2f(70, 70));
	skillshapeBg3.setSize(sf::Vector2f(70, 70));
	///////////////////////////////////////////////////////  Main menu   ///////////////////////////////////////////////////////////////
	// game over //
	sf::RectangleShape gameOverShape(sf::Vector2f(1600, 900));
	gameOverShape.setPosition(0, -900);
	gameOverShape.setFillColor(sf::Color::Black);
	sf::Text gameOverText("GAME OVER", zonebrownfont, 300);
	gameOverText.setOutlineThickness(2);
	sf::Text yourScoreText("YOUR SCORE : ", zonebrownfont, 120);
	yourScoreText.setOutlineThickness(1);
	gameOverText.setPosition(800 - gameOverText.getGlobalBounds().width / 2, -950);
	yourScoreText.setPosition(800 - yourScoreText.getGlobalBounds().width / 2, -680);
	sf::Text WinText("YOU WON", zonebrownfont, 300);
	WinText.setOutlineThickness(2);
	WinText.setPosition(800 - WinText.getGlobalBounds().width / 2, -950);

	sf::Texture mousTex;
	if (!mousTex.loadFromFile("picture/CURSUR.png"))
		std::cout << "LOAD cursor failed" << std::endl;
	sf::RectangleShape curRect(sf::Vector2f(50, 50));
	curRect.setTexture(&mousTex);

	sf::Texture newBullettex;
	if (!newBullettex.loadFromFile("picture/new_bullet.png"))
		std::cout << "LOAD newBullettex failed" << std::endl;

	sf::Texture newBulletTex2;
	if (!newBulletTex2.loadFromFile("picture/22ball50x50.png"))
		std::cout << "LOAD 22ball192x192 failed" << std::endl;

	sf::Texture type2tex;
	if (!type2tex.loadFromFile("picture/Jelly.png"))
		std::cout << "LOAD Jelly failed" << std::endl;

	sf::Texture exp1;
	if (!exp1.loadFromFile("picture/explosion354x343.png"))
		std::cout << "LOAD explosion354x343 failed" << std::endl;

	sf::Texture type3tex;
	if (!type3tex.loadFromFile("picture/dragonballz.png"))
		std::cout << "LOAD dragonballz.png failed" << std::endl;

	sf::Texture b4tex;
	if (!b4tex.loadFromFile("picture/fireRL.png"))
		std::cout << "LOAD fireRL failed" << std::endl;
	
	sf::Texture type4tex;
	if (!type4tex.loadFromFile("picture/batgreen.png"))
		std::cout << "LOAD batgreen failed" << std::endl;

	sf::Texture type5tex;
	if (!type5tex.loadFromFile("picture/Duke.png"))
		std::cout << "LOAD Duke failed" << std::endl;

	// FIRE BALL 2 //
	Bullet b1(newBullettex, 1);
	Bullet b2(newBulletTex2, 2);
	Bullet b3(firetex, 3);
	Bullet b4(b4tex, 4);
	std::vector<Bullet> bulletsVec;

	//Enermy
	Enermy e1(magetex, 1);
	Enermy e2(type2tex, 2);
	Enermy e3(type3tex, 3);
	Enermy e4(type4tex, 4);
	Enermy e5(type5tex, 5);
	std::vector<Enermy> enermyVec;

	//Explosion
	Explosion ex1(exp1, 1);
	std::vector<Explosion> explosionVec;

	//setting
	sf::Vector2f playerCenter;
	sf::Vector2f mouseCam = sf::Vector2f(0, 0);
	sf::Vector2f aimDir;
	sf::Vector2f aimDirNorm;
	
	// Music
	sf::Music menuMusic, gameoverMusic, bossMusic, ingameMusic;
	if (!ingameMusic.openFromFile("Music/ingameMusic.ogg"))
		std::cout << "Load ingameMusic Failed";
	if (!menuMusic.openFromFile("Music/menuMusic.ogg"))
		std::cout << "Load menuMusic Failed";
	if (!gameoverMusic.openFromFile("Music/gameover.ogg"))
		std::cout << "Load gameoverMusic Failed";
	if (!bossMusic.openFromFile("Music/BossMusic.ogg"))
		std::cout << "Load gameoverMusic Failed";
	menuMusic.setLoop(true);
	bossMusic.setLoop(true);
	ingameMusic.setLoop(true);

	menuMusic.setVolume(15);
	ingameMusic.setVolume(15);
	bossMusic.setVolume(15);

	//sf::SoundBuffer cursorSelectBuf;
	//if (!cursorSelectBuf.loadFromFile("Music/cursorSelect.ogg"))
	//	std::cout << "Load cursorSelect Failed";
	//sf::Sound cursorSelect(cursorSelectBuf);

	sf::Texture beamtex;
	if (!beamtex.loadFromFile("picture/beam.png"))
		std::cout << "Load beam Failed";
	sf::Sprite beamSprite(beamtex);
	beamSprite.setTextureRect(sf::IntRect(0, 0, 318, 145));

	//Shield			210*187			x39
	sf::Texture Shieldtex;
	if (!Shieldtex.loadFromFile("picture/Shield210x187x39.png"))
		std::cout << "Load Shield210x187x39 Failed";
	sf::Sprite shieldSprite(Shieldtex);
	shieldSprite.setTextureRect(sf::IntRect(0, 0, 0, 0));
	shieldSprite.setColor(sf::Color(255, 255, 255, 140));

	//beamSprite.setTextureRect(sf::IntRect(0, 0, 318, 145));//0 0 318*145  12x

	///////////////////////////////////////////////////////////  ตั้งค่าอื่นๆ ///////////////////////////////////////////////////////////////
	int animationframe8 = 0, animationframe5 = 0, animationframe3 = 0, animationframe12 = 0, animationfframe6 = 0, mageAnimation8 = 0, animationframe6slow = 0, animationframe10 = 0, lizardAttack5[20] = { 0 };
	float deltatime = 0, totaltime = 0, switchtime = 0.2, totaltime2 = 0, switchtime2 = 0.1, totaltime3 = 0, switchtime3 = 0.08;
	float AnimationAtack = 0, drag = 0;
	bool wizidle = false, wizLR = false, wizattack1 = false, wizattack2 = false;
	bool startjump = false;
	bool playergetdown = false;
	bool noway = false;
	bool skill1on = false, skill2on = false;
	sf::Vector2f waterEffectPosition;
	int maxlizard = 10;
	float gravity = 0, veloY = 5, veloX = 8;
	int GameTime, score = 0, coin = 10000000;
	bool increase = false;//change time (bg nigt/noon)
	int rgb = 255;
	int scoreLizWhenDie = 150, foodscore = 100;
	int lizardVision = 300;
	int playerHp = 10, playerMaxHp = 100, playerMana = 100, playerMaxMana = 100;
	bool playerImmune = false, ImmuneAnimateIncrease = false;
	float timeToNoImmune = 0, immuneAlpha = 255;
	int skill1manacost = 0, skill2manacost = 15;//20 15
	int manaregen = 1, foodMana = 10;
	int timeAmout = 1;
	float totaltimelizdaed = 0;
	int animationframelizdaed = 0;
	float debouceInput = 0;
	int menu = 0;
	float debouceLclick = 0;
	float debouceRclick = 0;
	int lizDeathAmount = 0;
	int wave = 0, chapter = 0;
	int mageDeathAmount = 0;
	int jellyDeathAmount = 0;
	int dragonDeathAmount = 0;
	bool win = false;
	bool beamOn = false;
	int animationBeam12 = 0;
	float animationBeam12tt = 0;
	bool shieldOn = false;
	int animationShield39 = 0;
	float animationShield39tt = 0;
	int skillSelect = 1;
	sf::Vector2f base;
	sf::Vector2f target;
	sf::Vector2f aimDirLizard;
	sf::Vector2f aimDirNormLizard;
	sf::String inputString;
	sf::Clock clock, clock2, clock3;
	///////////////////////////////////////////////////////////  ตั้งค่าอื่นๆ ///////////////////////////////////////////////////////////////
	//cam.zoom(4);
	menuMusic.play();
	while (window.isOpen())
	{
		//std::cout << lizDeathAmount << "  " << mageDeathAmount << "\n";
		//std::cout << clock.getElapsedTime().asSeconds() << " " << clock2.getElapsedTime().asSeconds() << " " << clock3.getElapsedTime().asSeconds() << "\n";
		//std::cout << mouseCam.x << "   " << mouseCam.y << "\n";
		sf::Event event;
		//cam update
		if (menu == 1)
		{
			if (playerrec.getPosition().x + playerrecsizex / 2 < 800)
			{
				cam.setCenter(800, 1450);
			}
			else if (playerrec.getPosition().x + playerrecsizex / 2 > 3840 - 800)
			{
				cam.setCenter(3040, 1450);
			}
			else
			{
				cam.setCenter(playerrec.getPosition().x + playerrecsizex / 2, 1450);
			}
		}

		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
			if (event.type == sf::Event::MouseWheelMoved)
				std::cout << event.mouseWheel.delta << "\n";
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
			window.close();
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) && !wizattack1 && !wizattack2 && menu == 1)
		{
			if (playerrec.getPosition().x + playerrecsizex < 3840 - 68)
				playerrec.move(veloX, 0);
			if (!startjump)
				wizsprite.setTextureRect(sf::IntRect(wizsizex * animationframe8, wizesizey * 7, wizsizex, wizesizey));
			wizidle = false;
			wizLR = 1;
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) && !wizattack1 && !wizattack2 && menu == 1)
		{
			if (playerrec.getPosition().x > 0)
				playerrec.move(-veloX, 0);
			if (!startjump)
				wizsprite.setTextureRect(sf::IntRect(wizsizex * animationframe8, wizesizey * 15, wizsizex, wizesizey));
			wizidle = false;
			wizLR = 0;
		}
		else if (!startjump && !wizattack1 && !wizattack2)
		{
			wizidle = true;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && !startjump && !wizattack1 && !wizattack2 && menu == 1)
		{
			startjump = true;
			noway = true;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::K) && menu == 1)
		{
			if (!wizattack1 && !wizattack2 && !startjump && playerMana - skill1manacost >= 0)
			{
				playerMana -= skill1manacost;
				wizattack1 = true;
				AnimationAtack = 0;
				animationframe12 = 0;
				skill1on = true;
				int dsfdsf = playerrec.getPosition().y;
				switch (dsfdsf)//เซ็ทตำแหน่งน้ำระเบิด
				{
				case 1710://rec1
					if (wizLR)
						waterEffectPosition = sf::Vector2f(playerrec.getPosition().x + 68, playerrec.getPosition().y + 20);
					else
						waterEffectPosition = sf::Vector2f(playerrec.getPosition().x - watertex.getSize().x / 12, playerrec.getPosition().y + 20);
					break;
				case 1550 - 90://rec2
					if (wizLR)
					{
						if (playerrec.getPosition().x + 68 > 390)
							waterEffectPosition = sf::Vector2f(playerrec.getPosition().x + 68, 1710 + 20);
						else
							waterEffectPosition = sf::Vector2f(playerrec.getPosition().x + 68, playerrec.getPosition().y + 20);
					}
					else
						waterEffectPosition = sf::Vector2f(playerrec.getPosition().x - watertex.getSize().x / 12, playerrec.getPosition().y + 20);
					break;
				case 1600 - 90://rec3
					if (wizLR)
					{
						if (playerrec.getPosition().x + 68 > 1493)
							waterEffectPosition = sf::Vector2f(playerrec.getPosition().x + 68, 1710 + 20);
						else
							waterEffectPosition = sf::Vector2f(playerrec.getPosition().x + 68, playerrec.getPosition().y + 20);
					}
					else
					{
						if (playerrec.getPosition().x - watertex.getSize().x / 12 < 785)
							waterEffectPosition = sf::Vector2f(playerrec.getPosition().x - watertex.getSize().x / 12, 1710 + 20);
						else
							waterEffectPosition = sf::Vector2f(playerrec.getPosition().x - watertex.getSize().x / 12, playerrec.getPosition().y + 20);
					}
					break;
				case 1535 - 90://rec4
					if (wizLR)
					{
						if (playerrec.getPosition().x + 68 > 2092)
							waterEffectPosition = sf::Vector2f(playerrec.getPosition().x + 68, 1710 + 20);
						else
							waterEffectPosition = sf::Vector2f(playerrec.getPosition().x + 68, playerrec.getPosition().y + 20);
					}
					else
					{
						if (playerrec.getPosition().x - watertex.getSize().x / 12 < 1779)
							waterEffectPosition = sf::Vector2f(playerrec.getPosition().x - watertex.getSize().x / 12, 1710 + 20);
						else
							waterEffectPosition = sf::Vector2f(playerrec.getPosition().x - watertex.getSize().x / 12, playerrec.getPosition().y + 20);
					}
					break;
				case 1610 - 90://rec5
					if (wizLR)
					{
						if (playerrec.getPosition().x + 68 > 2845)
							waterEffectPosition = sf::Vector2f(playerrec.getPosition().x + 68, 1710 + 20);
						else
							waterEffectPosition = sf::Vector2f(playerrec.getPosition().x + 68, playerrec.getPosition().y + 20);
					}
					else
					{
						if (playerrec.getPosition().x - watertex.getSize().x / 12 < 2385)
							waterEffectPosition = sf::Vector2f(playerrec.getPosition().x - watertex.getSize().x / 12, 1710 + 20);
						else
							waterEffectPosition = sf::Vector2f(playerrec.getPosition().x - watertex.getSize().x / 12, playerrec.getPosition().y + 20);
					}
					break;
				case 1300 - 90://rec6
					if (wizLR)
					{
						if (playerrec.getPosition().x + 68 > 1037)
							waterEffectPosition = sf::Vector2f(playerrec.getPosition().x + 68, 1600 - 90 + 20);
						else
							waterEffectPosition = sf::Vector2f(playerrec.getPosition().x + 68, playerrec.getPosition().y + 20);
					}
					else
					{
						if (playerrec.getPosition().x - watertex.getSize().x / 12 < 338)
							waterEffectPosition = sf::Vector2f(playerrec.getPosition().x - watertex.getSize().x / 12, 1550 - 90 + 20);
						else
							waterEffectPosition = sf::Vector2f(playerrec.getPosition().x - watertex.getSize().x / 12, playerrec.getPosition().y + 20);
					}
					break;
				case 1320 - 90://rec7
					if (wizLR)
					{
						if (playerrec.getPosition().x + 68 > 2397)
							waterEffectPosition = sf::Vector2f(playerrec.getPosition().x + 68, 1610 - 90 + 20);
						else
							waterEffectPosition = sf::Vector2f(playerrec.getPosition().x + 68, playerrec.getPosition().y + 20);
					}
					else
					{
						if (playerrec.getPosition().x - watertex.getSize().x / 12 < 1385)
							waterEffectPosition = sf::Vector2f(playerrec.getPosition().x - watertex.getSize().x / 12, 1600 - 90 + 20);
						else
							waterEffectPosition = sf::Vector2f(playerrec.getPosition().x - watertex.getSize().x / 12, playerrec.getPosition().y + 20);
					}
					break;
				case 1350 - 90://rec8
					if (wizLR)
					{
						if (playerrec.getPosition().x + 68 > 3493)
							waterEffectPosition = sf::Vector2f(playerrec.getPosition().x + 68, 1710 + 20);
						else
							waterEffectPosition = sf::Vector2f(playerrec.getPosition().x + 68, playerrec.getPosition().y + 20);
					}
					else
					{
						if (playerrec.getPosition().x - watertex.getSize().x / 12 < 2777)
							waterEffectPosition = sf::Vector2f(playerrec.getPosition().x - watertex.getSize().x / 12, 1610 - 90 + 20);
						else
							waterEffectPosition = sf::Vector2f(playerrec.getPosition().x - watertex.getSize().x / 12, playerrec.getPosition().y + 20);
					}
					break;

				}

				
				//waterreccheck.setPosition(sf::Vector2f(waterEffectPosition));
				waterEffectPosition = sf::Vector2f(-300, 0);
				/*

				rec4	R > 2092
						L < 1779
				rec5	R > 2845
						L < 2385
				rec6	R > 1037 re3
						L < 338  re2
				rec7	R > 2397 re4
						L < 1385 re2
				rec8	R > 3493 re1
						L < 2777 re5
				*/



				/*
				if (wizLR)
					waterEffectPosition = sf::Vector2f(playerrec.getPosition().x + 68, playerrec.getPosition().y + 20);
				else
					waterEffectPosition = sf::Vector2f(playerrec.getPosition().x - watertex.getSize().x/12, playerrec.getPosition().y + 20);Normal waterfall*/
			}

		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::L) && menu == 1)
		{
			if (!wizattack1 && !wizattack2 && !startjump && playerMana - skill2manacost >= 0)
			{
				playerMana -= skill2manacost;
				wizattack2 = true;
				AnimationAtack = 0;
				for (int re = 0; re < 10; re++)
				{
					if (!firebool[re])
					{
						if (wizLR)
						{
							firevector[re] = sf::Vector2f(playerrec.getPosition().x + 30, playerrec.getPosition().y + 10);
						}
						else
						{
							firevector[re] = sf::Vector2f(playerrec.getPosition().x - 70, playerrec.getPosition().y + 10);
						}
						fireLR[re] = wizLR;
						firebool[re] = true;
						firsttime[re] = true;
						break;
					}
				}
			}
		}

		//Main menu
		if (menu == 0)
		{
			clock2.restart().asSeconds();
			cam.setCenter(sf::Vector2f(800, -450));
			MenuBgMove(menuSky, -100);//เพื่อ?
			MenuBgMove(menuRock1, -50);
			MenuBgMove(menuRock2, -100);
			MenuBgMove(menuClound1, -110);
			MenuBgMove(menuClound2, -130);
			MenuBgMove(menuClound3, -150);
			MenuBgMove(menuClound4, -170);
			sf::Vector2f mouseMenu;
			mouseMenu = sf::Vector2f(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y - 900);
			if (starttext.getGlobalBounds().contains(mouseMenu))
			{
				starttext.setFillColor(sf::Color::Red);
				if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
				{
					menu = 10;
					inputString.clear();
				}
			}
			else if (howplaytext.getGlobalBounds().contains(mouseMenu))
			{
				howplaytext.setFillColor(sf::Color::Red);
				if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
				{
					//add
				}
			}
			else if (leaderboardtext.getGlobalBounds().contains(mouseMenu))
			{
				leaderboardtext.setFillColor(sf::Color::Red);
				if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
				{
					menu = 3;
					std::string word;
					std::ifstream ReadFile("Score/scoreFile.txt");
					do {
						ReadFile >> word;
						std::string name = word.substr(0, word.find(','));
						int num = std::stoi(word.substr(word.find(',') + 1, word.length()));
						vec.push_back(std::make_pair(name, num));
					} while (ReadFile.good());
					ReadFile.close();
					std::sort(vec.begin(), vec.end(), sortbysecdesc);
					for (int i = 0; i < 5; i++)
					{
						text[2 * i + 2].setString(std::to_string(i + 1) + ". " + vec[i].first);
						text[2 * i + 3].setString(std::to_string(vec[i].second));
					}
					vec.clear();
				}
			}
			else if (exittext.getGlobalBounds().contains(mouseMenu))
			{
				exittext.setFillColor(sf::Color::Red);
				if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
					window.close();
			}
			else
			{
				starttext.setFillColor(sf::Color::White);
				howplaytext.setFillColor(sf::Color::White);
				leaderboardtext.setFillColor(sf::Color::White);
				exittext.setFillColor(sf::Color::White);
			}
		}
		else if (menu == 10)
		{
			clock2.restart().asSeconds();
			cam.setCenter(sf::Vector2f(800, -450));
			MenuBgMove(menuSky, -100);//เพื่อ?
			MenuBgMove(menuRock1, -50);
			MenuBgMove(menuRock2, -100);
			MenuBgMove(menuClound1, -110);
			MenuBgMove(menuClound2, -130);
			MenuBgMove(menuClound3, -150);
			MenuBgMove(menuClound4, -170);
			sf::Vector2f mouseMenu;
			mouseMenu = sf::Vector2f(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y - 900);
			if (backtext.getGlobalBounds().contains(mouseMenu))
			{
				backtext.setFillColor(sf::Color::Red);
				if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
				{
					menu = 0;
					inputString.clear();
				}
			}
			else
			{
				backtext.setFillColor(sf::Color::White);
			}
			if (event.type == event.TextEntered && debouceInput < clock3.getElapsedTime().asSeconds())
			{
				debouceInput = clock3.getElapsedTime().asSeconds() + 0.2;
				if (event.text.unicode >= 33 && event.text.unicode <= 126 && inputString.getSize() <= 13 && event.text.unicode != 44)
				{
					inputString += event.text.unicode;
				}
				else if (event.text.unicode == 8)//backspace
				{
					inputString = inputString.substring(0, inputString.getSize() - 1);
				}
				else if (event.text.unicode == 13 && inputString.getSize() > 0)//enter
				{
					menu = 1;
					score = 0;
					playerHp = 100;
					playerrec.setPosition(0, 1800 - 90);
					rgb = 255;
					NameInGame.setString(inputString);
					debouceLclick = 0;
					debouceRclick = 0;
					enermyVec.clear();
					bulletsVec.clear();
					menuMusic.stop();
					ingameMusic.play();
					for (int s = 0; s < 7; s++)//สุ่ม mage
					{
						e1.sprite.setPosition(sf::Vector2f(rand() % 3840, 800));
						e1.currentVelocity = sf::Vector2f(5 + rand() % 6, 5 + rand() % 6);
						e1.sprite.setTextureRect(sf::IntRect(0, 0, 122, 110));
						e1.sprite.setScale(sf::Vector2f(1.5, 1.5));
						e1.Redbg.setPosition(e1.sprite.getPosition().x + 41.5, e1.sprite.getPosition().y);
						e1.HpGreen.setPosition(e1.sprite.getPosition().x + 41.5, e1.sprite.getPosition().y);
						e1.TimeAtk = 4 + (rand() % 40) / 10.0;
						enermyVec.push_back(Enermy(e1));
					}
				}
			}
			EnterNametext.setString("Enter name : " + inputString);
		}
		else if (menu == 3)
		{
			clock2.restart().asSeconds();
			cam.setCenter(sf::Vector2f(800, -450));
			MenuBgMove(menuSky, -100);//เพื่อ?
			MenuBgMove(menuRock1, -50);
			MenuBgMove(menuRock2, -100);
			MenuBgMove(menuClound1, -110);
			MenuBgMove(menuClound2, -130);
			MenuBgMove(menuClound3, -150);
			MenuBgMove(menuClound4, -170);
			sf::Vector2f mouseMenu;
			mouseMenu = sf::Vector2f(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y - 900);
			if (backtext.getGlobalBounds().contains(mouseMenu))
			{
				backtext.setFillColor(sf::Color::Red);
				if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
				{
					menu = 0;
				}
			}
			else
			{
				backtext.setFillColor(sf::Color::White);
			}
		}
		else if (menu == 99)
		{
			clock2.restart().asSeconds();
			cam.setCenter(sf::Vector2f(800, -450));
			sf::Vector2f mouseMenu;
			mouseMenu = sf::Vector2f(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y - 900);
			yourScoreText.setString("YOUR SCORE : " + std::to_string(score));
			yourScoreText.setPosition(800 - yourScoreText.getGlobalBounds().width / 2, -680);
			if (backtext.getGlobalBounds().contains(mouseMenu))
			{
				backtext.setFillColor(sf::Color::Red);
				if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
				{
					menu = 0;
					menuMusic.play();
					//gameOverMusic.stop();
				}
			}
			else
			{
				backtext.setFillColor(sf::Color::White);
			}
		}

		//rand() enermy
		switch (wave)
		{
		case 1:
			for (int s = 0; s < 7; s++)//สุ่ม mage
			{
				e1.sprite.setPosition(sf::Vector2f(rand() % 3840, 800));
				e1.currentVelocity = sf::Vector2f(5 + rand() % 6, 5 + rand() % 6);
				e1.sprite.setTextureRect(sf::IntRect(0, 0, 122, 110));
				e1.sprite.setScale(sf::Vector2f(1.5, 1.5));
				e1.Redbg.setPosition(e1.sprite.getPosition().x + 41.5, e1.sprite.getPosition().y);
				e1.HpGreen.setPosition(e1.sprite.getPosition().x + 41.5, e1.sprite.getPosition().y);
				e1.TimeAtk = 4 + (rand() % 40) / 10.0;
				enermyVec.push_back(Enermy(e1));
			}
			wave = 0;
			break;
		case 2:
			for (int s = 0; s < 7; s++)//สุ่ม mage
			{
				e1.sprite.setPosition(sf::Vector2f(rand() % 3840, 800));
				e1.currentVelocity = sf::Vector2f(5 + rand() % 6, 5 + rand() % 6);
				e1.sprite.setTextureRect(sf::IntRect(0, 0, 122, 110));
				e1.sprite.setScale(sf::Vector2f(1.5, 1.5));
				e1.Redbg.setPosition(e1.sprite.getPosition().x + 41.5, e1.sprite.getPosition().y);
				e1.HpGreen.setPosition(e1.sprite.getPosition().x + 41.5, e1.sprite.getPosition().y);
				e1.TimeAtk = 4 + (rand() % 40) / 10.0;
				enermyVec.push_back(Enermy(e1));
			}
			for (int d = 0; d < maxlizard; d++)
			{
				randomlizard(lizardrec[d], d);
				islizardDie[d] = true;
			}
			wave = 0;
			break;
		case 3:
			for (int s = 0; s < 20; s++)//สุ่ม mage
			{
				e1.sprite.setPosition(sf::Vector2f(rand() % 3840, 800));
				e1.currentVelocity = sf::Vector2f(5 + rand() % 6, 5 + rand() % 6);
				e1.sprite.setTextureRect(sf::IntRect(0, 0, 122, 110));
				e1.sprite.setScale(sf::Vector2f(1.5, 1.5));
				e1.Redbg.setPosition(e1.sprite.getPosition().x + 41.5, e1.sprite.getPosition().y);
				e1.HpGreen.setPosition(e1.sprite.getPosition().x + 41.5, e1.sprite.getPosition().y);
				e1.TimeAtk = 4 + (rand() % 40) / 10.0;
				enermyVec.push_back(Enermy(e1));
			}
			wave = 0;
			break;
		case 4:
			for (int s = 0; s < 4; s++)//สุ่ม Jelly
			{
				e2.sprite.setPosition(sf::Vector2f(rand() % 3840, 800));
				e2.currentVelocity = sf::Vector2f(3 + rand() % 6, 3 + rand() % 6);
				e2.sprite.setTextureRect(sf::IntRect(0, 0, 290, 274));
				e2.Redbg.setPosition(e2.sprite.getPosition().x + 70, e2.sprite.getPosition().y);
				e2.HpGreen.setPosition(e2.sprite.getPosition().x + 70, e2.sprite.getPosition().y);
				e2.TimeAtk = 3 + (rand() % 20) / 10.0;
				enermyVec.push_back(Enermy(e2));
				//สุ่ม Mage
				e1.sprite.setPosition(sf::Vector2f(rand() % 3840, 800));
				e1.currentVelocity = sf::Vector2f(5 + rand() % 6, 5 + rand() % 6);
				e1.sprite.setTextureRect(sf::IntRect(0, 0, 122, 110));
				e1.sprite.setScale(sf::Vector2f(1.5, 1.5));
				e1.Redbg.setPosition(e1.sprite.getPosition().x + 41.5, e1.sprite.getPosition().y);
				e1.HpGreen.setPosition(e1.sprite.getPosition().x + 41.5, e1.sprite.getPosition().y);
				e1.TimeAtk = 4 + (rand() % 40) / 10.0;
				enermyVec.push_back(Enermy(e1));
			}
			wave = 0;
			break;
		case 5:
			for (int i = 0; i < 8; i++)//สุ่ม Dragon
			{
				if (rand() % 2 == 0)
					e3.sprite.setPosition(-100, 1000 + rand() % 200);
				else
					e3.sprite.setPosition(3900, 1000 + rand() % 200);
				e3.currentVelocity = sf::Vector2f(4 + rand() % 5, 0);
				e3.sprite.setTextureRect(sf::IntRect(0, 0, 155, 170));
				e3.Redbg.setPosition(e3.sprite.getPosition().x + 35, e3.sprite.getPosition().y);
				e3.HpGreen.setPosition(e3.sprite.getPosition().x + 35, e3.sprite.getPosition().y);
				e3.TimeAtk = 3 + (rand() % 15) / 10.0;
				enermyVec.push_back(Enermy(e3));
			}
			for (int i = 0; i < 3; i++)//สุ่ม Jelly
			{
				e2.sprite.setPosition(sf::Vector2f(rand() % 3840, 800));
				e2.currentVelocity = sf::Vector2f(3 + rand() % 6, 3 + rand() % 6);
				e2.sprite.setTextureRect(sf::IntRect(0, 0, 290, 274));
				e2.Redbg.setPosition(e2.sprite.getPosition().x + 70, e2.sprite.getPosition().y);
				e2.HpGreen.setPosition(e2.sprite.getPosition().x + 70, e2.sprite.getPosition().y);
				e2.TimeAtk = 3 + (rand() % 20) / 10.0;
				enermyVec.push_back(Enermy(e2));
			}
			wave = 0;
			break;
		}
		//chapter
		if (chapter == 0)
		{
			if (mageDeathAmount == 0 && lizDeathAmount == 0)
			{
				wave = 2;
				chapter = 2;
			}
		}
		else if (chapter == 1)//skipped
		{
			//if (mageDeathAmount == 7 && lizDeathAmount == 10)
			//{
			//	chapter = 2;
			//	wave = 2;
			//}
		}
		else if (chapter == 2)
		{
			if (mageDeathAmount == 14 && lizDeathAmount == 10)
			{
				chapter = 3;
				wave = 3;
			}
		}
		else if (chapter == 3)
		{
			if (mageDeathAmount == 34 && lizDeathAmount == 10)
			{
				wave = 4;
				chapter = 4;
			}
		}
		else if (chapter == 4)
		{
			if (jellyDeathAmount == 4 && mageDeathAmount == 38)
			{
				wave = 5;
				chapter = 5;
			}
		}

		//Jump
		if (startjump)
		{
			if (playergetdown)
			{
				gravity = 25;
				playergetdown = false;
			}
			gravity += 1;
			if (gravity > 50)
				gravity = 50;
			veloY = -25 + gravity;
			playerrec.move(0, veloY);
			if (veloY >= 0 && playerrec.getGlobalBounds().intersects(groundsprite.getGlobalBounds()))
			{
				playerrec.setPosition(playerrec.getPosition().x, 1710);
				startjump = false;
				gravity = 0;
			}//groundsprite
			if (veloY >= 0 && playerfootrec.getGlobalBounds().intersects(rec2.getGlobalBounds()))
			{
				playerrec.setPosition(playerrec.getPosition().x, 1550 - 90);
				startjump = false;
				gravity = 0;
				noway = false;
			}//rec2
			if (veloY >= 0 && playerfootrec.getGlobalBounds().intersects(rec3.getGlobalBounds()))
			{
				playerrec.setPosition(playerrec.getPosition().x, 1600 - 90);
				startjump = false;
				gravity = 0;
				noway = false;
			}//rec3
			if (veloY >= 0 && playerfootrec.getGlobalBounds().intersects(rec4.getGlobalBounds()))
			{
				playerrec.setPosition(playerrec.getPosition().x, 1535 - 90);
				startjump = false;
				gravity = 0;
				noway = false;
			}//rec4
			if (veloY >= 0 && playerfootrec.getGlobalBounds().intersects(rec5.getGlobalBounds()))
			{
				playerrec.setPosition(playerrec.getPosition().x, 1610 - 90);
				startjump = false;
				gravity = 0;
				noway = false;
			}//rec5
			if (veloY >= 0 && playerfootrec.getGlobalBounds().intersects(rec6.getGlobalBounds()))
			{
				playerrec.setPosition(playerrec.getPosition().x, 1300 - 90);
				startjump = false;
				gravity = 0;
				noway = false;
			}//rec6
			if (veloY >= 0 && playerfootrec.getGlobalBounds().intersects(rec7.getGlobalBounds()))
			{
				playerrec.setPosition(playerrec.getPosition().x, 1320 - 90);
				startjump = false;
				gravity = 0;
				noway = false;
			}//rec7
			if (veloY >= 0 && playerfootrec.getGlobalBounds().intersects(rec8.getGlobalBounds()))
			{
				playerrec.setPosition(playerrec.getPosition().x, 1350 - 90);
				startjump = false;
				gravity = 0;
				noway = false;
			}//rec8
			//^^^^^^^^^^^^^^^^^^^^^^   Add RectangleShape here    ^^^^^^^^^^^^^
		}
		//getdown
		if (!playergetdown && !noway && playerrec.getPosition().y == 1550 - 90 && playerrec.getPosition().x + 24 > 500 + 15)
		{
			playergetdown = true;
			startjump = true;
		}//rec2
		if (!playergetdown && !noway && playerrec.getPosition().y == 1600 - 90 && (playerrec.getPosition().x + 24 > 900 + 700 + 15 || playerrec.getPosition().x + 24 < 900 - 15))
		{
			playergetdown = true;
			startjump = true;
		}//rec3
		if (!playergetdown && !noway && playerrec.getPosition().y == 1535 - 90 && (playerrec.getPosition().x + 24 < 1900 - 15 || playerrec.getPosition().x + 24 > 1900 + 300 + 15))
		{
			playergetdown = true;
			startjump = true;
		}//rec4
		if (!playergetdown && !noway && playerrec.getPosition().y == 1610 - 90 && (playerrec.getPosition().x + 24 < 2500 - 15 || playerrec.getPosition().x + 24 > 2500 + 450 + 15))
		{
			playergetdown = true;
			startjump = true;
		}//rec5
		if (!playergetdown && !noway && playerrec.getPosition().y == 1300 - 90 && (playerrec.getPosition().x + 24 < 450 - 15 || playerrec.getPosition().x + 24 > 450 + 700 + 15))
		{
			playergetdown = true;
			startjump = true;
		}//rec6
		if (!playergetdown && !noway && playerrec.getPosition().y == 1320 - 90 && (playerrec.getPosition().x + 24 < 1500 - 15 || playerrec.getPosition().x + 24 > 1500 + 1000 + 15))
		{
			playergetdown = true;
			startjump = true;
		}//rec7
		if (!playergetdown && !noway && playerrec.getPosition().y == 1350 - 90 && (playerrec.getPosition().x + 24 < 2900 - 15 || playerrec.getPosition().x + 24 > 2900 + 700 + 15))
		{
			playergetdown = true;
			startjump = true;
		}//rec8
		//^^^^^^^^^^^^^^^^^^^^^^   Add RectangleShape here    ^^^^^^^^^^^^^

		//animation
		if (wizidle && !startjump)//Idle
		{
			if (wizLR)
			{
				wizsprite.setTextureRect(sf::IntRect(wizsizex * animationframe5, wizesizey * 5, wizsizex, wizesizey));
			}
			else
			{
				wizsprite.setTextureRect(sf::IntRect(wizsizex * animationframe5, wizesizey * 13, wizsizex, wizesizey));
			}
		}
		if (startjump)//jump and down
		{
			if (veloY < 0 && !wizLR)
				wizsprite.setTextureRect(sf::IntRect(0, wizesizey * 14, wizsizex, wizesizey));
			else if (veloY >= 0 && !wizLR)
				wizsprite.setTextureRect(sf::IntRect(0, wizesizey * 11, wizsizex, wizesizey));
			else if (veloY < 0 && wizLR)
				wizsprite.setTextureRect(sf::IntRect(0, wizesizey * 6, wizsizex, wizesizey));
			else if (veloY >= 0 && wizLR)
				wizsprite.setTextureRect(sf::IntRect(0, wizesizey * 3, wizsizex, wizesizey));
		}
		if (wizattack1 && skill1on)
		{
			if (AnimationAtack == 7)
				wizattack1 = false;
			if (wizLR)
			{
				wizsprite.setTextureRect(sf::IntRect(wizsizex * AnimationAtack, wizesizey * 0, wizsizex, wizesizey));
			}
			else
			{
				wizsprite.setTextureRect(sf::IntRect(wizsizex * AnimationAtack, wizesizey * 8, wizsizex, wizesizey));
			}
		}
		if (wizattack2)
		{
			if (AnimationAtack == 7)
				wizattack2 = false;
			if (wizLR)
			{
				wizsprite.setTextureRect(sf::IntRect(wizsizex * AnimationAtack, wizesizey * 1, wizsizex, wizesizey));
			}
			else
			{
				wizsprite.setTextureRect(sf::IntRect(wizsizex * AnimationAtack, wizesizey * 9, wizsizex, wizesizey));
			}
		}
		if (skill1on)
		{
			watersprite.setPosition(waterEffectPosition);
			watersprite.setTextureRect(sf::IntRect((watertex.getSize().x / 12) * animationframe12, 0, watertex.getSize().x / 12, 91));
			if (animationframe12 == 11)
			{
				skill1on = false;
				watersprite.setPosition(0, 0);
			}
		}
		if (playerImmune)
		{
			wizsprite.setColor(sf::Color(255, 255, 255, immuneAlpha));
			if (timeToNoImmune <= clock2.getElapsedTime().asSeconds())
			{
				playerImmune = false;
				wizsprite.setColor(sf::Color(255, 255, 255, 255));
			}
			if (ImmuneAnimateIncrease)
			{
				immuneAlpha += 20;
				if (immuneAlpha >= 235)
					ImmuneAnimateIncrease = false;
			}
			else
			{
				immuneAlpha -= 20;
				if (immuneAlpha <= 30)
					ImmuneAnimateIncrease = true;
			}
		}
		for (int i = 0; i < maxlizard; i++)//Lizard activity
		{
			//Damaged check
			for (size_t p = 0; p < bulletsVec.size(); p++)
			{
				if (bulletsVec[p].Type == 2)
				{
					if (bulletsVec[p].sprite.getGlobalBounds().intersects(lizardrec[i].getGlobalBounds()))
					{
						bulletsVec.erase(bulletsVec.begin() + p);
						lizardhp[i] -= 20;//15
						break;
					}
				}
			}


			//สุ่มตำแหน่งด้านบน
			if (islizardDie[i])
			{
				//randomlizard(lizardrec[i], i);
				//islizardDie[i] = false;
			}

			//status
			switch (lizardstatus[i])
			{
			case idle:
				if (lizardLR[i])//R idle
				{
					lizardsprite[i].setTextureRect(sf::IntRect(256 * animationframe3, 0, 256, 256));
				}
				else//L idle
				{
					lizardsprite[i].setTextureRect(sf::IntRect(256 * animationframe3, 256 * 5, 256, 256));
				}
				break;
			case walk:
				switch (lizardWherePlatAreU[i])
				{
				case 1://on Ground (rec1)
					if (lizardLR[i])//R walk
					{
						if (lizardrec[i].getPosition().x > 3800)
						{
							lizardLR[i] = false;
						}
						else
						{
							lizardrec[i].move(lizardVeloX[i], 0);
						}
						lizardsprite[i].setTextureRect(sf::IntRect(256 * animationframe6slow, 256 * 1, 256, 256));
					}
					else//L walk
					{
						if (lizardrec[i].getPosition().x < 0 + 10)
						{
							lizardLR[i] = true;
						}
						else
						{
							lizardrec[i].move(-lizardVeloX[i], 0);
						}
						lizardsprite[i].setTextureRect(sf::IntRect(256 * animationframe6slow, 256 * 6, 256, 256));
					}
					break;
				case 2:
					if (lizardLR[i])//R walk
					{
						if (lizardrec[i].getPosition().x > 500 - 68)
						{
							lizardLR[i] = false;
						}
						else
						{
							lizardrec[i].move(lizardVeloX[i], 0);
						}
						lizardsprite[i].setTextureRect(sf::IntRect(256 * animationframe6slow, 256 * 1, 256, 256));
					}
					else//L walk
					{
						if (lizardrec[i].getPosition().x < 0 + 10)
						{
							lizardLR[i] = true;
						}
						else
						{
							lizardrec[i].move(-lizardVeloX[i], 0);
						}
						lizardsprite[i].setTextureRect(sf::IntRect(256 * animationframe6slow, 256 * 6, 256, 256));
					}
					break;
				case 3:
					if (lizardLR[i])//R walk
					{
						if (lizardrec[i].getPosition().x > 900 + 700 - 68)
						{
							lizardLR[i] = false;
						}
						else
						{
							lizardrec[i].move(lizardVeloX[i], 0);
						}
						lizardsprite[i].setTextureRect(sf::IntRect(256 * animationframe6slow, 256 * 1, 256, 256));
					}
					else//L walk
					{
						if (lizardrec[i].getPosition().x < 900 + 10)
						{
							lizardLR[i] = true;
						}
						else
						{
							lizardrec[i].move(-lizardVeloX[i], 0);
						}
						lizardsprite[i].setTextureRect(sf::IntRect(256 * animationframe6slow, 256 * 6, 256, 256));
					}
					break;
				case 4:
					if (lizardLR[i])//R walk
					{
						if (lizardrec[i].getPosition().x > 1900 + 300 - 68)
						{
							lizardLR[i] = false;
						}
						else
						{
							lizardrec[i].move(lizardVeloX[i], 0);
						}
						lizardsprite[i].setTextureRect(sf::IntRect(256 * animationframe6slow, 256 * 1, 256, 256));
					}
					else//L walk
					{
						if (lizardrec[i].getPosition().x < 1900 + 10)
						{
							lizardLR[i] = true;
						}
						else
						{
							lizardrec[i].move(-lizardVeloX[i], 0);
						}
						lizardsprite[i].setTextureRect(sf::IntRect(256 * animationframe6slow, 256 * 6, 256, 256));
					}
					break;
				case 5:
					if (lizardLR[i])//R walk
					{
						if (lizardrec[i].getPosition().x > 2500 + 450 - 68)
						{
							lizardLR[i] = false;
						}
						else
						{
							lizardrec[i].move(lizardVeloX[i], 0);
						}
						lizardsprite[i].setTextureRect(sf::IntRect(256 * animationframe6slow, 256 * 1, 256, 256));
					}
					else//L walk
					{
						if (lizardrec[i].getPosition().x < 2500 + 10)
						{
							lizardLR[i] = true;
						}
						else
						{
							lizardrec[i].move(-lizardVeloX[i], 0);
						}
						lizardsprite[i].setTextureRect(sf::IntRect(256 * animationframe6slow, 256 * 6, 256, 256));
					}
					break;
				case 6:
					if (lizardLR[i])//R walk
					{
						if (lizardrec[i].getPosition().x > 450 + 700 - 68)
						{
							lizardLR[i] = false;
						}
						else
						{
							lizardrec[i].move(lizardVeloX[i], 0);
						}
						lizardsprite[i].setTextureRect(sf::IntRect(256 * animationframe6slow, 256 * 1, 256, 256));
					}
					else//L walk
					{
						if (lizardrec[i].getPosition().x < 450 + 10)
						{
							lizardLR[i] = true;
						}
						else
						{
							lizardrec[i].move(-lizardVeloX[i], 0);
						}
						lizardsprite[i].setTextureRect(sf::IntRect(256 * animationframe6slow, 256 * 6, 256, 256));
					}
					break;
				case 7:
					if (lizardLR[i])//R walk
					{
						if (lizardrec[i].getPosition().x > 1500 + 1000 - 68)
						{
							lizardLR[i] = false;
						}
						else
						{
							lizardrec[i].move(lizardVeloX[i], 0);
						}
						lizardsprite[i].setTextureRect(sf::IntRect(256 * animationframe6slow, 256 * 1, 256, 256));
					}
					else//L walk
					{
						if (lizardrec[i].getPosition().x < 1500 + 10)
						{
							lizardLR[i] = true;
						}
						else
						{
							lizardrec[i].move(-lizardVeloX[i], 0);
						}
						lizardsprite[i].setTextureRect(sf::IntRect(256 * animationframe6slow, 256 * 6, 256, 256));
					}
					break;
				case 8:
					if (lizardLR[i])//R walk
					{
						if (lizardrec[i].getPosition().x > 2900 + 700 - 68)
						{
							lizardLR[i] = false;
						}
						else
						{
							lizardrec[i].move(lizardVeloX[i], 0);
						}
						lizardsprite[i].setTextureRect(sf::IntRect(256 * animationframe6slow, 256 * 1, 256, 256));
					}
					else//L walk
					{
						if (lizardrec[i].getPosition().x < 2900 + 10)
						{
							lizardLR[i] = true;
						}
						else
						{
							lizardrec[i].move(-lizardVeloX[i], 0);
						}
						lizardsprite[i].setTextureRect(sf::IntRect(256 * animationframe6slow, 256 * 6, 256, 256));
					}
					break;
				}
				break;
			case hurt:
				break;
			case attack:
				if (lizardLR[i])//R
				{
					lizardsprite[i].setTextureRect(sf::IntRect(256 * lizardAttack5[i], 256 * 3, 256, 256));
				}
				else//L
				{
					lizardsprite[i].setTextureRect(sf::IntRect(256 * lizardAttack5[i], 256 * 8, 256, 256));
				}
				break;
			case death:
				if (lizardLR[i])//R
				{
					lizardsprite[i].setTextureRect(sf::IntRect(256 * animationframelizdaed, 256 * 4, 256, 256));
				}
				else//L
				{
					lizardsprite[i].setTextureRect(sf::IntRect(256 * animationframelizdaed, 256 * 9, 256, 256));
				}
				break;
			}

			//slowed from skilled
			if (lizardSlow[i])
			{
				if (timeToNoSlow[i] <= clock2.getElapsedTime().asSeconds())
					lizardSlow[i] = false;
				lizardsprite[i].setColor(sf::Color(0, 180, 255));
			}
			else
			{
				lizardsprite[i].setColor(sf::Color(255, 255, 255, 255));
				lizardVeloX[i] = 2;
			}

			//auto lizard walk
			if (lizardrec[i].getPosition().y == playerrec.getPosition().y + 20 && !lizardDeadAnimationStart[i])//bot walk
			{
				//Right check
				if (playerrec.getPosition().x + 34 < lizardrec[i].getPosition().x + lizardVision && playerrec.getPosition().x + 34 > lizardrec[i].getPosition().x)
				{
					lizardstatus[i] = walk;
					lizardLR[i] = true;
				}
				//Left check
				else if (playerrec.getPosition().x + 34 > lizardrec[i].getPosition().x - lizardVision && playerrec.getPosition().x + 34 < lizardrec[i].getPosition().x)
				{
					lizardstatus[i] = walk;
					lizardLR[i] = false;
				}
			}

			//Immune && ตีผู้เล่น
			if (!playerImmune && playerrec.getGlobalBounds().intersects(lizardAttack[i].getGlobalBounds()) && !lizardDeadAnimationStart[i])
			{
				if (lizardLR[i])//มันหันขวา เราเลื่อนขวา
				{
					drag = 10.0;
					playerHp -= 10;
					playerImmune = true;
					timeToNoImmune = clock2.getElapsedTime().asSeconds() + 2;
				}
				else//มันหันซ้าย เราเลื่อนซ้าย
				{
					drag = -10.0;
					playerHp -= 10;
					playerImmune = true;
					timeToNoImmune = clock2.getElapsedTime().asSeconds() + 2;
				}
			}

			//เช็คชน skill 1
			if (waterreccheck.getGlobalBounds().intersects(lizardrec[i].getGlobalBounds()) && !lizardDeadAnimationStart[i])
			{
				if (lizardhp[i] - damageSkill1 <= 0)
				{
					score += scoreLizWhenDie;
					HowManylizardInPlattform[lizardWherePlatAreU[i]]--;
					lizarddrop(lizardrec[i].getPosition(), 50, 70, 0);
					animationframelizdaed = 0;//
					lizardstatus[i] = death;//
					lizardDeadAnimationStart[i] = true;
				}
				lizardhp[i] = lizardhp[i] - damageSkill1 <= 0 ? 0 : lizardhp[i] - damageSkill1;
				lizardSlow[i] = true;
				timeToNoSlow[i] = clock2.getElapsedTime().asSeconds() + 3;
				lizardVeloX[i] = .5;
			}

			//เช็คชน skill 2
			for (int j = 0; j < 10; j++)
			{
				if (firesprite[j].getGlobalBounds().intersects(lizardrec[i].getGlobalBounds()) && !lizardDeadAnimationStart[i])
				{
					if (lizardhp[i] - damageSkill2 <= 0)
					{
						score += scoreLizWhenDie;
						HowManylizardInPlattform[lizardWherePlatAreU[i]]--;
						lizarddrop(lizardrec[i].getPosition(), 50, 80, 0);
						animationframelizdaed = 0;//
						lizardstatus[i] = death;//
						lizardDeadAnimationStart[i] = true;
					}
					lizardhp[i] = lizardhp[i] - damageSkill2 <= 0 ? 0 : lizardhp[i] - damageSkill2;
					firebool[j] = false;
				}
			}

			//ตอนหน้าทิ่มพื้น
			if (lizardDeadAnimationStart[i] && animationframelizdaed == 5)
			{
				lizardDeadAnimationStart[i] = false;
				islizardDie[i] = true;
				lizardrec[i].setPosition(4000, 0);
			}

			//Lizard ตาย
			if (lizardhp[i] <= 0 && lizardstatus[i] != death && lizardfall[i] == false)
			{
				score += scoreLizWhenDie;
				HowManylizardInPlattform[lizardWherePlatAreU[i]]--;
				lizarddrop(lizardrec[i].getPosition(), 30, 0, 0);
				animationframelizdaed = 0;//
				lizardstatus[i] = death;//
				lizardDeadAnimationStart[i] = true;
				lizardhp[i] = 0;
				lizDeathAmount++;
			}

			// เพิ่มเติม <อื่นๆ>
		}

		//Game Over
		if (playerHp <= 0 && menu == 1 || (sf::Keyboard::isKeyPressed(sf::Keyboard::P) && menu == 1))
		{
			win = false;
			menu = 99;
			std::string word;
			std::ifstream ReadFile("Score/scoreFile.txt");
			do {
				ReadFile >> word;
				std::string name = word.substr(0, word.find(','));
				int num = std::stoi(word.substr(word.find(',') + 1, word.length()));
				vec.push_back(std::make_pair(name, num));
			} while (ReadFile.good());
			ReadFile.close();
			std::ofstream WriteFile("Score/scoreFile.txt");
			vec.push_back(std::make_pair(inputString, score));
			std::sort(vec.begin(), vec.end(), sortbysecdesc);
			for (int i = 0; i < 5; i++)
			{
				WriteFile << vec[i].first << "," << vec[i].second << std::endl;
				text[2 * i + 2].setString(std::to_string(i + 1) + ". " + vec[i].first);
				text[2 * i + 3].setString(std::to_string(vec[i].second));
			}
			vec.clear();
			WriteFile.close();

			gameoverMusic.play();
			ingameMusic.stop();
			bossMusic.stop();
		}

		//fire skilled check
		for (int p = 0; p < 10; p++)
		{
			if (firesprite[p].getPosition().x < -100 || firesprite[p].getPosition().x > 4100)
			{
				firebool[p] = false;
			}

			if (firebool[p])
			{
				if (firsttime[p])
				{
					firesprite[p].setPosition(firevector[p]);
					firsttime[p] = false;
				}
				if (fireLR[p])
				{
					firesprite[p].setTextureRect(sf::IntRect(234 * animationfframe6, 0, 234, 90));//firesprite[p].setTextureRect(sf::IntRect(100 * animationframe60, 0, 100, 100));
					firesprite[p].move(fireVeloX, 0);
				}
				else
				{
					firesprite[p].setTextureRect(sf::IntRect(234 * animationfframe6, 90, 234, 90));//firesprite[p].setTextureRect(sf::IntRect(100 * animationframe60, 100, 100, 100));
					firesprite[p].move(-fireVeloX, 0);
				}
			}
			else
			{
				firesprite[p].setPosition(0, 2000);
			}
		}

		//Hide cursor
		if (menu == 1)
			window.setMouseCursorVisible(false);
		else
			window.setMouseCursorVisible(true);

		//item collistion coin hearth
		for (int buff = 0; buff < 30; buff++)
		{
			hearthSprite[buff].setTextureRect(sf::IntRect(32 * animationframe10, 32 * 4, 32, 32));
			coinSprite[buff].setTextureRect(sf::IntRect(32 * animationframe10, 32 * 2, 32, 32));
			starSprite[buff].setTextureRect(sf::IntRect(32 * animationframe10, 32 * 1, 32, 32));
			if (playerrec.getGlobalBounds().intersects(hearthSprite[buff].getGlobalBounds()))
			{
				if (playerHp + 5 > playerMaxHp)
					playerHp = playerMaxHp;
				else
					playerHp += 5;
				hearthOnScreen[buff] = false;
				hearthSprite[buff].setPosition(sf::Vector2f(0, 0));
			}
			if (playerrec.getGlobalBounds().intersects(coinSprite[buff].getGlobalBounds()))
			{
				coinOnScreen[buff] = false;
				coin += rand() % 50 + 100;
				coinSprite[buff].setPosition(sf::Vector2f(0, 0));
			}
		}

		bgsprite.setColor(sf::Color(rgb, rgb, rgb));//BG color

		drag *= 0.95;
		if (playerrec.getPosition().x + drag < 3840 - 68 && playerrec.getPosition().x + drag > 0)
			playerrec.move(drag, 0);
		int m = 70;
		//HP & MANA
		bgHpBar.setPosition(cam.getCenter().x - 700 + m, cam.getCenter().y - 400 + 60);//<------- OLD = bgHpBar.setPosition(cam.getCenter().x - 700, cam.getCenter().y - 400);
		bgManaBar.setPosition(cam.getCenter().x - 700, cam.getCenter().y - 400 + 30);
		ManaBar.setPosition(cam.getCenter().x - 700, cam.getCenter().y - 400 + 30);
		HpBar.setPosition(cam.getCenter().x - 700 + m, cam.getCenter().y - 400 + 60);//<------- OLD = HpBar.setPosition(cam.getCenter().x - 700, cam.getCenter().y - 400);
		HpBar.setSize(sf::Vector2f(500.0 * playerHp / playerMaxHp, 20));
		ManaBar.setSize(sf::Vector2f(500.0 * playerMana / playerMaxMana, 20));
		if (500.0 * playerHp / playerMaxHp > 500.0 / 2)
			HpBar.setFillColor(sf::Color(255 - (((500.0 * playerHp / playerMaxHp) - 250.0) * 255 / 250.0), 255, 0));
		else if (500.0 * playerHp / playerMaxHp <= 500.0 / 2)
			HpBar.setFillColor(sf::Color(255, (500.0 * playerHp / playerMaxHp) * 255 / 250.0, 0));
		//Skill Select
		skillshapeBg1.setPosition(cam.getCenter().x, cam.getCenter().y - 380);
		skillshapeBg2.setPosition(cam.getCenter().x + 70, cam.getCenter().y - 380);
		skillshapeBg3.setPosition(cam.getCenter().x + 140, cam.getCenter().y - 380);
		switch (skillSelect)
		{
		case 1:
			skillshapeBg1.setOutlineThickness(4);
			skillshapeBg2.setOutlineThickness(2);
			skillshapeBg3.setOutlineThickness(2);	
			skillshapeBg1.setOutlineColor(sf::Color::Green);
			skillshapeBg2.setOutlineColor(sf::Color::Black);
			skillshapeBg3.setOutlineColor(sf::Color::Black);
			skillshapeBg1.setScale(1.2, 1.2);
			skillshapeBg1.setScale(1, 1);
			skillshapeBg1.setScale(1, 1);
			break;
		}

		//Text
		GameTime = clock2.getElapsedTime().asSeconds();
		timeLefttext.setString("Servive time : " + std::to_string(GameTime) + " s");
		timeLefttext.setPosition(cam.getCenter().x + 400, cam.getCenter().y - 400 + 36 + 20 - 50);
		scoretext.setString("SCORE : " + std::to_string(score));
		scoretext.setPosition(cam.getCenter().x + 400, cam.getCenter().y - 400 - 50);
		cointext.setString("COIN : " + std::to_string(coin));
		cointext.setPosition(cam.getCenter().x + 400, cam.getCenter().y - 400 + 36 + 20 + 36 + 20 - 50);
		hptext.setPosition(bgHpBar.getPosition().x + 250 - hptext.getGlobalBounds().width / 2, bgHpBar.getPosition().y - 18);//<------- OLD = hptext.setPosition(bgHpBar.getPosition().x + 250 - hptext.getGlobalBounds().width / 2, bgHpBar.getPosition().y - 18);
		hptext.setString(std::to_string(playerHp) + " / " + std::to_string(playerMaxHp));
		manatext.setPosition(bgManaBar.getPosition().x + 250 - manatext.getGlobalBounds().width / 2, bgManaBar.getPosition().y - 18);
		manatext.setString(std::to_string(playerMana) + " / " + std::to_string(playerMaxMana));
		NameInGame.setPosition(bgHpBar.getPosition().x, bgHpBar.getPosition().y - 115);
		albusSprite.setPosition(bgHpBar.getPosition().x - 70, bgHpBar.getPosition().y - 50);

		//Time updated
		deltatime = clock.restart().asSeconds();
		totaltime += deltatime;
		totaltime2 += deltatime;
		totaltime3 += deltatime;
		totaltimelizdaed += deltatime;
		if (totaltime >= switchtime)
		{
			totaltime -= switchtime;
			animationframe8++;
			animationframe5++;
			animationframe6slow++;
			animationframe3++;

			for (int i = 0; i < 20; i++)
			{
				lizardAttack5[i]++;
				if (lizardAttack5[i] == 5)
					lizardAttack5[i] = 0;
			}

			if (playerMana < playerMaxMana)
			{
				if (playerMana + manaregen > playerMaxMana)
					playerMana = playerMaxMana;
				else
					playerMana += manaregen;
			}

			if (animationframe8 == 8)
				animationframe8 = 0;
			if (animationframe5 == 5)
			{
				animationframe5 = 0;
			}
			if (animationframe6slow == 6)
				animationframe6slow = 0;
			if (animationframe3 == 3)
			{
				animationframe3 = 0;
			}
			if (increase)
			{
				rgb += 1;
				if (rgb >= 255)
					increase = false;
			}
			else
			{
				rgb -= 1;
				if (rgb < 10)
					increase = true;
			}


		}
		if (totaltime2 >= switchtime2)
		{
			totaltime2 -= switchtime2;
			AnimationAtack++;
			mageAnimation8++;
			animationframe10++;
			if (animationframe10 == 10)
				animationframe10 = 0;
			if (AnimationAtack == 8)
				AnimationAtack = 0;
			if (mageAnimation8 == 8)
				mageAnimation8 = 0;
		}
		if (totaltime3 >= switchtime3)
		{
			totaltime3 -= switchtime3;
			animationframe12++;
			animationfframe6++;
			if (animationframe12 == 12)
				animationframe12 = 0;
			if (animationfframe6 == 6)
				animationfframe6 = 0;
		}
		if (totaltimelizdaed >= 0.2)
		{
			totaltimelizdaed -= 0.2;
			animationframelizdaed++;
			if (animationframelizdaed == 6)
				animationframelizdaed = 0;
		}
		for (int i = 0; i < maxlizard; i++)//lizard time to chage activity
		{
			totalTimeToChange[i] += deltatime;
			if (totalTimeToChange[i] >= switchTimeChange[i])
			{
				totalTimeToChange[i] -= switchTimeChange[i];
				if (lizardstatus[i] == walk || lizardstatus[i] == idle)
				{
					lizardstatus[i] = (rand() % 2) + 1;//lizardATK
					if (lizardstatus[i] == walk)
					{
						int dsad = rand() % 2;
						if (dsad)
							lizardLR[i] = true;
						else
							lizardLR[i] = false;
					}
					if (lizardstatus[i] == attack)
					{
						lizardAttack5[i] = 0;
					}
				}
			}
			if (lizardstatus[i] == attack && lizardAttack5[i] == 4)
			{
				lizardstatus[i] = (rand() % 2) + 1;

				//Bullet Update
				//sf::Vector2f lizardCenter = sf::Vector2f(lizardrec[i].getPosition().x + lizardrec[i].getGlobalBounds().width / 2, lizardrec[i].getPosition().y + lizardrec[i].getGlobalBounds().height / 2);
				//sf::Vector2f aimDirLizard = playerCenter - lizardCenter;
				//sf::Vector2f aimDirNormLizard = aimDirLizard / sqrt(aimDirLizard.x * aimDirLizard.x + aimDirLizard.y * aimDirLizard.y);

				//b1.sprite.setPosition(lizardCenter);
				//b1.sprite.setScale(sf::Vector2f(0.5f, 0.5f));
				//b1.currVelocity = aimDirNormLizard * 10.0f;
				//bulletsVec.push_back(Bullet(b1));
			}
		}

		//อื่นๆ
		wizsprite.setPosition(playerrec.getPosition().x - 80, playerrec.getPosition().y - 48);
		playerfootrec.setPosition(playerrec.getPosition().x, playerrec.getPosition().y + 89);
		for (int i = 0; i < maxlizard; i++)
		{
			if (lizardhp[i] <= 0)
				lizardhp[i] = 0;
			lizardsprite[i].setPosition(lizardrec[i].getPosition().x - 120, lizardrec[i].getPosition().y - 76);
			lizardfootrect[i].setPosition(lizardrec[i].getPosition().x, lizardrec[i].getPosition().y + 255);
			lizardHpBar[i].setPosition(lizardrec[i].getPosition().x - 34, lizardrec[i].getPosition().y);
			lizardHpBar[i].setSize(sf::Vector2f(68.0 * lizardhp[i] / 100.0, 5));
			lizardHitBox[i].setSize(sf::Vector2f(lizardVision * 2, 90));
			lizardHitBox[i].setPosition(lizardrec[i].getPosition().x - lizardVision, lizardrec[i].getPosition().y);
			lizardAttack[i].setPosition(lizardrec[i].getPosition().x - 51, lizardrec[i].getPosition().y);
			if (lizardfall[i])//สุ่มตก
			{
				lizardstatus[i] = idle;
				switch (lizardWherePlatAreU[i])
				{
				case 1:
					if (lizardrec[i].getPosition().y >= 1800 - 70)
					{
						lizardrec[i].setPosition(lizardrec[i].getPosition().x, 1800 - 70);
						lizardfall[i] = false;
					}
					else//ร่วง
					{
						lizardrec[i].move(0, 3);
					}
					break;
				case 2:
					if (lizardrec[i].getPosition().y >= 1550 - 70)
					{
						lizardrec[i].setPosition(lizardrec[i].getPosition().x, 1550 - 70);
						lizardfall[i] = false;
					}
					else//ร่วง
					{
						lizardrec[i].move(0, 3);
					}
					break;
				case 3:
					if (lizardrec[i].getPosition().y >= 1600 - 70)
					{
						lizardrec[i].setPosition(lizardrec[i].getPosition().x, 1600 - 70);
						lizardfall[i] = false;
					}
					else//ร่วง
					{
						lizardrec[i].move(0, 3);
					}
					break;
				case 4:
					if (lizardrec[i].getPosition().y >= 1535 - 70)
					{
						lizardrec[i].setPosition(lizardrec[i].getPosition().x, 1535 - 70);
						lizardfall[i] = false;
					}
					else//ร่วง
					{
						lizardrec[i].move(0, 3);
					}
					break;
				case 5:
					if (lizardrec[i].getPosition().y >= 1610 - 70)
					{
						lizardrec[i].setPosition(lizardrec[i].getPosition().x, 1610 - 70);
						lizardfall[i] = false;
					}
					else//ร่วง
					{
						lizardrec[i].move(0, 3);
					}
					break;
				case 6:
					if (lizardrec[i].getPosition().y >= 1300 - 70)
					{
						lizardrec[i].setPosition(lizardrec[i].getPosition().x, 1300 - 70);
						lizardfall[i] = false;
					}
					else//ร่วง
					{
						lizardrec[i].move(0, 3);
					}
					break;
				case 7:
					if (lizardrec[i].getPosition().y >= 1320 - 70)
					{
						lizardrec[i].setPosition(lizardrec[i].getPosition().x, 1320 - 70);
						lizardfall[i] = false;
					}
					else//ร่วง
					{
						lizardrec[i].move(0, 3);
					}
					break;
				case 8:
					if (lizardrec[i].getPosition().y >= 1350 - 70)
					{
						lizardrec[i].setPosition(lizardrec[i].getPosition().x, 1350 - 70);
						lizardfall[i] = false;
					}
					else//ร่วง
					{
						lizardrec[i].move(0, 3);
					}
					break;
				}
			}
		}
		waterreccheck.setPosition(sf::Vector2f(3000, 3000));
		mouseCam.x = sf::Mouse::getPosition(window).x + cam.getCenter().x - 800;
		mouseCam.y = sf::Mouse::getPosition(window).y + 1000;

		curRect.setPosition(sf::Vector2f(mouseCam.x - 25, mouseCam.y - 25));

		//Bullet Update
		playerCenter = sf::Vector2f(playerrec.getPosition().x + 34, playerrec.getPosition().y + 45);
		aimDir = mouseCam - playerCenter;
		aimDirNorm = aimDir / sqrt(aimDir.x * aimDir.x + aimDir.y * aimDir.y);

		//shoot bullet
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && menu == 1 && debouceLclick < clock2.getElapsedTime().asSeconds())
		{
			debouceLclick = clock2.getElapsedTime().asSeconds() + 0.3;
			b2.sprite.setRotation(rotate(playerCenter, mouseCam));
			b2.sprite.setOrigin(0, 25);//50x50
			b2.sprite.setPosition(playerCenter);
			b2.sprite.setTextureRect(sf::IntRect(0, 0, 50, 50));
			b2.currVelocity = aimDirNorm * 15.0f;
			bulletsVec.push_back(Bullet(b2));
		}

		if (sf::Mouse::isButtonPressed(sf::Mouse::Right) && menu == 1 && debouceRclick < clock2.getElapsedTime().asSeconds())
		{
			//debouceRclick = clock2.getElapsedTime().asSeconds() + 0.2;

			//b3.sprite.setRotation(rotate(playerCenter, mouseCam));
			//b3.sprite.setOrigin(0, 45);//50x50
			//b3.sprite.setPosition(playerCenter);
			//b3.sprite.setTextureRect(sf::IntRect(0, 0, 234, 90));
			//b3.currVelocity = aimDirNorm * 15.0f;
			//bulletsVec.push_back(Bullet(b3));

			//b4.sprite.setRotation(rotate(playerCenter, mouseCam));
			//b4.sprite.setOrigin(0, 50);//50x50
			//b4.sprite.setPosition(playerCenter);
			//b4.sprite.setTextureRect(sf::IntRect(0, 0, 100, 100));
			//b4.sprite.setScale(sf::Vector2f(2, 2));
			//b4.currVelocity = aimDirNorm * 15.0f;
			//bulletsVec.push_back(Bullet(b4));

			//e2.sprite.setPosition(sf::Vector2f(rand() % 3840, 800));
			//e2.currentVelocity = sf::Vector2f(3 + rand() % 6, 3 + rand() % 6);
			//e2.sprite.setTextureRect(sf::IntRect(0, 0, 290, 274));
			//e2.Redbg.setPosition(e2.sprite.getPosition().x + 70, e2.sprite.getPosition().y);
			//e2.HpGreen.setPosition(e2.sprite.getPosition().x + 70, e2.sprite.getPosition().y);
			//e2.TimeAtk = 3 + (rand() % 20) / 10.0;
			//enermyVec.push_back(Enermy(e2));

			//if (rand() % 2 == 0)
			//	e3.sprite.setPosition(-100, 1000 + rand() % 200);
			//else
			//	e3.sprite.setPosition(3900, 1000 + rand() % 200);
			//e3.currentVelocity = sf::Vector2f(4 + rand() % 5, 0);
			//e3.sprite.setTextureRect(sf::IntRect(0, 0, 155, 170));
			//e3.Redbg.setPosition(e3.sprite.getPosition().x + 35, e3.sprite.getPosition().y);
			//e3.HpGreen.setPosition(e3.sprite.getPosition().x + 35, e3.sprite.getPosition().y);
			//e3.TimeAtk = 3 + (rand() % 15) / 10.0;
			//enermyVec.push_back(Enermy(e3));


			//For Enermy Onlyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyy
			//b4.sprite.setRotation(rotate(playerCenter, mouseCam));
			//b4.sprite.setOrigin(0, 50);//50x50
			//b4.sprite.setPosition(playerCenter);
			//b4.sprite.setTextureRect(sf::IntRect(0, 0, 100, 100));
			//b4.sprite.setScale(sf::Vector2f(2, 2));
			//b4.currVelocity = aimDirNorm * 15.0f;
			//bulletsVec.push_back(Bullet(b4));

			//e4.sprite.setPosition(mouseCam);
			//e4.currentVelocity = sf::Vector2f(3 + rand() % 6, 3 + rand() % 6);
			//e4.Redbg.setPosition(e4.sprite.getPosition().x + 20, e4.sprite.getPosition().y);
			//e4.HpGreen.setPosition(e4.sprite.getPosition().x + 20, e4.sprite.getPosition().y);
			//e4.TimeAtk = 3 + (rand() % 20) / 10.0;
			//enermyVec.push_back(Enermy(e4));

			//e5.sprite.setPosition(mouseCam);
			//e5.currentVelocity = sf::Vector2f(3 + rand() % 6, 3 + rand() % 6);
			//e5.Redbg.setPosition(e5.sprite.getPosition().x + 100, e5.sprite.getPosition().y);
			//e5.HpGreen.setPosition(e5.sprite.getPosition().x + 100, e5.sprite.getPosition().y);
			//e5.TimeAtk = 3 + (rand() % 20) / 10.0;
			//e5.sprite.setScale(2.5f, 2.5f);
			//enermyVec.push_back(Enermy(e5));
			beamOn = true;
			shieldOn = true;
		}
		else
		{
			beamOn = false;
			shieldOn = false;
		}

		//beam
		if (beamOn)
		{
			beamSprite.setRotation(rotate(playerCenter, mouseCam));
			beamSprite.setOrigin(0, 145.0 / 2.0);
			beamSprite.setPosition(playerCenter);
			beamSprite.setTextureRect(sf::IntRect(0, 145 * animationBeam12, 2000, 145));
		}
		else
		{
			beamSprite.setPosition(0, 0);
			beamSprite.setTextureRect(sf::IntRect(0, 0, 318, 145));
		}

		//shield
		if (shieldOn)
		{
			shieldSprite.setOrigin(210 / 2, 187 / 2.0);
			shieldSprite.setPosition(playerCenter);
			shieldSprite.setTextureRect(sf::IntRect(210 * animationShield39, 0, 210, 187));
		}
		else
		{
			shieldSprite.setPosition(0, 0);
			shieldSprite.setTextureRect(sf::IntRect(0, 0, 210, 187));
		}

		animationBeam12tt += deltatime;
		if (animationBeam12tt > 0.07)
		{
			animationBeam12tt -= 0.07;
			animationBeam12++;
			if (animationBeam12 == 12)
				animationBeam12 = 0;
		}

		animationShield39tt += deltatime;
		if (animationShield39tt > 0.07)
		{
			animationShield39tt -= 0.07;
			animationShield39++;
			if (animationShield39 == 39)
				animationShield39 = 0;
		}

		//Bullet and Enermy Erase
		for (size_t i = 0; i < bulletsVec.size(); i++)
		{
			if (bulletsVec[i].sprite.getPosition().x < 0 || bulletsVec[i].sprite.getPosition().x > 3840 || bulletsVec[i].sprite.getPosition().y < 0 || bulletsVec[i].sprite.getPosition().y > 2160)
			{
				bulletsVec.erase(bulletsVec.begin() + i);
				break;
			}//เช็คขอบ
			if (bulletsVec[i].Type == 1 && bulletsVec[i].sprite.getGlobalBounds().intersects(playerrec.getGlobalBounds()) && !playerImmune)
			{
				bulletsVec.erase(bulletsVec.begin() + i);
				playerHp -= 4;
				playerImmune = true;
				timeToNoImmune = clock2.getElapsedTime().asSeconds() + 2;
				break;
			}
			if (bulletsVec[i].Type == 4 && bulletsVec[i].sprite.getGlobalBounds().intersects(playerrec.getGlobalBounds()) && !playerImmune)
			{
				playerHp -= 20;
				ex1.sprite.setPosition(playerrec.getPosition());
				ex1.sprite.setTextureRect(sf::IntRect(0, 0, 354, 343));
				ex1.sprite.setScale(sf::Vector2f(0.3, 0.3));
				ex1.Type = 1;
				explosionVec.push_back(ex1);
				bulletsVec.erase(bulletsVec.begin() + i);
				playerImmune = true;
				timeToNoImmune = clock2.getElapsedTime().asSeconds() + 2;
				break;
			}
			bool breakk = false;
			for (size_t j = 0; j < enermyVec.size(); j++)
			{
				if (bulletsVec[i].Type == 2 && enermyVec[j].Type == 1 && bulletsVec[i].sprite.getGlobalBounds().intersects(enermyVec[j].sprite.getGlobalBounds()))
				{
					enermyVec[j].HP -= 20;//20
					if (enermyVec[j].HP <= 0)
					{
						lizarddrop(enermyVec[j].sprite.getPosition(), 3, 0, 0);
						ex1.sprite.setPosition(enermyVec[j].sprite.getPosition());
						ex1.sprite.setTextureRect(sf::IntRect(0, 0, 354, 343));
						ex1.sprite.setScale(sf::Vector2f(0.3, 0.3));
						ex1.Type = 1;
						explosionVec.push_back(ex1);
						enermyVec.erase(enermyVec.begin() + j);
						score += 200;
						mageDeathAmount++;
					}
					bulletsVec.erase(bulletsVec.begin() + i);
					breakk = true;
					break;
				}
				else if (bulletsVec[i].Type == 2 && enermyVec[j].Type == 2 && bulletsVec[i].sprite.getGlobalBounds().intersects(enermyVec[j].sprite.getGlobalBounds()))
				{
					enermyVec[j].HP -= 20;//20
					if (enermyVec[j].HP <= 0)
					{
						lizarddrop(enermyVec[j].sprite.getPosition(), 3, 0, 0);
						ex1.sprite.setPosition(enermyVec[j].sprite.getPosition());
						ex1.sprite.setTextureRect(sf::IntRect(0, 0, 354, 343));
						ex1.sprite.setScale(sf::Vector2f(1, 1));
						ex1.Type = 1;
						explosionVec.push_back(ex1);
						enermyVec.erase(enermyVec.begin() + j);
						score += 1000;
						jellyDeathAmount++;
					}
					bulletsVec.erase(bulletsVec.begin() + i);
					breakk = true;
					break;
				}
				else if (bulletsVec[i].Type == 2 && enermyVec[j].Type == 3 && bulletsVec[i].sprite.getGlobalBounds().intersects(enermyVec[j].sprite.getGlobalBounds()))
				{
					enermyVec[j].HP -= 20;//20
					if (enermyVec[j].HP <= 0)
					{
						lizarddrop(enermyVec[j].sprite.getPosition(), 3, 0, 0);
						ex1.sprite.setPosition(enermyVec[j].sprite.getPosition());
						ex1.sprite.setTextureRect(sf::IntRect(0, 0, 354, 343));
						ex1.sprite.setScale(sf::Vector2f(0.3, 0.3));
						ex1.Type = 1;
						explosionVec.push_back(ex1);
						enermyVec.erase(enermyVec.begin() + j);
						score += 500;
						dragonDeathAmount++;
					}
					bulletsVec.erase(bulletsVec.begin() + i);
					breakk = true;
					break;
				}
				else if (bulletsVec[i].Type == 2 && enermyVec[j].Type == 4 && bulletsVec[i].sprite.getGlobalBounds().intersects(enermyVec[j].sprite.getGlobalBounds()))
				{
					enermyVec[j].HP -= 20;
					if (enermyVec[j].HP <= 0)
					{
						enermyVec.erase(enermyVec.begin() + j);
						score += 100;
					}
					bulletsVec.erase(bulletsVec.begin() + i);
					breakk = true;
					break;
				}
				else if (bulletsVec[i].Type == 2 && enermyVec[j].Type == 5 && bulletsVec[i].sprite.getGlobalBounds().intersects(enermyVec[j].sprite.getGlobalBounds()))
				{
					enermyVec[j].HP -= 20;
					if (enermyVec[j].HP <= 0)
					{
						enermyVec.erase(enermyVec.begin() + j);
						score += 8000;
						menu = 99;
						win = true;
						std::string word;
						std::ifstream ReadFile("Score/scoreFile.txt");
						do {
							ReadFile >> word;
							std::string name = word.substr(0, word.find(','));
							int num = std::stoi(word.substr(word.find(',') + 1, word.length()));
							vec.push_back(std::make_pair(name, num));
						} while (ReadFile.good());
						ReadFile.close();
						std::ofstream WriteFile("Score/scoreFile.txt");
						vec.push_back(std::make_pair(inputString, score));
						std::sort(vec.begin(), vec.end(), sortbysecdesc);
						for (int i = 0; i < 5; i++)
						{
							WriteFile << vec[i].first << "," << vec[i].second << std::endl;
							text[2 * i + 2].setString(std::to_string(i + 1) + ". " + vec[i].first);
							text[2 * i + 3].setString(std::to_string(vec[i].second));
						}
						vec.clear();
						WriteFile.close();
						ingameMusic.stop();
						bossMusic.stop();
					}
					bulletsVec.erase(bulletsVec.begin() + i);
					breakk = true;
					break;
				}
			}
			if (breakk)
				break;
		}

		//Explosion Update
		for (size_t i = 0; i < explosionVec.size(); i++)
		{
			explosionVec[i].update(deltatime);
			if (explosionVec[i].animationFrame >= 8)
			{
				explosionVec.erase(explosionVec.begin() + i);
				break;
			}
		}
		//bullet updated Frame
		for (size_t i = 0; i < bulletsVec.size(); i++)
		{
			bulletsVec[i].updateFrame(deltatime);
			bulletsVec[i].sprite.move(bulletsVec[i].currVelocity);
		}
		//Enermy update
		for (size_t i = 0; i < enermyVec.size(); i++)
		{
			//bat And boss To player
			if (enermyVec[i].Type == 4 || enermyVec[i].Type == 5)
			{
				aimDirLizard = sf::Vector2f(playerCenter.x - enermyVec[i].sprite.getPosition().x - 170 * enermyVec[i].sprite.getScale().x/2 + 20, playerCenter.y - enermyVec[i].sprite.getPosition().y - 125 * enermyVec[i].sprite.getScale().y/2 + 30);
				aimDirNormLizard = aimDirLizard / sqrt(aimDirLizard.x * aimDirLizard.x + aimDirLizard.y * aimDirLizard.y);
				if(enermyVec[i].Type == 4)
					enermyVec[i].currentVelocity = aimDirNormLizard * (2.0f + rand() % 20 / 10.0f);
				else if(enermyVec[i].Type == 5)
					enermyVec[i].currentVelocity = aimDirNormLizard * (1.0f + rand() % 10 / 10.0f);
			}
			enermyVec[i].updateFrame(deltatime, cam.getCenter());
			enermyVec[i].sprite.move(enermyVec[i].currentVelocity);
			enermyVec[i].Redbg.move(enermyVec[i].currentVelocity);
			enermyVec[i].HpGreen.move(enermyVec[i].currentVelocity);
			if ( enermyVec[i].Type == 1 && enermyVec[i].totalTimeATK > enermyVec[i].TimeAtk)
			{
				enermyVec[i].totalTimeATK -= enermyVec[i].TimeAtk;

				base = sf::Vector2f(enermyVec[i].sprite.getPosition().x, enermyVec[i].sprite.getPosition().y);
				aimDirLizard = playerCenter - base;
				aimDirNormLizard = aimDirLizard / sqrt(aimDirLizard.x * aimDirLizard.x + aimDirLizard.y * aimDirLizard.y);

				b1.sprite.setPosition(base);
				b1.sprite.setScale(sf::Vector2f(0.5f, 0.5f));
				b1.currVelocity = aimDirNormLizard * 10.0f;//Speed
				bulletsVec.push_back(Bullet(b1));
			}
			else if (enermyVec[i].Type == 2 && enermyVec[i].totalTimeATK > enermyVec[i].TimeAtk)
			{
				enermyVec[i].totalTimeATK -= enermyVec[i].TimeAtk;
				//ซ้าย
				base = sf::Vector2f(enermyVec[i].sprite.getPosition().x + 145, enermyVec[i].sprite.getPosition().y + 120);
				target = sf::Vector2f(enermyVec[i].sprite.getPosition().x + 145 - 1, enermyVec[i].sprite.getPosition().y + 120);
				aimDirLizard = target - base;
				aimDirNormLizard = aimDirLizard / sqrt(aimDirLizard.x * aimDirLizard.x + aimDirLizard.y * aimDirLizard.y);
				b1.sprite.setPosition(base);
				b1.sprite.setScale(sf::Vector2f(0.5f, 0.5f));
				b1.currVelocity = aimDirNormLizard * 10.0f;//Speed
				bulletsVec.push_back(Bullet(b1));
				//ขวา
				base = sf::Vector2f(enermyVec[i].sprite.getPosition().x + 145, enermyVec[i].sprite.getPosition().y + 120);
				target = sf::Vector2f(enermyVec[i].sprite.getPosition().x + 145 + 1, enermyVec[i].sprite.getPosition().y + 120);
				aimDirLizard = target - base;
				aimDirNormLizard = aimDirLizard / sqrt(aimDirLizard.x * aimDirLizard.x + aimDirLizard.y * aimDirLizard.y);
				b1.sprite.setPosition(base);
				b1.sprite.setScale(sf::Vector2f(0.5f, 0.5f));
				b1.currVelocity = aimDirNormLizard * 10.0f;//Speed
				bulletsVec.push_back(Bullet(b1));
				//ล่าง
				base = sf::Vector2f(enermyVec[i].sprite.getPosition().x + 145, enermyVec[i].sprite.getPosition().y + 120);
				target = sf::Vector2f(enermyVec[i].sprite.getPosition().x + 145, enermyVec[i].sprite.getPosition().y + 120 + 1);
				aimDirLizard = target - base;
				aimDirNormLizard = aimDirLizard / sqrt(aimDirLizard.x * aimDirLizard.x + aimDirLizard.y * aimDirLizard.y);
				b1.sprite.setPosition(base);
				b1.sprite.setScale(sf::Vector2f(0.5f, 0.5f));
				b1.currVelocity = aimDirNormLizard * 10.0f;//Speed
				bulletsVec.push_back(Bullet(b1));
				//บน
				base = sf::Vector2f(enermyVec[i].sprite.getPosition().x + 145, enermyVec[i].sprite.getPosition().y + 120);
				target = sf::Vector2f(enermyVec[i].sprite.getPosition().x + 145, enermyVec[i].sprite.getPosition().y + 120 - 1);
				aimDirLizard = target - base;
				aimDirNormLizard = aimDirLizard / sqrt(aimDirLizard.x * aimDirLizard.x + aimDirLizard.y * aimDirLizard.y);
				b1.sprite.setPosition(base);
				b1.sprite.setScale(sf::Vector2f(0.5f, 0.5f));
				b1.currVelocity = aimDirNormLizard * 10.0f;//Speed
				bulletsVec.push_back(Bullet(b1));
				//บนขวา
				base = sf::Vector2f(enermyVec[i].sprite.getPosition().x + 145, enermyVec[i].sprite.getPosition().y + 120);
				target = sf::Vector2f(enermyVec[i].sprite.getPosition().x + 145 + 1, enermyVec[i].sprite.getPosition().y + 120 - 1);
				aimDirLizard = target - base;
				aimDirNormLizard = aimDirLizard / sqrt(aimDirLizard.x * aimDirLizard.x + aimDirLizard.y * aimDirLizard.y);
				b1.sprite.setPosition(base);
				b1.sprite.setScale(sf::Vector2f(0.5f, 0.5f));
				b1.currVelocity = aimDirNormLizard * 10.0f;//Speed
				bulletsVec.push_back(Bullet(b1));
				//บนซ้าย
				base = sf::Vector2f(enermyVec[i].sprite.getPosition().x + 145, enermyVec[i].sprite.getPosition().y + 120);
				target = sf::Vector2f(enermyVec[i].sprite.getPosition().x + 145 - 1, enermyVec[i].sprite.getPosition().y + 120 - 1);
				aimDirLizard = target - base;
				aimDirNormLizard = aimDirLizard / sqrt(aimDirLizard.x * aimDirLizard.x + aimDirLizard.y * aimDirLizard.y);
				b1.sprite.setPosition(base);
				b1.sprite.setScale(sf::Vector2f(0.5f, 0.5f));
				b1.currVelocity = aimDirNormLizard * 10.0f;//Speed
				bulletsVec.push_back(Bullet(b1));
				//ล่างขวา
				base = sf::Vector2f(enermyVec[i].sprite.getPosition().x + 145, enermyVec[i].sprite.getPosition().y + 120);
				target = sf::Vector2f(enermyVec[i].sprite.getPosition().x + 145 + 1, enermyVec[i].sprite.getPosition().y + 120 + 1);
				aimDirLizard = target - base;
				aimDirNormLizard = aimDirLizard / sqrt(aimDirLizard.x * aimDirLizard.x + aimDirLizard.y * aimDirLizard.y);
				b1.sprite.setPosition(base);
				b1.sprite.setScale(sf::Vector2f(0.5f, 0.5f));
				b1.currVelocity = aimDirNormLizard * 10.0f;//Speed
				bulletsVec.push_back(Bullet(b1));
				//ล่างซ้าย
				base = sf::Vector2f(enermyVec[i].sprite.getPosition().x + 145, enermyVec[i].sprite.getPosition().y + 120);
				target = sf::Vector2f(enermyVec[i].sprite.getPosition().x + 145 - 1, enermyVec[i].sprite.getPosition().y + 120 + 1);
				aimDirLizard = target - base;
				aimDirNormLizard = aimDirLizard / sqrt(aimDirLizard.x * aimDirLizard.x + aimDirLizard.y * aimDirLizard.y);
				b1.sprite.setPosition(base);
				b1.sprite.setScale(sf::Vector2f(0.5f, 0.5f));
				b1.currVelocity = aimDirNormLizard * 10.0f;//Speed
				bulletsVec.push_back(Bullet(b1));
			}
			else if (enermyVec[i].Type == 3 && enermyVec[i].totalTimeATK > enermyVec[i].TimeAtk)
			{
				enermyVec[i].totalTimeATK -= enermyVec[i].TimeAtk;
				//ยิงลงล่าง
				base = sf::Vector2f(enermyVec[i].sprite.getPosition().x, enermyVec[i].sprite.getPosition().y);
				target = sf::Vector2f(enermyVec[i].sprite.getPosition().x, enermyVec[i].sprite.getPosition().y+ 1);
				aimDirLizard = target - base;
				aimDirNormLizard = aimDirLizard / sqrt(aimDirLizard.x * aimDirLizard.x + aimDirLizard.y * aimDirLizard.y);
				b4.sprite.setRotation(rotate(enermyVec[i].sprite.getPosition(), sf::Vector2f(enermyVec[i].sprite.getPosition().x, enermyVec[i].sprite.getPosition().y + 1)));
				b4.sprite.setOrigin(0, 50);//50x50
				b4.sprite.setPosition(enermyVec[i].sprite.getPosition());
				b4.sprite.setTextureRect(sf::IntRect(0, 0, 100, 100));
				b4.sprite.setScale(sf::Vector2f(2, 2));
				b4.currVelocity = aimDirNormLizard * 15.0f;
				bulletsVec.push_back(Bullet(b4));
			}
			else if (enermyVec[i].Type == 5 && enermyVec[i].totalTimeATK > enermyVec[i].TimeAtk)
			{
				enermyVec[i].totalTimeATK -= enermyVec[i].TimeAtk;
				//bat spawn
				int randdom = rand();
				if (randdom % 100 < 10);
					for (short int r = 0; r < 7; r++)
					{
						e4.sprite.setPosition(enermyVec[i].sprite.getPosition().x + 170 / 2 * enermyVec[i].sprite.getScale().x, enermyVec[i].sprite.getPosition().y + 125 / 2 * enermyVec[i].sprite.getScale().y);
						e4.currentVelocity = sf::Vector2f(3 + rand() % 6, 3 + rand() % 6);
						e4.Redbg.setPosition(e4.sprite.getPosition().x + 20, e4.sprite.getPosition().y);
						e4.HpGreen.setPosition(e4.sprite.getPosition().x + 20, e4.sprite.getPosition().y);
						e4.TimeAtk = 3 + (rand() % 20) / 10.0;
						enermyVec.push_back(Enermy(e4));
					}
			}
		}
		//Draw
		window.setView(cam);
		window.clear();

		if (menu == 0)
		{
			window.draw(menuSky);
			window.draw(menuRock1);
			window.draw(menuRock2);
			window.draw(menuClound1);
			window.draw(menuClound2);
			window.draw(menuClound3);
			window.draw(menuClound4);

			window.draw(gameNametext);
			window.draw(starttext);
			window.draw(leaderboardtext);
			window.draw(howplaytext);
			window.draw(exittext);
			window.draw(DevNametext);
		}
		else if (menu == 10)
		{
			window.draw(menuSky);
			window.draw(menuRock1);
			window.draw(menuRock2);
			window.draw(menuClound1);
			window.draw(menuClound2);
			window.draw(menuClound3);
			window.draw(menuClound4);

			window.draw(EnterNametext);
			window.draw(enter1_13C);

			window.draw(backtext);
		}
		else if (menu == 3)
		{
			window.draw(menuSky);
			window.draw(menuRock1);
			window.draw(menuRock2);
			window.draw(menuClound1);
			window.draw(menuClound2);
			window.draw(menuClound3);
			window.draw(menuClound4);

			window.draw(highscoretext);
			for (int i = 0; i < 12; i++)
				window.draw(text[i]);

			window.draw(backtext);
		}
		else if (menu == 99)
		{
			window.draw(gameOverShape);
			window.draw(backtext);
			if (win)
				window.draw(WinText);
			else
				window.draw(gameOverText);
			window.draw(yourScoreText);
			for (int i = 2; i < 12; i++)
				window.draw(text[i]);
		}

		window.draw(bgsprite);
		window.draw(undergroundsprite);
		window.draw(groundsprite);
		window.draw(rec2);
		window.draw(rec3);
		window.draw(rec4);
		window.draw(rec5);
		window.draw(rec6);
		window.draw(rec7);
		window.draw(rec8);

		magesprite.setTextureRect(sf::IntRect(122 * mageAnimation8, 0, 122, 110));
		magesprite.setPosition(500, 1500);
		//window.draw(magesprite);//

		for (int i = 0; i < maxlizard; i++)
		{
			window.draw(lizardHpBar[i]);
			//window.draw(lizardHitBox[i]);//Lizard vistion
			//window.draw(lizardrec[i]);//
			window.draw(lizardsprite[i]);
			//window.draw(lizardAttack[i]);//Lizard Hit box
		}
		for (int i = 0; i < 30; i++)
		{
			window.draw(hearthSprite[i]);
			window.draw(coinSprite[i]);
			window.draw(starSprite[i]);
		}


		window.draw(watersprite);
		for (int t = 0; t < 10; t++)
			window.draw(firesprite[t]);
		window.draw(wizsprite);
		//window.draw(playerrec);// player hit box

		if (menu == 1)
		{
			for (size_t i = 0; i < bulletsVec.size(); i++)
			{
				window.draw(bulletsVec[i].sprite);
			}
			for (long long i = enermyVec.size() - 1 ; i >= 0; i--)
			{
				window.draw(enermyVec[i].sprite);
				window.draw(enermyVec[i].Redbg);
				window.draw(enermyVec[i].HpGreen);
			}

			for (size_t i = 0; i < explosionVec.size(); i++)
			{
				window.draw(explosionVec[i].sprite);
			}

			window.draw(beamSprite);
			window.draw(shieldSprite);

			window.draw(timeLefttext);
			window.draw(scoretext);
			window.draw(cointext);

			window.draw(bgHpBar);
			//window.draw(bgManaBar);
			window.draw(HpBar);
			//window.draw(ManaBar);
			window.draw(hptext);
			//window.draw(manatext);

			window.draw(NameInGame);
			window.draw(albusSprite);

			switch (skillSelect)
			{
			case 1:
				window.draw(skillshapeBg2);
				window.draw(skillshapeBg3);
				window.draw(skillshapeBg1);
				break;
			}
		}

		window.draw(curRect);
		window.display();
	}
	return 0;
}
/*
		----------rec6--------			--------------------rec7---------				----rec8---

------rec2-------		-------rec3--------		----rec4-----			------rec5-------

----------------------------------groundtetex----------------------------------------------------------
*/