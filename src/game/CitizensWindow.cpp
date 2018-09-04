#include "CitizensWindow.h"
#include "resource/StylesheetManager.h"
#include "gui/Gui.h"
#include "gui/GuiText.h"
#include "gui/GuiButton.h"
#include "gui/GuiTable.h"
#include "gui/GuiScrollArea.h"
#include "gui/GuiVBoxLayout.h"
#include "gui/GuiHBoxLayout.h"
#include "city/City.h"
#include "city/Person.h"
#include "util/format.h"

CitizensWindow::CitizensWindow(Id listenerId, MessageBus* messageBus, StylesheetManager* stylesheetManager, City& city) :
    GuiWindow("Citizens", stylesheetManager->getStylesheet("window")), mListenerId(listenerId),
    mMessageBus(messageBus), mStylesheetManager(stylesheetManager), mCity(city), mTable(nullptr)
{
    mMessageBus->addMailbox(mMailbox);
    mCity.subscribe(mMailbox.getId());
}

CitizensWindow::~CitizensWindow()
{
    mCity.unsubscribe(mMailbox.getId());
    mMessageBus->removeMailbox(mMailbox);
}

void CitizensWindow::setUp()
{
    // Create table
    std::vector<std::string> names{"Name", "Age", "Work"};
    mTable = mGui->createWithDefaultName<GuiTable>(names, mStylesheetManager->getStylesheet("table"));

    // Scroll area
    GuiScrollArea* scrollArea = mGui->createWithDefaultName<GuiScrollArea>(sf::Vector2i(400, 200), mStylesheetManager->getStylesheet("scrollarea"));
    scrollArea->add(mTable);
    scrollArea->setLayout(std::make_unique<GuiVBoxLayout>());

    // Window
    add(scrollArea);
    setOutsidePosition(sf::Vector2f(50.0f, 50.0f));
    setLayout(std::make_unique<GuiVBoxLayout>(8.0f, GuiLayout::Margins{8.0f, 8.0f, 8.0f, 8.0f}));

    // Add rows
    for (Person* citizen : mCity.getCitizens())
        addCitizen(citizen);

    subscribe(mListenerId);
}

void CitizensWindow::update()
{
    // Read messages
    while (!mMailbox.isEmpty())
    {
        Message message = mMailbox.get();
        if (message.type == MessageType::CITY)
        {
            const City::Event& event = message.getInfo<City::Event>();
            switch (event.type)
            {
                case City::Event::Type::NEW_CITIZEN:
                    addCitizen(event.person);
                    break;
                default:
                    break;
            }
        }
    }

    for (std::size_t i = 0; i < mCitizens.size(); ++i)
    {
        static_cast<GuiText*>(mTable->getCellContent(i, 1))->setString(format("%d", mCitizens[i]->getAge(mCity.getYear())));
        static_cast<GuiText*>(mTable->getCellContent(i, 2))->setString(mCitizens[i]->getWorkStatus());
    }
}

void CitizensWindow::addCitizen(Person* person)
{
    mCitizens.push_back(person);
    std::string fullName = person->getFullName();

    // Person button
    GuiWidget* personButton = mGui->create<GuiButton>("openPersonWindow" + std::to_string(person->getId()) + "|" + mTable->getName(), mStylesheetManager->getStylesheet("button"));
    personButton->setLayout(std::make_unique<GuiHBoxLayout>(0.0f, GuiLayout::Margins{2.0f, 2.0f, 2.0f, 2.0f}));
    personButton->add(mGui->createWithDefaultName<GuiText>(fullName, 12, mStylesheetManager->getStylesheet("darkText")));
    personButton->subscribe(mListenerId);

    // Add row
    mTable->addRow({
        personButton,
        mGui->createWithDefaultName<GuiText>("", 12, mStylesheetManager->getStylesheet("darkText")),
        mGui->createWithDefaultName<GuiText>("", 12, mStylesheetManager->getStylesheet("darkText")),
    });
}

void CitizensWindow::removeCitizen(Person* person)
{
    std::size_t i = std::find(mCitizens.begin(), mCitizens.end(), person) - mCitizens.begin();
    mTable->removeRow(i);
    mCitizens.erase(mCitizens.begin() + i);
}
