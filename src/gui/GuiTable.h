#pragma once

#include "gui/GuiWidget.h"

class GuiTable : public GuiWidget
{
public:
    GuiTable(std::vector<std::string> columnNames, const XmlDocument* style);
    GuiTable(const PropertyList& properties);

    virtual void setUp() override;

    void addRow(const std::vector<GuiWidget*>& row, bool isTitle = false);

private:
    std::vector<std::string> mColumnNames;
    std::vector<float> mRowHeights;
    std::vector<float> mColumnWidths;

    void updateRow(std::size_t i);
    void updateColumn(std::size_t j);
};
