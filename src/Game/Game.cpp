#include <SFML/Graphics.hpp>

#include "Headers/GameManager.hpp"
#include "Headers/Scenes.hpp"

#include <iostream>

using sf::RenderWindow;
using sf::VideoMode;

constexpr int SCREEN_WIDTH = 800;
constexpr int SCREEN_HEIGHT = 600;

constexpr int MAX_FPS = 60;

const std::string FONT_FILEPATH = RESOURCES_PATH "arial.ttf";

void main()
{
	RenderWindow window(VideoMode({ SCREEN_WIDTH, SCREEN_HEIGHT }), "Nacre Engine", sf::Style::Close);
	window.setFramerateLimit(MAX_FPS);

	NacreCoordinator& nc = NacreCoordinator::getInstance();

	// components registration
	nc.registerComponent<Component::Position>();
	nc.registerComponent<Component::Transform>();
	nc.registerComponent<Component::Origin>();
	nc.registerComponent<Component::Button>();
	nc.registerComponent<Component::Text>();
	nc.registerComponent<Component::NextScene>();
	nc.registerComponent<Component::ZIndex>();
	nc.registerComponent<Component::Velocity>();
	nc.registerComponent<Component::Speed>();
	nc.registerComponent<Component::PlayerController>();
	nc.registerComponent<Component::Drag>();
	nc.registerComponent<Component::Sprite>();
	nc.registerComponent<Component::Texture>();
	nc.registerComponent<Component::TexturesContainer>();
	nc.registerComponent<Component::Color>();

	sf::Font font;
	if (!font.openFromFile(FONT_FILEPATH))
	{
		throw std::runtime_error("Font not found.");
	}

	playScene
	(
		window,
		Scene::MENU,
		font
	);
}