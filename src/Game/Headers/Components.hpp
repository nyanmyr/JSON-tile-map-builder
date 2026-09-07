#ifndef COMPONENTS_HPP
#define COMPONENTS_HPP

#include <SFML/Graphics.hpp>
#include "Scenes.hpp"
#include "Enums.hpp"
#include <optional>
#include <string>

namespace Component {
	struct Position;
	struct Transform;
	struct Origin;
	struct Button;
	struct Text;
	struct NextScene;
	struct ZIndex;
	struct Velocity;
	struct Speed;
	struct PlayerController;
	struct Drag;
	struct Sprite;
	struct Texture;
	struct TexturesContainer;
	struct Color;
}

struct Component::Position
{
	double x = 0.0;
	double y = 0.0;
};

struct Component::Transform
{
	double width = 0.0;
	double height = 0.0;
};

struct Component::Origin
{
	double offsetX = 0.0;
	double offsetY = 0.0;
};

struct Component::Button
{
	double clickedDuration = 0.0;
	double enabled = true;
	double clickedTimer = 0.0;
	bool clicked = false;
};

struct Component::Text
{
	std::optional<sf::Text> box {};
	std::string string = "";
	int size = 12;
	sf::Color color = sf::Color::White;
	Enum::TextFormat format = Enum::TextFormat::MIDDLE;
};

struct Component::NextScene
{
	Scene next = Scene::MENU;
	bool active = false;
};

struct Component::ZIndex
{
	int index = 0;
	bool visible = true;
};

struct Component::Velocity
{
	double minX = 0.0;
	double minY = 0.0;
	double maxX = 0.0;
	double maxY = 0.0;
	double x = 0.0;
	double y = 0.0;
};

struct Component::Speed
{
	double x = 0.0;
	double y = 0.0;
};

struct Component::PlayerController
{
	bool enabled = false;
};

struct Component::Drag
{
	double x = 0.0;
	double y = 0.0;
};

struct Component::Sprite
{
	std::optional<sf::Sprite> body {};
};

struct Component::Texture
{
	Enum::Texture data;
};

struct Component::TexturesContainer
{
	std::unordered_map<Enum::Texture, sf::Texture> map;
};

struct Component::Color
{
	sf::Color col = sf::Color(sf::Color::White);
};


#endif