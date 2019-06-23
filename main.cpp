#include <SFML/Graphics.hpp>
#include <iostream>
#include <sstream>
#include <vector>
#include <list>
#include "mission.h"
#include "view.h"
#include "level.h"

#include "TinyXML/tinyxml.h"
using namespace sf;

class Entity
{
public:
	std::vector<Object> obj;
	float dx, dy, x, y, speed, moveTimer;
	int w, h, health;
	float CurrentFrame;
	bool life, isMove, onGround;
	Texture texture;
	Sprite sprite;
	String name;
	Entity(Image &image, String Name, float X, float Y, int W, int H)
	{
		x = X; y = Y; w = W; h = H; name = Name; moveTimer = 0;
		speed = 0; health = 100; dx = 0; dy = 0;
		life = true; onGround = false; isMove = false;
		CurrentFrame = 0;
		texture.loadFromImage(image);
		sprite.setTexture(texture);
		sprite.setOrigin(w / 2, h / 2);
	}
	FloatRect getRect()
	{
		return FloatRect(x, y, w, h);
	}
};
class Player :public Entity
{
public:
	float px, py;
	enum { left, right, up, down, jump, stay } state;
	int playerScore;
	bool isShoot;
	Player(Image &image, String Name, Level &lev, float X, float Y, int W, int H) :Entity(image, Name, X, Y, W, H)
	{
		px = 0; py = 0;
		playerScore = isShoot = 0; state = stay; obj = lev.GetAllObjects();
		if (name == "Player1")
		{
			sprite.setTextureRect(IntRect(0, 10, w, h));
			sprite.setScale(0.5f, 0.5f);
		}
	}
	void setPlayerCoord()
	{
		px = x;
		py = y;
	}
	float getplayercoordinateX()
	{
		return px;
	}
	float getplayercoordinateY()
	{
		return py;
	}
	void control()
	{
		if (Keyboard::isKeyPressed) {
			if (Keyboard::isKeyPressed(Keyboard::Left))
			{
				state = left; speed = 0.1;
				CurrentFrame += 0.01 * 1.8;
				if (CurrentFrame > 4) CurrentFrame -= 4;
				sprite.setTextureRect(IntRect(120 * int(CurrentFrame), 160, 120, 120));
			}
			if (Keyboard::isKeyPressed(Keyboard::Right))
			{
				state = right; speed = 0.1;
				CurrentFrame += 0.01 * 1.8;
				if (CurrentFrame > 4) CurrentFrame -= 4;
				sprite.setTextureRect(IntRect(120 * int(CurrentFrame), 10, 120, 120));
			}

			if ((Keyboard::isKeyPressed(Keyboard::Up)) && (onGround)) {
				state = jump; dy = -0.6; onGround = false;
			}

			if (Keyboard::isKeyPressed(Keyboard::Down)) {
				state = down;
			}
			if (Keyboard::isKeyPressed(Keyboard::Space)) {
				isShoot = true;
			}
		}
	}
	void checkCollisionWithMap(float Dx, float Dy)
	{
		for (int i = 0; i<obj.size(); i++)
			if (getRect().intersects(obj[i].rect))
			{
				if (obj[i].name == "solid")
				{
					if (Dy>0) { y = obj[i].rect.top - h;  dy = 0; onGround = true; }
					if (Dy<0) { y = obj[i].rect.top + obj[i].rect.height;   dy = 0; }
					if (Dx>0) { x = obj[i].rect.left - w; }
					if (Dx<0) { x = obj[i].rect.left + obj[i].rect.width; }
				}
			}
	}

	void update(float time)
	{
		control();
		setPlayerCoord();
		switch (state)
		{
		case right:dx = speed; break;
		case left:dx = -speed; break;
		case up: break;
		case down: dx = 0; break;
		case stay: break;
		}
		x += dx*time;
		checkCollisionWithMap(dx, 0);
		y += dy*time;
		checkCollisionWithMap(0, dy);
		sprite.setPosition(x + w / 2, y + h / 2);
		if (health <= 0) { life = false; }
		if (!isMove) { speed = 0; }
		setPlayerCoordinateForView(x, y);
		if (life) { setPlayerCoordinateForView(x, y); }
		dy = dy + 0.0015*time;
	}
	
};
class Enemy :public Entity
{
public:
	float ex, ey;
	Enemy(Image &image, String Name, Level &lvl, float X, float Y, int W, int H) :Entity(image, Name, X, Y, W, H)
	{
		obj = lvl.GetObjects("solid");
		ex = 0; ey = 0;
		if (name == "EasyEnemy")
		{
			sprite.setTextureRect(IntRect(0, 310, w, h));
			sprite.setScale(0.5f, 0.5f);
		}
	}
	void setEnemyCoord()
	{
		ex = x;
		ey = y;
	}
	void setEnemyX(float time, float px, float py)
	{
		float distance = sqrt((px - ex)*(px - ex) + (py - ey)*(py - ey));
		std::cout << "distance=" << distance << " " << "px= " << px << " " << "py=" << py << " " << "ex=" << x << " " << "ey=" << y << "\n";
		if (150 < distance)
		{
			isMove = true;
			x += 0.1*time*(px - ex) / distance;
		}
		if (distance < 150)
		{
			isMove = false;
			if (!isMove)
			{
				time = 0;
			}
		}
		if (x > px) { dx = -0.1; }
		else if (x < px) { dx = 0.1; }
	}
	void checkCollisionWithMap(float Dx, float Dy)
	{


		for (int i = 0; i<obj.size(); i++)
			if (getRect().intersects(obj[i].rect))
			{
				if (Dy>0) { y = obj[i].rect.top - h;  dy = 0; }
				if (Dy<0) { y = obj[i].rect.top + obj[i].rect.height;   dy = 0; }
				if (Dx>0) { x = obj[i].rect.left - w; }
				if (Dx<0) { x = obj[i].rect.left + obj[i].rect.width; }
			}
	}

	void update(float time, float px, float py)
	{
		if (name == "EasyEnemy")
		{
			setEnemyCoord();
			setEnemyX(time, px, py);
			checkCollisionWithMap(dx, 0);
			sprite.setPosition(x + w / 2, y + h / 2);
			if (health <= 0) { life = false; }
			if (dx < 0)
			{
				CurrentFrame += 0.005*time;
				if (CurrentFrame > 4) CurrentFrame -= 4;
				sprite.setTextureRect(IntRect(100 * int(CurrentFrame), 470, 100, 120));
			}

			if (dx > 0)
			{
				CurrentFrame += 0.005*time;
				if (CurrentFrame > 4) CurrentFrame -= 4;
				sprite.setTextureRect(IntRect(100 * int(CurrentFrame), 310, 100, 120));
			}
		}
	}
};
class Bullet :public Entity
{
public:
	int direction;
	Bullet(Image &image, String Name, Level &lvl, float X, float Y, int W, int H, int dir) :Entity(image, Name, X, Y, W, H)
	{
		obj = lvl.GetObjects("solid");
		x = X;
		y = Y;
		direction = dir;
		speed = 0.4;
		w = h = 28;
		life = true;
	}

	void update(float time) 
	{
		switch (direction)
		{
		case 0: dx = -speed; dy = 0;   break;
		case 1: dx = speed; dy = 0;   break;
		case 2: dx = 0; dy = -speed;   break;
		case 3: dx = 0; dy = -speed;   break;
		case 4: dx = 0; dy = -speed;   break;
		case 5: dx = 0; dy = -speed;   break;
		}

		x += dx*time;
		y += dy*time;

		if (x <= 0) x = 1;
		if (y <= 0) y = 1;

		for (int i = 0; i < obj.size(); i++)
		{
			if (getRect().intersects(obj[i].rect))
			{
				life = false;
			}
		}

		sprite.setPosition(x + w, y + h);
	}
};
class Well :public Entity
{
public:
	bool isSelect;
	Well(Image &image, String Name, Level &lvl, float X, float Y, int W, int H, int dir) :Entity(image, Name, X, Y, W, H)
	{
		obj = lvl.GetObjects("solid");
		x = X;
		y = Y;
		speed = 0.0;
		w = h = 50;
		life = true;
		isMove = false;
		isSelect = false;
		

	}
};
int main()
{
	RenderWindow window(VideoMode(640, 480), "Sergey Zinovev");
	view.reset(FloatRect(0, 0, 640, 480));

	Level lvl;
	lvl.LoadFromFile("D:/MyGames/SuperGames/images/map.tmx");

	Image heroImage;
	heroImage.loadFromFile("D:/MyGames/SuperGames/images/worker.png");
	heroImage.createMaskFromColor(Color(255, 255, 255));

	Image easyEnemyImage;
	easyEnemyImage.loadFromFile("D:/MyGames/SuperGames/images/worker.png");
	easyEnemyImage.createMaskFromColor(Color(255, 255, 255));

	Image BulletImage;
	BulletImage.loadFromFile("D:/MyGames/SuperGames/images/bullet.png");
	BulletImage.createMaskFromColor(Color(255, 255, 255));

	std::list<Bullet*> bullets;
	std::list<Bullet*>::iterator it;
	std::list<Enemy*> enemys;
	std::list<Enemy*>::iterator it1;

	Object player = lvl.GetObject("player");
	Player p(heroImage, "Player1", lvl, player.rect.left, player.rect.top, 50, 50);
	std::vector<Object> e = lvl.GetObjects("EasyEnemy");
	for (int i = 0; i < e.size(); i++)
	{
		enemys.push_back(new Enemy(easyEnemyImage, "EasyEnemy", lvl, e[i].rect.left, e[i].rect.top, 50, 50));
	}
	std::vector<Object> bull = lvl.GetObjects("Bullet");
	std::vector<Object> well = lvl.GetObjects("well");
	Clock clock;
	while (window.isOpen())
	{
		float time = clock.getElapsedTime().asMicroseconds();

		clock.restart();
		time = time / 800;

		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
			if (p.isShoot == true) { p.isShoot = false; bullets.push_back(new Bullet(BulletImage, "Bullet", lvl, p.x, p.y, 16, 16, p.state)); }
		}
		p.update(time);
		for (it1 = enemys.begin(); it1 != enemys.end(); it1++) { (*it1)->update(time, p.getplayercoordinateX(), p.getplayercoordinateY()); }
		for (it = bullets.begin(); it != bullets.end(); it++) { (*it)->update(time); }
		for (it1 = enemys.begin(); it1 != enemys.end();)
		{
			Enemy *b = *it1;
			b->update(time, p.getplayercoordinateX(), p.getplayercoordinateY());
			if (b->life == false) { it1 = enemys.erase(it1); delete b; }
			else it1++;
		}
		for (it = bullets.begin(); it != bullets.end();)
		{
			Bullet *g = *it;
			g->update(time);
			if (g->life == false) { it = bullets.erase(it); delete g; }
			else it++;
		}
		for (it = bullets.begin(); it != bullets.end(); it++)
		{
			for (it1 = enemys.begin(); it1 != enemys.end(); it1++)
			{
				if ((*it1)->getRect().intersects((*it)->getRect()))
				{
					if ((*it1)->name == "EasyEnemy")
					{
						(*it1)->dx = 0;
						(*it1)->health = 0;
						(*it)->life = false;
					}
				}
			}
		}
		window.setView(view);
		window.clear(Color(77, 83, 140));
		lvl.Draw(window);
		for (it1 = enemys.begin(); it1 != enemys.end(); it1++) 
		{
			window.draw((*it1)->sprite);
		}
		for (it = bullets.begin(); it != bullets.end(); it++) 
		{
			window.draw((*it)->sprite); 
		}
		window.draw(p.sprite);
		window.display();
	}
	return 0;
}