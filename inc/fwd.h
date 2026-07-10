#pragma once

#include <memory>

namespace BBUI
{

// window.h

enum ModifierKey : uint8_t;
enum InputButton : uint16_t;
enum CursorType : uint8_t;
struct InputResult;

class Window_t;
typedef std::shared_ptr<Window_t> Window;
class Surface;
class Surface_OpenGL;

// rendering.h

struct Texture;
struct Font;
struct Vertex;
typedef unsigned int Index;

class Renderer_t;
typedef std::shared_ptr<Renderer_t> Renderer;
class Backend;
class Backend_Opengl;

struct Primitive_t;
typedef std::shared_ptr<Primitive_t> Primitive;
struct RectPrimitive_t;
typedef std::shared_ptr<RectPrimitive_t> RectPrimitive;
class IconPrimitive_t;
typedef std::shared_ptr<IconPrimitive_t> IconPrimitive;
class TextPrimitive_t;
typedef std::shared_ptr<TextPrimitive_t> TextPrimitive;
class QuadPrimitive_t;
typedef std::shared_ptr<QuadPrimitive_t> QuadPrimitive;

// canvas.h

class Style_t;
typedef std::shared_ptr<Style_t> Style;
struct Transform;

class Canvas_t;
typedef std::shared_ptr<Canvas_t> Canvas;
class Component_t;
typedef std::shared_ptr<Component_t> Component;

// components.h

// PYRT??

} // namespace BBUI