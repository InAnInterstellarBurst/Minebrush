/*
* field.cpp is part of this program. Copyright Connor M
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
#include "field.hpp"
#include "window.hpp"

const wxColour tile::kUncoveredColour = { 211, 211, 211 };

tile::tile(window *w, wxGridSizer *uigrid, int index) : m_index(index)
{
	m_button = new wxButton(w, index + window::kBtnIdOffset);
	uigrid->Add(m_button, 1, wxEXPAND);
	m_button->Bind(wxEVT_RIGHT_DOWN, &window::right_click);
	m_button->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &window::btn_click);
}

/*
* @return The number of tiles revealed
* */
int tile::uncover(const minefield &field)
{
	if(m_flagged)
		return 0;
	if(mine)
		return -1;

	m_uncovered = true;
	m_button->Disable();
	m_button->SetBackgroundColour(kUncoveredColour);

	int neighbours = field.count_neighbours(m_index);
	if(neighbours > 0)
		m_button->SetLabel(std::to_string(neighbours));
	return 1 + uncover_neighbours(field, neighbours);
}

// This function is absolutely fucking awful
// but it's the only I can get this to work
int tile::toggle_flag(int flags)
{
	if(m_flagged) {
		m_flagged = false;
		m_button->SetLabel("");
		return flags + 1;
	}

	if(flags > 0 && !m_flagged) {
		m_flagged = true;
		m_button->SetLabel("!");
		return flags - 1;
	}

	return 0;
}

int tile::uncover_neighbours(const minefield &field, int neighbours)
{
	int uncoveredNeighbours = 0;
	const auto revproc = [&](const tile &i) {
		if(!i.mine && !i.m_uncovered && !i.m_flagged) {
			uncoveredNeighbours++;
			i.m_uncovered = true;
			i.m_button->Disable();
			i.m_button->SetBackgroundColour(kUncoveredColour);
			int n = field.count_neighbours(i.m_index);
			if(n != 0)
				i.m_button->SetLabel(std::to_string(n));
		}
	};

	field.iterate_neighbours(m_index, (field.area() / field.mine_count()) - neighbours, revproc);
	return uncoveredNeighbours;
}


minefield::minefield(window *w, wxGridSizer *uigrid, int mines, int gridSize, int flags) :
	m_mines(mines), m_gridSize(gridSize), m_activeTiles((gridSize * gridSize) - mines),
	m_flagsRemaining(flags), m_totalFlags(flags)
{
	m_field.reserve(gridSize * gridSize);
	for(int y = 0; y < gridSize; y++) {
		for(int x = 0; x < gridSize; x++) {
			m_field.emplace_back(w, uigrid, as_index(x, y));
		}
	}
}

/*
* @return true if game restarts
*/
bool minefield::reveal_tile(int tileindex)
{
	int tilesrevealed = m_field[tileindex].uncover(*this);
	if(tilesrevealed != -1) {
		m_activeTiles -= tilesrevealed;
		if(m_activeTiles == 0) {
			wxMessageDialog win(nullptr, "Wow, I am very impressed");
			win.ShowModal();
			wxExit();
		}
	} else {
		// Loss
		wxMessageDialog retry(nullptr, "I'm in awe that you managed to loose, I really am.\nTry again?",
			"Drink Barry's red cola", wxYES_NO | wxCENTRE);
		if(retry.ShowModal() == wxID_YES) {
			clear_field();
			return true;
		} else {
			wxMessageDialog d(nullptr, "Ok, looser");
			d.ShowModal();
			wxExit();
		}
	}

	return false;
}

void minefield::clear_field()
{
	for(auto &t : m_field) {
		t.deflag();
		t.mine = false;
		t.get_btn()->Enable();
		t.get_btn()->SetLabel("");
		t.get_btn()->SetBackgroundColour({});
	}

	m_flagsRemaining = m_totalFlags;
}

void minefield::flag(int clickindex)
{
	m_flagsRemaining = m_field[clickindex].toggle_flag(m_flagsRemaining);
}

void minefield::populate_field(int clickindex)
{
	std::srand(static_cast<uint32_t>(time(nullptr)));
	for(int i = 0; i < m_mines; i++) {
		int index = rand() % (m_gridSize * m_gridSize);
		if(index == clickindex || m_field[index].mine) {
			// We don't want mines on the first square or where there is already a mine, but we do want 100 mines
			i--;
			continue;
		}

		m_field[index].mine = true;
		if(kShowMines)
			m_field[index].get_btn()->SetLabel("Mine");
	}
}

int minefield::count_neighbours(int tileindex) const
{
	int count = 0;
	const auto countproc = [&](const tile &i) {
		if(i.mine)
			count++;
	};

	iterate_neighbours(tileindex, 1, countproc);
	return count;
}

void minefield::iterate_neighbours(int centreindex, int radius, auto &foreachproc) const
{
	auto[ox, oy] = index_to_position(centreindex);
	for(int dy = -radius; dy < radius * 2; dy++) {
		for(int dx = -radius; dx < radius * 2; dx++) {
			int x = ox + dx;
			int y = oy + dy;

			// Omit centre + bounds check
			if(dx == 0 && dy == 0)
				continue;
			if(x < 0 || y < 0 || x >= m_gridSize || y >= m_gridSize)
				continue;

			int index = as_index(x, y);
			foreachproc(m_field[index]);
		}
	}
}

std::pair<int, int> minefield::index_to_position(int index) const
{
	return std::make_pair(index % m_gridSize, index / m_gridSize);
}
