#include "canvas.h"

using namespace BBUI;

void Transform::setPosition(glm::vec2 p)
{
    if (position != p)
    {
        position = p;
        setModified();
    }
}

void Transform::setSize(glm::vec2 s)
{
    if (size != s)
    {
        size = s;
        setModified();
    }
}

void Transform::setAnchor(Anchor internal, Anchor external)
{
    if (anchor_internal != internal || anchor_external != external)
    {
        anchor_internal = internal;
        anchor_external = external;
        setModified();
    }
}

void Transform::usePositionSize()
{
    if (use_offsets)
    {
        use_offsets = false;
        setModified();
    }
}

void Transform::setTopLeftOffset(glm::vec2 offset)
{
    if (top_left_offset != offset)
    {
        top_left_offset = offset;
        setModified();
    }
}

void Transform::setBottomRightOffset(glm::vec2 offset)
{
    if (bottom_right_offset != offset)
    {
        bottom_right_offset = offset;
        setModified();
    }
}

void Transform::setTopLeftAnchor(Anchor anchor)
{
    if (top_left_relative != anchor)
    {
        top_left_relative = anchor;
        setModified();
    }
}

void Transform::setBottomRightAnchor(Anchor anchor)
{
    if (bottom_right_relative != anchor)
    {
        bottom_right_relative = anchor;
        setModified();
    }
}

void Transform::useOffsets()
{
    if (!use_offsets)
    {
        use_offsets = true;
        setModified();
    }
}

bool Transform::checkModified()
{
    bool tmp = modified;
    modified = false;
    return tmp;
}

void Transform::setModified()
{
    modified        = true;
    needs_recompute = true;
}

std::array<glm::vec2, 2> Transform::getRenderPositionAndSize()
{
    if (modified)
    {
        auto parent_data = owner->getParent() ? owner->getParent()->transform.getRenderPositionAndSize()
                                 : std::array<glm::vec2, 2>{ position, size };
        if (use_offsets)
        {
            glm::vec2 top_left_anchor_pos     = { 0, 0 };
            glm::vec2 bottom_right_anchor_pos = { 0, 0 };
            switch (top_left_relative)
            {
            case ANCHOR_TOP_LEFT:      top_left_anchor_pos = { 0, 0 }; break;
            case ANCHOR_TOP_CENTER:    top_left_anchor_pos = { parent_data[1].x / 2.0f, 0 }; break;
            case ANCHOR_TOP_RIGHT:     top_left_anchor_pos = { parent_data[1].x, 0 }; break;
            case ANCHOR_MIDDLE_LEFT:   top_left_anchor_pos = { 0, parent_data[1].y / 2.0f }; break;
            case ANCHOR_MIDDLE_CENTER: top_left_anchor_pos = parent_data[1] / 2.0f; break;
            case ANCHOR_MIDDLE_RIGHT:
                top_left_anchor_pos = { parent_data[1].x, parent_data[1].y / 2.0f };
                break;
            case ANCHOR_BOTTOM_LEFT: top_left_anchor_pos = { 0, parent_data[1].y }; break;
            case ANCHOR_BOTTOM_CENTER:
                top_left_anchor_pos = { parent_data[1].x / 2.0f, parent_data[1].y };
                break;
            case ANCHOR_BOTTOM_RIGHT: top_left_anchor_pos = parent_data[1]; break;
            }
            top_left_anchor_pos += parent_data[0];

            switch (bottom_right_relative)
            {
            case ANCHOR_TOP_LEFT:      bottom_right_anchor_pos = { 0, 0 }; break;
            case ANCHOR_TOP_CENTER:    bottom_right_anchor_pos = { parent_data[1].x / 2.0f, 0 }; break;
            case ANCHOR_TOP_RIGHT:     bottom_right_anchor_pos = { parent_data[1].x, 0 }; break;
            case ANCHOR_MIDDLE_LEFT:   bottom_right_anchor_pos = { 0, parent_data[1].y / 2.0f }; break;
            case ANCHOR_MIDDLE_CENTER: bottom_right_anchor_pos = parent_data[1] / 2.0f; break;
            case ANCHOR_MIDDLE_RIGHT:
                bottom_right_anchor_pos = { parent_data[1].x, parent_data[1].y / 2.0f };
                break;
            case ANCHOR_BOTTOM_LEFT: bottom_right_anchor_pos = { 0, parent_data[1].y }; break;
            case ANCHOR_BOTTOM_CENTER:
                bottom_right_anchor_pos = { parent_data[1].x / 2.0f, parent_data[1].y };
                break;
            case ANCHOR_BOTTOM_RIGHT: bottom_right_anchor_pos = parent_data[1]; break;
            }
            bottom_right_anchor_pos += parent_data[0];

            glm::vec2 top_left     = top_left_anchor_pos + top_left_offset;
            glm::vec2 bottom_right = bottom_right_anchor_pos + bottom_right_offset;

            computed_position = top_left;
            computed_size     = bottom_right - top_left;
            needs_recompute   = false;
        }
        else
        {
            glm::vec2 parent_anchor_pos = { 0, 0 };
            switch (anchor_external)
            {
            case ANCHOR_TOP_LEFT:      parent_anchor_pos = { 0, 0 }; break;
            case ANCHOR_TOP_CENTER:    parent_anchor_pos = { parent_data[1].x / 2.0f, 0 }; break;
            case ANCHOR_TOP_RIGHT:     parent_anchor_pos = { parent_data[1].x, 0 }; break;
            case ANCHOR_MIDDLE_LEFT:   parent_anchor_pos = { 0, parent_data[1].y / 2.0f }; break;
            case ANCHOR_MIDDLE_CENTER: parent_anchor_pos = parent_data[1] / 2.0f; break;
            case ANCHOR_MIDDLE_RIGHT:
                parent_anchor_pos = { parent_data[1].x, parent_data[1].y / 2.0f };
                break;
            case ANCHOR_BOTTOM_LEFT: parent_anchor_pos = { 0, parent_data[1].y }; break;
            case ANCHOR_BOTTOM_CENTER:
                parent_anchor_pos = { parent_data[1].x / 2.0f, parent_data[1].y };
                break;
            case ANCHOR_BOTTOM_RIGHT: parent_anchor_pos = parent_data[1]; break;
            }
            parent_anchor_pos += parent_data[0];

            glm::vec2 self_anchor_pos = { 0, 0 };
            switch (anchor_internal)
            {
            case ANCHOR_TOP_LEFT:      self_anchor_pos = { 0, 0 }; break;
            case ANCHOR_TOP_CENTER:    self_anchor_pos = { size.x / 2.0f, 0 }; break;
            case ANCHOR_TOP_RIGHT:     self_anchor_pos = { size.x, 0 }; break;
            case ANCHOR_MIDDLE_LEFT:   self_anchor_pos = { 0, size.y / 2.0f }; break;
            case ANCHOR_MIDDLE_CENTER: self_anchor_pos = size / 2.0f; break;
            case ANCHOR_MIDDLE_RIGHT:  self_anchor_pos = { size.x, size.y / 2.0f }; break;
            case ANCHOR_BOTTOM_LEFT:   self_anchor_pos = { 0, size.y }; break;
            case ANCHOR_BOTTOM_CENTER: self_anchor_pos = { size.x / 2.0f, size.y }; break;
            case ANCHOR_BOTTOM_RIGHT:  self_anchor_pos = size; break;
            }

            computed_position = (parent_anchor_pos - self_anchor_pos) + position;
            computed_size     = size;
            needs_recompute   = false;
        }
    }

    return { computed_position, computed_size };
}