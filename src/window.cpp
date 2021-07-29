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

constexpr int gridsize = 10;
constexpr int minecount = 50;

window::window() : wxFrame(nullptr, wxID_ANY, "Minebrush", wxPoint(30, 30), wxSize(1280, 800))
{
	wxGridSizer *grid = new wxGridSizer(gridsize, gridsize, 1, 1);
	if(m_field == nullptr)
		m_field = std::make_unique<minefield>(this, grid, minecount, gridsize);
	this->SetSizer(grid);
	grid->Layout();
}

void window::btn_click(wxCommandEvent &evt)
{
	if(m_firstclick)
		m_field->populate_field(evt.GetId() - kBtnIdOffet);

#if 0
	field_item &tile = window::m_minefield[as_index<size_t>(evt)];
	if(tile.isMine) {
		wxMessageDialog d(nullptr, "It's ok, someone has to be the failure :)\nPlay again?", "Drink barry's red cola", wxYES_NO | wxCENTRE);
		d.Layout();
		if(d.ShowModal() == wxID_YES) {
			// Restart game
			window::m_firstclick = true;
			for(auto &f : window::m_minefield) {
				f.isMine = false;
				f.button->Enable();
				f.button->SetLabel("");
			}
		} else {
			wxMessageDialog(nullptr, "Alrght, looser").ShowModal();
			wxExit();
		}
	} else {
		m_minefield[as_index<size_t>(evt)].handle_press();
		printf("Uncovered %d\n", uncovered);
		if(uncovered == m_threshold) {
			wxMessageDialog d(nullptr, "Well done", ":)");
			d.ShowModal();
			wxExit();
		}

	}
#endif

	evt.Skip();
}
