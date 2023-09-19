#include "Shader.h"

Shader::Shader(const std::string& vertexPath, const std::string& fragmentPath)
{
    m_id = create_shader(vertexPath, fragmentPath);
}

Shader::~Shader()
{
    glDeleteProgram(m_id);
}

void Shader::use()
{
    glUseProgram(m_id);
}

void Shader::set_uniform(const std::string& name, bool value) const
{
    glUniform1i(glGetUniformLocation(m_id, name.c_str()), (int)value); 
}

void Shader::set_uniform(const std::string& name, int value) const
{
    glUniform1i(glGetUniformLocation(m_id, name.c_str()), value); 
}

void Shader::set_uniform(const std::string& name, float value) const
{
    glUniform1f(glGetUniformLocation(m_id, name.c_str()), value);
}

void Shader::set_uniform(const std::string& name, const glm::vec2 &value) const
{ 
    glUniform2fv(glGetUniformLocation(m_id, name.c_str()), 1, &value[0]); 
}
void Shader::set_uniform(const std::string& name, float x, float y) const
{ 
    glUniform2f(glGetUniformLocation(m_id, name.c_str()), x, y); 
}
// ------------------------------------------------------------------------
void Shader::set_uniform(const std::string& name, const glm::vec3 &value) const
{ 
    glUniform3fv(glGetUniformLocation(m_id, name.c_str()), 1, &value[0]); 
}
void Shader::set_uniform(const std::string& name, float x, float y, float z) const
{ 
    glUniform3f(glGetUniformLocation(m_id, name.c_str()), x, y, z); 
}
// ------------------------------------------------------------------------
void Shader::set_uniform(const std::string& name, const glm::vec4 &value) const
{ 
    glUniform4fv(glGetUniformLocation(m_id, name.c_str()), 1, &value[0]); 
}
void Shader::set_uniform(const std::string& name, float x, float y, float z, float w) const
{ 
    glUniform4f(glGetUniformLocation(m_id, name.c_str()), x, y, z, w); 
}
// ------------------------------------------------------------------------
void Shader::set_uniform(const std::string& name, const glm::mat2 &mat) const
{
    glUniformMatrix2fv(glGetUniformLocation(m_id, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}
// ------------------------------------------------------------------------
void Shader::set_uniform(const std::string& name, const glm::mat3 &mat) const
{
    glUniformMatrix3fv(glGetUniformLocation(m_id, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}
// ------------------------------------------------------------------------
void Shader::set_uniform(const std::string& name, const glm::mat4 &mat) const
{
    glUniformMatrix4fv(glGetUniformLocation(m_id, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

std::string Shader::parse_shader(const std::string& filepath)
{
    std::ifstream stream(filepath);
    std::string line;
    std::stringstream shaderString;
    
    while (getline(stream, line))
    {
        shaderString << line << std::endl;
    }

    return shaderString.str();
}

unsigned int Shader::compile_shader(unsigned int type, const std::string& source)
{
    unsigned int id = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    // Check compilation status of the shader
    int  success;
    char infoLog[512];
    glGetShaderiv(id, GL_COMPILE_STATUS, &success);
    
    if(!success)
    {
        glGetShaderInfoLog(id, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::COMPILATION_FAILED\n" << infoLog << std::endl;
        glDeleteShader(id);
        return 0;
    }

    return id;
}

unsigned int Shader::create_shader(const std::string& vertexShaderFilePath, const std::string& fragmentShaderFilePath)
{   
    std::string vertexShader = parse_shader(vertexShaderFilePath);
    std::string fragmentShader = parse_shader(fragmentShaderFilePath);

    unsigned int program = glCreateProgram();
    unsigned int vs = compile_shader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = compile_shader(GL_FRAGMENT_SHADER, fragmentShader);
    
    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);

    // Check linking status of the program
    int  success;
    char infoLog[512];
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    
    if(!success) {
        glGetProgramInfoLog(program, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }

    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}