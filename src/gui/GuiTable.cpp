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

#include "gui/GuiTable.h"
#include "gui/Gui.h"
#include "gui/GuiLabel.h"
#include "gui/GuiVBoxLayout.h"
#include "gui/GuiHBoxLayout.h"
#include "resource/XmlDocument.h"

GuiTable::GuiTable(std::vector<std::string> columnNames, const XmlDocument* style) :
    GuiWidget(style), mColumnNames(std::move(columnNames)), mColumnWidths(mColumnNames.size(), 0.0f)
{


}

GuiTable::GuiTable(const PropertyList& properties) : GuiWidget(properties)
{

}

void GuiTable::setUp()
{
    // Create columns
    for (std::size_t i = 0; i < mColumnNames.size(); ++i)
    {
        GuiWidget* column = mGui->createWithDefaultName<GuiWidget>();
        column->setLayout(std::make_unique<GuiVBoxLayout>());
        add(column);
    }

    // Add titles
    std::vector<GuiWidget*> titles;
    for (const std::string& columnName : mColumnNames)
        titles.push_back(mGui->createWithDefaultName<GuiLabel>(columnName, 12, &mStyle->getFirstChildByName("title")));
    addRow(titles, true);

    // Set Layout
    setLayout(std::make_unique<GuiHBoxLayout>());
}

void GuiTable::updateSize()
{
    updateTable();
    for (GuiWidget* widget : mChildren)
        widget->setFixedInsideSize(widget->getContentSize());
    if (mSizePolicies[0] == SizePolicy::FIT_TO_CONTENT || mSizePolicies[1] == SizePolicy::FIT_TO_CONTENT)
    {
        sf::Vector2f contentSize = getContentSize();
        if (mSizePolicies[0] == SizePolicy::FIT_TO_CONTENT && contentSize.x != mInsideSize.x)
            onContentWidthChanged(contentSize.x);
        if (mSizePolicies[1] == SizePolicy::FIT_TO_CONTENT && contentSize.y != mInsideSize.y)
            onContentHeightChanged(contentSize.y);
    }
}

GuiWidget* GuiTable::getCell(std::size_t i, std::size_t j)
{
    return mChildren[j]->getChildren()[i + 1];
}

GuiWidget* GuiTable::getCellContent(std::size_t i, std::size_t j)
{
    return mChildren[j]->getChildren()[i + 1]->getChildren()[0];
}

void GuiTable::addRow(const std::vector<GuiWidget*>& row, bool isTitle)
{
    mRowHeights.push_back(0.0f);
    for (std::size_t j = 0; j < mChildren.size(); ++j)
    {
        // Create cell
        GuiWidget* cell = nullptr;
        if (isTitle)
            cell = mGui->createWithDefaultName<GuiWidget>(&mStyle->getFirstChildByName("title"));
        else
            cell = mGui->createWithDefaultName<GuiWidget>(&mStyle->getFirstChildByName("cell"));
        cell->add(row[j]);
        cell->setLayout(std::make_unique<GuiVBoxLayout>(GuiLayout::HAlignment::Center, GuiLayout::VAlignment::Center, 0.0f, GuiLayout::Margins{3.0f, 3.0f, 3.0f, 3.0f}));
        mChildren[j]->add(cell);
    }
}

void GuiTable::removeRow(std::size_t i)
{
    for (std::size_t j = 0; j < mChildren.size(); ++j)
        mChildren[j]->remove(i + 1);
    mRowHeights.erase(mRowHeights.begin() + i + 1);
}

void GuiTable::clear()
{
    for (int i = mRowHeights.size() - 2; i >= 0; --i)
        removeRow(i);
}

void GuiTable::updateTable()
{
    // Compute sizes
    std::fill(mColumnWidths.begin(), mColumnWidths.end(), 0.0f);
    std::fill(mRowHeights.begin(), mRowHeights.end(), 0.0f);
    for (std::size_t j = 0; j < mColumnWidths.size(); ++j)
    {
        for (std::size_t i = 0; i < mRowHeights.size(); ++i)
        {
            GuiWidget* cell = mChildren[j]->getChildren()[i];
            cell->updateSize();
            sf::Vector2f size = cell->getContentSize();
            mColumnWidths[j] = std::max(mColumnWidths[j], size.x);
            mRowHeights[i] = std::max(mRowHeights[i], size.y);
        }
    }

    // Set sizes
    for (std::size_t j = 0; j < mColumnWidths.size(); ++j)
    {
        for (std::size_t i = 0; i < mRowHeights.size(); ++i)
        {
            GuiWidget* cell = mChildren[j]->getChildren()[i];
            cell->setFixedInsideSize(sf::Vector2f(mColumnWidths[j], mRowHeights[i]));
        }
    }
}
