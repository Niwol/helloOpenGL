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
    Renderer(int width = 500, int height = 500);
    ~Renderer();

    /**
     * Adds the render object to the renderer
     */
//    void addRenderObject(std::shared_ptr<RenderObject> ro,
//                         bool printWarning = false);
//
//    /**
//     * Removes the render object from the renderer
//     */
//    void removeRenderObject(std::shared_ptr<RenderObject> ro,
//                            bool printWarning = false);
//
//    /**
//     * Adds the point light to the renderer
//     */
//    void addPointLight(std::shared_ptr<PointLight> pointLight,
//                       bool printWarning = false);
//
//    /**
//     * Removes the point light from the renderer
//     */
//    void removePointLight(std::shared_ptr<PointLight> pointLight,
//                          bool printWarning = false);

    /**
     * Sets the camera of the renderer
     */
    void setCamera(std::shared_ptr<Camera> cam);

    /**
     * Gieves the dimensions of the window to the renderer
     */
    void setDimension(int width, int height);

    /**
     * Sets up all shaders and draw all render objects attached to the renderer
     */
    void draw();

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
    // Temporary
//    std::map<uint, std::shared_ptr<RenderObject>> m_renderObjects;
//    std::map<uint, std::shared_ptr<PointLight>> m_pointLights;
    std::shared_ptr<Camera> m_camera;

    int m_width;
    int m_height;

    uint m_nextID;
    uint32_t m_shadowShaderID;

    std::unique_ptr<ShaderManager> m_shaderManager;
};
