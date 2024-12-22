#pragma once

#include <application.hpp>

#include <ecs/world.hpp>
#include <systems/forward-renderer.hpp>
#include <systems/free-camera-controller.hpp>
#include <systems/player-controller.hpp>
#include <systems/movement.hpp>
#include <asset-loader.hpp>

// This state shows how to use the ECS framework and deserialization.
class Playstate: public our::State {

    our::World world;
    our::ForwardRenderer renderer;
    our::FreeCameraControllerSystem cameraController;
    our::MovementSystem movementSystem;
    our::playerControllerSystem playerController;

    int health, power_up, damage;
    int duration_minutes, duration_seconds, speeddown_factor, clock;
    int extra_time;
    bool extra_time_flag = false;
    bool damage_flag = false;
    bool power_up_flag = false;

    void onInitialize() override {
        // First of all, we get the scene configuration from the app config
        std::string config_path = "config/play_mode.jsonc";
        std::ifstream file_in(config_path);
        if(!file_in){
            std::cerr << "Couldn't open file: " << config_path << std::endl;
            return;
        }
        nlohmann::json app_config = nlohmann::json::parse(file_in, nullptr, true, true);
        auto& config = app_config["scene"];
        // If we have assets in the scene config, we deserialize them
        if(config.contains("assets")){
            our::deserializeAllAssets(config["assets"]);
        }
        // If we have a world in the scene config, we use it to populate our world
        if(config.contains("world")){
            world.deserialize(config["world"]);
        }
        // Initialize health
        health = app_config["health"];
        power_up = app_config["power-up"];
        damage = app_config["damage"];
        // Initialize timer
        duration_minutes = app_config["duration-minutes"], duration_seconds = app_config["duration-seconds"];
        speeddown_factor = app_config["speeddown-factor"], clock = speeddown_factor;
        extra_time = app_config["extra_time"];
        // We initialize the camera controller system since it needs a pointer to the app
        cameraController.enter(getApp());
        playerController.enter(getApp());
        // Then we initialize the renderer
        auto size = getApp()->getFrameBufferSize();
        renderer.initialize(size, config["renderer"]);
    }

    void addExtraTime() {
        extra_time_flag = true;
    }
    
    void addDamage() {
        damage_flag = true;
    }

    void addPowerUp() {
        power_up_flag = true;
    }

    void onDraw(double deltaTime) override {
        // Here, we just run a bunch of systems to control the world logic
        // Timer System
        std::string remaining_time = std::to_string(duration_minutes) + ":" +
                ((duration_seconds >= 10) ? std::to_string(duration_seconds) : "0" + std::to_string(duration_seconds));
        if (duration_minutes == 0 && duration_seconds <= 30 && duration_seconds % 2 == 0) {
            getApp()->printTextCenter(remaining_time, 1, 5, 255, 0, 0, 255);   // print in Red to indicate time is running out
        }
        else {
            getApp()->printTextCenter(remaining_time, 1, 5, 255, 255, 255, 255);   // print in White
        }
        if (duration_minutes == 0 && duration_seconds == 0) {
            getApp()->changeState("loss");
        }
        if (health <= 0) {
            getApp()->changeState("loss");
        }
        clock = (clock - 1 + speeddown_factor) % speeddown_factor;
        if (clock == 0) duration_seconds = (duration_seconds - 1 + 60) % 60;
        if (duration_seconds == 59 && clock == 0) duration_minutes--;

        // Add extra time if collected timeup
        if(extra_time_flag){
            if(duration_seconds + extra_time >= 60) {
                duration_seconds = (duration_seconds + extra_time) % 60;
                duration_minutes++;
            } else {
                duration_seconds = (duration_seconds + extra_time) % 60;
            }
            extra_time_flag = false;
        }

        // Health System
        std::string health_str = "Health: " + std::to_string(health);
        getApp()->printTextInBox(health_str, 0, 1, 5, 0, 0, 0, 255);

        // Add damage if encountered mine
        if(damage_flag){
            health -= damage;
            damage_flag = false;
        }

        if(power_up_flag){
            health = std::min(100, health + power_up); // the 100 should be replaced with the max health (not be hardcoded)
            power_up_flag = false;
        }

        // Update
        movementSystem.update(&world, (float)deltaTime);
        cameraController.update(&world, (float)deltaTime);
        playerController.update(&world, (float)deltaTime);
        // And finally we use the renderer system to draw the scene
        renderer.render(&world);

        // Get a reference to the keyboard object
        auto& keyboard = getApp()->getKeyboard();

        if(keyboard.justPressed(GLFW_KEY_ESCAPE)){
            // If the escape key is pressed in this frame, return to menu
            getApp()->changeState("menu");
        }
    }

    void onDestroy() override {
        // Don't forget to destroy the renderer
        renderer.destroy();
        // On exit, we call exit for the camera controller system to make sure that the mouse is unlocked
        cameraController.exit();
        // Clear the world
        world.clear();
        // and we delete all the loaded assets to free memory on the RAM and the VRAM
        our::clearAllAssets();
    }
};