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

#ifndef _Commnad_
#define _Commnad_

// Includes
#include <vector>
#include <string>
#include "Argument.h"
using namespace std;

// Global variables
extern vector<Label*> Labels;
extern string CompareTypes[];
extern string SpecialTypes[];

class Command
{
	private:

		// Private Stuff
		string _name;
		unsigned int _line;
		vector<Argument*> _arguments;

	public:

		// Adress in memory
		unsigned int Adress;

		// Constructor
		Command(string name, unsigned int line)
		{
			// Set up
			_name = name;
			_line = line;
		}

		// Destructor
		~Command()
		{
			// Clean
			_name.clear();
			_arguments.clear();
		}
		
		// Command name
		string GetName()
		{
			return _name;
		}
		
		// Line with this command
		unsigned int GetLine()
		{
			return _line;
		}

		// Add argument
		void AddArgument(string arg)
		{
			// Create argument
			Argument *a = new Argument(arg);
			
			// Add to list
			_arguments.push_back(a);
		}

		// Get argument at given index
		Argument* GetArgumentAt(int index)
		{
			return _arguments[index];
		}
		
		// Get command text info
		string GetText()
		{
			ostringstream res;
			res << _name << ' ';
			
			for(int i = 0; i < _arguments.size(); i++)
			{
				// Switch argument type
				switch(_arguments[i]->Type)
				{
					// Invalid argument
					case AT_Invalid: res << "Invalid argument"; break;

					// Constant value
					case AT_Const: res << _arguments[i]->Value; break;

					// Register number
					case AT_Reg: res << "Reg[" << _arguments[i]->Value << ']'; break;

					// Ram memory adress
					case AT_Ram: res << "Ram[" << _arguments[i]->Value << ']'; break;

					// Ram memory adress at register number
					case AT_RamAtReg: res << "Ram[Reg[" << _arguments[i]->Value << "]]"; break;

					// Adress in the program memory
					case AT_Adress: res << "Adress: " << _arguments[i]->Value; break;

					// Compare type
					case AT_Compare: res << CompareTypes[_arguments[i]->Value]; break;

					// Special data
					case AT_Special: res << SpecialTypes[_arguments[i]->Value]; break;
				}
				
				res << ", ";
			}
			
			res << "at line " << _line;
			
			return res.str();
		}
		
		// Convert command to another one
		void Convert(string name)
		{
			// Set up
			_name = name;

			// Clean arguments
			_arguments.clear();
		}
		
		// Update arguments
		void UpdateArguments()
		{
			// Update all label adresses
			for(int i = 0; i < _arguments.size(); i++)
			{
  				// Check type: Adress = 5
				if(_arguments[i]->Type == 5)
				{
					// Cache previous
					Argument *arg = _arguments[i];
					
					// Create new one
					_arguments[i] = new Argument(arg->Text);
					
					// Delete previous
					delete arg;
				}
			}
		}
};

#endif
