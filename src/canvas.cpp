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
        if (glm::vec2(window->getSize()) != size_override) size_override = window->getSize();
    }
    arrangeComponents();

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

Component Canvas_t::insert(Component component, Component parent)
{
    if (component->canvas.lock() && component->canvas.lock() != shared_from_this()) { return nullptr; }
    if (component->canvas.lock() != shared_from_this()) { return nullptr; }
    if (!(component->canvas.lock()))
    {
        component->canvas = shared_from_this();
        all_components.push_back(component);
    }
    if (component->parent.lock())
    {
        auto& arr = component->parent.lock()->children;
        for (auto it = arr.begin(); it != arr.end(); ++it)
        {
            if (*it == component)
            {
                arr.erase(it);
                break;
            }
        }
        component->parent.reset();
    }
    parent->children.push_back(component);
    component->parent = parent;
    parent->transform.setModified();
    return component;
}

void Canvas_t::redrawComponents()
{
    renderer->clear();
    root->redrawSelfAndChildren();
}

void Canvas_t::arrangeComponents()
{
    root->transform.setPosition({ 0, 0 });
    root->transform.setSize(size_override);
    root->arrangeSelfAndChildren();
}

void Transform::setPosition(glm::vec2 p)
{
    if (position != p)
    {
        position = p;
        modified = true;
    }
}

void Transform::setSize(glm::vec2 s)
{
    if (size != s)
    {
        size     = s;
        modified = true;
    }
}

void Transform::setAnchor(Anchor internal, Anchor external)
{
    if (anchor_internal != internal || anchor_external != external)
    {
        anchor_internal = internal;
        anchor_external = external;
        modified        = true;
    }
}

void Transform::usePositionSize()
{
    if (use_offsets)
    {
        use_offsets = false;
        modified    = true;
    }
}

void Transform::setTopLeftOffset(glm::vec2 offset)
{
    if (top_left_offset != offset)
    {
        top_left_offset = offset;
        modified        = true;
    }
}

void Transform::setBottomRightOffset(glm::vec2 offset)
{
    if (bottom_right_offset != offset)
    {
        bottom_right_offset = offset;
        modified            = true;
    }
}

void Transform::setTopLeftAnchor(Anchor anchor)
{
    if (top_left_relative != anchor)
    {
        top_left_relative = anchor;
        modified          = true;
    }
}

void Transform::setBottomRightAnchor(Anchor anchor)
{
    if (bottom_right_relative != anchor)
    {
        bottom_right_relative = anchor;
        modified              = true;
    }
}

void Transform::useOffsets()
{
    if (!use_offsets)
    {
        use_offsets = true;
        modified    = true;
    }
}

bool Transform::checkModified()
{
    bool tmp = modified;
    modified = false;
    return tmp;
}

Component Component_t::findChildByName(const std::string& name) const
{
    for (auto& comp : children)
    {
        if (comp->internal_name == name) return comp;
    }
}

void Component_t::updateSelfAndChildren()
{
    update();
    for (auto& c : children) c->updateSelfAndChildren();
}

void Component_t::redrawSelfAndChildren()
{
    redraw();
    for (auto& c : children) c->redrawSelfAndChildren();
}

void Component_t::arrangeSelfAndChildren()
{
    if (transform.checkModified())
    {
        arrange();
        for (auto& c : children) c->transform.setModified();

        redraw();
    }
    for (auto& c : children) c->arrangeSelfAndChildren();
}
