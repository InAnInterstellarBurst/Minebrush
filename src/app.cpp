/*
* app.cpp is part of this program. Copyright Connor M
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
#include <wx/wx.h>

#include "app.hpp"
#include "window.hpp"

wxIMPLEMENT_APP(app);

const std::string gplnotice = "========================================================================\n"
	"This program is free software: you can redistribute it and/or modify\n"
	"it under the terms of the GNU General Public License as published by\n"
	"the Free Software Foundation, either version 3 of the License, or\n"
	"(at your option) any later version.\n\n"
	"This program is distributed in the hope that it will be useful,\n"
	"but WITHOUT ANY WARRANTY; without even the implied warranty of\n"
	"MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the\n"
	"GNU General Public License for more details.\n\n"
	"You should have received a copy of the GNU General Public License\n"
	"along with this program.  If not, see <https://www.gnu.org/licenses/>.\n"
	"========================================================================";

bool app::OnInit()
{
	int flags = 10;
	int gridsize = 20;
	int minecount = 100;

	for(int i = 0; i < argc; i++) {
		if(strcmp(argv[i].c_str(), "-size") == 0) {
			gridsize = atoi(argv[++i].c_str());
		} else if(strcmp(argv[i].c_str(), "-mines") == 0) {
			minecount = atoi(argv[++i].c_str());
		} else if(strcmp(argv[i].c_str(), "-flags") == 0) {
			flags = atoi(argv[++i].c_str());
		} else if(strcmp(argv[i].c_str(), "-showmines") == 0) {
			kShowMines = true;
		}
	}

	if(gridsize == 0 || minecount == 0 || (gridsize * gridsize) / 2 <= minecount || ((gridsize * gridsize) / 100) * 20 >= minecount) {
		std::cout << "Invalid game arguements.\n\tGridsize != 0\n\tMinecount != 0\n\tGrid must be more than 20% mines\n\tGrid cannot be more than 50% mines\n";
		wxExit();
	}

	std::cout << gplnotice << std::endl;
	m_win = new window(gridsize, minecount, flags);
	m_win->Show();
	return true;
}
