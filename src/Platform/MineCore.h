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

#ifndef _MineCore_
#define _MineCore_

// Includes
#include <iostream>
#include <stdio.h>
#include <bitset>
#include <string>
#include <vector>
#include <fstream>
#include "../Command.h"
using namespace std;

namespace MysticAsm
{
	class MineCore : public Platform
	{
		public:
			MineCore()
			{}
			
		// Parse command after reading
		bool Parse(vector<Command*> &Commands)
		{
			// Process all commands
			for(int i = 0; i < Commands.size(); i++)
			{
				// Cache
				Command *cmd = Commands[i];
				string name = cmd->GetName();
				
				// Exit
				if(name == "exit")
				{
					// Convert to 'int 0'
					cmd->Convert("int");
					cmd->AddArgument("0");
				}
			}
			
			// Good
			return false;
		}
		
		// Set up commands adresses
		void ComputeAdresses(vector<Command*> &Commands, unsigned int globalOffset)
		{
			// All commands has 1 memory cell
			for(int i = 0; i < Commands.size(); i++)
			{
				// Set adress
				Commands[i]->Adress = globalOffset + i;
			}
		}
		
		// Serialize program to the file
		bool WriteToFile(vector<Command*> &Commands, vector<Label*> &Labels, ofstream &oFile)
		{
			// Temporary variables
			unsigned int i, s, o;
			unsigned char c[4];
			Command *cmd;
			Argument *a1, *a2, *a3, *a4, *a5;
			string str;

			// Write program
			oFile << "\n================= Program =================\n\n  Adress - Command -  r0   -   r1   -   r2\n";

			// Serialize all commands
			for(i = 0; i < Commands.size(); i++)
			{
				// Cache
				cmd = Commands[i];
				c[0] = 0;
				c[1] = 0;
				c[2] = 0;
				c[3] = 0;

				// int r0
				if(cmd->GetName() == "int")
				{
					// Cache argument
					a1 = cmd->GetArgumentAt(0);
					
					// Check argument
					if(a1->Type != AT_Const)
					{
						// Error
						printf("Invalid \'int\' command argument at line %u", cmd->GetLine());
						return true;
					}
					
					// Set up
					c[0] = 1;
					c[1] = a1->Value;
				}
				else if(cmd->GetName() == "mov")
				{
					// Cache arguments
					a1 = cmd->GetArgumentAt(0);
					a2 = cmd->GetArgumentAt(1);
					
					// mov Reg[r1], r0
					if(a1->Type == AT_Reg && a2->Type == AT_Const)
					{
						// Set up
						c[0] = 2;
						c[1] = a2->Value;
						c[2] = a1->Value;
					}
					// mov Ram[r1], Reg[r2]
					else if(a1->Type == AT_Ram && a2->Type == AT_Reg)
					{
						// Set up
						c[0] = 22;
						c[2] = a1->Value;
						c[3] = a2->Value;
					}
				}
				else if(cmd->GetName() == "sub")
				{
					// Cache arguments
					a1 = cmd->GetArgumentAt(0);
					a2 = cmd->GetArgumentAt(1);

					// ALU r0 Reg[r2], r1
					if(a1->Type == AT_Reg && a2->Type == AT_Const)
					{
						// Set up
						c[0] = 6;
						c[1] = 1;
						c[2] = a2->Value;
						c[3] = a1->Value;
					}
					// ALU r0 Reg[r2], Reg[r1]
					else if(a1->Type == AT_Reg && a2->Type == AT_Reg)
					{
						// Set up
						c[0] = 5;
						c[1] = 1;
						c[2] = a2->Value;
						c[3] = a1->Value;
					}
				}
				else if(cmd->GetName() == "if")
				{
					// Cache arguments
					a1 = cmd->GetArgumentAt(0);
					a2 = cmd->GetArgumentAt(1);
					a3 = cmd->GetArgumentAt(2);
					a5 = cmd->GetArgumentAt(4);
					
					// goto r0 if Reg[r1] == Reg[r2]
					if(a1->Type == AT_Reg && a3->Type == AT_Reg && a2->Type == 6 && a2->Value == 0)
					{
						// Set up
						c[0] = 10;
						c[1] = a5->Value;
						c[2] = a1->Value;
						c[3] = a3->Value;
					}
					// goto r0 if Reg[r1] != Reg[r2]
					else if(a1->Type == AT_Reg && a3->Type == AT_Reg && a2->Type == 6 && a2->Value == 1)
					{
						// Set up
						c[0] = 11;
						c[1] = a5->Value;
						c[2] = a1->Value;
						c[3] = a3->Value;
					}
					// goto r0 if Reg[r1] > Reg[r2]
					else if(a1->Type == AT_Reg && a3->Type == AT_Reg && a2->Type == 6 && a2->Value == 2)
					{
						// Set up
						c[0] = 12;
						c[1] = a5->Value;
						c[2] = a1->Value;
						c[3] = a3->Value;
					}
					// goto r0 if Reg[r1] >= Reg[r2]
					else if(a1->Type == AT_Reg && a3->Type == AT_Reg && a2->Type == 6 && a2->Value == 3)
					{
						// Set up
						c[0] = 13;
						c[1] = a5->Value;
						c[2] = a1->Value;
						c[3] = a3->Value;
					}
					// goto r0 if Reg[r1] > Reg[r2]
					else if(a1->Type == AT_Reg && a3->Type == AT_Reg && a2->Type == 6 && a2->Value == 4)
					{
						// Set up
						c[0] = 12;
						c[1] = a5->Value;
						c[2] = a3->Value;
						c[3] = a1->Value;
					}
					// goto r0 if Reg[r1] >= Reg[r2]
					else if(a1->Type == AT_Reg && a3->Type == AT_Reg && a2->Type == 6 && a2->Value == 4)
					{
						// Set up
						c[0] = 13;
						c[1] = a5->Value;
						c[2] = a3->Value;
						c[3] = a1->Value;
					}
				}
				
				// Check if command was created
				if(c[0] == 0)
				{
					// Error
					printf("Command \'%s\' at line %u is not supported by MineCore.\n", cmd->GetName().c_str(), cmd->GetLine());
					return true;
				}
				
				// Write command adress in ROM
				s = cmd->Adress / 16;
				o = cmd->Adress % 16;
				stringstream ss;
				ss << ' ' << s << ':' << o << ": ";
				str = ss.str();
				while(str.size() < 10)
					str.insert(0, " ");
				oFile << str;
				
				// Write CPU command
				oFile << bitset<8>(c[0]) << ' ' << bitset<8>(c[1]) << ' ' << bitset<8>(c[2]) << ' ' << bitset<8>(c[3]);
				
				// Write command info
				oFile << "   ; " << cmd->GetText() << endl;
			}
			
			// Write program info
			oFile << "\n================== Info ===================\n\nProgram base adress: " << Commands[0]->Adress;
			oFile << "\nProgram size: " << (Commands.size() * 4);
			oFile << " bytes\nCommands: " << Commands.size();
			oFile << "\nLabels: " << Labels.size() << endl;
			
			
			
			


/*
// TODO usunac to
			oFile << "\n\n\n\nTODO.. dev info. will be deleted\nCommands ("<< Commands.size() << "):"<<endl;
			for(int i=0;i<Commands.size();i++)
			{
				oFile<< i << ". " << Commands[i]->GetText() << ", at " << Commands[i]->Adress << endl;
			}
			oFile << "Labels ("<< Labels.size() << "):"<<endl;
			for(int i=0;i<Labels.size();i++)
			{
				oFile<< i << ". " << Labels[i]->GetText() << ", at " <<  Labels[i]->Adress << endl;
			}
			*/
			



			// Good
			return false;
		}
	};
}

#endif
