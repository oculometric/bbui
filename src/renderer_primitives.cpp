#include "rendering.h"

using namespace BBUI;

Primitive_t::~Primitive_t() { getRenderer()->releasePrimitive(backing_id); }

std::vector<Vertex> Primitive_t::getGeometry()
{
    glm::vec2 p = position;
    return makeQuad(p, { p.x + size.x, p.y }, { p.x, p.y + size.y }, p + size, position.z, { 0, 0 },
        { 1, 1 }, { 1, 1, 1, 1 }, { 1, 0, 1, 0 }, { 4, 0, 0, 0 }, { 0, 0, 0, 0 });
}

void Primitive_t::setPosition(glm::vec3 _position)
{
    position = _position;
    setModified();
}

void Primitive_t::setSize(glm::vec2 _size)
{
    size = _size;
    setModified();
}

size_t Primitive_t::getQuadCount() { return 1; }

Renderer Primitive_t::getRenderer() { return renderer.lock(); }

void Primitive_t::setModified() { getRenderer()->modified_primitives.emplace(backing_id); }

std::vector<Vertex> Primitive_t::makeQuad(glm::vec2 p1, glm::vec2 p2, glm::vec2 p3, glm::vec2 p4, float z,
    glm::vec2 uv_tl, glm::vec2 uv_br, glm::vec4 colour_1, glm::vec4 colour_2, glm::vec4 data_1,
    glm::vec4 data_2)
{
    glm::vec3 _p1 = { p1, z };
    glm::vec3 _p2 = { p2, z };
    glm::vec3 _p3 = { p3, z };
    glm::vec3 _p4 = { p4, z };

    glm::vec2 uv_tr = { uv_br.x, uv_tl.y };
    glm::vec2 uv_bl = { uv_tl.x, uv_br.y };

    return {
        Vertex{ _p1, colour_1, colour_2, data_1, data_2, uv_tl },
        Vertex{ _p2, colour_1, colour_2, data_1, data_2, uv_tr },
        Vertex{ _p3, colour_1, colour_2, data_1, data_2, uv_bl },
        Vertex{ _p4, colour_1, colour_2, data_1, data_2, uv_br },
    };
}

void RectPrimitive_t::setFill(glm::vec4 _fill_colour)
{
    fill_colour = _fill_colour;
    setModified();
}

void RectPrimitive_t::setBorder(glm::vec4 _border_colour)
{
    border_colour = _border_colour;
    setModified();
}

void RectPrimitive_t::setPattern(int _pattern_index)
{
    pattern_index = _pattern_index;
    setModified();
}

void RectPrimitive_t::setBorders(Borders _borders)
{
    borders = _borders;
    setModified();
}

std::vector<Vertex> RectPrimitive_t::getGeometry()
{
    glm::vec2 p = position;
    return makeQuad(p, { p.x + size.x, p.y }, { p.x, p.y + size.y }, p + size, position.z, { 0, 0 },
        { 1, 1 }, fill_colour, border_colour, { 1, size, static_cast<float>(pattern_index) },
        { static_cast<float>(borders), 0, 0, 0 });
}

void IconPrimitive_t::setForeground(glm::vec4 _foreground)
{
    foreground = _foreground;
    setModified();
}

void IconPrimitive_t::setBackground(glm::vec4 _background)
{
    background = _background;
    setModified();
}

void IconPrimitive_t::setIconIndex(int _icon_index)
{
    icon_index = _icon_index;
    setModified();
}

std::vector<Vertex> BBUI::IconPrimitive_t::getGeometry()
{
    glm::vec2 p = position;
    return makeQuad(p, { p.x + size.x, p.y }, { p.x, p.y + size.y }, p + size, position.z, { 0, 0 },
        { 1, 1 }, foreground, background, { 1, size, static_cast<float>(icon_index) }, { 0, 0, 0, 0 });
}

void TextPrimitive_t::setForeground(glm::vec4 _foreground)
{
    foreground = _foreground;
    setModified();
}

void TextPrimitive_t::setBackground(glm::vec4 _background)
{
    background = _background;
    setModified();
}

void TextPrimitive_t::setText(const std::string& _content)
{
    content = _content;
    setModified();
}

void TextPrimitive_t::setFormat(const Format& _format)
{
    format = _format;
    setModified();
}

std::vector<Vertex> TextPrimitive_t::getGeometry()
{
    float line_width     = size.x;
    glm::vec2 glyph_size = getRenderer()->getCharacterSize();
    glm::vec2 char_size  = glyph_size;
    char_size *= format.font_size / char_size.y;
    size_t chars_per_line =
        glm::max(glm::floor((line_width - char_size.x) / (char_size.x + format.char_spacing)), 0.0f) + 1.0f;

    std::vector<std::string> lines;
    if (format.flags & TEXT_FLAGS_WRAP)
    {
        size_t line_start = 0;
        size_t line_end;
        do
        {
            line_end = content.find('\n', line_start);
            if (line_end - line_start > chars_per_line) line_end = line_start + chars_per_line;
            size_t cutoff = line_end;
            while (cutoff > line_start)
            {
                if (content[cutoff] == ' ' || content[cutoff] == '\n') break;
                --cutoff;
            }
            if (cutoff == line_start) cutoff = line_end;
            lines.push_back(content.substr(line_start, cutoff - line_start));
            if (content[cutoff] == '\n' || content[cutoff] == ' ') line_start = cutoff + 1;
            else
                line_start = cutoff;
        } while (line_start < content.size());
    }
    else
        lines.push_back(content);

    std::vector<float> line_starts;
    std::vector<float> line_lengths;
    for (const auto& l : lines)
    {
        float length = (l.size() * (char_size.x + format.char_spacing)) - format.char_spacing;
        line_lengths.push_back(length);
        switch (format.align)
        {
        case TEXT_ALIGN_LEFT:   line_starts.push_back(0.0f); break;
        case TEXT_ALIGN_CENTER: line_starts.push_back(glm::floor((line_width - length) / 2.0f)); break;
        case TEXT_ALIGN_RIGHT:  line_starts.push_back(glm::floor(line_width - length)); break;
        }
    }

    if (format.flags & TEXT_FLAGS_CLIP)
    {
        size_t max_lines =
            glm::max(glm::ceil((size.y - char_size.y) / (char_size.y + format.line_spacing)), 0.0f);
        if (lines.size() > max_lines) lines.resize(max_lines);

        const float hbuffer = char_size.x + format.char_spacing;
        for (size_t i = 0; i < lines.size(); ++i)
        {
            float line_start = line_starts[i];
            float line_end   = line_start + line_lengths[i];
            while (line_start <= -hbuffer && !lines[i].empty())
            {
                lines[i].erase(0);
                line_start += hbuffer;
            }
            while (line_end >= line_width + hbuffer && !lines[i].empty())
            {
                lines[i].pop_back();
                line_end -= hbuffer;
            }
            line_starts[i]  = line_start;
            line_lengths[i] = line_end - line_start;
        }
    }

    std::vector<Vertex> vertices;
    glm::vec2 line_top_left      = { 0.0f, 0.0f };
    const float margin           = 1.0f;
    const glm::vec2 p            = position;
    const glm::vec2 uv_to_pixels = (glyph_size / (glyph_size + margin + margin)) / char_size;
    for (size_t i = 0; i < lines.size(); ++i)
    {
        line_top_left.x         = line_starts[i];
        const std::string& line = lines[i];
        for (char c : line)
        {
            glm::vec2 uv_tl = margin / (glyph_size + margin + margin);
            glm::vec2 uv_br = uv_tl + (glyph_size / (glyph_size + margin + margin));

            glm::vec2 skew = { 0, 0 };
            int flags      = 0;
            if (format.flags & TEXT_FLAGS_ITALIC) skew.x = glm::round(char_size.x / 2.0f);
            if (format.flags & TEXT_FLAGS_BOLD) flags |= TEXT_FLAGS_BOLD;
            if (format.flags & TEXT_FLAGS_UNDERLINE) flags |= TEXT_FLAGS_UNDERLINE;
            if (format.flags & TEXT_FLAGS_STRIKETHROUGH) flags |= TEXT_FLAGS_STRIKETHROUGH;

            glm::vec2 tl = line_top_left + skew;
            glm::vec2 tr = line_top_left + glm::vec2{ char_size.x, 0 } + skew;
            glm::vec2 bl = line_top_left + glm::vec2{ 0, char_size.y };
            glm::vec2 br = line_top_left + char_size;

            if (format.flags & TEXT_FLAGS_CLIP && bl.y > size.y)
            {
                float subtract_amount_px = glm::min(char_size.y, bl.y - size.y);
                float subtract_amount_uv = uv_to_pixels.y * subtract_amount_px;
                bl.y -= subtract_amount_px;
                br.y -= subtract_amount_px;
                uv_br.y -= subtract_amount_uv;
            }

            if (format.flags & TEXT_FLAGS_CLIP && tl.x < 0.0f)
            {
                float add_amount_px = glm::min(char_size.x, 0.0f - tl.x);
                float add_amount_uv = uv_to_pixels.x * add_amount_px;
                tl.x += add_amount_px;
                bl.x += add_amount_px;
                uv_tl.x += add_amount_uv;
            }

            if (format.flags & TEXT_FLAGS_CLIP && tr.x > size.x)
            {
                float subtract_amount_px = glm::min(char_size.x, tr.x - size.x);
                float subtract_amount_uv = uv_to_pixels.x * subtract_amount_px;
                tr.x -= subtract_amount_px;
                br.x -= subtract_amount_px;
                uv_br.x -= subtract_amount_uv;
            }

            auto quad = makeQuad(tl + p, tr + p, bl + p, br + p, position.z, uv_tl, uv_br, foreground,
                background, glm::vec4{ 0, char_size, static_cast<float>(c) },
                glm::vec4{ static_cast<float>(flags), 0, 0, 0 });
            vertices.insert(vertices.end(), quad.begin(), quad.end());

            line_top_left.x += char_size.x + format.char_spacing;
        }
        line_top_left.y += char_size.y + format.line_spacing;
    }
    return vertices;
}

size_t TextPrimitive_t::getQuadCount() { return content.size(); }

void QuadPrimitive_t::setColourA(glm::vec4 _colour_a)
{
    colour_a = _colour_a;
    setModified();
}

void QuadPrimitive_t::setColourB(glm::vec4 _colour_b)
{
    colour_b = _colour_b;
    setModified();
}

void QuadPrimitive_t::setUVTopLeft(glm::vec2 _uv_top_left)
{
    uv_top_left = _uv_top_left;
    setModified();
}

void QuadPrimitive_t::setUVBottomRight(glm::vec2 _uv_bottom_right)
{
    uv_bottom_right = _uv_bottom_right;
    setModified();
}

void QuadPrimitive_t::setData1(glm::vec4 _data_1)
{
    data_1 = _data_1;
    setModified();
}

void QuadPrimitive_t::setData2(glm::vec4 _data_2)
{
    data_2 = _data_2;
    setModified();
}

std::vector<Vertex> QuadPrimitive_t::getGeometry()
{
    glm::vec2 p = position;
    return makeQuad(p, { p.x + size.x, p.y }, { p.x, p.y + size.y }, p + size, position.z, uv_top_left,
        uv_bottom_right, colour_a, colour_b, data_1, data_2);
}
