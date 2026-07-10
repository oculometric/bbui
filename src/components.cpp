#include "components.h"

#include "window.h"

using namespace BBUI;

void Button_t::redraw()
{
    if (!prim_text) prim_text = getRenderer()->addText();
    if (!prim_icon) prim_icon = getRenderer()->addIcon();
    if (!prim_box) prim_box = getRenderer()->addRect();

    const glm::vec2 position = transform.getRenderPositionAndSize()[0];
    const glm::vec2 size     = transform.getRenderPositionAndSize()[1];

    prim_box->setPattern(pressed ? 2 : 0);
    prim_box->setPosition({ position, 0 });
    prim_box->setSize(size);

    if (icon_name.empty()) prim_icon->setSize({ 0, 0 });
    else
    {
        float icon_spacing = getStyle()->getConstant(Style_t::CONST_BUTTON_AROUNDICON);
        float icon_size    = getStyle()->getConstant(Style_t::CONST_GENERIC_ICONSIZE);
        float click_offset = pressed ? getStyle()->getConstant(Style_t::CONST_BUTTON_CLICKOFFSET) : 0;
        prim_icon->setPosition({
            glm::vec2{ icon_spacing, icon_spacing }
            + position + click_offset, 0.01f
        });
        prim_icon->setSize({ icon_size, icon_size });
        prim_icon->setIconIndex(getStyle()->getIcon(icon_name));
    }

    prim_text->setText(button_text);
    prim_text->setFormat(getStyle()->getTextFormatting(Style_t::TEXT_BUTTON));
    // TODO: position/size text correctly
    prim_text->setSize({ 30, 30 });
    prim_text->setPosition({ 0, 0, 0.01f });
}

void Button_t::mousePressed(InputButton button, glm::vec2 local_position)
{
    pressed = true;
    markDirty();
}

void Button_t::mouseReleased(InputButton button, glm::vec2 local_position)
{
    if (pressed && callback) callback();
    pressed = false;
    markDirty();
}

void Button_t::mouseExited()
{
    pressed = false;
    markDirty();
}

void Button_t::keyReleased(InputButton key)
{
    if (key == InputButton::KEY_ENTER || key == ' ')
    {
        if (callback) callback();
    }
}

void Button_t::setText(const std::string& text)
{
    button_text = text;
    markDirty();
}

void Button_t::setIcon(const std::string& name)
{
    icon_name = name;
    markDirty();
}

void Button_t::setCallback(std::function<void(void)> function) { callback = function; }

void Button_t::setTooltip(const std::string& tooltip) { tooltip_text = tooltip; }
