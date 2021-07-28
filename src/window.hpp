/*
* window.hpp is part of this program. Copyright Connor M
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
#include "gridutl.hpp"

const int mineCount = 10;

struct field_item {
	bool isMine = false;
	bool uncovered = false;
	wxButton *button = nullptr;

	void handle_press();
};

class window : public wxFrame {
friend struct field_item;
public:
	window();

	static int uncovered;

	static int count_neighbouring_mines(int btnid);
	static void iterate_neighbours(int centreid, int radius, auto &foreachproc);
private:
	static int m_threshold;
	static bool m_firstclick;
	static std::array<field_item, gridWidth * gridHeight> m_minefield;

	static void btn_click(wxCommandEvent &evt);
	static void gen_grid(const wxCommandEvent &evt);
};
