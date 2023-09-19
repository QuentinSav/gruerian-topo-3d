#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>
  
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
  
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
    void set_uniform(const std::string &name, const glm::vec2 &value) const;
    void set_uniform(const std::string &name, float x, float y) const;
    void set_uniform(const std::string &name, const glm::vec3 &value) const;
    void set_uniform(const std::string &name, float x, float y, float z) const;
    void set_uniform(const std::string &name, const glm::vec4 &value) const;
    void set_uniform(const std::string &name, float x, float y, float z, float w) const;
    void set_uniform(const std::string &name, const glm::mat2 &mat) const;
    void set_uniform(const std::string &name, const glm::mat3 &mat) const;
    void set_uniform(const std::string &name, const glm::mat4 &mat) const;

    std::string parse_shader(const std::string& filepath);
    unsigned int compile_shader(unsigned int type, const std::string& source);
    unsigned int create_shader(const std::string& vertexShaderFilePath, const std::string& fragmentShaderFilePath);

};
  
#endif