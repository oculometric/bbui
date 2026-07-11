#pragma once

#include "fwd.h"

#include <glm/glm.hpp>
#include <map>
#include <memory>
#include <set>
#include <string>
#include <vector>

namespace BBUI
{

struct Texture final
{
    unsigned char* data;
    size_t size;

    static Texture getDefaultSliceTexture();
    static Texture getDefaultIconTexture();
};

struct Font final
{
    Texture regular_atlas;
    Texture bold_atlas;
    glm::vec2 glyph_size;

    static Font getRobotoFont();
    static Font getIBMFont();
};

struct Vertex final
{
    glm::vec3 position;
    glm::vec4 colour_1;
    glm::vec4 colour_2;
    glm::vec4 data_1;
    glm::vec4 data_2;
    glm::vec2 uv;
};

class Backend
{
protected:
    Font current_font;

public:
    Backend(Font font, Texture slice_atlas, Texture icon_atlas) : current_font(font) {}
    Backend()                                = delete;
    Backend(const Backend& other)            = delete;
    Backend& operator=(const Backend& other) = delete;
    virtual ~Backend()                       = default;

    glm::vec2 getFontGlyphSize() const { return current_font.glyph_size; }

    virtual void mesh(const std::vector<Vertex>& vertices, const std::vector<Index>& indices,
        Renderer renderer) {};
    virtual void bind(Window window, Renderer renderer) const {};
    virtual void draw(Window window) const {};
};

class Backend_OpenGL final : public Backend
{
private:
    typedef unsigned int Handle;

    Handle vertex_buffer       = 0;
    Handle index_buffer        = 0;
    Handle vertex_array_object = 0;
    unsigned int index_count   = 0;

    Handle shader_program = 0;

    Handle text_atlas_texture      = 0;
    Handle text_bold_atlas_texture = 0;
    Handle slice_atlas_texture     = 0;
    Handle icon_atlas_texture      = 0;

public:
    Backend_OpenGL(Font font, Texture slice_atlas, Texture icon_atlas);
    Backend_OpenGL()                                       = delete;
    Backend_OpenGL(const Backend_OpenGL& other)            = delete;
    Backend_OpenGL& operator=(const Backend_OpenGL& other) = delete;
    ~Backend_OpenGL() override;

    void mesh(const std::vector<Vertex>& vertices, const std::vector<Index>& indices,
        Renderer renderer) override;
    void bind(Window window, Renderer renderer) const override;
    void draw(Window window) const override;
};

struct Primitive_t
{
    friend class Renderer_t;

protected:
    glm::vec3 position = { 0, 0, 0 };
    glm::vec2 size     = { 10, 10 };

private:
    uint64_t backing_id;
    std::weak_ptr<Renderer_t> renderer;

public:
    virtual ~Primitive_t();

    virtual std::vector<Vertex> getGeometry();
    virtual size_t getQuadCount();

    void setPosition(glm::vec3 _position);
    void setSize(glm::vec2 _size);

protected:
    Primitive_t() = default;
    Renderer getRenderer();
    void setModified();
    static std::vector<Vertex> makeQuad(glm::vec2 p1, glm::vec2 p2, glm::vec2 p3, glm::vec2 p4, float z,
        glm::vec2 uv_tl, glm::vec2 uv_br, glm::vec4 colour_1, glm::vec4 colour_2, glm::vec4 data_1,
        glm::vec4 data_2);

private:
    Primitive_t(const Primitive_t& other)            = delete;
    Primitive_t& operator=(const Primitive_t& other) = delete;
};

struct RectPrimitive_t final : public Primitive_t
{
    friend class Renderer_t;

public:
    enum Borders : uint8_t
    {
        RECT_BORDERS_ALL        = 0b1111,
        RECT_BORDERS_NONE       = 0b0000,
        RECT_BORDERS_TOP        = 0b0001,
        RECT_BORDERS_BOTTOM     = 0b0010,
        RECT_BORDERS_HORIZONTAL = 0b0011,
        RECT_BORDERS_LEFT       = 0b0100,
        RECT_BORDERS_RIGHT      = 0b1000,
        RECT_BORDERS_VERTICAL   = 0b1100,
    };

protected:
    glm::vec4 fill_colour   = { 0.8f, 0.8f, 0.8f, 1.0f };
    glm::vec4 border_colour = { 1.0f, 1.0f, 1.0f, 1.0f };
    int pattern_index       = 0;
    Borders borders         = RECT_BORDERS_ALL;

public:
    void setFill(glm::vec4 _fill_colour);
    void setBorder(glm::vec4 _border_colour);
    void setPattern(int _pattern_index);
    void setBorders(Borders _borders);

    std::vector<Vertex> getGeometry() override;

private:
    RectPrimitive_t()                                        = default;
    RectPrimitive_t(const RectPrimitive_t& other)            = delete;
    RectPrimitive_t& operator=(const RectPrimitive_t& other) = delete;
};

class IconPrimitive_t final : public Primitive_t
{
    friend class Renderer_t;

protected:
    glm::vec4 foreground = { 1, 1, 1, 1 };
    glm::vec4 background = { 0, 0, 0, 0 };
    int icon_index       = 0;

public:
    void setForeground(glm::vec4 _foreground);
    void setBackground(glm::vec4 _background);
    void setIconIndex(int _icon_index);

    std::vector<Vertex> getGeometry() override;

private:
    IconPrimitive_t()                                        = default;
    IconPrimitive_t(const IconPrimitive_t& other)            = delete;
    IconPrimitive_t& operator=(const IconPrimitive_t& other) = delete;
};

class TextPrimitive_t final : public Primitive_t
{
    friend class Renderer_t;

public:
    enum Align : uint8_t
    {
        TEXT_ALIGN_LEFT,
        TEXT_ALIGN_CENTER,
        TEXT_ALIGN_RIGHT
    };

    enum Flags : uint8_t
    {
        TEXT_FLAGS_NONE          = 0b000000,
        TEXT_FLAGS_BOLD          = 0b000001,
        TEXT_FLAGS_ITALIC        = 0b000010,
        TEXT_FLAGS_UNDERLINE     = 0b000100,
        TEXT_FLAGS_STRIKETHROUGH = 0b001000,
        TEXT_FLAGS_WRAP          = 0b010000,
        TEXT_FLAGS_CLIP          = 0b100000
    };

    struct Format final
    {
        Align align        = TEXT_ALIGN_LEFT;
        Flags flags        = TEXT_FLAGS_NONE;
        float char_spacing = 1.0f;
        float line_spacing = 1.0f;
        float font_size    = 24.0f;
    };

protected:
    glm::vec4 foreground = { 1, 1, 1, 1 };
    glm::vec4 background = { 1, 1, 1, 0 };
    std::string content;
    Format format;

public:
    void setForeground(glm::vec4 _colour);
    void setBackground(glm::vec4 _colour);
    void setText(const std::string& _content);
    void setFormat(const Format& _format);

    std::vector<Vertex> getGeometry() override;
    size_t getQuadCount() override;

private:
    TextPrimitive_t()                                        = default;
    TextPrimitive_t(const TextPrimitive_t& other)            = delete;
    TextPrimitive_t& operator=(const TextPrimitive_t& other) = delete;
};

typedef TextPrimitive_t::Format TextFormat;
typedef TextPrimitive_t::Flags TextFlags;
typedef TextPrimitive_t::Align TextAlign;

inline TextFlags operator|(TextFlags a, TextFlags b)
{ return static_cast<TextFlags>(static_cast<uint8_t>(a) | static_cast<uint8_t>(b)); }

class QuadPrimitive_t final : public Primitive_t
{
    friend class Renderer_t;

protected:
    glm::vec4 colour_a        = { 1, 1, 1, 1 };
    glm::vec4 colour_b        = { 1, 0, 1, 1 };
    glm::vec2 uv_top_left     = { 0, 0 };
    glm::vec2 uv_bottom_right = { 1, 1 };
    glm::vec4 data_1          = { 0, 0, 0, 0 };
    glm::vec4 data_2          = { 0, 0, 0, 0 };

public:
    void setColourA(glm::vec4 _colour_a);
    void setColourB(glm::vec4 _colour_b);
    void setUVTopLeft(glm::vec2 _uv_top_left);
    void setUVBottomRight(glm::vec2 _uv_bottom_right);
    void setData1(glm::vec4 _data_1);
    void setData2(glm::vec4 _data_2);

    std::vector<Vertex> getGeometry() override;

private:
    QuadPrimitive_t()                                        = default;
    QuadPrimitive_t(const QuadPrimitive_t& other)            = delete;
    QuadPrimitive_t& operator=(const QuadPrimitive_t& other) = delete;
};

class Renderer_t final : public std::enable_shared_from_this<Renderer_t>
{
    friend class Primitive_t;

private:
    struct BackingInternal final
    {
        Index vertex_start;
        Index index_start;
        Index quad_count;
    };

private:
    std::unique_ptr<Backend> backend;

    std::vector<Vertex> vertices;
    std::vector<Index> indices;

    const size_t max_dead_quads = 512;
    size_t dead_quads           = 0;

    uint64_t next_primitive_id = 3;

    std::set<uint64_t> modified_primitives;
    std::map<uint64_t, std::pair<std::weak_ptr<Primitive_t>, BackingInternal>> primitives;

public:
    bool force_dithered_alpha = true;
    bool pixel_perfect        = true; // TODO: if enabled, snap all elements to pixels
    bool anti_alias           = true;
    bool ensure_ordering      = false; // TODO: if enabled, enforce bottom up drawing

public:
    Renderer_t();
    Renderer_t(const Renderer_t& other)            = delete;
    Renderer_t& operator=(const Renderer_t& other) = delete;
    ~Renderer_t()                                  = default;

    RectPrimitive addRect();
    IconPrimitive addIcon();
    TextPrimitive addText();
    QuadPrimitive addQuad();

    glm::vec2 getGlyphSize() const;

    void draw(Window window);
    void clear();

private:
    void configurePrimitive(Primitive primitive);
    void ensureBacking(uint64_t primitive_id, size_t capacity);
    void releasePrimitive(uint64_t primitive_id);
};

}; // namespace BBUI
