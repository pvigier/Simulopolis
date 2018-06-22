#include "PersonWindow.h"
#include "resource/StylesheetManager.h"
#include "city/Person.h"
#include "gui/Gui.h"
#include "gui/GuiText.h"
#include "gui/GuiImage.h"
#include "gui/GuiWindow.h"
#include "gui/GuiVBoxLayout.h"
#include "gui/GuiHBoxLayout.h"

PersonWindow::PersonWindow(Gui* gui, StylesheetManager* stylesheetManager, const std::string& windowId,
    const Person& person, int year) :
    mPerson(person), mWindow(nullptr), mImage(nullptr)
{
    // Zoom
    mRenderTexture.create(96, 96);
    sf::Sprite sprite(mRenderTexture.getTexture());
    mImage = gui->create<GuiImage>(windowId + "ZoomImage", sprite);

    // Personal info
    auto infoWidget = gui->create<GuiWidget>(windowId + "InfoWidget");
    auto firstNameText = gui->create<GuiText>(windowId + "FirstNameText", "First name: " + person.getFirstName(), 10, stylesheetManager->getStylesheet("text"));
    auto lastNameText = gui->create<GuiText>(windowId + "LastNameText", "Last name: " + person.getLastName(), 10, stylesheetManager->getStylesheet("text"));
    auto ageText = gui->create<GuiText>(windowId + "AgeText", "Age: " + std::to_string(person.getAge(year)), 10, stylesheetManager->getStylesheet("text"));
    auto stateText = gui->create<GuiText>(windowId + "StateText", "State: " + std::to_string(static_cast<int>(person.getState())), 10, stylesheetManager->getStylesheet("text"));
    infoWidget->add(firstNameText);
    infoWidget->add(lastNameText);
    infoWidget->add(ageText);
    infoWidget->add(stateText);
    infoWidget->setLayout(std::make_unique<GuiVBoxLayout>(GuiLayout::HAlignment::Left, GuiLayout::VAlignment::Top, 3.0f));

    // Top widget
    auto topWidget = gui->create<GuiWidget>(windowId + "TopWidget");
    topWidget->add(mImage);
    topWidget->add(infoWidget);
    topWidget->setLayout(std::make_unique<GuiHBoxLayout>(GuiLayout::HAlignment::Left, GuiLayout::VAlignment::Top, 8.0f));

    // Bottom widget
    auto bottomWidget = gui->create<GuiWidget>(windowId + "BottomWidget");
    auto sleepText = gui->create<GuiText>(windowId + "SleepText", "Sleep: " + std::to_string(person.getSleep()), 10, stylesheetManager->getStylesheet("text"));
    auto hygieneText = gui->create<GuiText>(windowId + "HygieneText", "Hygiene: " + std::to_string(person.getHygiene()), 10, stylesheetManager->getStylesheet("text"));
    auto safetyText = gui->create<GuiText>(windowId + "SafetyText", "Safety: " + std::to_string(person.getSafety()), 10, stylesheetManager->getStylesheet("text"));
    auto hungerText = gui->create<GuiText>(windowId + "HungerText", "Hunger: " + std::to_string(person.getHunger()), 10, stylesheetManager->getStylesheet("text"));
    auto happinessText = gui->create<GuiText>(windowId + "HapinessText", "Happiness: " + std::to_string(person.getHappiness()), 10, stylesheetManager->getStylesheet("text"));
    bottomWidget->add(sleepText);
    bottomWidget->add(hygieneText);
    bottomWidget->add(safetyText);
    bottomWidget->add(hungerText);
    bottomWidget->add(happinessText);
    bottomWidget->setLayout(std::make_unique<GuiVBoxLayout>(GuiLayout::HAlignment::Left, GuiLayout::VAlignment::Top, 3.0f));

    // Window
    mWindow = gui->createRoot<GuiWindow>(windowId, person.getFullName(), stylesheetManager->getStylesheet("window"));
    mWindow->add(topWidget);
    mWindow->add(bottomWidget);
    mWindow->setPosition(sf::Vector2f(50.0f, 50.0f));
    std::unique_ptr<GuiVBoxLayout> layout(new GuiVBoxLayout(GuiLayout::HAlignment::Left, GuiLayout::VAlignment::Top, 8.0f));
    layout->setMargins(GuiLayout::Margins{8.0f, 8.0f, 8.0f, 8.0f});
    mWindow->setLayout(std::move(layout));
}

PersonWindow::~PersonWindow()
{
    //dtor
}

GuiWindow* PersonWindow::getWindow()
{
    return mWindow;
}

sf::RenderTexture& PersonWindow::getRenderTexture()
{
    return mRenderTexture;
}

sf::View PersonWindow::getView()
{
    sf::Vector2f center = mPerson.getCar().getKinematic().getPosition();
    return sf::View(center, sf::Vector2f(mRenderTexture.getSize()));
}
