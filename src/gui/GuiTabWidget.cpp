#include "GuiTabWidget.h"

GuiTabWidget::GuiTabWidget() : GuiWidget()
{
    //ctor
}

GuiTabWidget::GuiTabWidget(const PropertyList& properties) : GuiWidget(properties)
{

}

void GuiTabWidget::setCurrentTab(std::size_t currentTab)
{
    for (std::size_t i = 0; i < mChildren.size(); ++i)
    {
        if (i == currentTab)
            mChildren[i]->setVisible(true);
        else
            mChildren[i]->setVisible(false);
    }
}
