#pragma once

#include "window.h"

#include <memory>

namespace BBUI
{

class Style
{
    // TODO: this!
};

class Transform
{
    // TODO: this!
    // provides position, size (as pixels or as fraction, maybe as characters based on style?)
    // provides anchors
    // provides rotation (and anchor)
    // provides matrix
};

typedef std::shared_ptr<class Component_t> Component;

class Component_t
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

protected:
    Transform transform;
    // TODO: everything has to be protected/private - changes to the component means calls to the renderer

private:
    std::vector<Component> children;
    std::weak_ptr<Component_t> parent;
    bool has_style_override = false;
    Style style_override;

public:
    Component_t()                            = default;
    Component_t(const Component_t& other)    = delete;
    Component_t(Component_t&& other)         = delete;
    void operator=(const Component_t& other) = delete;
    void operator=(Component_t&& other)      = delete;
    virtual ~Component_t()                   = default;

    virtual void update() {};
    virtual void redraw() {};

    virtual void mousePressed(InputButton button, glm::vec2 local_position) {}
    virtual void mouseReleased(InputButton button, glm::vec2 local_position) {}
    virtual void mouseClicked(InputButton button, glm::vec2 local_position) {}
    virtual void mouseEntered() {}
    virtual void mouseExited() {}
    virtual void mouesMoved(glm::vec2 delta) {}
    virtual void mouseDragged(glm::vec2 delta) {}

    virtual void keyPressed(InputButton key) {}
    virtual void keyReleased(InputButton key) {}
    virtual void keyRepeated(InputButton key) {}
    virtual void charInput(unsigned int character) {}

    virtual bool isFocusable() { return false; }
    virtual FocusAdvance advanceFocus() { return ADVANCE_EXTERNAL; }

    virtual void arrangeChildren();
    virtual glm::vec2 getMinimumSize() const { return { 0, 0 }; }
    virtual glm::vec2 getMaximumSize() const { return { -1, -1 }; }

    virtual Tooltip getTooltip() const { return Tooltip(); }

    glm::vec2 localToCanvas(glm::vec2 point) const;
    glm::vec2 canvasToLocal(glm::vec2 point) const;

    Component findChildByName(const std::string& name) const;
    void addChild(Component child);
    std::vector<Component>::iterator getChildrenBegin() { return children.begin(); }
    std::vector<Component>::iterator getChildrenEnd() { return children.end(); }
    Style getStyle() const;
    void setStyleOverride(Style style);
    void clearStyleOverride();
};

typedef std::shared_ptr<class Renderer_t> Renderer;

class Canvas_t
{
private:
    Style style;
    Renderer renderer;
    bool has_size_override = false;
    glm::vec2 size_override;
    Component focused_component;

    // TODO: allows configuration of window menu, right-click menu

public:
    Canvas_t()                            = delete;
    Canvas_t(const Canvas_t& other)       = delete;
    Canvas_t(Canvas_t&& other)            = delete;
    void operator=(const Canvas_t& other) = delete;
    void operator=(Canvas_t&& other)      = delete;
    virtual ~Canvas_t()                   = default;

    void draw();
    void checkInput();

    void setStyle(Style style);
    void setSizeOverride(glm::vec2 size);
    void clearSizeOverride();

private:
    // TODO: input event/arrange functions are ONLY called when needed
    void arrangeComponents();
};

typedef std::shared_ptr<Canvas_t> Canvas;

} // namespace BBUI