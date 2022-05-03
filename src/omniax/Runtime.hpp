#ifndef __RUNTIME_HPP__
#define __RUNTIME_HPP__

/**
 * This file contains the includes for the whole
 * OmniaX base framework.
 * 
 * This header needs to be included before creating,
 * your main application, In order to properly
 * setup GLAD, GLFW and imgui.
 * NOTE: This header needs to be included ONLY ONCE,
 *       and only when you create your main application
 *       which extends ox::Application.
 *       If you want to include the whole OmniaX
 *       base framework in other files other than
 *       your main application file, include in
 *       those files the <omniax/OmniaX.hpp> header
 *       instead.
**/

#include <omniax/runtime/Application.hpp>
#include <omniax/OmniaX.hpp>

#endif