#include "mine.hpp"
#include "../ecs/entity.hpp"
#include "../deserialize-utils.hpp"

namespace our {

    void Mine::deserialize(const nlohmann::json& data){
        if(!data.is_object()) return;
    }
}