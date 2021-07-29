/*
* field.hpp is part of this program. Copyright Connor M
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

#pragma once

class window;
class minefield;

class tile {
public:
	bool mine = false;

	tile(window *w, wxGridSizer *uigrid, int index);
	int toggle_flag(int remaining);
	int uncover(const minefield &field);

	inline void deflag() { m_flagged = false; }
	inline wxButton *get_btn() const { return m_button; }
private:
	int m_index;
	bool m_flagged = false;
	mutable bool m_uncovered = false;
	wxButton *m_button = nullptr;

	int uncover_neighbours(const minefield &field, int neighbours);
};

class minefield {
public:
	minefield(window *w, wxGridSizer *uigrid, int mines, int gridSize, int flags);

	bool reveal_tile(int tileindex);
	void clear_field();
	void flag(int clickindex);
	void populate_field(int clickindex);

	int count_neighbours(int tileindex) const;
	void iterate_neighbours(int centreindex, int radius, auto &foreachproc) const;
private:
	int m_mines;
	int m_gridSize;
	int m_activeTiles;
	int m_flagsRemaining;
	const int m_totalFlags;
	std::vector<tile> m_field;

	std::pair<int, int> index_to_position(int index) const;
	inline int as_index(int x, int y) const { return y * m_gridSize + x; }
};
