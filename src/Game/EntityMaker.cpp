#include <SFML/Graphics.hpp>
#include "../Engine/NacreCoordinator.hpp"
#include "Headers/EntityMaker.hpp"
#include "Headers/Components.hpp"
#include "Headers/Enums.hpp"

NacreCoordinator& entityMakerNC = NacreCoordinator::getInstance();

Entity makePlayer
(
	const Enum::Texture texture,
	const sf::Vector2f pos,
	const sf::Vector2f size,
	const sf::Vector2f minVelocity,
	const sf::Vector2f maxVelocity,
	const sf::Vector2f speed,
	const sf::Vector2f drag,
	const sf::Color col
)
{
	Entity entity = entityMakerNC.createEntity();

	entityMakerNC.addComponent
	(
		entity,
		Component::Position
		{ 
			pos.x,
			pos.y
		}
	);
	entityMakerNC.addComponent
	(
		entity,
		Component::ZIndex
		{
			1,
			true
		}
	);
	entityMakerNC.addComponent
	(
		entity,
		Component::Origin
		{
			size.x / 2.0,
			size.y / 2.0
		}
	);
	entityMakerNC.addComponent
	(
		entity,
		Component::Velocity
		{
			minVelocity.x,
			minVelocity.y,
			maxVelocity.x,
			maxVelocity.y
		}
	);
	entityMakerNC.addComponent
	(
		entity,
		Component::Speed
		{
			speed.x,
			speed.y
		}
	);
	entityMakerNC.addComponent
	(
		entity,
		Component::Drag
		{
			drag.x,
			drag.y
		}
	);
	entityMakerNC.addComponent
	(
		entity,
		Component::PlayerController
		{
			true
		}
	);
	entityMakerNC.addComponent
	(
		entity,
		Component::Transform
		{
			size.x,
			size.y
		}
	);
	entityMakerNC.addComponent
	(
		entity,
		Component::Texture{ texture }
	);
	entityMakerNC.addComponent
	(
		entity,
		Component::Sprite{}
	);

	entityMakerNC.addComponent
	(
		entity,
		Component::Color{ col }
	);

	return entity;
}

Entity makeButton
(
	const Enum::Texture texture,
	const sf::Vector2f pos,
	const sf::Vector2f size,
	const Scene scene,
	const std::string str,
	const sf::Font& font,
	const sf::Color col
)
{
	Entity entity = entityMakerNC.createEntity();

	entityMakerNC.addComponent(
		entity,
		Component::Position
		{
			pos.x,
			pos.y
		}
	);
	entityMakerNC.addComponent
	(
		entity,
		Component::Transform
		{
			size.x,
			size.y
		}
	);
	entityMakerNC.addComponent
	(
		entity,
		Component::Origin
		{
			size.x / 2.0,
			size.y / 2.0
		}
	);
	entityMakerNC.addComponent
	(
		entity,
		Component::Button
		{
			0.125f,
			true
		}
	);

	sf::Text text(font);
	entityMakerNC.addComponent
	(
		entity,
		Component::Text
		{
			text,
			str,
			64,
			sf::Color::Black,
			Enum::TextFormat::MIDDLE
		}
	);
	entityMakerNC.addComponent
	(
		entity,
		Component::NextScene
		{
			scene,
			false
		}
	);
	entityMakerNC.addComponent
	(
		entity,
		Component::ZIndex
		{
			1,
			true
		}
	);
	entityMakerNC.addComponent
	(
		entity,
		Component::Transform
		{
			size.x,
			size.y
		}
	);
	entityMakerNC.addComponent
	(
		entity,
		Component::Texture{ texture }
	);
	entityMakerNC.addComponent
	(
		entity,
		Component::Sprite{}
	);

	entityMakerNC.addComponent
	(
		entity,
		Component::Color{ col }
	);

	return entity;
}

Entity makeLoadedTexturesContainer()
{
	Entity entity = entityMakerNC.createEntity();

	entityMakerNC.addComponent
	(
		entity,
		Component::TexturesContainer{}
	);

	return entity;
}