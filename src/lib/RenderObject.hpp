#pragma once

#include "Geometry/mesh.hpp"
#include "ShaderProgram.hpp"
#include "fwd.hpp"
#include "lib/Materials/Material.hpp"
#include "lib/Materials/BlinnPhong.hpp"
#include "lib/Materials/SimpleMaterial.hpp"

#include "opengl_stuff.h"
#include <glm.hpp>

#include <memory>

class RenderObject {
    friend class Renderer;

public:
    RenderObject();
    ~RenderObject();

    /**
     * Applyes the transform matrix to the current modle matrix
     */
    void transform(glm::mat4 transform);

    /**
     * Resets the model matrix to an identity matrix
     */
    void resetModel();

    /**
     * This function takes ownership of the mesh and deletes it when it is
     * destroyed or if an other mesh is set
     */
    void setMesh(std::unique_ptr<Mesh> mesh);

    /**
     * Sets the texture for the render objects
     */
    inline void setMaterial(std::shared_ptr<Material> material) {
        m_material = material;
    }

    /**
     * Gets the texture of the render objects
     */
    inline std::shared_ptr<Material> getMaterial() { return m_material; }

    /**
     * Returns a raw pointer to the mesh owned by the render object
     */
    Mesh *getMesh();

    /**
     * Returns the model matrix of the render object
     */
    glm::mat4 getModelMatrix();

private:
    std::shared_ptr<Material> m_material;

    std::unique_ptr<Mesh> m_mesh;
    glm::mat4 m_modelMatix = glm::mat4(1.0f);

    // ID used by the renderer to keep track of the objects
    uint m_rendererID;
};
