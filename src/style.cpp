#include "canvas.h"

using namespace BBUI;

TextFormat Style_t::getTextFormatting(const std::string& identifier) const
{
    auto it = text_formats.find(identifier);
    if (it == text_formats.end())
        return TextFormat{ TextAlign::TEXT_ALIGN_LEFT, TextFlags::TEXT_FLAGS_NONE, 1.0f, 2.0f, 24.0f };
    return it->second;
}

float Style_t::getConstant(const std::string& identifier) const
{
    auto it = constants.find(identifier);
    if (it == constants.end()) return 0.0f;
    return it->second;
}

int Style_t::getIcon(const std::string& identifier) const
{
    auto it = icons.find(identifier);
    if (it == icons.end()) return 0;
    return it->second;
}