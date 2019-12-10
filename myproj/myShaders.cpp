#include "myShaders.h"
#include <iostream>


myShaders::myShaders()
{
}

myShaders::~myShaders()
{
	clear();
}

void myShaders::clear()
{
	for (size_t i = 0;i < all_shaders.size(); i++)
		delete all_shaders[i];
	all_shaders.clear();
}

void myShaders::addShader(myShader *shader, std::string name)
{
	if (shader == nullptr) return;

	byname[name] = all_shaders.size();
	all_shaders.push_back(shader);
}

myShader*& myShaders::operator[](const std::string name)
{
	assert(byname.count(name) != 0);
	
	return all_shaders[byname[name]];
}

myShader*& myShaders::operator[](const unsigned int index)
{
	assert(index >= 0 && index < size());

	return all_shaders[index];
}

size_t myShaders::size() const
{
	return all_shaders.size();
}
