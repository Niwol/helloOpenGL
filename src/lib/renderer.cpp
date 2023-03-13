#include "renderer.hpp"
#include "fwd.hpp"
#include "lib/Camera.hpp"
#include "lib/RenderObject.hpp"
#include "lib/Scene.hpp"
#include "lib/ShaderManager.hpp"
#include "lib/ShaderProgram.hpp"
#include <GL/gl.h>
#include <GL/glext.h>
#include <cstdint>
#include <cstdio>
#include <memory>

Renderer::Renderer(int width, int height)
    : m_width{width}, m_height{height}, m_nextID{1} 
{
    m_shaderManager = std::make_unique<ShaderManager>();

    auto shadowShader = std::make_shared<ShaderProgram>();
    shadowShader->setVertexShader("../src/shaders/Utils/shadowVolume.vs");
    shadowShader->setGeometryShader("../src/shaders/Utils/shadowVolume.gs");
    shadowShader->setFragmentShader("../src/shaders/Utils/shadowVolume.fs");

    shadowShader->createProgram(true);

    m_shadowShaderID = m_shaderManager->addShaderProgram(shadowShader, 
                                                         "ShadowVolume");
}

Renderer::~Renderer() {}

//void Renderer::addRenderObject(std::shared_ptr<RenderObject> ro, 
//                               bool printWarning) 
//{
//    static uint nextID = 1;
//    try {
//        m_renderObjects.at(ro->m_rendererID);
//
//        if (printWarning)
//            std::cout << "WARNING: Renderer::addRenderObject: renderer already "
//                "contains the render object"
//                << std::endl;
//    } catch (std::out_of_range &e) {
//        ro->m_rendererID = nextID;
//        m_renderObjects.insert(
//                std::pair<uint, std::shared_ptr<RenderObject>>(nextID, ro));
//        nextID++;
//    }
//}
//
//void Renderer::removeRenderObject(std::shared_ptr<RenderObject> ro,
//        bool printWarning) {
//    if (m_renderObjects.erase(ro->m_rendererID) == 0 && printWarning)
//        std::cout << "WARNING: Renderer::removeRenderObject: renderer didn't "
//            "contain the render object"
//            << std::endl;
//}
//
//void Renderer::addPointLight(std::shared_ptr<PointLight> pointLight,
//        bool printWarning) {
//
//    static uint nextID = 1;
//    try {
//        m_pointLights.at(pointLight->m_rendererID);
//
//        if (printWarning)
//            std::cout << "WARNING: Renderer::addRenderObject: renderer already "
//                "contains the point light"
//                << std::endl;
//    } catch (std::out_of_range &e) {
//        pointLight->m_rendererID = nextID;
//        m_pointLights.insert(
//                std::pair<uint, std::shared_ptr<PointLight>>(nextID, pointLight));
//        nextID++;
//    }
//}
//
//void Renderer::removePointLight(std::shared_ptr<PointLight> pointLight,
//        bool printWarning) {
//    if (m_pointLights.erase(pointLight->m_rendererID) == 0 && printWarning)
//        std::cout << "WARNING: Renderer::removeRenderObject: renderer didn't "
//            "contain the render object"
//            << std::endl;
//}

void Renderer::setCamera(std::shared_ptr<Camera> cam) { m_camera = cam; }
void Renderer::setDimension(int width, int height) {
    m_width = width;
    m_height = height;
}

void Renderer::draw() {

//    glm::mat4 perspective = glm::perspective(
//            glm::radians(45.0f), float(m_width) / float(m_height), 0.1f, 100.0f);
//
//    // Setting lights in their shader programs
//    int i = 0;
//    for (auto &pair : m_pointLights) {
//        auto pl = pair.second;
//        for (auto &sp : pl->m_shaderPrograms) {
//            sp->use();
//            sp->setVec3("pointLights[" + std::to_string(i) + "].position",
//                    pl->m_position);
//            sp->setVec3("pointLights[" + std::to_string(i) + "].ambient",
//                    pl->m_ambient);
//            sp->setVec3("pointLights[" + std::to_string(i) + "].diffuse",
//                    pl->m_diffuse);
//            sp->setVec3("pointLights[" + std::to_string(i) + "].specular",
//                    pl->m_specular);
//            sp->setInt("nbPointLights", i + 1);
//        }
//        i++;
//    }
//
//    // Drawing render objects
//    for (auto &pair : m_renderObjects) {
//        auto ro = pair.second;
//
//        auto shaderProgram = ro->getShaderProgram();
//        Mesh *mesh = ro->getMesh();
//
//        if (shaderProgram) {
//            shaderProgram->use();
//            shaderProgram->setMat4("model", ro->getModelMatrix());
//            shaderProgram->setMat4("view", m_camera->getViewMatrix());
//            shaderProgram->setMat4("projection", perspective);
//            shaderProgram->setVec3("viewPos", m_camera->getPosition());
//
//            std::shared_ptr<Material> m;
//            if ((m = ro->getMaterial())) {
//
//                // Set shader material
//                shaderProgram->setInt("material.shininess", m->shininess);
//
//                if (m->isTextureMaterial) {
//                    glActiveTexture(GL_TEXTURE0);
//                    glBindTexture(GL_TEXTURE_2D, m->diffuseTexture->m_textureID);
//                    shaderProgram->setInt("material.diffuse", 0);
//
//                    glActiveTexture(GL_TEXTURE1);
//                    glBindTexture(GL_TEXTURE_2D, m->specularTexture->m_textureID);
//                    shaderProgram->setInt("material.specular", 1);
//
//                } else {
//                    shaderProgram->setVec3("material.diffuse", m->diffuse);
//                    shaderProgram->setVec3("material.specular", m->specular);
//                    shaderProgram->setFloat("material.roughness", m->roughness);
//                    shaderProgram->setFloat("material.metallic", m->metallic);
//                }
//            }
//
//            glBindVertexArray(mesh->m_VAO);
//            glDrawElements(mesh->m_mode, mesh->m_nbIndices, GL_UNSIGNED_INT, 0);
//        } else {
//            std::cout
//                << "Renderer::draw: WARNING: render object has no shader program set"
//                << std::endl;
//        }
//    }
//    glBindVertexArray(0);
}

void Renderer::render(const Scene& scene, const Camera& camera,
                      RenderMode renderMode) 
{
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    switch (renderMode) 
    {
        case RenderMode::Default:
        {
            defaultRender(scene, camera);
        } break;

        case RenderMode::Normal:
        {
            normalRender(scene, camera);
        } break;

        case RenderMode::Depth:
        {
            depthRender(scene, camera);

        } break;
    }
}


void Renderer::defaultRender(const Scene& scene, const Camera& camera)
{
    glEnable(GL_STENCIL_TEST);

    // Z pepass
    glDepthMask(GL_TRUE);
    zPrepass(scene, camera);

    for(auto& light : scene.lights)
    {        
        shadowVolumeIntoStencil(scene, light, camera);

        glStencilFunc(GL_EQUAL, 0x0, 0xff);
        glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);

        glDepthFunc(GL_EQUAL);
        glEnable(GL_BLEND);
        glBlendFunc(GL_ONE, GL_ONE);

        for(auto& ro : scene.objects)
        {
            auto material = ro->getMaterial();
            uint32_t shaderID = material->getShaderID();

            auto shader = m_shaderManager->getShader(shaderID).value();
            
            shader->use();

            setCameraUniforms(camera, *shader);
            light->setShaderUniforms(*shader);
            material->setShaderUniforms(*shader);
            shader->setMat4("model", ro->getModelMatrix());

            drawObject(*ro);
        }

        glDisable(GL_BLEND);
        glDepthFunc(GL_LESS);
        glClear(GL_STENCIL_BUFFER_BIT);
    }

    ambientRender(scene, camera);

    glBindVertexArray(0);
}

void Renderer::normalRender(const Scene& scene, const Camera& camera)
{
    auto shader = m_shaderManager->getShader(DefaultShaders::Shader_Normal).value();

    shader->use();

    setCameraUniforms(camera, *shader);

    for(auto& obj : scene.objects)
    {
        shader->setMat4("model", obj->getModelMatrix());
        drawObject(*obj);
    }
}

void Renderer::depthRender(const Scene& scene, const Camera& camera)
{
    auto shader = m_shaderManager->getShader(DefaultShaders::Shader_Depth).value();

    shader->use();

    setCameraUniforms(camera, *shader);

    for(auto& obj : scene.objects)
    {
        shader->setMat4("model", obj->getModelMatrix());
        drawObject(*obj);
    }
}

void Renderer::zPrepass(const Scene& scene, const Camera& camera)
{
    //glDrawBuffer(GL_NONE);

    auto shader = m_shaderManager->getShader(DefaultShaders::Shader_Black).value();

    shader->use();
    setCameraUniforms(camera, *shader);

    for(auto& obj : scene.objects)
    {
        shader->setMat4("model", obj->getModelMatrix());
        drawObject(*obj);
    }

    glBindVertexArray(0);
}

void Renderer::shadowVolumeIntoStencil(const Scene& scene, 
                                       std::shared_ptr<Light> light,
                                       const Camera& camera)
{
    glDrawBuffer(GL_NONE);

    glDepthMask(GL_FALSE);
    glEnable(GL_DEPTH_CLAMP);
    glDisable(GL_CULL_FACE);

    glStencilFunc(GL_ALWAYS, 0, 0xff);

    glStencilOpSeparate(GL_BACK, GL_KEEP, GL_INCR_WRAP, GL_KEEP);
    glStencilOpSeparate(GL_FRONT, GL_KEEP, GL_DECR_WRAP, GL_KEEP);

    auto shader = m_shaderManager->getShader(m_shadowShaderID).value();
    
    shader->use();

    setCameraUniforms(camera, *shader);
    light->setShaderUniforms(*shader);
    
    for(auto& obj : scene.objects)
    {
        shader->setMat4("model", obj->getModelMatrix());
        drawObject(*obj);
    }

    glDisable(GL_DEPTH_CLAMP);
    glEnable(GL_CULL_FACE);

    glDepthMask(GL_TRUE);
    glDrawBuffer(GL_BACK);
}

void Renderer::ambientRender(const Scene& scene, const Camera& camera)
{
    glDepthFunc(GL_EQUAL);
    glEnable(GL_BLEND);
    glBlendFunc(GL_ONE, GL_ONE);

    auto shader = m_shaderManager->getShader(DefaultShaders::Shader_Ambient).value();

    shader->use();

    setCameraUniforms(camera, *shader);

    for(auto& light : scene.lights)
    {
        for(auto& obj : scene.objects)
        {
            shader->setVec3("lightColor", light->m_color);
            obj->getMaterial()->setShaderAmbient(*shader);
            shader->setMat4("model", obj->getModelMatrix());
            
            drawObject(*obj);
        }
    }

    glDisable(GL_BLEND);
    glDepthFunc(GL_LESS);
}

void Renderer::setCameraUniforms(const Camera& camera, ShaderProgram& shader)
{
    glm::mat4 perspective = glm::perspective(
            glm::radians(45.0f), 
            float(camera.getWidth()) / float(camera.getHeight()),
            0.1f, 
            500.0f);

    shader.setMat4("projection", perspective);
    shader.setMat4("view", camera.getViewMatrix());
    shader.setVec3("viewPos", camera.getPosition());
}

void Renderer::drawObject(RenderObject& object)
{
    auto mesh = object.getMesh();
    glBindVertexArray(mesh->m_VAO);
    glDrawElements(mesh->m_mode, mesh->m_nbIndices, GL_UNSIGNED_INT, 0);
}

