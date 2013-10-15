/*
Copyright (C) 2013 Wojciech Figat

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*/

#ifndef _Label_
#define _Label_

// Includes
#include <string>
using namespace std;

class Label
{
	private:

		// Private Stuff
		string _name;
		unsigned int _line;

	public:

		// Next command
		void *NextCommand;
		
		// Jump adress in program memory
		unsigned int Adress;

		// Constructor
		Label(string name, unsigned int line)
		{
			// Set up
			_name = name;
			_line = line;
			NextCommand = NULL;
		}

		// Destructor
		~Label()
		{
			// Clean
			NextCommand = NULL;
			_name.clear();
		}

		// Label name
		string GetName()
		{
			return _name;
		}

		// Line with this label
		unsigned int GetLine()
		{
			return _line;
		}
		
		// Get label text info
		string GetText()
		{
			ostringstream res;
			res << _name + " at line " << _line;
			return res.str();
		}
};

#endif
