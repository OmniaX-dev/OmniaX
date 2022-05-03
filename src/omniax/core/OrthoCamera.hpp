#ifndef __ORTHO_CAMERA_HPP__
#define __ORTHO_CAMERA_HPP__

#include <omniax/utils/BaseObject.hpp>
#include <omniax/vendor/glm/glm.hpp>
#include <omniax/utils/Types.hpp>

namespace ox
{
    class OrthoCamera : public BaseObject
    {
        public:
            inline OrthoCamera(void) { invalidate(); }
            //inline OrthoCamera(float left, float right, float bottom, float top, float near = -1.0f, float far = 1.0f) { create(left, right, bottom, top, near, far); }
            OrthoCamera(float left, float right, float bottom, float top, float near = -1.0f, float far = 1.0f);
            OrthoCamera& create(float left, float ight, float bottom, float top, float near = -1.0f, float far = 1.0f);

            inline const glm::vec3& getPosition(void) const { return m_position; }
            inline void setPosition(const glm::vec3& pos) { m_position = pos; __update_view_proj_mat(); }

            inline float getRotation(void) const { return m_rotation; }
            inline void setRotation(float rot) { m_rotation = rot; __update_view_proj_mat(); }

            inline const glm::mat4& getProjectionMatrix(void) const { return m_projection; }
            inline const glm::mat4& getViewMatrix(void) const { return m_view; }
            inline const glm::mat4& getViewProjectionMatrix(void) const { return m_viewProj; }

            String toString(void) const override;

        private:
            void __update_view_proj_mat(void);

        private:
            glm::mat4 m_projection { 1.0f };
            glm::mat4 m_view { 1.0f };
            glm::mat4 m_viewProj { 1.0f };
            glm::vec3 m_position { 0.0f, 0.0f, 0.0f };
            float m_rotation { 0.0f };
    };

    typedef OrthoCamera Camera2D;
}

#endif