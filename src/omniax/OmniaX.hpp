#ifndef __OMNIA_X_HPP__
#define __OMNIA_X_HPP__

/** Third Party **/
#include <omniax/vendor/imgui/imgui.h>
#include <omniax/vendor/TermColor.hpp>
#include <omniax/vendor/FastNoiseLite.hpp>
#include <omniax/vendor/glm/glm.hpp>
#include <omniax/vendor/glm/gtc/matrix_transform.hpp>

/** Utils **/
#include <omniax/utils/Logger.hpp>
#include <omniax/utils/Defines.hpp>
#include <omniax/utils/Utils.hpp>
#include <omniax/utils/DataFile.hpp>
#include <omniax/utils/Random.hpp>

/** Core **/
#include <omniax/core/Errors.hpp>
#include <omniax/core/ResourceManager.hpp>
#include <omniax/core/GLBuffers.hpp>
#include <omniax/core/Renderer2D.hpp>
#include <omniax/core/DataStructures.hpp>
#include <omniax/core/OrthoCamera.hpp>
#include <omniax/core/Input.hpp>
#include <omniax/core/GameObject.hpp>
#include <omniax/core/Particles.hpp>
#include <omniax/core/Lighting.hpp>

/** Graphics **/
//#include <omniax/graphics/Animator.hpp>
#include <omniax/graphics/TileAnimation.hpp>
#include <omniax/graphics/BitmapFont.hpp>

/** Physics **/
#include <omniax/physics/PhysicsObject.hpp>

/** C++ Standard Library **/
#include <cmath>
#include <iostream>
#include <vector>
#include <fstream>

#endif