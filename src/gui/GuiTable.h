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
