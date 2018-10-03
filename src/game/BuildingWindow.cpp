/* Simulopolis
 * Copyright (C) 2018 Pierre Vigier
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

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
    mStylesheetManager(stylesheetManager), mBuilding(building),
    mImage(nullptr), mStockText(nullptr), mPreparedGoodsText(nullptr), mTable(nullptr)
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
            mPreparedGoodsText = mGui->createWithDefaultName<GuiText>("", 12, mStylesheetManager->getStylesheet("darkText"));
            infoWidget->add(mStockText);
            infoWidget->add(mPreparedGoodsText);
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
    applyStyle();
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
        {
            const Business& business = static_cast<const Business&>(mBuilding);
            mStockText->setString(format("Stock: %d/%d", business.getStock(), business.getMaxSizeStock()));
            mPreparedGoodsText->setString(format("Prepared goods: %d", business.getPreparedGoods()));
        }
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
