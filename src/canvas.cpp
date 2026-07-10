#include "canvas.h"

#include "rendering.h"
#include "window.h"

using namespace BBUI;

Component_t::Component_t() { transform.owner = shared_from_this(); }

glm::vec2 Component_t::localToCanvas(glm::vec2 point)
{ return point + transform.getRenderPositionAndSize()[0]; }

glm::vec2 Component_t::canvasToLocal(glm::vec2 point)
{ return point - transform.getRenderPositionAndSize()[0]; }

void Component_t::requestFocus() { canvas.lock()->setFocus(shared_from_this()); }

Component Component_t::findChildByName(const std::string& name) const
{
    for (auto& comp : children)
    {
        if (comp->internal_name == name) return comp;
    }
    return nullptr;
}

Component Component_t::addChild(Component child)
{ return canvas.lock()->insert(child, shared_from_this()); }

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

Canvas_t::Canvas_t()
{
    renderer = std::make_shared<Renderer_t>();
    root     = std::make_shared<Component_t>();
}

void Canvas_t::update() { root->updateSelfAndChildren(); }

void Canvas_t::checkInput(Window window)
{
    std::vector<Component> components_in_search_order;
    size_t extend_start = 0;
    components_in_search_order.push_back(root);
    while (extend_start < components_in_search_order.size())
    {
        auto& arr = components_in_search_order[extend_start]->children;
        components_in_search_order.insert(components_in_search_order.end(), arr.begin(), arr.end());
        ++extend_start;
    }

    glm::vec2 mouse_pos   = window->getMousePosition();
    glm::vec2 mouse_delta = window->getMouseDelta();
    Component now_under_mouse;
    for (auto it = components_in_search_order.rbegin(); it != components_in_search_order.rend(); ++it)
    {
        std::array<glm::vec2, 2> position_size = (*it)->transform.getRenderPositionAndSize();
        if (mouse_pos.x >= position_size[0].x && mouse_pos.y >= position_size[0].y &&
            mouse_pos.x < position_size[0].x + position_size[1].x &&
            mouse_pos.y < position_size[0].y + position_size[1].y)
        {
            now_under_mouse = *it;
            break;
        }
    }
    // mouse enter/exit
    if (now_under_mouse != last_under_mouse)
    {
        if (now_under_mouse) now_under_mouse->mouseEntered();
        if (last_under_mouse) last_under_mouse->mouseExited();
    }
    // mouse drag/move
    bool is_a_mouse_down = false;
    if (window->isMouseDown(InputButton::MOUSE_LEFT))
    {
        is_a_mouse_down = true;
        if (now_under_mouse) now_under_mouse->mouseDragged(mouse_delta, InputButton::MOUSE_LEFT);
    }
    if (window->isMouseDown(InputButton::MOUSE_MIDDLE))
    {
        is_a_mouse_down = true;
        if (now_under_mouse) now_under_mouse->mouseDragged(mouse_delta, InputButton::MOUSE_MIDDLE);
    }
    if (window->isMouseDown(InputButton::MOUSE_RIGHT))
    {
        is_a_mouse_down = true;
        if (now_under_mouse) now_under_mouse->mouseDragged(mouse_delta, InputButton::MOUSE_RIGHT);
    }
    if (!is_a_mouse_down && now_under_mouse) now_under_mouse->mouseMoved(mouse_delta);
    // mouse pressed/released/clicked
    if (window->wasMousePressed(InputButton::MOUSE_LEFT))
    {
        mouse_down_position[0] = mouse_pos;
        if (now_under_mouse)
            now_under_mouse->mousePressed(InputButton::MOUSE_LEFT,
                now_under_mouse->canvasToLocal(mouse_pos));
    }
    if (window->wasMouseReleased(InputButton::MOUSE_LEFT))
    {
        if (now_under_mouse)
            now_under_mouse->mouseReleased(InputButton::MOUSE_LEFT,
                now_under_mouse->canvasToLocal(mouse_pos));
        if ((mouse_pos - mouse_down_position[0]).length() < 2.0f && now_under_mouse)
            now_under_mouse->mouseClicked(InputButton::MOUSE_LEFT,
                now_under_mouse->canvasToLocal(mouse_pos));
    }
    if (window->wasMousePressed(InputButton::MOUSE_MIDDLE))
    {
        mouse_down_position[1] = mouse_pos;
        if (now_under_mouse)
            now_under_mouse->mousePressed(InputButton::MOUSE_MIDDLE,
                now_under_mouse->canvasToLocal(mouse_pos));
    }
    if (window->wasMouseReleased(InputButton::MOUSE_MIDDLE))
    {
        if (now_under_mouse)
            now_under_mouse->mouseReleased(InputButton::MOUSE_MIDDLE,
                now_under_mouse->canvasToLocal(mouse_pos));
        if ((mouse_pos - mouse_down_position[1]).length() < 2.0f && now_under_mouse)
            now_under_mouse->mouseClicked(InputButton::MOUSE_MIDDLE,
                now_under_mouse->canvasToLocal(mouse_pos));
    }
    if (window->wasMousePressed(InputButton::MOUSE_RIGHT))
    {
        mouse_down_position[2] = mouse_pos;
        if (now_under_mouse)
            now_under_mouse->mousePressed(InputButton::MOUSE_RIGHT,
                now_under_mouse->canvasToLocal(mouse_pos));
    }
    if (window->wasMouseReleased(InputButton::MOUSE_RIGHT))
    {
        if (now_under_mouse)
            now_under_mouse->mouseReleased(InputButton::MOUSE_RIGHT,
                now_under_mouse->canvasToLocal(mouse_pos));
        if ((mouse_pos - mouse_down_position[2]).length() < 2.0f && now_under_mouse)
            now_under_mouse->mouseClicked(InputButton::MOUSE_RIGHT,
                now_under_mouse->canvasToLocal(mouse_pos));
    }
    last_under_mouse = now_under_mouse;
    // chars
    unsigned int chr = window->getCharEvent();
    while (chr != 0)
    {
        focused_component->charInput(chr);
        chr = window->getCharEvent();
    }
    // keys up/down
    InputResult key = window->getKeyEvent();
    while (key.key != 0)
    {
        bool consumed_by_canvas = false;
        if (key.key == KEY_TAB && (key.pressed || key.repeat))
        {
            if (focused_component && focused_component->advanceFocus() == Component_t::ADVANCE_EXTERNAL)
            {
                consumed_by_canvas = true;
                if (key.modifiers & MODIFIER_SHIFT) recedeFocus();
                else
                    advanceFocus();
            }
        }

        if (!consumed_by_canvas)
        {
            if (key.pressed) focused_component->keyPressed(key.key);
            else if (key.repeat)
                focused_component->keyRepeated(key.key);
            else
                focused_component->keyReleased(key.key);
        }
    }
}

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

void Canvas_t::setFocus(Component component)
{
    if (component->canvas.lock() != shared_from_this()) focused_component = nullptr;
    else
        focused_component = component;
}

void Canvas_t::advanceFocus()
{
    bool step_next_parent = false;
    do
    {
        // if current has children, step into
        if (focused_component->children.empty() && !step_next_parent)
            focused_component = focused_component->children[0];
        else if (auto parent = focused_component->parent.lock())
        {
            // if current has parent
            step_next_parent = false;
            auto it          = parent->children.begin();
            for (; it != parent->children.end(); ++it)
            {
                if (*it == focused_component)
                {
                    ++it;
                    // step to next sibling
                    if (it != parent->children.end()) focused_component = *it;
                    else
                    {
                        // or step up to parent and step to next parent
                        focused_component = parent;
                        step_next_parent  = true;
                    }
                }
            }
        }
        else
            step_next_parent = false;
    } while (!focused_component->isFocusable() || step_next_parent);
}

void Canvas_t::recedeFocus()
{
    // TODO: step focus backward
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