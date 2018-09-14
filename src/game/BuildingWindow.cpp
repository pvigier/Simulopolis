#include "BuildingWindow.h"
#include "resource/StylesheetManager.h"
#include "city/Building.h"
#include "city/Company.h"
#include "city/Housing.h"
#include "city/Industry.h"
#include "city/Business.h"
#include "city/Service.h"
#include "gui/Gui.h"
#include "gui/GuiText.h"
#include "gui/GuiImage.h"
#include "gui/GuiTable.h"
#include "gui/GuiVBoxLayout.h"
#include "gui/GuiHBoxLayout.h"
#include "util/format.h"

BuildingWindow::BuildingWindow(StylesheetManager* stylesheetManager, const Building& building) :
    GuiWindow(format("%s %d", Tile::typeToString(building.getType()).c_str(), building.getId()), stylesheetManager->getStylesheet("window")),
    mStylesheetManager(stylesheetManager), mBuilding(building), mImage(nullptr), mStockText(nullptr), mTable(nullptr)
{

}

BuildingWindow::~BuildingWindow()
{
    //dtor
}

void BuildingWindow::setUp()
{
    // Zoom
    mRenderTexture.create(96, 96);
    sf::Sprite sprite(mRenderTexture.getTexture());
    mImage = mGui->createWithDefaultName<GuiImage>(sprite);

    // Personal info
    auto infoWidget = mGui->createWithDefaultName<GuiWidget>();
    auto typeText = mGui->createWithDefaultName<GuiText>("Type: " + Tile::typeToString(mBuilding.getType()), 12, mStylesheetManager->getStylesheet("darkText"));
    auto ownerText = mGui->createWithDefaultName<GuiText>("Owner: " + mBuilding.getOwner()->getName(), 12, mStylesheetManager->getStylesheet("darkText"));
    infoWidget->add(typeText);
    infoWidget->add(ownerText);
    infoWidget->setLayout(std::make_unique<GuiVBoxLayout>(3.0f));

    // Top widget
    auto topWidget = mGui->createWithDefaultName<GuiWidget>();
    topWidget->add(mImage);
    topWidget->add(infoWidget);
    topWidget->setLayout(std::make_unique<GuiHBoxLayout>(8.0f));

    // Bottom widget
    auto bottomWidget = mGui->createWithDefaultName<GuiWidget>();
    bottomWidget->setLayout(std::make_unique<GuiVBoxLayout>(3.0f));

    // Cases
    if (mBuilding.isHousing())
    {
        // Table
        std::vector<std::string> names = {"Tenant", "Rent"};
        mTable = mGui->createWithDefaultName<GuiTable>(names, mStylesheetManager->getStylesheet("table"));
        for (std::size_t i = 0; i < static_cast<const Housing&>(mBuilding).getLeases().size(); ++i)
        {
            mTable->addRow({
                mGui->createWithDefaultName<GuiText>("", 12, mStylesheetManager->getStylesheet("darkText")),
                mGui->createWithDefaultName<GuiText>("", 12, mStylesheetManager->getStylesheet("darkText")),
            });
        }
    }
    else
    {
        if (mBuilding.isBusiness())
        {
            mStockText = mGui->createWithDefaultName<GuiText>("", 12, mStylesheetManager->getStylesheet("darkText"));
            infoWidget->add(mStockText);
        }
        // Table
        std::vector<std::string> names = {"Employee", "Work", "Salary"};
        mTable = mGui->createWithDefaultName<GuiTable>(names, mStylesheetManager->getStylesheet("table"));
        for (std::size_t i = 0; i < Company::getEmployees(&mBuilding)->size(); ++i)
        {
            mTable->addRow({
                mGui->createWithDefaultName<GuiText>("", 12, mStylesheetManager->getStylesheet("darkText")),
                mGui->createWithDefaultName<GuiText>("", 12, mStylesheetManager->getStylesheet("darkText")),
                mGui->createWithDefaultName<GuiText>("", 12, mStylesheetManager->getStylesheet("darkText")),
            });
        }
    }
    bottomWidget->add(mTable);

    // Window
    add(topWidget);
    add(bottomWidget);
    setOutsidePosition(sf::Vector2f(50.0f, 50.0f));
    setLayout(std::make_unique<GuiVBoxLayout>(8.0f, GuiLayout::Margins{8.0f, 8.0f, 8.0f, 8.0f}));
}

void BuildingWindow::update()
{
    if (mBuilding.isHousing())
    {
        // Table
        const std::vector<std::unique_ptr<Lease>>& leases = static_cast<const Housing&>(mBuilding).getLeases();
        for (std::size_t i = 0; i < leases.size(); ++i)
        {
            static_cast<GuiText*>(mTable->getCellContent(i, 0))->setString(leases[i]->getTenantName());
            static_cast<GuiText*>(mTable->getCellContent(i, 1))->setString(format("$%.2f", leases[i]->getRent()));
        }
    }
    else
    {
        // Info
        if (mBuilding.isBusiness())
            mStockText->setString(format("Stock: %d", static_cast<const Business&>(mBuilding).getStock()));
        // Table
        const std::vector<std::unique_ptr<Work>>* employees = Company::getEmployees(&mBuilding);
        for (std::size_t i = 0; i < employees->size(); ++i)
        {
            static_cast<GuiText*>(mTable->getCellContent(i, 0))->setString((*employees)[i]->getEmployeeName());
            static_cast<GuiText*>(mTable->getCellContent(i, 1))->setString(Work::typeToString((*employees)[i]->getType()));
            static_cast<GuiText*>(mTable->getCellContent(i, 2))->setString(format("$%.2f", (*employees)[i]->getSalary()));
        }
    }
}

const Building& BuildingWindow::getBuilding() const
{
    return mBuilding;
}

sf::RenderTexture& BuildingWindow::getRenderTexture()
{
    return mRenderTexture;
}

sf::View BuildingWindow::getView()
{
    sf::Vector2f center = mBuilding.getPosition() + sf::Vector2f(66.0f, 48.0f);
    return sf::View(center, 2.0f * sf::Vector2f(mRenderTexture.getSize()));
}
