#pragma once

#include "../ecs/entity.hpp"
#include "camera.hpp"
#include "mesh-renderer.hpp"
#include "free-camera-controller.hpp"
#include "movement.hpp"
#include "wall.hpp"
#include "zwall.hpp"
#include "pickup.hpp"
#include "enemy.hpp"
#include "player.hpp"
#include "player-controller.hpp"
#include"light.hpp"
namespace our
{

    // Given a json object, this function picks and creates a component in the given entity
    // based on the "type" specified in the json object which is later deserialized from the rest of the json object
    inline void deserializeComponent(const nlohmann::json &data, Entity *entity)
    {
        std::string type = data.value("type", "");
        Component *component = nullptr;
        // TODO: (Req 8) Add an option to deserialize a "MeshRendererComponent" to the following if-else statement
        if (type == CameraComponent::getID())
        {
            component = entity->addComponent<CameraComponent>();
        }
        else if (type == FreeCameraControllerComponent::getID())
        {
            component = entity->addComponent<FreeCameraControllerComponent>();
        }
        else if (type == MovementComponent::getID())
        {
            component = entity->addComponent<MovementComponent>();
        }
        else if (type == MeshRendererComponent::getID())
        {
            component = entity->addComponent<MeshRendererComponent>();
        }
        else if (type == wall::getID()) {
            component = entity->addComponent<wall>();
        }
        else if (type == zwall::getID()) {
            component = entity->addComponent<zwall>();
        }
        else if (type == Pickup::getID()) {
            component = entity->addComponent<Pickup>();
        }
        else if(type == LightComponent::getID()){
            component = entity->addComponent<LightComponent>();
        }
        else if (type == Player::getID()) {
            component = entity->addComponent<Player>();
        }
        else if (type == PlayerControllerComponent::getID()) {
            component = entity->addComponent<PlayerControllerComponent>();
        } 
        else if (type == Enemy::getID()) {
            component = entity->addComponent<Enemy>();
        }
        if (component)
            component->deserialize(data);
    }
}