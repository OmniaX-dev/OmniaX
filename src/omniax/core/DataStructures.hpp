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

    class Transform2D
    {
        public:
            float rotation { 0.0f };
            Vec2 translation { 0.0f, 0.0f };
            Vec2 scale { 1.0f, 1.0f };
            bool centeredOrigin { false };
    };
}
 
 #endif
