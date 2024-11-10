#include "entity.hpp"
#include "../deserialize-utils.hpp"

#include <glm/gtx/euler_angles.hpp>

namespace our
{

    // This function computes and returns a matrix that represents this transform
    // Remember that the order of transformations is: Scaling, Rotation then Translation
    // HINT: to convert euler angles to a rotation matrix, you can use glm::yawPitchRoll
    glm::mat4 Transform::toMat4() const
    {
        // TODO: (Req 3) Write this function

        // // glm::mat4 matrix = glm::scale(glm::mat4(1.0f), scale);
        // // scale scale the matrix
        // glm::mat4 matrix = glm::scale(glm::mat4(1.0f), scale);

        // // glm::yawPitchRoll(rotation.y, rotation.x, rotation.z);
        // // rotate the matrix
        // matrix = matrix * glm::yawPitchRoll(rotation.y, rotation.x, rotation.z);

        // //  glm::mat4 matrix = glm::translate(glm::mat4(1.0f), translation);
        // // translate the matrix
        // matrix = glm::translate(matrix, position);

        glm::mat4 translationMat = glm::translate(glm::mat4(1.0f), position);
        glm::mat4 RotationMat = glm::yawPitchRoll(rotation.y, rotation.x, rotation.z);
        glm::mat4 scalingMat = glm::scale(glm::mat4(1.0f), scale);


        return translationMat * RotationMat * scalingMat;
        // return glm::mat4(1.0f);
    }

    // Deserializes the entity data and components from a json object
    void Transform::deserialize(const nlohmann::json &data)
    {
        position = data.value("position", position);
        rotation = glm::radians(data.value("rotation", glm::degrees(rotation)));
        scale = data.value("scale", scale);
    }

}