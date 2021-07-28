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

constexpr size_t gridWidth = 20;
constexpr size_t gridHeight = 20;

constexpr size_t grid_index(size_t x, size_t y)
{
	return y * gridHeight + x;
}

constexpr std::pair<int, int> id_to_btn_index(int btnid)
{
	return std::make_pair((btnid - 10000) % static_cast<int>(gridWidth), (btnid - 10000) / static_cast<int>(gridHeight));
}

class window : public wxFrame {
public:
	window();
	~window();
private:
	std::array<wxButton*, gridWidth * gridHeight> m_buttons;

	static void btn_click(wxCommandEvent &evt);
};
