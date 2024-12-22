#pragma once

#include "../ecs/world.hpp"
#include "../components/camera.hpp"
#include "../components/free-camera-controller.hpp"

#include "../application.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/trigonometric.hpp>
#include <glm/gtx/fast_trigonometry.hpp>

#include "../components/wall.hpp"
#include "../components/zwall.hpp"
#include "../components/player.hpp"

#define COLLIDED_WITH_XWALL 1
#define COLLIDED_WITH_ZWALL -1
#define NO_COLLISION 0

namespace our
{

    // The free camera controller system is responsible for moving every entity which contains a FreeCameraControllerComponent.
    // This system is added as a slightly complex example for how use the ECS framework to implement logic. 
    // For more information, see "common/components/free-camera-controller.hpp"
    class FreeCameraControllerSystem {
        Application* app; // The application in which the state runs
        bool mouse_locked = false; // Is the mouse locked
        bool iscolide;
    public:

        // When a state enters, it should call this function and give it the pointer to the application
        void enter(Application* app){
            this->app = app;
        }

        // This should be called every frame to update all entities containing a FreeCameraControllerComponent 
        void update(World* world, float deltaTime) {}

        // Collision detection handling
        int detectCollision(World*World, glm::vec3& position){

            glm::vec3 xwallPosition;
            glm::vec3 zwallPosition;
            
            auto entities = World->getEntities();

            for(auto entity : entities)
            {
                if(entity->getComponent<wall>())
                {
                    xwallPosition = entity->localTransform.position;
                    if(abs(position.x - xwallPosition.x)  <= 6 && abs(position.z - xwallPosition.z) <= 2)
                    {
                        return COLLIDED_WITH_XWALL;
                    }

                }
                if(entity->getComponent<zwall>())
                {
                    zwallPosition = entity->localTransform.position;

                    if(abs(position.z-zwallPosition.z) <= 6 && abs(position.x - zwallPosition.x)  <= 2)
                    {
                        return COLLIDED_WITH_ZWALL;
                    }
                    
                }
            }
            return NO_COLLISION;
        }

        // When the state exits, it should call this function to ensure the mouse is unlocked
        void exit(){
            if(mouse_locked) {
                mouse_locked = false;
                app->getMouse().unlockMouse(app->getWindow());
            }
        }

    };

}