#pragma once

class GuiWidget;

class GuiEvent
{
public:
    enum class Type{BUTTON_RELEASED, WINDOW_CLOSED};

    GuiWidget* widget;
    Type type;

    GuiEvent(GuiWidget* widget, Type type);
};
