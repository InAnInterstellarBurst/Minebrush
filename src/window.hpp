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
#include "field.hpp"

class window : public wxFrame {
friend struct field_item;
public:
	window(int gridsize, int minecount);

	static void btn_click(wxCommandEvent &evt);
	constexpr static int kBtnIdOffset = 10000;
private:
	static bool m_firstclick;
	static std::unique_ptr<minefield> m_field;
};
