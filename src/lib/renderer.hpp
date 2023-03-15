#pragma once

#include "RenderObject.hpp"
#include "ShaderProgram.hpp"
#include "Camera.hpp"
#include "Scene.hpp"
#include "lib/ShaderManager.hpp"
#include "lib/Light/Light.hpp"

#include <cstdint>
#include <map>
#include <memory>
#include <vector>

enum class RenderMode
{
    Default,
    Normal,
    Depth,
};


class Renderer {
public:
    Renderer();
    ~Renderer();

    void render(const Scene& scene, const Camera& camera, 
                RenderMode renderMode);

private:
    void defaultRender(const Scene& scene, const Camera& camera);
    void normalRender(const Scene& scene, const Camera& camera);
    void depthRender(const Scene& scene, const Camera& camera);

    void zPrepass(const Scene& scene, const Camera& camera);
    void shadowVolumeIntoStencil(const Scene& scene, 
                                 std::shared_ptr<Light> light, 
                                 const Camera& camera);
    void ambientRender(const Scene& scene, const Camera& camera);

    void setCameraUniforms(const Camera& camera, ShaderProgram& shader);
    void drawObject(RenderObject& object);


private:
    uint32_t m_shadowShaderID;
    std::unique_ptr<ShaderManager> m_shaderManager;
};
