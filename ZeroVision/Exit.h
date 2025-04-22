#pragma once

#include "Scene.h"
#include <SFML/Graphics/CircleShape.hpp>
#include "GlobalVariables.h"

class Exit : public Scene {
public:
	Exit() {
		GlobalVariables::window->close();
	}
};