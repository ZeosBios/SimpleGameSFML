#include <SFML/Graphics.hpp>
using namespace sf;

sf::View view;

void setPlayerCoordinateForView(float x, float y) 
{
	float tempX = x; float tempY = y;

	//if (x < 320) tempX = 320;
	if (x < 319) tempX = 319;
	if (y > 280) tempY = 280;

	view.setCenter(tempX, tempY);
}


