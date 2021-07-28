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

void field_item::handle_press()
{
	uncovered = true;
	button->Disable();
	window::uncovered++;

	int neighbours = window::count_neighbouring_mines(button->GetId());
	if(neighbours > 0)
		button->SetLabel(std::to_string(neighbours));

	const auto revproc = [&](field_item &i) {
		if(!i.isMine && !i.uncovered) {
			i.button->Disable();
			i.uncovered = true;
			window::uncovered++;
			int n = window::count_neighbouring_mines(i.button->GetId());
			if(n != 0)
				i.button->SetLabel(std::to_string(n));
		}
	};

	window::iterate_neighbours(button->GetId(), 2 - neighbours, revproc);
}


int window::uncovered = 0;
int window::m_threshold = (gridWidth * gridHeight) - mineCount;

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
		gen_grid(evt);

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

	evt.Skip();
}

void window::gen_grid(const wxCommandEvent &evt)
{
	std::srand(static_cast<uint32_t>(time(NULL)));
	for(int i = 0; i < mineCount; i++) {
		size_t index = static_cast<size_t>(rand()) % (gridWidth * gridHeight);
		if(index == as_index<size_t>(evt) || window::m_minefield[index].isMine) {
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
