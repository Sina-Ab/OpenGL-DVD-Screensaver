#pragma once

#include <glad/glad.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>



class Shader {
public:
    unsigned int ID;

    Shader(const char* vertexPath, const char* fragmentPath){
        std::string vertexCode;
        std::string fragmentCode;
        std::ifstream vShaderFile;
        std::ifstream fShaderFile;

        vShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
        fShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
        try {
            // open files
            vShaderFile.open(vertexPath);
            fShaderFile.open(fragmentPath);
            std::stringstream vShaderStream, fShaderStream;
            // read file's buffer contents into streams
            vShaderStream << vShaderFile.rdbuf();
            fShaderStream << fShaderFile.rdbuf();
            // close file handlers
            vShaderFile.close();
            fShaderFile.close();
            // convert stream into string
            vertexCode   = vShaderStream.str();
            fragmentCode = fShaderStream.str();
        }
        catch(std::ifstream::failure e) {
            std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
        }
        const char* vShaderCode = vertexCode.c_str();
        const char* fShaderCode = fragmentCode.c_str();

        // --Shader compile
        unsigned int vShader;
        unsigned int fShader;

        vShader = glCreateShader(GL_VERTEX_SHADER);
        fShader = glCreateShader(GL_FRAGMENT_SHADER);

        glShaderSource(vShader, 1, &vShaderCode, NULL);
        glCompileShader(vShader);
        glShaderSource(fShader, 1, &fShaderCode, NULL);
        glCompileShader(fShader);

        // --Shader link
        ID = glCreateProgram();

        glAttachShader(ID, vShader);
        glAttachShader(ID, fShader);

        glLinkProgram(ID);

        // --Check
        int  success[3];
        char infoLog[512];
        glGetShaderiv(vShader, GL_COMPILE_STATUS, &success[0]);
        glGetShaderiv(fShader, GL_COMPILE_STATUS, &success[1]);
        glGetProgramiv(ID, GL_LINK_STATUS, &success[2]);
        if(!success[0])
        {
            glGetShaderInfoLog(vShader, 512, NULL, infoLog);
            std::cout << "(Shader.hpp) ERROR::SHADER::VERTEX_SHADER::COMPILATION_FAILED\n" << infoLog << std::endl;
        } else {
            std::cout << "(Shader.hpp) VERTEX_SHADER compiled successfully" << std::endl;
        }
        if(!success[1])
        {
            glGetShaderInfoLog(fShader, 512, NULL, infoLog);
            std::cout << "(Shader.hpp) ERROR::SHADER::FRAGMENT_SHADER::COMPILATION_FAILED\n" << infoLog << std::endl;
        } else {
            std::cout << "(Shader.hpp) FRAGMENT_SHADER compiled successfully" << std::endl;
        }
        if(!success[2]) {
            glGetProgramInfoLog(ID, 512, NULL, infoLog);
            std::cout << "(Shader.hpp) ERROR::SHADER::Link_FAILED\n" << infoLog << std::endl;
        } else {
            std::cout << "(Shader.hpp) Shaders linked successfully" << std::endl;
        }

        // --Delete
        glDeleteShader(vShader);
        glDeleteShader(fShader);
    }


    void use(){
        glUseProgram(ID);
    }
    void setBool(const std::string &name, GLboolean value) const
    {
        glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
    }
    void setInt(const std::string &name, GLint value) const
    {
        glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
    }
    void setFloat(const std::string &name, GLfloat value) const
    {
        glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
    }
    void setFloat4v(const std::string &name, GLfloat* value) const
    {
        glUniform4fv(glGetUniformLocation(ID, name.c_str()),1,value);
    }
    void setMatFloat4(const std::string &name, GLboolean transpose, GLfloat* value) const
    {
        glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, transpose, value);
    }
};
