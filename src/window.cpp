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
	if(window::m_firstclick)
		gen_grid(evt.GetId());

	field_item &tile = window::m_minefield[static_cast<size_t>(evt.GetId() - 10000)];
	if(tile.isMine) {
		wxMessageDialog d(nullptr, "You are dead. Play again?", "Drink barry's red cola", wxYES_NO | wxCENTRE);
		d.Layout();
		if(d.ShowModal() == wxID_YES) {
			// Restart game
			window::m_firstclick = true;
			for(auto &f : window::m_minefield) {
				f.isMine = false;
				f.uncovered = false;
				f.button->Enable();
				f.button->SetLabel("");
			}
		} else {
			wxMessageDialog(nullptr, "Alrght, looser").ShowModal();
			wxExit();
		}
	} else {
		tile.uncovered = true;
		tile.button->Disable();

		int neighbours = count_neighbouring_mines(evt.GetId());
		if(neighbours > 0)
			tile.button->SetLabel(std::to_string(neighbours));

		const auto revproc = [&](field_item &i) {
			if(!i.isMine) {
				i.uncovered = true;
				i.button->Disable();

				int n = count_neighbouring_mines(i.button->GetId());
				if(n != 0)
					i.button->SetLabel(std::to_string(n));
			}
		};

		iterate_neighbours(evt.GetId(), 2 - neighbours, revproc);
	}

	evt.Skip();
}

void window::gen_grid(int btnid)
{
	std::srand(static_cast<uint32_t>(time(NULL)));
	for(int i = 0; i < 100; i++) {
		int coord = rand() % static_cast<int>(gridWidth * gridHeight);
		size_t index = static_cast<size_t>(coord);
		if(coord == btnid - 10000 || window::m_minefield[index].isMine) {
			// We don't want mines on the first square or where there is already a mine, but we do want 100 mines
			i--;
			continue;
		}

		window::m_minefield[index].isMine = true;
		window::m_minefield[index].button->SetLabel("Mine");
		window::m_firstclick = false;
	}
}

int window::count_neighbouring_mines(int btnid)
{
	int count = 0;
	const auto countproc = [&](field_item &i) {
		if(i.isMine)
			count++;
	};

	iterate_neighbours(btnid, 1, countproc);
	return count;
}

void window::iterate_neighbours(int centreid, int radius, auto &foreachproc)
{
	auto[ox, oy] = id_to_btn_position(centreid);
	for(int dy = -radius; dy < radius * 2; dy++) {
		for(int dx = -radius; dx < radius * 2; dx++) {
			int x = ox + dx;
			int y = oy + dy;

			// Omit centre + bounds check
			if(dx == 0 && dy == 0)
				continue;
			if(x < 0 || y < 0 || x >= static_cast<int>(gridWidth) || y >= static_cast<int>(gridHeight))
				continue;
			size_t index = grid_index(static_cast<size_t>(x), static_cast<size_t>(y));
			foreachproc(window::m_minefield[index]);
		}
	}
}
