#pragma once

#include "message/Mailbox.h"
#include "gui/GuiWindow.h"

class MessageBus;
class StylesheetManager;
class GuiButton;
class GuiTabWidget;
class GuiInput;
class City;

class PoliciesWindow : public GuiWindow
{
public:
    PoliciesWindow(MessageBus* messageBus, StylesheetManager* stylesheetManager, City& city);
    ~PoliciesWindow();

    virtual void setUp() override;
    virtual void tearDown() override;

    void update();

private:
    StylesheetManager* mStylesheetManager;
    Mailbox mMailbox;
    GuiWidget* mTabButtonsWidget;
    std::vector<GuiButton*> mTabButtons;
    GuiTabWidget* mTabWidget;
    std::vector<GuiInput*> mInputs;
    City& mCity;

    void createLine(GuiWidget* tab, const std::string& label, const std::string& value,
        const std::string& regex, const std::string& suffix = "");
    void updateTab(std::size_t tab);
};
