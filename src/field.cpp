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

tile::tile(window *w, wxGridSizer *uigrid, int index) : m_index(index)
{
	m_button = new wxButton(w, index + window::kBtnIdOffet);
	uigrid->Add(m_button);
	m_button->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &window::btn_click);
}

bool tile::uncover(const minefield &field)
{
	if(mine)
		return false;

	m_uncovered = true;
	m_button->Disable();

	int neighbours = field.count_neighbours(m_index);
	if(neighbours > 0)
		m_button->SetLabel(std::to_string(neighbours));
	uncover_neighbours(field, neighbours);
	return true;
}

void tile::uncover_neighbours(const minefield &field, int neighbours)
{
	const auto revproc = [&](tile &i) {
		if(!i.mine && !i.m_uncovered) {
			i.m_button->Disable();
			i.m_uncovered = true;
			int n = field.count_neighbours(i.m_index);
			if(n != 0)
				i.m_button->SetLabel(std::to_string(n));
		}
	};

	field.iterate_neighbours(m_index, 2 - neighbours, revproc);
}


minefield::minefield(window *w, wxGridSizer *uigrid, int mines, int gridSize) :
	m_mines(mines), m_gridSize(gridSize)
{
	m_field.reserve(gridSize * gridSize);
	for(int y = 0; y < gridSize; y++) {
		for(int x = 0; x < gridSize; x++) {
			m_field.emplace_back(w, uigrid, as_index(x, y));
		}
	}
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
		m_field[index].get_btn()->SetLabel("Mine");
	}
}

int minefield::count_neighbours(int tileindex)
{
	int count = 0;
	const auto countproc = [&](tile &i) {
		if(i.mine)
			count++;
	};

	iterate_neighbours(tileindex, 1, countproc);
	return count;
}

void minefield::iterate_neighbours(int centreindex, int radius, auto &foreachproc)
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
