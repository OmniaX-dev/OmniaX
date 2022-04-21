#include "OrthoCamera.hpp"
#include <omniax/vendor/glm/gtc/matrix_transform.hpp>
#include <omniax/utils/Utils.hpp>

namespace ox
{
    OrthoCamera& OrthoCamera::create(float left, float right, float bottom, float top, float near, float far)
    {
        m_projection = glm::ortho(left, right, bottom, top, near, far);
        __update_view_proj_mat();
        return *this;
    }

    String OrthoCamera::toString(void) const
    {
        StringEditor se = getTypeName();
        se.add(" position = { ");
        se.addf(m_position.x).add(", ");
        se.addf(m_position.y).add(", ");
        se.addf(m_position.z).add(" } rotation = { ").addf(m_rotation).add(" } ");
        return se.str();
    }

    void OrthoCamera::__update_view_proj_mat(void)
    {
        glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_position);
        transform = glm::rotate(transform, glm::radians(m_rotation), glm::vec3(0, 0, 1));

        m_view = glm::inverse(transform);
        m_viewProj = m_projection * m_view;

        setTypeName("ox::OrthoCamera");
        validate();
    }
}