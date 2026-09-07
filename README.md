Nacre Engine is a Game Engine that implements an Entity-Component-System (ECS) architecture. It is my personal project and is currently a work in progress.

Built with SFML (3.1.0).

*Build version is found in the latest commit.*

### Mission Statement

> The dedicated purpose of the Nacre Engine is give me an easy way to make simple and small-scale games much more efficiently and in a structured manner. Additionally, it serves as a way for me to learn how the ECS structure works.
> 

### ECS Architecture

- **Entity** - unique identifier for an object
- **Component** - a container that defines an object’s state and variables
- **System** - logic processor that manipulates component data

### Current Features

- **Entity Manager** - handles the creation and destruction of entities
- **Component Manager** - handles the registration, retrieval, creation, and removal of component arrays
- **Component Array** - holds the instances of components in one data structure
- **Nacre Coordinator** - links the entity and component manager together, in addition to giving the ability for coordinated deletion of entities and its attached components
- **Systems** - manipulates component arrays for separation of logic and data
- **Scenes** - contains levels
- **Entity Maker** - gives a handy way of instantiating entities with parameters for easier instantiation