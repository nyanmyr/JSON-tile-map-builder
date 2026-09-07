#include "Headers/Systems.hpp"

#include <SFML/Graphics.hpp>

NacreCoordinator& systemsNC = NacreCoordinator::getInstance();

// -------------------------------------------------------
// start systems
// -------------------------------------------------------
void Start::setText(sf::Font& font)
{
	auto& textArray = systemsNC.getComponentArray<Component::Text>();

	for (auto& [entity, textObj] : textArray->getAll())
	{
		textObj.box.value().setString(textObj.string);
		textObj.box.value().setCharacterSize(textObj.size);
		textObj.box.value().setFillColor(textObj.color);
	}
}
void Start::setTextOrigin()
{
	auto& textArray = systemsNC.getComponentArray<Component::Text>();
	auto& transformArray = systemsNC.getComponentArray<Component::Transform>();

	double offsetX;
	double offsetY;

	for (auto& [entity, textObj] : textArray->getAll())
	{
		if (!transformArray->hasData(entity))
		{
			continue;
		}

		Component::Transform& transform = transformArray->getData(entity);

		switch (textObj.format)
		{
		case Enum::TextFormat::TOP:
			offsetX = textObj.box.value().getLocalBounds().size.x / 2;
			offsetY = textObj.box.value().getLocalBounds().size.y;
			break;
		case Enum::TextFormat::BOTTOM:
			offsetX = textObj.box.value().getLocalBounds().size.x / 2;
			offsetY = textObj.box.value().getLocalBounds().size.y / 2;
			break;
		case Enum::TextFormat::MIDDLE:
		default:
			offsetX = textObj.box.value().getLocalBounds().size.x / 2;
			offsetY = (textObj.box.value().getLocalBounds().size.y / 2) + (textObj.box.value().getLocalBounds().size.y / 4);
			break;
		}

		textObj.box.value().setOrigin
		(
			sf::Vector2f
			(
				offsetX,
				offsetY
			)
		);
	}
}
void Start::setSpriteOrigin()
{
	auto& originArray = systemsNC.getComponentArray<Component::Origin>();
	auto& spriteArray = systemsNC.getComponentArray<Component::Sprite>();

	for (auto& [entity, spriteObj] : spriteArray->getAll())
	{
		if (!originArray->hasData(entity))
		{
			continue;
		}

		Component::Origin& originObj = originArray->getData(entity);

		spriteObj.body->setOrigin
		(
			sf::Vector2f
			(
				originObj.offsetX / spriteObj.body->getScale().x,
				originObj.offsetY / spriteObj.body->getScale().y
			)
		);
	}
}
void Start::loadTextures(Entity loadedTextures)
{
	auto& texturesContainerArray = systemsNC.getComponentArray<Component::TexturesContainer>();

	if (!texturesContainerArray->hasData(loadedTextures))
	{
		return;
	}

	Component::TexturesContainer& containerObj = texturesContainerArray->getData(loadedTextures);

	containerObj.map.emplace(Enum::Texture::TEXTURE_PLACEHOLDER, sf::Texture(SPRITES_PATH "placeholder_texture.jpeg"));
}
void Start::loadSprites(Entity loadedTextures)
{
	auto& spriteArray = systemsNC.getComponentArray<Component::Sprite>();
	auto& transformArray = systemsNC.getComponentArray<Component::Transform>();
	auto& textureArray = systemsNC.getComponentArray<Component::Texture>();
	auto& texturesContainerArray = systemsNC.getComponentArray<Component::TexturesContainer>();

	if (!texturesContainerArray->hasData(loadedTextures))
	{
		return;
	}

	Component::TexturesContainer& containerObj = texturesContainerArray->getData(loadedTextures);

	for (auto& [entity, spriteObj] : spriteArray->getAll())
	{
		if (!transformArray->hasData(entity) ||
			!textureArray->hasData(entity))
		{
			continue;
		}

		Component::Texture& textureObj = textureArray->getData(entity);
		Component::Transform& transformObj = transformArray->getData(entity);

		spriteObj.body.emplace(containerObj.map[textureObj.data]);
		spriteObj.body->setScale
		(
			sf::Vector2f
			(
				transformObj.width / spriteObj.body->getGlobalBounds().size.x,
				transformObj.height / spriteObj.body->getGlobalBounds().size.y
			)
		);
	}
}
void Start::setColor()
{
	auto& colorArray = systemsNC.getComponentArray<Component::Color>();
	auto& spriteArray = systemsNC.getComponentArray<Component::Sprite>();

	for (auto& [entity, colorObj] : colorArray->getAll())
	{
		if (!spriteArray->hasData(entity))
		{
			continue;
		}

		Component::Sprite& spriteObj = spriteArray->getData(entity);

		spriteObj.body->setColor(colorObj.col);
	}
}

// -------------------------------------------------------
// control systems
// -------------------------------------------------------
const double DEFAULT_SCALE_X = 1.0;
const double DEFAULT_SCALE_Y = 1.0;

const double HOVER_SCALE_X = 1.1;
const double HOVER_SCALE_Y = 1.1;

const double CLICKED_SCALE_X = 0.9;
const double CLICKED_SCALE_Y = 0.9;

void Control::buttonClicks
(
	const sf::Vector2i mouseVector,
	const DeltaTime dt
)
{
	auto& buttonArray = systemsNC.getComponentArray<Component::Button>();
	auto& originArray = systemsNC.getComponentArray<Component::Origin>();
	auto& transformArray = systemsNC.getComponentArray<Component::Transform>();
	auto& positionArray = systemsNC.getComponentArray<Component::Position>();

	for (auto& [entity, buttonObj] : buttonArray->getAll())
	{
		if (!buttonObj.enabled ||
			!originArray->hasData(entity) ||
			!positionArray->hasData(entity) ||
			!transformArray->hasData(entity))
		{
			continue;
		}

		// buttonArray must have a shape, origin, and text
		////std::cout << "button.top: " << button.top << "\n";
		////std::cout << "button.left: " << button.left << "\n";
		const Component::Origin& originObj = originArray->getData(entity);
		const Component::Transform& transformObj = transformArray->getData(entity);
		const Component::Position& posObj = positionArray->getData(entity);

		if (mouseVector.x > posObj.x - originObj.offsetX &&
			mouseVector.x < posObj.x + transformObj.width - originObj.offsetX &&
			mouseVector.y > posObj.y - originObj.offsetY &&
			mouseVector.y < posObj.y + transformObj.height - originObj.offsetY)
		{
			buttonObj.clicked = true;
			buttonObj.clickedTimer = buttonObj.clickedDuration;
		}
	}
}
void Control::doPlayerControl
(
	const Entity player,
	const DeltaTime dt
)
{
	auto& velocityArray = systemsNC.getComponentArray<Component::Velocity>();
	auto& speedArray = systemsNC.getComponentArray<Component::Speed>();
	auto& playerControllerArray = systemsNC.getComponentArray<Component::PlayerController>();

	if (!velocityArray->hasData(player) ||
		!speedArray->hasData(player) ||
		!playerControllerArray->hasData(player))
	{
		return;
	}

	const Component::PlayerController playerControllerObj = playerControllerArray->getData(player);
	const Component::Speed speedObj = speedArray->getData(player);
	Component::Velocity& velocityObj = velocityArray->getData(player);

	if (!playerControllerObj.enabled)
	{
		return;
	}

	double newSpeedX = 0.0;
	double newSpeedY = 0.0;

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
	{
		newSpeedY = -speedObj.y;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
	{
		newSpeedY = speedObj.y;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
	{
		newSpeedX = -speedObj.x;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
	{
		newSpeedX = speedObj.x;
	}

	// applies the speed (even if there aren't any changes)
	velocityObj.x += (newSpeedX * dt);
	velocityObj.y += (newSpeedY * dt);

	if (velocityObj.x > velocityObj.maxX)
	{
		velocityObj.x = velocityObj.maxX;
	}
	else if (velocityObj.x < velocityObj.minX)
	{
		velocityObj.x = velocityObj.minX;
	}

	if (velocityObj.y > velocityObj.maxY)
	{
		velocityObj.y = velocityObj.maxY;
	}
	else if (velocityObj.y < velocityObj.minY)
	{
		velocityObj.y = velocityObj.minY;
	}

}

// -------------------------------------------------------
// update systems
// -------------------------------------------------------
void Update::doButtons
(
	const sf::Vector2i mouseVector,
	const DeltaTime dt
) {
	auto& spriteArray = systemsNC.getComponentArray<Component::Sprite>();
	auto& buttonArray = systemsNC.getComponentArray<Component::Button>();
	auto& originArray = systemsNC.getComponentArray<Component::Origin>();
	auto& textArray = systemsNC.getComponentArray<Component::Text>();
	auto& nextSceneArray = systemsNC.getComponentArray<Component::NextScene>();
	auto& transformArray = systemsNC.getComponentArray<Component::Transform>();
	auto& positionArray = systemsNC.getComponentArray<Component::Position>();

	for (auto& [entity, buttonObj] : buttonArray->getAll())
	{
		if (!buttonObj.enabled)
		{
			continue;
		}

		// buttonArray must have a shape, origin, and text
		if (!originArray->hasData(entity) ||
			!positionArray->hasData(entity) ||
			!spriteArray->hasData(entity))
		{
			continue;
		}

		////std::cout << "button.top: " << button.top << "\n";
		////std::cout << "button.left: " << button.left << "\n";
		Component::Origin& originObj = originArray->getData(entity);
		Component::Transform& transformObj = transformArray->getData(entity);
		Component::Position& posObj = positionArray->getData(entity);
		Component::Sprite& spriteObj = spriteArray->getData(entity);

		if (buttonObj.clickedTimer <= 0)
		{
			spriteObj.body->setScale
			(
				sf::Vector2f
				(
					DEFAULT_SCALE_X * (transformObj.width / spriteObj.body->getTexture().getSize().x),
					DEFAULT_SCALE_Y * (transformObj.height / spriteObj.body->getTexture().getSize().y)
				)
			);

			if (textArray->hasData(entity))
			{
				Component::Text& text = textArray->getData(entity);
				text.box->setScale
				(
					sf::Vector2f
					(
						DEFAULT_SCALE_X,
						DEFAULT_SCALE_Y
					)
				);
			}
		}
		else
		{
			buttonObj.clickedTimer -= dt;
			if (buttonObj.clickedTimer <= 0)
			{
				buttonObj.clicked = true;

				if (nextSceneArray->hasData(entity))
				{
					////std::cout << "starting next scene." << "\n";
					Component::NextScene& nextScene = nextSceneArray->getData(entity);
					nextScene.active = true;
				}
			}
		}

		// button hovering
		if (mouseVector.x > posObj.x - originObj.offsetX &&
			mouseVector.x < posObj.x + transformObj.width - originObj.offsetX &&
			mouseVector.y > posObj.y - originObj.offsetY &&
			mouseVector.y < posObj.y + transformObj.height - originObj.offsetY &&
			buttonObj.clickedTimer <= 0)
		{
			spriteObj.body->setScale
			(
				sf::Vector2f
				(
					HOVER_SCALE_X * (transformObj.width / spriteObj.body->getTexture().getSize().x),
					HOVER_SCALE_Y * (transformObj.height / spriteObj.body->getTexture().getSize().y)
				)
			);

			if (textArray->hasData(entity))
			{
				Component::Text& text = textArray->getData(entity);
				text.box->setScale
				(
					sf::Vector2f
					(
						HOVER_SCALE_X,
						HOVER_SCALE_Y
					)
				);
			}
		}

		// button clicking
		if (buttonObj.clickedTimer > 0)
		{
			spriteObj.body->setScale
			(
				sf::Vector2f
				(
					CLICKED_SCALE_X * (transformObj.width / spriteObj.body->getTexture().getSize().x),
					CLICKED_SCALE_Y * (transformObj.height / spriteObj.body->getTexture().getSize().y)
				)
			);

			if (textArray->hasData(entity))
			{
				Component::Text& text = textArray->getData(entity);
				text.box->setScale
				(
					sf::Vector2f
					(
						CLICKED_SCALE_X,
						CLICKED_SCALE_Y
					)
				);
			}
		}
	}
}
void Update::doNextScene
(
	sf::RenderWindow& window,
	sf::Font& font
)
{
	auto& nextSceneArray = systemsNC.getComponentArray<Component::NextScene>();

	bool playNext = false;
	Scene playNextScene;

	for (auto& [entity, nextSceneObj] : nextSceneArray->getAll())
	{
		// buttons must have a shape, origin, and text
		if (nextSceneObj.active)
		{
			//std::cout << "active: " << nextScene.next << "\n";
			playNext = true;
			playNextScene = nextSceneObj.next;
			break;
		}
	}

	if (playNext)
	{
		systemsNC.destroyAll();
		playScene
		(
			window,
			playNextScene,
			font
		);
		window.close();
	}
}
void Update::move(const DeltaTime dt)
{
	auto& velocityArray = systemsNC.getComponentArray<Component::Velocity>();
	auto& positionArray = systemsNC.getComponentArray<Component::Position>();

	for (auto& [entity, velocityObj] : velocityArray->getAll())
	{
		if (!positionArray->hasData(entity))
		{
			continue;
		}

		Component::Position& posObj = positionArray->getData(entity);
		posObj.x += (velocityObj.x * dt);
		posObj.y += (velocityObj.y * dt);
	}
}
void Update::drag(const DeltaTime dt)
{
	auto& velocityArray = systemsNC.getComponentArray<Component::Velocity>();
	auto& dragArray = systemsNC.getComponentArray<Component::Drag>();

	for (auto& [entity, velocityObj] : velocityArray->getAll())
	{
		if (!dragArray->hasData(entity))
		{
			continue;
		}

		Component::Drag dragObj = dragArray->getData(entity);

		// can't be exactly 0.0 because it will drift aimlessly
		velocityObj.x = velocityObj.x < -0.1 ? velocityObj.x + (dragObj.x * dt) :
			velocityObj.x > 0.1f ? velocityObj.x - (dragObj.x * dt) : 0.0;
		velocityObj.y = velocityObj.y < -0.1 ? velocityObj.y + (dragObj.y * dt) :
			velocityObj.y > 0.1f ? velocityObj.y - (dragObj.y * dt) : 0.0;
	}
}

// -------------------------------------------------------
// rendering systems
// -------------------------------------------------------
void Render::doZIndex(std::queue<Entity>& renderQueue)
{
	auto& zIndexArray = systemsNC.getComponentArray<Component::ZIndex>();

	std::vector<std::pair<int, Entity>> renderVector;
	for (auto& [entity, zIndexObj] : zIndexArray->getAll())
	{
		if (zIndexObj.visible) renderVector.emplace_back(zIndexObj.index, entity);
	}
	std::sort(renderVector.begin(), renderVector.end());

	for (auto& [zIndex, entity] : renderVector)
	{
		renderQueue.push(entity);
	}
}
void Render::render
(
	sf::RenderWindow& window,
	std::queue<Entity>& renderQueue
)
{
	auto& spriteArray = systemsNC.getComponentArray<Component::Sprite>();
	auto& positionArray = systemsNC.getComponentArray<Component::Position>();
	auto& textArray = systemsNC.getComponentArray<Component::Text>();

	while (!renderQueue.empty())
	{
		Entity& popped = renderQueue.front();
		////std::cout << "popped: " << popped << "\n";

		if (!positionArray->hasData(popped))
		{
			// this means the entity does not have a position component
			continue;
		}

		Component::Position& posObj = positionArray->getData(popped);

		if (spriteArray->hasData(popped))
		{
			Component::Sprite& spriteObj = spriteArray->getData(popped);

			spriteObj.body->setPosition
			(
				sf::Vector2f
				(
					posObj.x,
					posObj.y
				)
			);
			window.draw(spriteObj.body.value());
		}

		if (textArray->hasData(popped))
		{
			Component::Text& textObj = textArray->getData(popped);

			textObj.box->setPosition
			(
				sf::Vector2f
				(
					posObj.x,
					posObj.y
				)
			);
			window.draw(textObj.box.value());
		}

		renderQueue.pop();
	}
}