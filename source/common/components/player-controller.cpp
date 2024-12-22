#include "player-controller.hpp"
#include "../ecs/entity.hpp"
#include "../deserialize-utils.hpp"

namespace our {
    void PlayerControllerComponent::deserialize(const nlohmann::json& data){
        if(!data.is_object()) return;
    }
}