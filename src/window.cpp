/*
* window.cpp is part of this program. Copyright Connor M
*     This program is free software: you can redistribute it and/or modify
*     it under the terms of the GNU General Public License as published by
*     the Free Software Foundation, either version 3 of the License, or
*     (at your option) any later version.
*
*     This program is distributed in the hope that it will be useful,
*     but WITHOUT ANY WARRANTY; without even the implied warranty of
*     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*     GNU General Public License for more details.
*
*     You should have received a copy of the GNU General Public License
*     along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#include "pch.hpp"
#include "window.hpp"

bool window::m_firstclick = false;
std::array<field_item, gridWidth * gridHeight> window::m_minefield;

window::window() : wxFrame(nullptr, wxID_ANY, "Minebrush", wxPoint(30, 30), wxSize(1280, 800))
{
	wxGridSizer *grid = new wxGridSizer(gridWidth, gridHeight, 1, 1);
	for(size_t y = 0; y < gridHeight; y++) {
		for(size_t x = 0; x < gridWidth; x++) {
			size_t index = grid_index(x, y);
			m_minefield[index].button = new wxButton(this, static_cast<wxWindowID>(10000 + index));
			grid->Add(m_minefield[index].button, 1, wxEXPAND);
			m_minefield[index].button->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &window::btn_click);
		}
	}

	this->SetSizer(grid);
	grid->Layout();
}

void window::btn_click(wxCommandEvent &evt)
{
	if(window::m_firstclick) {
		srand(time(NULL));
		for(int i = 0; i < 30; i++) {
			int coord = rand() % (gridWidth * gridHeight);
			window::m_minefield[coord].isMine = true;
			window::m_minefield[coord].button->SetLabel("Mine");
		}
	}

	evt.Skip();
}
