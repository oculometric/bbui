#include "rendering.h"

using namespace BBUI;

Renderer::Renderer()
{
    backend.reset(new Backend_OpenGL(Font::getRobotoFont(), Texture::getDefaultSliceTexture(),
        Texture::getDefaultIconTexture()));
}

std::shared_ptr<Renderer::Text> Renderer::createText()
{
    auto object              = std::make_shared<Text>();
    object->backing.renderer = this->shared_from_this();
    // TODO: add the backing to a list of backings so that we can modify them when reallocation happens!!!
    return object;
}

std::shared_ptr<Renderer::NineSlice> Renderer::createNineSlice()
{
    auto object              = std::make_shared<NineSlice>();
    object->backing.renderer = this->shared_from_this();
    return object;
}

std::shared_ptr<Renderer::Icon> Renderer::createIcon()
{
    auto object              = std::make_shared<Icon>();
    object->backing.renderer = this->shared_from_this();
    return object;
}

std::shared_ptr<Renderer::Quad> Renderer::createQuad()
{
    auto object              = std::make_shared<Quad>();
    object->backing.renderer = this->shared_from_this();
    return object;
}

void Renderer::draw(std::shared_ptr<Window> window)
{
    if (source_modified) backend->mesh(vertices, indices);
    source_modified = false;
    // TODO: handle realloc upon max dead quads
    backend->bind(window);
    backend->draw(window);
}
