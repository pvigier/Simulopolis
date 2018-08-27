#include "gui/GuiTable.h"
#include "gui/Gui.h"
#include "gui/GuiText.h"
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
        titles.push_back(mGui->createWithDefaultName<GuiText>(columnName, 12, &mStyle->getFirstChildByName("title")));
    addRow(titles, true);

    // Set Layout
    setLayout(std::make_unique<GuiHBoxLayout>());
}

void GuiTable::updateSize()
{
    updateTable();
    for (GuiWidget* widget : mChildren)
        widget->setFixedInsideSize(widget->getContentSize());
    if (!mFixedSize)
        onContentSizeChanged(getContentSize());
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
    setDirty();
}

void GuiTable::removeRow(std::size_t i)
{
    for (std::size_t j = 0; j < mChildren.size(); ++j)
        mChildren[j]->remove(i + 1);
    mRowHeights.erase(mRowHeights.begin() + i + 1);
    setDirty();
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
