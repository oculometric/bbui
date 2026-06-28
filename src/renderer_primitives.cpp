#include "rendering.h"

using namespace BBUI;

void Renderer_t::Primitive::setPosition(glm::vec3 _position)
{
    position = _position;
    update();
}

void Renderer_t::Primitive::setSize(glm::vec2 _size)
{
    size = _size;
    update();
}

void Renderer_t::Primitive::update()
{
    backing.ensure(1);
    glm::vec2 p = position;
    backing.write(p, { p.x + size.x, p.y }, { p.x, p.y + size.y }, p + size, position.z, { 0, 0 }, { 1, 1 },
        colour_a, colour_b, { 4, 0, 0, 0 }, { 0, 0, 0, 0 });
}

void Renderer_t::Text_t::setForeground(glm::vec4 _colour)
{
    colour_a = _colour;
    update();
}

void Renderer_t::Text_t::setBackground(glm::vec4 _colour)
{
    colour_b = _colour;
    update();
}

void Renderer_t::Text_t::setText(const std::string& _content)
{
    content = _content;
    update();
}

void Renderer_t::Text_t::setFormat(const Format& _format)
{
    format = _format;
    update();
}

void Renderer_t::NineSlice_t::setFill(glm::vec4 _colour)
{
    colour_a = _colour;
    update();
}

void Renderer_t::NineSlice_t::setBorder(glm::vec4 _colour)
{
    colour_b = _colour;
    update();
}

void Renderer_t::NineSlice_t::setPattern(int _pattern_index)
{
    pattern_index = _pattern_index;
    update();
}

void Renderer_t::NineSlice_t::setBorders(Borders _borders)
{
    borders = _borders;
    update();
}

void Renderer_t::NineSlice_t::update()
{
    backing.ensure(1);
    glm::vec2 p = position;
    backing.write(p, { p.x + size.x, p.y }, { p.x, p.y + size.y }, p + size, position.z, { 0, 0 }, { 1, 1 },
        colour_a, colour_b, { 1, size, static_cast<float>(pattern_index) },
        { static_cast<float>(borders), 0, 0, 0 });
}

void Renderer_t::Icon_t::setForeground(glm::vec4 _colour)
{
    colour_a = _colour;
    update();
}

void Renderer_t::Icon_t::setBackground(glm::vec4 _colour)
{
    colour_b = _colour;
    update();
}

void Renderer_t::Icon_t::setIconIndex(int _icon_index)
{
    icon_index = _icon_index;
    update();
}

void Renderer_t::Icon_t::update()
{
    backing.ensure(1);
    glm::vec2 p = position;
    backing.write(p, { p.x + size.x, p.y }, { p.x, p.y + size.y }, p + size, position.z, { 0, 0 }, { 1, 1 },
        colour_a, colour_b, { 1, size, static_cast<float>(icon_index) }, { 0, 0, 0, 0 });
}

void Renderer_t::Quad_t::setColourA(glm::vec4 _colour_a)
{
    colour_a = _colour_a;
    update();
}

void Renderer_t::Quad_t::setColourB(glm::vec4 _colour_b)
{
    colour_b = _colour_b;
    update();
}

void Renderer_t::Quad_t::setUVTopLeft(glm::vec2 _uv_top_left)
{
    uv_top_left = _uv_top_left;
    update();
}

void Renderer_t::Quad_t::setUVBottomRight(glm::vec2 _uv_bottom_right)
{
    uv_bottom_right = _uv_bottom_right;
    update();
}

void Renderer_t::Quad_t::setData1(glm::vec4 _data_1)
{
    data_1 = _data_1;
    update();
}

void Renderer_t::Quad_t::setData2(glm::vec4 _data_2)
{
    data_2 = _data_2;
    update();
}

void Renderer_t::Quad_t::update()
{
    backing.ensure(1);
    glm::vec2 p = position;
    backing.write(p, { p.x + size.x, p.y }, { p.x, p.y + size.y }, p + size, position.z, uv_top_left,
        uv_bottom_right, colour_a, colour_b, data_1, data_2);
}
