#include "fps.h"


fps::fps()
{

}

void fps::draw_FPS(sf::Text & t)
{
	FPS++;
	sf::Time elapsed = clock1.getElapsedTime();
	if (elapsed.asSeconds() > 1.0f)
	{
		t.setString(std::to_string(FPS));
		clock1.restart();
		FPS = 0;
	}
}


fps::~fps()
{
}
