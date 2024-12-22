#include "timeup.hpp"
#include "../ecs/entity.hpp"
#include "../deserialize-utils.hpp"

namespace our {

    void Timeup::deserialize(const nlohmann::json& data){
        if(!data.is_object()) return;
    }
}