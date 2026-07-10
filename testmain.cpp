#include "inc/bbui.h"

#include <iostream>

int main()
{
    BBUI::Window w = BBUI::Window_t::create("my test window", {});
    BBUI::Canvas c = std::make_shared<BBUI::Canvas_t>();
    auto b         = std::make_shared<BBUI::Button_t>();
    c->insert(b, nullptr);
    b->setText("button text");
    b->setCallback([]() -> void { std::cout << "clicked" << std::endl; });
    b->setIcon(BBUI::Style_t::ICON_CLOSE);
    b->setTooltip("this is a button");
    b->transform.setPosition({ 0, 0 });
    b->transform.setSize({ 100, 100 });

    while (!w->shouldClose())
    {
        c->update();

        w->poll();
        c->checkInput(w);

        w->clear();
        c->draw(w);

        w->present();
    }

    return 0;
}