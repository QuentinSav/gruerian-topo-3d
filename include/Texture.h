#ifndef TEXTURE_H
#define TEXTURE_H

#include <glad/glad.h>
  
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include <stb_image.h>

class Texture
{
public:
    // the program ID
    unsigned int m_id;
    unsigned int m_index;
  
    // Constructor: reads and builds the shader
    Texture(unsigned int index);
    
    // use/activate the shader
    void bind();

    // Utility functions
    void generate(const std::string name) const;  
};
  
#endif