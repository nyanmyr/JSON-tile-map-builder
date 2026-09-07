#ifndef NACRE_MANAGER_HPP
#define NACRE_MANAGER_HPP

#include "ComponentManager.hpp"
#include "EntityManager.hpp"

class NacreCoordinator
{
private:
	ComponentManager& cm = ComponentManager::getInstance();
	EntityManager& em = EntityManager::getInstance();

	NacreCoordinator() {}

	NacreCoordinator(const NacreCoordinator&) = delete;
	NacreCoordinator& operator=(const NacreCoordinator&) = delete;
public:
	static NacreCoordinator& getInstance()
	{
		static NacreCoordinator instance;
		return instance;
	}

	template<typename T>
	void registerComponent()
	{
		cm.registerComponent<T>();
	}

	template<typename T>
	void addComponent(Entity entity, T component)
	{
		cm.addComponent(entity, component);
	}

	template<typename T>
	void removeComponent(Entity entity)
	{
		cm.removeComponent<T>(entity);
	}

	template<typename T>
	T& getComponent(Entity entity)
	{
		T& component = cm.getComponent(entity);
		return component;
	}

	template<typename T>
	std::shared_ptr<ComponentArray<T>> getComponentArray()
	{
		std::shared_ptr<ComponentArray<T>> componentArray = cm.getComponentArray<T>();
		return componentArray;
	}

	Entity& createEntity()
	{
		Entity entity = em.createEntity();
		return entity;
	}

	bool& isAlive(Entity entity)
	{
		bool isAlive = em.isAlive(entity);
		return isAlive;
	}

	void deleteEntity(Entity entity)
	{
		cm.entityDestroyed(entity);
		em.destroyEntity(entity);
	}

	void destroyAll()
	{
		cm.allEntitiesDestroyed();
		em.destroyAllEntities();
	}
};

#endif