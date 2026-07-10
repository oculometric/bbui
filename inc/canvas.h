#pragma once

#include "fwd.h"
#include "rendering.h"

#include <array>
#include <glm/glm.hpp>
#include <map>
#include <memory>

namespace BBUI
{

class Style_t
{
public:
    static constexpr const char* TEXT_LABEL       = "Style::TEXT::LABEL";
    static constexpr const char* TEXT_BUTTON      = "Style::TEXT::BUTTON";
    static constexpr const char* TEXT_TITLE       = "Style::TEXT::TITLE";
    static constexpr const char* TEXT_MINIHEADING = "Style::TEXT::MINIHEADING";
    static constexpr const char* TEXT_SMALL       = "Style::TEXT::SMALL";

    static constexpr const char* CONST_GENERIC_SPACING        = "Style::CONST::GENERIC::SPACING";
    static constexpr const char* CONST_GENERIC_LINEHEIGHT     = "Style::CONST::GENERIC::LINEHEIGHT";
    static constexpr const char* CONST_GENERIC_ICONSIZE       = "Style::CONST::GENERIC::ICONSIZE";
    static constexpr const char* CONST_GENERIC_EDGEWIDTH      = "Style::CONST::GENERIC::EDGEWIDTH";
    static constexpr const char* CONST_GENERIC_ABOVETEXT      = "Style::CONST::GENERIC::ABOVETEXT";
    static constexpr const char* CONST_MENU_BEFOREITEM        = "Style::CONST::MENU::BEFOREITEM";
    static constexpr const char* CONST_LABEL_AROUNDTEXT       = "Style::CONST::LABEL::AROUNDTEXT";
    static constexpr const char* CONST_BUTTON_AROUNDTEXT      = "Style::CONST::BUTTON::AROUNDTEXT";
    static constexpr const char* CONST_BUTTON_BETWEENICONTEXT = "Style::CONST::BUTTON::BETWEENICONTEXT";
    static constexpr const char* CONST_BUTTON_AROUNDICON      = "Style::CONST::BUTTON::AROUNDICON";
    static constexpr const char* CONST_BUTTON_CLICKOFFSET     = "Style::CONST::BUTTON::CLICKOFFSET";
    static constexpr const char* CONST_RADIOBUTTON_BETWEENOPTIONS =
        "Style::CONST::RADIOBUTTON::BETWEENOPTIONS";
    static constexpr const char* CONST_RADIOBUTTON_BETWEENTAGTEXT =
        "Style::CONST::RADIOBUTTON::BETWEENTAGTEXT";
    static constexpr const char* CONST_PANEL_AROUNDCONTENT  = "Style::CONST::PANEL::AROUNDCONTENT";
    static constexpr const char* CONST_PANEL_BANNERHEIGHT   = "Style::CONST::PANEL::BANNERHEIGHT";
    static constexpr const char* CONST_DIALOG_BUTTONSPACING = "Style::CONST::DIALOG_BUTTONSPACING";

    static constexpr const char* ICON_CLOSE            = "Style::ICON::CLOSE";
    static constexpr const char* ICON_MINIMISE         = "Style::ICON::MINIMISE";
    static constexpr const char* ICON_REFRESH          = "Style::ICON::REFRESH";
    static constexpr const char* ICON_TRIANGLEUP       = "Style::ICON::TRIANGLEUP";
    static constexpr const char* ICON_TRIANGLEDOWN     = "Style::ICON::TRIANGLEDOWN";
    static constexpr const char* ICON_TRIANGLELEFT     = "Style::ICON::TRIANGLELEFT";
    static constexpr const char* ICON_TRIANGLERIGHT    = "Style::ICON::TRIANGLERIGHT";
    static constexpr const char* ICON_ARROWUP          = "Style::ICON::ARROWUP";
    static constexpr const char* ICON_ARROWDOWN        = "Style::ICON::ARROWDOWN";
    static constexpr const char* ICON_ARROWLEFT        = "Style::ICON::ARROWLEFT";
    static constexpr const char* ICON_ARROWRIGHT       = "Style::ICON::ARROWRIGHT";
    static constexpr const char* ICON_UNFOLDED         = "Style::ICON::UNFOLDED";
    static constexpr const char* ICON_FOLDED           = "Style::ICON::FOLDED";
    static constexpr const char* ICON_UNDO             = "Style::ICON::UNDO";
    static constexpr const char* ICON_REDO             = "Style::ICON::REDO";
    static constexpr const char* ICON_SAVE             = "Style::ICON::SAVE";
    static constexpr const char* ICON_NEW              = "Style::ICON::NEW";
    static constexpr const char* ICON_IMPORT           = "Style::ICON::IMPORT";
    static constexpr const char* ICON_EXPORT           = "Style::ICON::EXPORT";
    static constexpr const char* ICON_WARNINGTRIANGLE  = "Style::ICON::WARNINGTRIANGLE";
    static constexpr const char* ICON_QUESTIONTRIANGLE = "Style::ICON::QUESTIONTRIANGLE";
    static constexpr const char* ICON_CHECKMARK        = "Style::ICON::CHECKMARK";
    static constexpr const char* ICON_SMALLDOT         = "Style::ICON::SMALLDOT";
    static constexpr const char* ICON_FILE             = "Style::ICON::FILE";
    static constexpr const char* ICON_FOLDER           = "Style::ICON::FOLDER";
    static constexpr const char* ICON_WORLD            = "Style::ICON::WORLD";
    static constexpr const char* ICON_TARGET           = "Style::ICON::TARGET";
    static constexpr const char* ICON_WRENCH           = "Style::ICON::WRENCH";
    static constexpr const char* ICON_MAGNIFIER        = "Style::ICON::MAGNIFIER";
    static constexpr const char* ICON_HEART            = "Style::ICON::HEART";
    static constexpr const char* ICON_CLUB             = "Style::ICON::CLUB";
    static constexpr const char* ICON_SPADE            = "Style::ICON::SPADE";
    static constexpr const char* ICON_DIAMOND          = "Style::ICON::DIAMOND";
    static constexpr const char* ICON_EYE              = "Style::ICON::EYE";
    static constexpr const char* ICON_CASSETTE         = "Style::ICON::CASSETTE";
    static constexpr const char* ICON_BOXHOLLOW        = "Style::ICON::BOXHOLLOW";
    static constexpr const char* ICON_BOXFILLED        = "Style::ICON::BOXFILLED";
    static constexpr const char* ICON_BOXCORNERS       = "Style::ICON::BOXCORNERS";
    static constexpr const char* ICON_BOXHIGHLIGHT     = "Style::ICON::BOXHIGHLIGHT";
    static constexpr const char* ICON_CIRCLEHOLLOW     = "Style::ICON::CIRCLEHOLLOW";
    static constexpr const char* ICON_CIRCLEFILLED     = "Style::ICON::CIRCLEFILLED";
    static constexpr const char* ICON_TRIANGLEHOLLOW   = "Style::ICON::TRIANGLEHOLLOW";
    static constexpr const char* ICON_TRIANGLEFILLED   = "Style::ICON::TRIANGLEFILLED";
    static constexpr const char* ICON_TRIANGLECORNERS  = "Style::ICON::TRIANGLECORNERS";
    static constexpr const char* ICON_CHAINLINKED      = "Style::ICON::CHAINLINKED";
    static constexpr const char* ICON_CHAINBROKEN      = "Style::ICON::CHAINBROKEN";
    static constexpr const char* ICON_CUBE             = "Style::ICON::CUBE";
    static constexpr const char* ICON_SPHERE           = "Style::ICON::SPHERE";
    static constexpr const char* ICON_CAMERA           = "Style::ICON::CAMERA";
    static constexpr const char* ICON_MICROPHONE       = "Style::ICON::MICROPHONE";
    static constexpr const char* ICON_AXES             = "Style::ICON::AXES";
    static constexpr const char* ICON_GIZMOMOVE        = "Style::ICON::GIZMOMOVE";
    static constexpr const char* ICON_GIZMOROTATE      = "Style::ICON::GIZMOROTATE";
    static constexpr const char* ICON_GIZMOSCALE       = "Style::ICON::GIZMOSCALE";

private:
    Font font;
    Texture slice_texture;
    Texture icon_texture;

    std::map<std::string, TextFormat> text_formats;
    std::map<std::string, float> constants;
    std::map<std::string, int> icons;

public:
    Style_t(const std::string& stylesheet);
    Style_t(const Style_t& other)            = delete;
    Style_t& operator=(const Style_t& other) = delete;
    ~Style_t()                               = default;

    Font getFont() const { return font; }
    Texture getSliceTexture() const { return slice_texture; }
    Texture getIconTexture() const { return icon_texture; }

    TextFormat getTextFormatting(const std::string& identifier) const;
    float getConstant(const std::string& identifier) const;
    int getIcon(const std::string& identifier) const;

    static Style getDefaultStyle();

private:
    Style_t() = default;
};

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
    Component_t* owner = nullptr;

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
    Component_t(const Component_t& other)            = delete;
    Component_t& operator=(const Component_t& other) = delete;
    virtual ~Component_t()                           = default;

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
    Component addChild(Component child);
    std::vector<Component>::iterator getChildrenBegin() { return children.begin(); }
    std::vector<Component>::iterator getChildrenEnd() { return children.end(); }
    Component getParent() const { return parent.lock(); }
    Style getStyle() const;
    Renderer getRenderer() const;
    void markDirty() { transform.setModified(); }

private:
    void updateSelfAndChildren();
    void redrawSelfAndChildren();
    void arrangeSelfAndChildren();
};

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
    Canvas_t(const Canvas_t& other)            = delete;
    Canvas_t& operator=(const Canvas_t& other) = delete;
    ~Canvas_t()                                = default;

    void update();
    void checkInput(Window window);
    void draw(Window window);

    void setStyle(Style new_style);
    Style getStyle() const { return style; }
    Renderer getRenderer() const { return renderer; }
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

} // namespace BBUI