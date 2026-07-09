#include "rendering.h"

#include <cstring>

using namespace BBUI;

void Renderer_t::Backing::ensure(Index capacity)
{
    auto r = renderer.lock();
    if (!r) return;
    capacity = glm::max(capacity, 1u);
    auto it  = r->backing_map.find(id);
    if (it == r->backing_map.end())
    {
        id                 = r->next_backing_id++;
        r->backing_map[id] = BackingInternal{ 0, 0, 0 };
        it                 = r->backing_map.find(id);
    }
    if (capacity < it->second.quad_count)
    {
        Index difference  = it->second.quad_count - capacity;
        Index start_index = it->second.vertex_start + (capacity * 4);
        memset(r->vertices.data() + (start_index * sizeof(Vertex)), 0, difference * sizeof(Vertex));

        r->source_modified = true;
    }
    else if (capacity > it->second.quad_count)
    {
        Index count       = it->second.quad_count;
        Index start_index = it->second.vertex_start;
        memset(r->vertices.data() + (start_index * sizeof(Vertex)), 0, (count * 4) * sizeof(Vertex));
        memset(r->indices.data() + (it->second.index_start * sizeof(Index)), 0xFF,
            (count * 6) * sizeof(Index));
        for (auto v = r->vertices.begin() + start_index;
            v != r->vertices.begin() + start_index + (count * 4); ++v)
        {
            v->data_2 = { 0, 0, 0, -1 };
        }
        r->dead_quads += count;

        it->second.vertex_start = r->vertices.size();
        r->vertices.resize(r->vertices.size() + (capacity * 4));
        it->second.index_start = r->indices.size();
        r->indices.resize(r->indices.size() + (capacity * 6));
        it->second.quad_count = capacity;
        count                 = it->second.quad_count;
        start_index           = it->second.vertex_start;
        memset(r->vertices.data() + (start_index * sizeof(Vertex)), 0, (count * 4) * sizeof(Vertex));
        for (size_t i = it->second.index_start; i < it->second.index_start + (count * 6); i += 6)
        {
            r->indices[i + 0] = start_index + 0;
            r->indices[i + 1] = start_index + 3;
            r->indices[i + 2] = start_index + 1;
            r->indices[i + 3] = start_index + 0;
            r->indices[i + 4] = start_index + 2;
            r->indices[i + 5] = start_index + 3;
        }

        r->source_modified = true;
    }
}

void Renderer_t::Backing::write(glm::vec2 p1, glm::vec2 p2, glm::vec2 p3, glm::vec2 p4, float z,
    glm::vec2 uv_tl, glm::vec2 uv_br, glm::vec4 colour_1, glm::vec4 colour_2, glm::vec4 data_1,
    glm::vec4 data_2, Index offset)
{
    auto r = renderer.lock();
    if (!r) return;

    auto it = r->backing_map.find(id);
    if (it == r->backing_map.end()) return;
    if (offset >= it->second.quad_count) return;

    glm::vec3 _p1 = r->transform * glm::vec3{ p1, 1 };
    _p1.z         = z;
    glm::vec3 _p2 = r->transform * glm::vec3{ p2, 1 };
    _p2.z         = z;
    glm::vec3 _p3 = r->transform * glm::vec3{ p3, 1 };
    _p3.z         = z;
    glm::vec3 _p4 = r->transform * glm::vec3{ p4, 1 };
    _p4.z         = z;

    glm::vec2 uv_tr = { uv_br.x, uv_tl.y };
    glm::vec2 uv_bl = { uv_tl.x, uv_br.y };

    Index base            = it->second.vertex_start + (offset * 4);
    r->vertices[base + 0] = Vertex{ _p1, colour_1, colour_2, data_1, data_2, uv_tl };
    r->vertices[base + 1] = Vertex{ _p2, colour_1, colour_2, data_1, data_2, uv_tr };
    r->vertices[base + 2] = Vertex{ _p3, colour_1, colour_2, data_1, data_2, uv_bl };
    r->vertices[base + 3] = Vertex{ _p4, colour_1, colour_2, data_1, data_2, uv_br };
}

void Renderer_t::Backing::release()
{
    auto r = renderer.lock();
    if (!r) return;
    auto it = r->backing_map.find(id);
    if (it == r->backing_map.end()) return;

    Index count       = it->second.quad_count;
    Index start_index = it->second.vertex_start;
    memset(r->vertices.data() + (start_index * sizeof(Vertex)), 0x00, (count * 4) * sizeof(Vertex));
    memset(r->indices.data() + (it->second.index_start * sizeof(Index)), 0xFF, (count * 6) * sizeof(Index));
    for (auto v = r->vertices.begin() + start_index; v != r->vertices.begin() + start_index + (count * 4);
        ++v)
    {
        v->data_2 = { 0, 0, 0, -1 };
    }
    r->dead_quads += count;

    r->backing_map.erase(it);

    r->source_modified = true;
    id                 = 0;
}

Renderer_t::Renderer_t()
{
    backend.reset(new Backend_OpenGL(Font::getRobotoFont(), Texture::getDefaultSliceTexture(),
        Texture::getDefaultIconTexture()));
}

Renderer_t::Text Renderer_t::createText()
{
    auto object              = std::make_shared<Text_t>();
    object->backing.renderer = this->shared_from_this();
    return object;
}

Renderer_t::NineSlice Renderer_t::createNineSlice()
{
    auto object              = std::make_shared<NineSlice_t>();
    object->backing.renderer = this->shared_from_this();
    return object;
}

Renderer_t::Icon Renderer_t::createIcon()
{
    auto object              = std::make_shared<Icon_t>();
    object->backing.renderer = this->shared_from_this();
    return object;
}

Renderer_t::Quad Renderer_t::createQuad()
{
    auto object              = std::make_shared<Quad_t>();
    object->backing.renderer = this->shared_from_this();
    return object;
}

void Renderer_t::draw(Window window)
{
    // TODO: check if there are modified elements
    // TODO: check to see if their backing is valid or they want a different bigger backing size this time (and fix that first)
    // TODO: check if we need to realloc (max dead quads), in which case call clear
    // TODO: request new data from each one (if we realloced, or that specific one was modified), and clear the modified list


    // if (source_modified) backend->mesh(vertices, indices, shared_from_this());
    // source_modified = false;
    // if (next_backing_id == 0) next_backing_id = 3;
    // // TODO: handle realloc upon max dead quads - move everything backwards and modify the backing data to
    // // preserve the timeline
    backend->bind(window, shared_from_this());
    backend->draw(window);
}

void Renderer_t::clear()
{
    // vertices.clear();
    // indices.clear();
    // backing_map.clear();
    // source_modified = true;
}