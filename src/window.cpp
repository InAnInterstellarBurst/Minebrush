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

#include <algorithm>

bool window::m_firstclick = true;
std::unique_ptr<minefield> window::m_field = nullptr;

window::window(int gridsize, int minecount, int flags) : wxFrame(nullptr, wxID_ANY, "Minebrush", wxPoint(30, 30), wxSize(1280, 800))
{
	wxGridSizer *grid = new wxGridSizer(gridsize, gridsize, 1, 1);
	if(m_field == nullptr)
		m_field = std::make_unique<minefield>(this, grid, minecount, gridsize, flags);

	this->SetSizer(grid);
	grid->Layout();
}

void window::btn_click(wxCommandEvent &evt)
{
	if(m_firstclick) {
		m_field->populate_field(evt.GetId() - kBtnIdOffset);
		m_firstclick = false;
	}

	// This seems a bit nonsensical, if reveal tile returns true
	// the game was reset
	if(m_field->reveal_tile(evt.GetId() - kBtnIdOffset))
		m_firstclick = true;
	evt.Skip();
}

void window::right_click(wxMouseEvent &evt)
{
	m_field->flag(evt.GetId() - kBtnIdOffset);
}
