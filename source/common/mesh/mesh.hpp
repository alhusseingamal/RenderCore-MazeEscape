#pragma once

#include <glad/gl.h>
#include "vertex.hpp"

namespace our
{

#define ATTRIB_LOC_POSITION 0
#define ATTRIB_LOC_COLOR 1
#define ATTRIB_LOC_TEXCOORD 2
#define ATTRIB_LOC_NORMAL 3

    class Mesh
    {
        // Here, we store the object names of the 3 main components of a mesh:
        // A vertex array object, A vertex buffer and an element buffer
        unsigned int VBO, EBO;
        unsigned int VAO;
        // We need to remember the number of elements that will be draw by glDrawElements
        GLsizei elementCount;

    public:
        // The constructor takes two vectors:
        // - vertices which contain the vertex data.
        // - elements which contain the indices of the vertices out of which each rectangle will be constructed.
        // The mesh class does not keep a these data on the RAM. Instead, it should create
        // a vertex buffer to store the vertex data on the VRAM,
        // an element buffer to store the element data on the VRAM,
        // a vertex array object to define how to read the vertex & element buffer during rendering
        Mesh(const std::vector<Vertex> &vertices, const std::vector<unsigned int> &elements)
        {
            // TODO: (Req 2) Write this function
            //  remember to store the number of elements in "elementCount" since you will need it for drawing
            //  For the attribute locations, use the constants defined above: ATTRIB_LOC_POSITION, ATTRIB_LOC_COLOR, etc
            elementCount = (int)elements.size();
            // Generate a Vertex Array Object (VAO) and bind it
            glGenVertexArrays(1, &VAO);
            glBindVertexArray(VAO);

            // Generate a Vertex Buffer Object (VBO) and bind it
            glGenBuffers(1, &VBO);
            glBindBuffer(GL_ARRAY_BUFFER, VBO);

            // Upload the vertex data to the VBO
            glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

            // Generate an Element Buffer Object (EBO) and bind it
            glGenBuffers(1, &EBO);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

            // Upload the element (index) data to the EBO
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, elements.size() * sizeof(unsigned int), elements.data(), GL_STATIC_DRAW);

            // Set up the vertex attribute pointer for the position attribute
            // ATTRIB_LOC_POSITION: The location of the position attribute in the shader
            // 3: The number of components per vertex attribute (vec3)
            // GL_FLOAT: The data type of each component (float)
            // GL_FALSE: Do not normalize the data
            // sizeof(Vertex): The byte offset between consecutive vertex attributes
            // (void*)offsetof(Vertex, position): The offset of the first component of the position attribute in the buffer
            glVertexAttribPointer(ATTRIB_LOC_POSITION, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, position));

            // make it available to the shader
            glEnableVertexAttribArray(ATTRIB_LOC_POSITION);
            // Set up the vertex attribute pointer for the color attribute
            // we normalize the color data as the shader expects it to be in [0, 1] range
            glVertexAttribPointer(ATTRIB_LOC_COLOR, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex), (void *)offsetof(Vertex, color));
            glEnableVertexAttribArray(ATTRIB_LOC_COLOR);

            glVertexAttribPointer(ATTRIB_LOC_TEXCOORD, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, tex_coord));
            glEnableVertexAttribArray(ATTRIB_LOC_TEXCOORD);

            glVertexAttribPointer(ATTRIB_LOC_NORMAL, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, normal));
            glEnableVertexAttribArray(ATTRIB_LOC_NORMAL);
            // Unbind the VAO to disable changes to it (it's a good thing to do this so we don't accidentally change it)
            glBindVertexArray(0);
        }

        // this function should render the mesh
        void draw()
        {
            // TODO: (Req 2) Write this function
            //  Bind the VAO
            glBindVertexArray(VAO);
            //  Call glDrawElements with the correct arguments
            glDrawElements(GL_TRIANGLES, elementCount, GL_UNSIGNED_INT, 0);
            //  Unbind the VAO after drawing
            glBindVertexArray(0);
        }

        // this function should delete the vertex & element buffers and the vertex array object
        ~Mesh()
        {
            // TODO: (Req 2) Write this function
            //  Delete the VBO, EBO, and VAO
            glDeleteBuffers(1, &VBO);
            glDeleteBuffers(1, &EBO);
            glDeleteVertexArrays(1, &VAO);
        }

        Mesh(Mesh const &) = delete;
        Mesh &operator=(Mesh const &) = delete;
    };

}