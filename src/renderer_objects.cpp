#include "rendering.h"

using namespace BBUI;

void Renderer::Object::setPosition(glm::vec3 _position)
{
    position = _position;
    update();
}

void Renderer::Object::setSize(glm::vec2 _size)
{
    size = _size;
    update();
}

void Renderer::Object::update()
{
    backing.ensure(1);
    glm::vec2 p = position;
    backing.write(p, { p.x + size.x, p.y }, { p.x, p.y + size.y }, p + size, position.z, { 0, 0 }, { 1, 1 },
        colour_a, colour_b, { 4, 0, 0, 0 }, { 0, 0, 0, 0 });
}
