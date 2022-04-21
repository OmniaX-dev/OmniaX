#ifndef __DATA_STRUCTURES_HPP__
#define __DATA_STRUCTURES_HPP__

#include <omniax/utils/Geometry.hpp>
#include <omniax/graphics/Color.hpp>
#include <omniax/core/GLBuffers.hpp>

namespace ox
{
    struct tVertex
    {
        Vec3 position;
        Color::FloatCol color;
        Vec2 texCoords;
        float texIndex;

        inline static VertexBufferLayout getVertexBufferLayout(void)
        {
            VertexBufferLayout layout;
            layout.push<float>(3);
            layout.push<float>(4);
            layout.push<float>(2);
            layout.push<float>(1);
            return layout;
        }
    };
}
 
 #endif
