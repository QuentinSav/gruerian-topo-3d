#include "Mesh.h"

Mesh::Mesh()
{

}

Mesh::~Mesh()
{
    // Need to free memory of ptr_dataset
    //GDALClose(static_cast<GDALDatasetH>(m_ptr_dataset));

    m_vertices.clear();

    glDeleteVertexArrays(1, &m_VAO);
    glDeleteBuffers(1, &m_VBO);
}

void Mesh::loadPredefinedVertices()
{
    m_vertices = {        
        {-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f},
        { 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f}, 
        { 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f}, 
        { 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f}, 
        {-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f}, 
        {-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f}, 

        {-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f},
        { 0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f},
        { 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f},
        { 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f},
        {-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f},
        {-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f},

        {-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f},
        {-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f},
        {-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f},
        {-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f},
        {-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f},
        {-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f},

        { 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f},
        { 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f},
        { 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f},
        { 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f},
        { 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f},
        { 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f},

        {-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f},
        { 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f},
        { 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f},
        { 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f},
        {-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f},
        {-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f},

        {-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f},
        { 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f},
        { 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f},
        { 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f},
        {-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f},
        {-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f}
    };

    m_faces = {
        {0, 1, 2},
        {3, 4, 5},
        
        {6, 7, 8},
        {9, 10, 11},
        
        {12, 13, 14},
        {15, 16, 17},

        {18, 19, 20},
        {21, 22, 23},

        {24, 25, 26},
        {27, 28, 29},

        {30, 31, 32},
        {33, 34, 35}
    };
}

void Mesh::setup()
// TODO should be done in the renderer
{
    
    // Generate a buffer with unique ID VBO
    glGenVertexArrays(1, &m_VAO); 
    glGenBuffers(1, &m_VBO);  
    glGenBuffers(1, &m_EBO); 
    
    //Bind VAO first
    glBindVertexArray(m_VAO);
    //Bind the created buffer to the GL_ARRAY_BUFFER
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
    
    // Copy the vertex data into buffer's memory
    glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(Vertex), m_vertices.data(), GL_STATIC_DRAW);  
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_faces.size() * sizeof(Face), m_faces.data(), GL_STATIC_DRAW);
    
    // Positions attributes
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Normal attributes
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3* sizeof(float)));
    glEnableVertexAttribArray(1);
    
    glBindVertexArray(0); 
}

void Mesh::link(Shader* shader)
{
    m_shaders.push_back(shader);
}

std::vector<Shader*> Mesh::getLinkedShaders()
{
    return m_shaders;
}

void Mesh::link(Texture* texture)
{
    m_textures.push_back(texture);
}

std::vector<Texture*> Mesh::getLinkedTextures()
{
    return m_textures;
}

void Mesh::addTransform(glm::mat4 model_transform)
{
    m_model_transforms.push_back(model_transform);
}

std::vector<glm::mat4> Mesh::getModelTransforms()
{
    return m_model_transforms;
}

void Mesh::bindVertexArray()
// TODO should be done in the renderer
{
    glBindVertexArray(m_VAO);
}

void Mesh::draw()
{
    glDrawElements(GL_TRIANGLES, 3 * m_faces.size(), GL_UNSIGNED_INT, 0); 
}