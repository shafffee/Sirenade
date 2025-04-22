#pragma once
#include <SFML/Graphics.hpp>

class Scene {
public:
	Scene(){}
	virtual ~Scene() {}

	virtual void onUpdate() {};

};