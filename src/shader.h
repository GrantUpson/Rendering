#pragma once
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>


class Shader
{
public:
    unsigned int ID;

    Shader(const char* vertexPath, const char* fragmentPath);

    void Use() const;
    void SetBool(const std::string &name, bool value) const;
    void SetInt(const std::string &name, int value) const;
    void SetFloat(const std::string &name, float value) const;

private:
    void CheckCompileErrors(unsigned int shader, std::string type);
};