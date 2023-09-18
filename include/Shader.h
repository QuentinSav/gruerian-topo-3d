#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>
  
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
  
class Shader
{
public:
    // the program ID
    unsigned int m_id;
  
    // Constructor: reads and builds the shader
    Shader(const std::string& vertexPath, const std::string& fragmentPath);
    ~Shader();
    
    // use/activate the shader
    void use();

    // Utility functions
    void set_uniform(const std::string& name, bool value) const;  
    void set_uniform(const std::string& name, int value) const;   
    void set_uniform(const std::string& name, float value) const;
    std::string parse_shader(const std::string& filepath);
    unsigned int compile_shader(unsigned int type, const std::string& source);
    unsigned int create_shader(const std::string& vertexShaderFilePath, const std::string& fragmentShaderFilePath);

};
  
#endif