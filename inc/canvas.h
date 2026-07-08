#pragma once

#include "window.h"

#include <memory>

namespace BBUI
{

class Style
{
    // TODO: this!
};

class Component_t;

struct Transform
{
public:
    enum UnitBits : uint8_t
    {
        UNIT_BITS_PIXELS   = 0b00,
        UNIT_BITS_FRACTION = 0b01,
        UNIT_BITS_CHARS    = 0b10,
        UNIT_BITS_POINTS   = 0b11,
    };

    enum Units : uint8_t
    {
        UNITS_POSITION_X_PIXELS = (UNIT_BITS_PIXELS << 0),
        UNITS_POSITION_Y_PIXELS = (UNIT_BITS_PIXELS << 2),
        UNITS_POSITION_PIXELS   = UNITS_POSITION_X_PIXELS | UNITS_POSITION_Y_PIXELS,
        UNITS_SIZE_X_PIXELS     = (UNIT_BITS_PIXELS << 4),
        UNITS_SIZE_Y_PIXELS     = (UNIT_BITS_PIXELS << 6),
        UNITS_SIZE_PIXELS       = UNITS_SIZE_X_PIXELS | UNITS_SIZE_Y_PIXELS,
        UNITS_PIXELS            = UNITS_POSITION_PIXELS | UNITS_SIZE_PIXELS,

        UNITS_POSITION_X_FRACTION = (UNIT_BITS_FRACTION << 0),
        UNITS_POSITION_Y_FRACTION = (UNIT_BITS_FRACTION << 2),
        UNITS_POSITION_FRACTION   = UNITS_POSITION_X_FRACTION | UNITS_POSITION_Y_FRACTION,
        UNITS_SIZE_X_FRACTION     = (UNIT_BITS_FRACTION << 4),
        UNITS_SIZE_Y_FRACTION     = (UNIT_BITS_FRACTION << 6),
        UNITS_SIZE_FRACTION       = UNITS_SIZE_X_FRACTION | UNITS_SIZE_Y_FRACTION,
        UNITS_FRACTION            = UNITS_POSITION_FRACTION | UNITS_SIZE_FRACTION,

        UNITS_POSITION_X_CHARS = (UNIT_BITS_CHARS << 0),
        UNITS_POSITION_Y_CHARS = (UNIT_BITS_CHARS << 2),
        UNITS_POSITION_CHARS   = UNITS_POSITION_X_CHARS | UNITS_POSITION_Y_CHARS,
        UNITS_SIZE_X_CHARS     = (UNIT_BITS_CHARS << 4),
        UNITS_SIZE_Y_CHARS     = (UNIT_BITS_CHARS << 6),
        UNITS_SIZE_CHARS       = UNITS_SIZE_X_CHARS | UNITS_SIZE_Y_CHARS,
        UNITS_CHARS            = UNITS_POSITION_CHARS | UNITS_SIZE_CHARS,

        UNITS_POSITION_X_POINTS = (UNIT_BITS_POINTS << 0),
        UNITS_POSITION_Y_POINTS = (UNIT_BITS_POINTS << 2),
        UNITS_POSITION_POINTS   = UNITS_POSITION_X_POINTS | UNITS_POSITION_Y_POINTS,
        UNITS_SIZE_X_POINTS     = (UNIT_BITS_POINTS << 4),
        UNITS_SIZE_Y_POINTS     = (UNIT_BITS_POINTS << 6),
        UNITS_SIZE_POINTS       = UNITS_SIZE_X_POINTS | UNITS_SIZE_Y_POINTS,
        UNITS_POINTS            = UNITS_POSITION_POINTS | UNITS_SIZE_POINTS,
    };

    enum Anchor
    {
        ANCHOR_TOP_LEFT,
        ANCHOR_TOP_CENTER,
        ANCHOR_TOP_RIGHT,
        ANCHOR_MIDDLE_LEFT,
        ANCHOR_MIDDLE_CENTER,
        ANCHOR_MIDDLE_RIGHT,
        ANCHOR_BOTTOM_LEFT,
        ANCHOR_BOTTOM_CENTER,
        ANCHOR_BOTTOM_RIGHT
    };

public:
    std::weak_ptr<Component_t> owner;

private:
    glm::vec2 position     = { 0, 0 };
    glm::vec2 size         = { 10, 10 };
    Anchor anchor_internal = ANCHOR_TOP_LEFT;
    Anchor anchor_external = ANCHOR_TOP_LEFT;

    glm::vec2 top_left_offset     = { 0, 0 };
    Anchor top_left_relative      = ANCHOR_TOP_LEFT;
    glm::vec2 bottom_right_offset = { 0, 0 };
    Anchor bottom_right_relative  = ANCHOR_BOTTOM_RIGHT;
    bool use_offsets              = false;
    Units units                   = UNITS_PIXELS;

    glm::vec2 computed_position;
    glm::vec2 computed_size;

    bool modified        = true;
    bool needs_recompute = true;

public:
    glm::vec2 getPosition() const { return position; }
    void setPosition(glm::vec2 p);
    glm::vec2 getSize() const { return size; }
    void setSize(glm::vec2 s);
    Anchor getAnchor() const { return top_left_relative; }
    void setAnchor(Anchor internal, Anchor external);
    void usePositionSize();

    glm::vec2 getTopLeftOffset() const { return top_left_offset; }
    void setTopLeftOffset(glm::vec2 offset);
    glm::vec2 getBottomRightOffset() const { return bottom_right_offset; }
    void setBottomRightOffset(glm::vec2 offset);
    Anchor getTopLeftAnchor() const { return top_left_relative; }
    void setTopLeftAnchor(Anchor anchor);
    Anchor getBottomRightAnchor() const { return bottom_right_relative; }
    void setBottomRightAnchor(Anchor anchor);
    void useOffsets();

    bool checkModified();
    void setModified();
    std::array<glm::vec2, 2> getRenderPositionAndSize();
};

typedef std::shared_ptr<class Component_t> Component;
class Canvas_t;

class Component_t : public std::enable_shared_from_this<Component_t>
{
    friend class Canvas_t;

public:
    enum FocusAdvance
    {
        ADVANCE_INTERNAL,
        ADVANCE_EXTERNAL
    };

    struct Tooltip
    {
        bool present        = true;
        std::string text    = "Tooltip";
        int icon            = 0;
        std::string subtext = "This is the tooltip";
    };

public:
    std::string internal_name;
    Transform transform;

protected:
    // TODO: everything has to be protected/private - changes to the component means calls to the renderer

private:
    std::vector<Component> children;
    std::weak_ptr<Component_t> parent;
    std::weak_ptr<Canvas_t> canvas;

public:
    Component_t();
    Component_t(const Component_t& other)    = delete;
    Component_t(Component_t&& other)         = delete;
    void operator=(const Component_t& other) = delete;
    void operator=(Component_t&& other)      = delete;
    virtual ~Component_t()                   = default;

    // called once per frame
    virtual void update() {}
    // called to completely redraw the component, refreshing all render primitives
    virtual void redraw() {}
    // arranges children of the component, modifying their transforms
    virtual void arrange() {}

    // called when a mouse button begins to be pressed down
    virtual void mousePressed(InputButton button, glm::vec2 local_position) {}
    // called when a mouse button stops being pressed down
    virtual void mouseReleased(InputButton button, glm::vec2 local_position) {}
    // called when a mouse button is pressed and released in quick succession without dragging
    virtual void mouseClicked(InputButton button, glm::vec2 local_position) {}
    // called when the mouse enters the bounds of the component
    virtual void mouseEntered() {}
    // called when the mouse leaves the bounds of the component
    virtual void mouseExited() {}
    // called when the mouse moves within the bounds of the component
    virtual void mouseMoved(glm::vec2 delta) {}
    // called when the mouse moves within the bounds of the component, and a button is held down
    virtual void mouseDragged(glm::vec2 delta, InputButton button) {}

    // called when a keyboard key begins to be pressed down
    virtual void keyPressed(InputButton key) {}
    // called when a keyboard key stops being pressed down
    virtual void keyReleased(InputButton key) {}
    // called when a keyboard key is held down
    virtual void keyRepeated(InputButton key) {}
    // called when a character event arrives
    virtual void charInput(unsigned int character) {}

    // determines whether the component can be highlighted and navigated to/from with TAB
    virtual bool isFocusable() { return false; }
    // determines whether pressing TAB should navigate to the next component or not
    virtual FocusAdvance advanceFocus() { return ADVANCE_EXTERNAL; }

    // determines the minimum size for the component
    virtual glm::vec2 getMinimumSize() const { return { 0, 0 }; }
    // determines the maximum size for the component
    virtual glm::vec2 getMaximumSize() const { return { -1, -1 }; }

    // determines the tooltip (if any) shown when the mouse hovers on the component
    virtual Tooltip getTooltip() const { return Tooltip(); }

    glm::vec2 localToCanvas(glm::vec2 point);
    glm::vec2 canvasToLocal(glm::vec2 point);
    void requestFocus();

    Component findChildByName(const std::string& name) const;
    Component addChild(Component child) { return canvas.lock()->insert(child, shared_from_this()); }
    std::vector<Component>::iterator getChildrenBegin() { return children.begin(); }
    std::vector<Component>::iterator getChildrenEnd() { return children.end(); }
    Component getParent() const { return parent.lock(); }

private:
    void updateSelfAndChildren();
    void redrawSelfAndChildren();
    void arrangeSelfAndChildren();
};

typedef std::shared_ptr<class Renderer_t> Renderer;

class Canvas_t : public std::enable_shared_from_this<Canvas_t>
{
private:
    Style style;
    Renderer renderer;
    bool has_size_override = false;
    glm::vec2 size_override;
    Component focused_component;
    Component last_under_mouse;
    glm::vec2 mouse_down_position[3];
    Component root;
    std::vector<Component> all_components;

    // TODO: allows configuration of window menu, right-click menu

public:
    Canvas_t();
    Canvas_t(const Canvas_t& other)       = delete;
    Canvas_t(Canvas_t&& other)            = delete;
    void operator=(const Canvas_t& other) = delete;
    void operator=(Canvas_t&& other)      = delete;
    ~Canvas_t()                           = default;

    void update();
    void checkInput(Window window);
    void draw(Window window);

    void setStyle(Style new_style);
    void setSizeOverride(glm::vec2 size);
    void clearSizeOverride();
    
    Component insert(Component component, Component parent);
    void setFocus(Component component);
    void advanceFocus();
    void recedeFocus();

private:
    void redrawComponents();
    void arrangeComponents();
};

typedef std::shared_ptr<Canvas_t> Canvas;

} // namespace BBUI