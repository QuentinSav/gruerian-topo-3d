#include "Texture.h"

Texture::Texture(unsigned int index) :
    m_index(index)
{
    glGenTextures(1, &m_id);
}

void Texture::generate(const std::string filePath) const
{
    glBindTexture(GL_TEXTURE_2D, m_id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true); 
    unsigned char *data = stbi_load(filePath.c_str(), &width, &height, &nrChannels, 0);
    if (data)
    {   
        if (filePath.find(".jpg") != std::string::npos) 
        {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
        }
        else if (filePath.find(".png") != std::string::npos)
        {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
        }
        else 
        {
            std::cout << "Unsupported file format" << std::endl;    
        }
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);

}

void Texture::bind()
{
    glActiveTexture(GL_TEXTURE0 + m_index);
    glBindTexture(GL_TEXTURE_2D, m_id); 
}