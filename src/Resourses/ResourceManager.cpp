#include "ResourceManager.h"
#include "../Renderer/ShaderProgram.h"
#include<sstream>
#include<fstream>
#include <iostream>
#define STB_IMAGE_IMPLEMENTATION
#define  STBI_ONLY_PNG
#include "stb_image.h"
ResourceManager::ResourceManager(const std::string& executablePath) 
{
	size_t found =executablePath.find_last_of("/\\");
	m_path = executablePath.substr(0, found);


}

const std::string ResourceManager:: getFileString(const std::string& relativeFilePath)
{
	std::ifstream f;
	f.open( m_path + "/" + relativeFilePath.c_str(), std::ios::in | std::ios::binary);
	if (!f.is_open()) 
	{
		std::cerr << "Failed to open file: " << relativeFilePath << std::endl;
		return std::string{};
	}

	std::stringstream buffer;
	buffer << f.rdbuf();
	return buffer.str();
}
std::shared_ptr<Renderer::ShaderProgram> ResourceManager::loadShaders(const std::string& shaderName, const std::string& vertexPath, const std::string& fragmentPath)
{
	std::string vertexString = getFileString(vertexPath);
	if (vertexString.empty())
	{
		std::cerr << "No vertex shader!" << std::endl;
		return nullptr;
	}

	std::string fragmentString = getFileString(fragmentPath);
	if (fragmentString.empty())
	{
		std::cerr << "No vertex shader!" << std::endl;
		return nullptr;
	}

	
	
	
	std::shared_ptr<Renderer::ShaderProgram>&newShader=m_ShaderPrograms.emplace(shaderName, std::make_shared<Renderer::ShaderProgram>(vertexString, fragmentString)).first->second;
	if (newShader->isCompiled())
	{
		return newShader;
	}

	std::cerr << "Can/t load shader program\n"
		<< "Vertex: " << vertexPath << "\n"
		<< "Fragment: " << fragmentPath << std::endl;
	return nullptr;

}

std::shared_ptr<Renderer::ShaderProgram>ResourceManager::getShaderProgram(const std::string& shaderName)
{
	ShaderProgramMap::const_iterator it = m_ShaderPrograms.find(shaderName);
	if (it != m_ShaderPrograms.end())
	{
		return it->second;
	}
	std::cerr << "Can't find the shader program: " << shaderName << std::endl;
	return nullptr;
}
void ResourceManager::loadTexture(const std::string& textureName, const std::string& texturePath) 
{
	int channels = 0;
	int width = 0;
	int height = 0;

	stbi_set_flip_vertically_on_load(true);
	unsigned char*pixels=stbi_load(std::string(m_path + "/" + texturePath).c_str(), &width, &height, &channels, 0);

	if (!pixels) 
	{
		std::cerr << "Can't load image: " << texturePath << std::endl;
		return;
	}
	stbi_image_free(pixels);
}
