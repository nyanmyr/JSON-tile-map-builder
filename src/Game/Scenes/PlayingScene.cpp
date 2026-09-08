#include <SFML/Graphics.hpp>
#include <nlohmann/json.hpp>

#include "../src/Game/Headers/GameManager.hpp"
#include "../src/Game/Headers/Scenes.hpp"

#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <iomanip>

using JSON = nlohmann::json;

void playingScene
(
	sf::RenderWindow& window,
	sf::Font& font
)
{
	std::ifstream read(RESOURCES_PATH "tilemap.json");

	if (!read.is_open())
	{
		std::cerr << "Error: Failed to open tilemap.json.\n";
		return;
	}

	JSON jsonData;
	try {
		jsonData = JSON::parse(read);
	}
	catch (const JSON::parse_error& e) {
		std::cerr << "JSON Parsing Error: " << e.what() << "\n";
		return;
	}

	std::vector<Entity> tiles{};

	for (const auto& obj : jsonData["tilemap"])
	{
		std::string textureString = obj["texture"];
		//std::cout << "textureString: " << textureString << "\n";
		
		Enum::Texture loadedTexture = Enum::Texture::TEXTURE_PLACEHOLDER;

		// meant to be extensible
		if (textureString == "TEXTURE_PLACEHOLDER")
		{
			loadedTexture = Enum::Texture::TEXTURE_PLACEHOLDER;
		} 
		else
		{
			loadedTexture = Enum::Texture::TEXTURE_PLACEHOLDER;
		}

		double posXDouble = obj["posX"];
		double posYDouble = obj["posY"];
		double sizeXDouble = obj["sizeX"];
		double sizeYDouble = obj["sizeY"];

		std::string colorString = obj["color"];
		sf::Color loadedColor = sf::Color::White;

		// again meant to be extensible
		if (colorString == "White")
		{
			loadedColor = sf::Color::White;
		}
		else if (colorString == "Black")
		{
			loadedColor = sf::Color::Black;
		}
		else if (colorString == "Red")
		{
			loadedColor = sf::Color::Red;
		}
		else if (colorString == "Blue")
		{
			loadedColor = sf::Color::Blue;
		}
		else if (colorString == "Cyan")
		{
			loadedColor = sf::Color::Cyan;
		}
		else if (colorString == "Green")
		{
			loadedColor = sf::Color::Green;
		}
		else if (colorString == "Magenta")
		{
			loadedColor = sf::Color::Magenta;
		}
		else if (colorString == "Yellow")
		{
			loadedColor = sf::Color::Yellow;
		}
		else
		{
			loadedColor = sf::Color::White;
		}

		tiles.emplace_back
		(
			makeObject
			(
				loadedTexture,
				sf::Vector2f(
					posXDouble,
					posYDouble
				),
				sf::Vector2f(
					sizeXDouble,
					sizeYDouble
				),
				loadedColor
			)
		);
		//std::cout << "test: " << "\n";
		//std::string loadedString = jsonData["object1"]["name"];
	}

	//std::cout << "tiles: " << static_cast<int>(tiles.size())<< "\n";

	NacreCoordinator& nc = NacreCoordinator::getInstance();

	// game state variables
	sf::Clock clock;
	std::queue<Entity> renderQueue;

	Entity loadedTextures = makeLoadedTexturesContainer();

	// entity instantiation
	Entity player = makePlayer
	(
		Enum::Texture::TEXTURE_PLACEHOLDER,
		sf::Vector2f(
			window.getDefaultView().getSize().x / 2.0,
			window.getDefaultView().getSize().y / 2.0
		),
		{
			40.0,
			40.0
		},
		{
			-300.0,
			-300.0
		},
		{
			300.0,
			300.0
		},
		{
			500.0,
			500.0
		},
		{
			50.0,
			50.0
		},
		sf::Color::Green
	);

	// onstart systems
	Start::setText(font); // font system is limited to one font
	Start::setTextOrigin();

	Start::loadTextures(loadedTextures);
	Start::loadSprites(loadedTextures);
	Start::setSpriteOrigin();
	Start::setColor();

	while (window.isOpen())
	{
		DeltaTime dt = clock.restart().asSeconds();

		while (const std::optional event = window.pollEvent())
		{
			if (event->is<sf::Event::Closed>())
			{
				window.close();
			}
		}

		// systems
		Control::doPlayerControl
		(
			player,
			dt
		);
		Update::move(dt);
		Update::drag(dt);

		window.clear();
		// render systems
		Render::doZIndex(renderQueue);
		Render::render
		(
			window,
			renderQueue
		);
		window.display();
	}

	std::ofstream write(RESOURCES_PATH "tilemap.json");

	if (write.is_open())
	{
		write << std::setw(4) << jsonData;
		write.close();
		std::cout << "JSON file successfully created.\n";
	}
	else
	{
		std::cerr << "Error: failed to open file for writing.\n";
	}
}