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

#ifndef _Argument_
#define _Argument_

// Includes
#include <string>
#include "ShuntingYard.h"
using namespace std;

// Global variables
extern unsigned int Lines;
extern vector<Label*> Labels;

// Argument type
enum ArgType
{
	// Invalid argument
	AT_Invalid = 0,

	// Constant value
	AT_Const = 1,

	// Register number
	AT_Reg = 2,

	// Ram memory adress
	AT_Ram = 3,
	
	// Ram memory adress at register number
	AT_RamAtReg = 4,
	
	// Adress in the program memory
	AT_Adress = 5,
	
	// Compare type
	AT_Compare = 6,
	
	// Special data
	AT_Special = 7,
};

// Compare types
int CompareTypes_Size = 6;
string CompareTypes[] =
{
	"==",
	"!=",
	">",
	">=",
	"<",
	"=<"
};

// Special types
int SpecialTypes_Size = 1;
string SpecialTypes[] =
{
	"goto"
};


class Argument
{
	public:

		// Argument text
		string Text;

		// Argument type
		ArgType Type;
		
		// Argument value
		unsigned int Value;

		// Constructor
		Argument(string arg)
		{
			// Set up
			Text = arg;
			Type = AT_Invalid;
			Value = 0;

			// Check all labels
			for(int i = 0; i < Labels.size(); i++)
			{
				// Compare label name
				if(Labels[i]->GetName() == arg)
				{
					Value = Labels[i]->Adress;
					Type = AT_Adress;
					return;
				}
			}

			// Calculator
			Calculator calc;

			// Check if it is 'Reg[x]'
			if((arg.find("Reg") == 0 || arg.find("reg") == 0) && arg.size() >= 6 && arg[3] == '[' && arg[arg.size() - 1] == ']')
   			{
				// Get string inside []
				string tmp = arg.substr(4, arg.size() - 5);
				
				try
				{
					// Get register index
					Value = calc.calculate(tmp);
					Type = AT_Reg;
				}
				catch(string w)
				{
					Type = AT_Invalid;
					printf("Cannot parse register index at line %u. %s\n", Lines, w.c_str());
				}
				return;
			}
			
			// Check if it is 'Ram[x]' or 'Ram[Reg[x]]'
			if((arg.find("Ram") == 0 || arg.find("ram") == 0) && arg.size() >= 6 && arg[3] == '[' && arg[arg.size() - 1] == ']')
   			{
				// Get string inside []
				string tmp = arg.substr(4, arg.size() - 5);

				try
				{
					// Get register index
					Value = calc.calculate(tmp);
					Type = AT_Ram;
				}
				catch(string w)
				{
					// Try to check if it is 'Ram[Reg[x]]'
					if((tmp.find("Reg") == 0 || arg.find("reg") == 0) && arg.size() >= 6 && arg[3] == '[' && arg[arg.size() - 1] == ']')
					{
						// Get string inside []
						tmp = tmp.substr(4, tmp.size() - 5);
				
						// Try to calculate expresion
						try
		            	{
							Value = calc.calculate(tmp);
							Type = AT_RamAtReg;
		            	}
		           		catch(string w)
		            	{
							// Error
							Type = AT_Invalid;
							printf("Cannot parse register index at line %u. %s\n", Lines, w.c_str());
		            	}
					}
					else
					{
						// Error
						Type = AT_Invalid;
						printf("Cannot parse RAM index at line %u. %s\n", Lines, w.c_str());
					}
				}
				return;
			}
			
			// Check all special types
			for(int i = 0; i < SpecialTypes_Size; i++)
			{
				if(SpecialTypes[i] == arg)
				{
					Value = i;
					Type = AT_Special;
					return;
				}
			}
			
			// Check all compare types
			for(int i = 0; i < CompareTypes_Size; i++)
			{
				if(CompareTypes[i] == arg)
				{
					Value = i;
					Type = AT_Compare;
					return;
				}
			}
			
			// Try to calculate expresion
			try
        	{
				Value = calc.calculate(arg);
				Type = AT_Const;
        	}
       		catch(string w)
        	{
				// Error
				Type = AT_Invalid;
				printf("Cannot parse constant expresion at line %u. %s\n", Lines, w.c_str());
        	}
		}

		// Destructor
		~Argument()
		{
			// Clean
			Text.clear();
			Type = AT_Invalid;
			Value = 0;
		}
};

#endif
