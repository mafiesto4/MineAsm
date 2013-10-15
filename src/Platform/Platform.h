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

#ifndef _Platform_
#define _Platform_

// Includes
#include <iostream>
#include <stdio.h>
#include <string>
#include <sstream>
#include <vector>
#include <fstream>
#include "../Label.h"
#include "../Command.h"
using namespace std;

namespace MysticAsm
{
	class Platform
	{
		public:

		// Parse command after reading
		virtual bool Parse(vector<Command*> &Commands)
		{
			// Error
			return true;
		}

		// Set up commands adresses
		virtual void ComputeAdresses(vector<Command*> &Commands, unsigned int globalOffset)
		{}
		
		// Serialize program to the file
		virtual bool WriteToFile(vector<Command*> &Commands, vector<Label*> &Labels, ofstream &oFile)
		{
			// Error
			return true;
		}
	};
}

#endif
