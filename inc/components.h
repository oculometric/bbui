#pragma once

#include "canvas.h"
#include "fwd.h"

#include <functional>

namespace BBUI
{

// class DemoComponent : public Component_t
// {
// private:
//     // all properties should be private, accessible only via getter/setters which also set the transform
//     // modified flag!

// public:
//     // if the component does things every frame, override this
//     // void update() override;
//     // inside this, set ALL the properties of ALL your render primitives
//     void redraw() override;
//     // if the component has custom arrangement behaviour, override this
//     // void arrange() overide;

//     // void mousePressed(InputButton button, glm::vec2 local_position) override;
//     // void mouseReleased(InputButton button, glm::vec2 local_position) override;
//     // void mouseClicked(InputButton button, glm::vec2 local_position) override;
//     // void mouseEntered() override;
//     // void mouseExited() override;
//     // void mouseMoved(glm::vec2 delta) override;
//     // void mouseDragged(glm::vec2 delta, InputButton button);

//     // void keyPressed(InputButton key) override;
//     // void keyReleased(InputButton key) override;
//     // void keyRepeated(InputButton key) override;
//     // void charInput(unsigned int character) override;

//     // bool isFocusable() override;
//     // FocusAdvance advanceFocus() override;
//     // glm::vec2 getMinimumSize() const override;
//     // glm::vec2 getMaximumSize() const override;
//     // Tooltip getTooltip() const override;
// };

class Button_t : public Component_t
{
private:
    std::function<void(void)> callback;
    std::string tooltip_text;
    std::string button_text;
    std::string icon_name;

    bool pressed = false;
    TextPrimitive prim_text;
    IconPrimitive prim_icon;
    RectPrimitive prim_box;

public:
    // inside this, set ALL the properties of ALL your render primitives
    void redraw() override;

    void mousePressed(InputButton button, glm::vec2 local_position) override;
    void mouseReleased(InputButton button, glm::vec2 local_position) override;
    void mouseExited() override;

    void keyReleased(InputButton key) override;

    bool isFocusable() override { return true; }
    Tooltip getTooltip() const override { return Tooltip{ !tooltip_text.empty(), tooltip_text }; }

    void setText(const std::string& text);
    void setIcon(const std::string& name);
    void clearIcon() { setIcon(""); }
    void setCallback(std::function<void(void)> function);
    void setTooltip(const std::string& tooltip);

    // TODO: clickable button element, can have text, icon
    // callback event on click
    // enter if focused
};

class Label : public Component_t
{
    // TODO: simple label, can have text, icon
};

class Panel : public Component_t
{
    // TODO: contains other components, arranges them, has a background
};

// class Menu : Component
// {
//     // TODO: a vertical auto-arranging component, always on top
//     // can contain buttons, labels, dividers, and submenus
//     // callbacks for each button
// };

// class ArrangeBox : Component
// {
//     // TODO: subdivides its space and arranges its children according to a grid pattern of rows/columns
// };

// class Slider : Component
// {
//     // TODO: allows the user to drag select within a range
//     // callback events on change, slide release
// };

// class Selector : Component
// {
//     // TODO: allows the user to pick from a selection
//     // callback event on change
// };

// class RadioButton : Component
// {
//     // TODO: allows the user to pick from several vertically arranged clickable labels
//     // callback event on change
// };

// class CheckButton : Component
// {
//     // TODO: allows the user to check/uncheck a clickable label
//     // callback event on change
// };

// class WindowBox : Component
// {
//     // TODO: provides a title and a drawable area
//     // optionally can be moved and/or resized
//     // optionally can have icon-only buttons top left/right
//     // callbacks for each button, moved, move release
// };

// class DialogBox : Component
// {
//     // TODO: provides one, two, or three buttons, a title, an icon, and a message block
//     // callbacks for each button
//     // button types (so escape, enter can be bound automatically)
// };

// class TextBox : Component
// {
//     // TODO: provides single line text editing functionality
//     // callback event on change, enter
//     // optional restrictions for numbers, floats
// };

// class TextArea : Component
// {
//     // TODO: provides large scale multiline editing
//     // callback on change
// };

} // namespace BBUI