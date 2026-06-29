#include "canvas.h"

#include "rendering.h"

using namespace BBUI;

Canvas_t::Canvas_t()
{
    renderer = std::make_shared<Renderer_t>();
    root     = std::make_shared<Component_t>();
}

void Canvas_t::update() { root->updateSelfAndChildren(); }

void Canvas_t::draw(Window window)
{
    if (!has_size_override)
    {
        if (glm::vec2(window->getSize()) != size_override)
        {
            size_override = window->getSize();
            arrangeComponents();
        }
    }

    renderer->draw(window);
}

void Canvas_t::setStyle(Style new_style)
{
    style = new_style;
    renderer->clear();
    arrangeComponents();
}

void Canvas_t::setSizeOverride(glm::vec2 size)
{
    size_override     = size;
    has_size_override = true;
    arrangeComponents();
}

void Canvas_t::clearSizeOverride()
{
    has_size_override = false;
    arrangeComponents();
}

void Canvas_t::redrawComponents()
{
    renderer->clear();
    root->redrawSelfAndChildren();
}

void Canvas_t::arrangeComponents()
{
    root->transform.position = { 0, 0 };
    root->transform.size     = size_override;
    root->arrangeSelfAndChildren();
}
