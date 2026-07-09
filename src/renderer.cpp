#include "rendering.h"

#include <cstring>

using namespace BBUI;

Renderer_t::Renderer_t()
{
    backend.reset(new Backend_OpenGL(Font::getRobotoFont(), Texture::getDefaultSliceTexture(),
        Texture::getDefaultIconTexture()));
}

RectPrimitive Renderer_t::addRect()
{
    std::shared_ptr<RectPrimitive_t> primitive;
    primitive.reset(new RectPrimitive_t());
    configurePrimitive(std::static_pointer_cast<Primitive_t>(primitive));
    return primitive;
}

IconPrimitive Renderer_t::addIcon()
{
    std::shared_ptr<IconPrimitive_t> primitive;
    primitive.reset(new IconPrimitive_t());
    configurePrimitive(std::static_pointer_cast<Primitive_t>(primitive));
    return primitive;
}

TextPrimitive Renderer_t::addText()
{
    std::shared_ptr<TextPrimitive_t> primitive;
    primitive.reset(new TextPrimitive_t());
    configurePrimitive(std::static_pointer_cast<Primitive_t>(primitive));
    return primitive;
}

QuadPrimitive Renderer_t::addQuad()
{
    std::shared_ptr<QuadPrimitive_t> primitive;
    primitive.reset(new QuadPrimitive_t());
    configurePrimitive(std::static_pointer_cast<Primitive_t>(primitive));
    return primitive;
}

glm::vec2 Renderer_t::getGlyphSize() const { return backend->getFontGlyphSize(); }

void Renderer_t::draw(Window window)
{
    bool realloc_ocurred  = false;
    size_t new_dead_quads = dead_quads;
    for (const uint64_t primitive_id : modified_primitives)
    {
        if (primitives[primitive_id].first.lock()->getQuadCount() >
            primitives[primitive_id].second.quad_count)
            new_dead_quads += primitives[primitive_id].second.quad_count;
    }
    if (new_dead_quads > max_dead_quads)
    {
        clear();
        realloc_ocurred = true;
    }

    bool source_modified = realloc_ocurred || !modified_primitives.empty();
    for (const auto primitive_set : primitives)
    {
        if (modified_primitives.contains(primitive_set.first) || realloc_ocurred)
        {
            auto geometry = primitive_set.second.first.lock()->getGeometry();
            ensureBacking(primitive_set.first, geometry.size());
            memcpy(vertices.data() + primitive_set.second.second.vertex_start, geometry.data(),
                geometry.size() * sizeof(Vertex));
        }
    }
    modified_primitives.clear();

    if (source_modified) backend->mesh(vertices, indices, shared_from_this());
    if (next_primitive_id == 0) next_primitive_id = 3;
    backend->bind(window, shared_from_this());
    backend->draw(window);
}

void Renderer_t::clear()
{
    vertices.clear();
    indices.clear();
    for (auto& primitive_set : primitives) primitive_set.second.second = { 0, 0, 0 };
}

void Renderer_t::configurePrimitive(Primitive prim)
{
    uint64_t id      = next_primitive_id++;
    prim->backing_id = id;
    prim->renderer   = shared_from_this();

    BackingInternal backing = { 0, 0, 0 };
    primitives[id]          = { prim, backing };

    modified_primitives.insert(id);
}

void Renderer_t::ensureBacking(uint64_t primitive_id, size_t capacity)
{
    capacity = glm::max(capacity, 1ul);
    auto it  = primitives.find(primitive_id);
    if (it == primitives.end()) throw std::runtime_error("invalid render primitive");
    if (capacity < it->second.second.quad_count)
    {
        Index difference   = it->second.second.quad_count - capacity;
        Index first_vertex = it->second.second.vertex_start + (capacity * 4);
        memset(vertices.data() + first_vertex, 0, difference * sizeof(Vertex));
    }
    else if (capacity > it->second.second.quad_count)
    {
        Index count        = it->second.second.quad_count;
        Index first_vertex = it->second.second.vertex_start;
        Index first_index  = it->second.second.index_start;
        memset(vertices.data() + first_vertex, 0, (count * 4) * sizeof(Vertex));
        memset(indices.data() + first_index, 0xFF, (count * 6) * sizeof(Index));
        // TODO: necessary/inefficient?
        for (size_t i = first_vertex; i < first_vertex + (count * 4); ++i)
            vertices[i].data_2 = { 0, 0, 0, -1 };
        dead_quads += count;

        it->second.second.vertex_start = vertices.size();
        vertices.resize(vertices.size() + (capacity * 4));
        it->second.second.index_start = indices.size();
        indices.resize(indices.size() + (capacity * 6));
        it->second.second.quad_count = capacity;
        count                        = it->second.second.quad_count;
        first_vertex                 = it->second.second.vertex_start;
        // TODO: necessary/inefficient?
        memset(vertices.data() + first_vertex, 0, (count * 4) * sizeof(Vertex));
        for (size_t i = first_index; i < first_index + (count * 6); i += 6)
        {
            indices[i + 0] = first_vertex + 0;
            indices[i + 1] = first_vertex + 3;
            indices[i + 2] = first_vertex + 1;
            indices[i + 3] = first_vertex + 0;
            indices[i + 4] = first_vertex + 2;
            indices[i + 5] = first_vertex + 3;
        }
        modified_primitives.insert(primitive_id); // TODO: necessary/ineffcient?
    }
}

void Renderer_t::releasePrimitive(uint64_t primitive_id)
{
    auto it = primitives.find(primitive_id);
    if (it == primitives.end()) throw std::runtime_error("invalid render primitive");
    Index count        = it->second.second.quad_count;
    Index first_vertex = it->second.second.vertex_start;
    Index first_index  = it->second.second.index_start;
    memset(vertices.data() + first_vertex, 0, (count * 4) * sizeof(Vertex));
    memset(indices.data() + first_index, 0xFF, (count * 6) * sizeof(Index));
    // TODO: necessary/inefficient?
    for (size_t i = first_vertex; i < first_vertex + (count * 4); ++i) vertices[i].data_2 = { 0, 0, 0, -1 };
    dead_quads += count;
    primitives.erase(it);
    modified_primitives.erase(primitive_id);
}