#ifndef SCENES_HPP
#define SCENES_HPP

#include <SFML/Graphics.hpp>

#include <stdexcept>

enum Scene
{
	MENU,
	PLAYING
};

void playScene
(
	sf::RenderWindow& window,
	Scene scene,
	sf::Font& font
);
void menuScene
(
	sf::RenderWindow& window,
	sf::Font& font
);
void playingScene
(
	sf::RenderWindow& window,
	sf::Font& font
);


#endif