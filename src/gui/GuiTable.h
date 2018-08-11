#pragma once

#include "gui/GuiWidget.h"

class GuiTable : public GuiWidget
{
public:
    GuiTable(std::vector<std::string> columnNames, const XmlDocument* style);
    GuiTable(const PropertyList& properties);

    virtual void setUp() override;
    virtual void updateSize() override;

    GuiWidget* getCell(std::size_t i, std::size_t j);
    GuiWidget* getCellContent(std::size_t i, std::size_t j);
    void addRow(const std::vector<GuiWidget*>& row, bool isTitle = false);
    void removeRow(std::size_t i);
    void clear();

private:
    std::vector<std::string> mColumnNames;
    std::vector<float> mRowHeights;
    std::vector<float> mColumnWidths;

    void updateTable();
};
