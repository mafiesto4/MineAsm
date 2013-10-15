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

// Includes
#include <iostream>
#include <stdio.h>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include "Platform/Platform.h"
#include "Platform/MineCore.h"
using namespace std;
using namespace MysticAsm;

// Info and Version
#define InfoVersion "MineAsm, v1.0\n"

// Global variables
unsigned int Lines = 0;
vector<Label*> Labels;

// Commands with no arguments
int Commands_0_Size = 1;
string Commands_0[] =
{
	"exit"
};

// Commands with 1 argument
int Commands_1_Size = 1;
string Commands_1[] =
{
	"int"
};

// Commands with 2 arguments
int Commands_2_Size = 2;
string Commands_2[] =
{
	"mov",
	"sub"
};

// Compare string
static int CompareString(const char* base, const char* next)
{
	int i = 0, diff = 0;
	
	while(base[i] != '\0')
	{
		if(base[i] != next[i])
		{
			diff++;
		}

		i++;
	}
	
	return diff;
}

static int CompareStrings(string *array, string val, int size)
{
	for(int i = 0 ; i < size; i++)
	{
		if(array[i] == val)
		{
			return i;
		}
	}
	
	return -1;
}

static void RemoveWhitespaces(string &str)
{
    for (size_t i = 0; i < str.length(); i++)
	{
        if (str[i] == ' ' || str[i] == '\n' || str[i] == '\t')
		{
            str.erase(i, 1);
            i--;
		}
	}
}

// Read oken from string until separator
string ReadToken(string &file)
{
	string token;
	
	while(file.size() > 0 && file[0] != ' ' && file[0] != '\t')
	{
		if(file[0] == '\n')
		{
			Lines++;
			break;
		}
		
		token += file[0];
		
		file = file.substr(1);
	}
	
	if(file.size() > 0)
	{
		file = file.substr(1);
	}

	return token;
}

// Show help
static void ShowHelp()
{
	// Show help
	printf("Usage:\n\t-v - Verbose mode\n\t-f <file> - File to compile\n\t-o <file> - Output file\n\t-p <platform> - Destination platform (mc)\n\t-b <adress> - Set base adress of program in memory(default is 0)\n");
}

// Main function
int main(int argc, char *argv[])
{
	// Variables
	bool verboseMode = false;
	unsigned int globalOffset = 0;
	const char *outputFile = 0, *inputFile = 0, *tmp, *platform = "mc";
	int i = 1;
	
	// Parse input arguments
	while(i < argc)
	{
		// Cache
		tmp = argv[i++];

		// Verbose Mode
		if(CompareString(tmp, "-v") == 0)
		{
			verboseMode = true;
		}
		// Base adress
		else if(CompareString(tmp, "-b") == 0)
		{
			// Check if there is more arguments
			if(i >= argc)
			{
				// Error
				printf("Not enough arguments. Missing program base adress.");
				return 1;
			}
			
			try
			{
				// Get program base adress
				stringstream stmp;
				stmp << argv[i++];

				// Calculate
				Calculator calc;
				globalOffset = calc.calculate(stmp.str());
			}
			catch(string w)
			{
				printf("Cannot parse progrm base adress. %s\n", w.c_str());
				return 1;
			}
		}
		// Help
		else if(CompareString(tmp, "-h") == 0 || CompareString(tmp, "?") == 0 || CompareString(tmp, "help") == 0)
		{
			// Help
			ShowHelp();
			return 0;
		}
		// Input file
		else if(CompareString(tmp, "-f") == 0)
		{
			// Check if there is more arguments
			if(i >= argc)
			{
				// Error
				printf("Not enough arguments. Missing input file.");
				return 1;
			}
			
			// Set input file
			inputFile = argv[i++];
		}
		// Output file
		else if(CompareString(tmp, "-o") == 0)
		{
			// Check if there is more arguments
			if(i >= argc)
			{
				// Error
				printf("Not enough arguments. Missing output file.");
				return 1;
			}

			// Set output file
			outputFile = argv[i++];
		}
		// Destination platform
		else if(CompareString(tmp, "-p") == 0)
		{
			// Check if there is more arguments
			if(i >= argc)
			{
				// Error
				printf("Not enough arguments. Missing desination platform.");
				return 1;
			}

			// Set destination platform
			platform = argv[i++];
		}
		else
		{
			// Error
			printf("Invalid argument \'%s\'.", tmp);
			return 1;
		}
	}
	if(argc == 1)
	{
		// Help
		ShowHelp();
		return 0;
	}
	
	// Validate paths
	if(inputFile == 0)
	{
		// Error
		printf("No input file.");
		return 1;
	}
	if(outputFile == 0)
	{
		outputFile = "a.txt";
	}
	
	// Print info
	if(verboseMode)
	{
		printf(InfoVersion);
		printf("Input file: \'%s\'\nOutput file: \'%s\'\nDestination platform: \'%s\'\n", inputFile, outputFile, platform);
	}
	
	// Select destination platform
	Platform *p;
	if(CompareString(platform, "mc") == 0)
	{
		p = new MineCore();
	}
	else
	{
		printf("Invalid destination platform: \'%s\'.", platform);
		return 1;
	}

	// Open file
	string file, line, tmpF;
	ifstream pFile (inputFile, ifstream::in);
  	if (!pFile.is_open())
	{
		printf("Cannot open file.");
		return 1;
	}

	// Read whole content
	while(getline(pFile, line))
	{
		tmpF += line + '\n';
	}
	
	// Close file
	pFile.close();
	
	// Parse content
	for(int i = 0; i < tmpF.size(); i++)
	{
		// Find double white chars
		if(i + 1 < tmpF.size() && (tmpF[i] == '\t' || tmpF[i] == ' ' || tmpF[i] == '\v') && tmpF[i] == tmpF[i + 1])
		{
			continue;
		}
		// Convert chars to constant values
		if(i + 2 < tmpF.size() && tmpF[i] == '\'')
		{
			// Value to write
			int val;

			// Check next char
			i++;
			if(i + 2 < tmpF.size() && tmpF[i] == '\\')
			{
				// Switch special char
				i++;
				switch(tmpF[i])
				{
					case '0': val = 0; break;
					case 'a': val = 7; break;
					case 'b': val = 8; break;
					case 't': val = 9; break;
					case 'n': val = 10; break;
					case 'v': val = 11; break;
					case 'f': val = 12; break;
					case 'r': val = 13; break;
					case 'e': val = 27; break;
					case '\"': val = 34; break;
					case '\'': val = 39; break;
					case '\\': val = 92; break;
					case '?': val = 63; break;
					
					default: goto INVALID_CHARS_SEQUENCE;
				}
			}
			else
			{
				// Cast single char
				val = tmpF[i];
			}
			
			// Check closing '
			i++;
			if(i + 1 < tmpF.size() && tmpF[i] == '\'')
			{
				// Add value to output
				ostringstream oss;
				oss << val;
				file += oss.str();
			}
			else
			{
				// Error
				INVALID_CHARS_SEQUENCE:
				printf("Invalid special char sequence: \'%c\'.", tmpF[i]);
				return 1;
			}
		}
		// Comments
		else if(tmpF[i] == ';')
		{
			// Read whole line until end
			while(i + 1 < tmpF.size() && tmpF[i + 1] != '\n')
			{
				i++;
			}
		}
		else
		{
			file += tmpF[i];
		}
	}
	
	
	
	
	
	// TODO Preprocessing
	//cout << '\n' << tmpF << "-------------------------------\n";
	/*
	// Preprocess file
	tmpF = file;
	file.clear();
	for(int i = 0; i < tmpF.size(); i++)
	{
		// Find '#' char
		if(tmpF
	}
	*/
	//cout << file;

	// Compile file
	tmpF = file;
	file.clear();
	string token;
	vector<Command*> Commands;
	int tmpI;
	Command *cmd;
	Label *l = 0;
	while(tmpF.size() > 0)
	{
		// Read token
		cmd = 0;
		token = ReadToken(tmpF);

		// Check if is whitespace
		RemoveWhitespaces(token);
		if(token.empty())
		{
			// Check next token
			continue;
		}

		// Check if ends with ':'
		if(token[token.size() - 1] == ':')
		{
			// Check if label was created
			if(l != NULL)
			{
				// Warning
				printf("Warning! Do not create label after label. First at line %u, second at line %u\n", l->GetLine(), Lines);
			}
			else
			{
				// Create label
				token.erase(token.size() - 1, 1);
				l = new Label(token, Lines);
				Labels.push_back(l);
			}

			// Goto the next token
			continue;
		}
		
		// Check command
		tmpI = CompareStrings(Commands_0, token, Commands_0_Size);
		if(tmpI != -1)
		{
			// No arguments
			cmd = new Command(token, Lines);
		}
		else
		{
			tmpI = CompareStrings(Commands_1, token, Commands_1_Size);
			if(tmpI != -1)
			{
				// 1 argument
				cmd = new Command(token, Lines);

				// Read argument
				string arg1 = ReadToken(tmpF);

				// Add argument
				cmd->AddArgument(arg1);
			}
			else
			{
				tmpI = CompareStrings(Commands_2, token, Commands_2_Size);
				if(tmpI != -1)
				{
					// 2 arguments
					cmd = new Command(token, Lines);
					
					// Read arguments
					string arg1 = ReadToken(tmpF);
					string arg2 = ReadToken(tmpF);

					// Parse arguments
					if(arg1[arg1.size() - 1] != ',')
					{
						// Error
						printf("Not enough arguments. Command \'%s\', at line %u needs 2 arguments.\n", token.c_str(), Lines);
						return 1;
					}
					arg1.erase(arg1.size() - 1, 1);

					// Add arguments
					cmd->AddArgument(arg1);
					cmd->AddArgument(arg2);
				}
				else
				{
					// if
					if(token == "if")
					{
						// 5 arguments
						cmd = new Command(token, Lines);
						
						// Read arguments
						string arg1 = ReadToken(tmpF);
						string arg2 = ReadToken(tmpF);
						string arg3 = ReadToken(tmpF);
						string arg4 = ReadToken(tmpF);
						string arg5 = ReadToken(tmpF);

						// Parse arguments
						if(arg4 != "goto")
						{
							// Error
							printf("Error! Invalid command \'if\', at line %u.\n", Lines);
							return 1;
						}
						
						// Add arguments
						cmd->AddArgument(arg1);
						cmd->AddArgument(arg2);
						cmd->AddArgument(arg3);
						cmd->AddArgument(arg4);
						cmd->AddArgument(arg5);
					}
				}
			}
		}
		
		// Check if command created using this token
		if(cmd != 0)
		{
			// Add to vector
			Commands.push_back(cmd);
			
			// Check if there is label before command
			if(l != NULL)
			{
				l = Labels[Labels.size() - 1];
				l->NextCommand = cmd;
				l = NULL;
			}
		}
		else
		{
			// Error
			printf("Unexpected token: \'%s\', at line %u.\n", token.c_str(), Lines);
			return 1;
		}
	}
	
	// Check if there is label at the end of the file
	if(l != 0)
	{
		// Warning
		printf("Warning! Label at line %u is useless, because it is at the end of the program.\n", l->GetLine());
	}
	
	// Check if there are any commands loaded
	if(Commands.size() == 0)
	{
		// Error
		printf("Error! There is no commands in file.\n");
		return 1;
	}

	// Print some info in verbose mode
	if(verboseMode)
	{
		printf("Parsing...\n");
	}
	
	// Perform parsing for destination platform
	if(p->Parse(Commands))
	{
		// Error
		printf("Parsing failded!\n");
		return 1;
	}
	
	// Print some info in verbose mode
	if(verboseMode)
	{
		printf("Platform specific parsing done. Commands: %u\n", Commands.size());
	}
	
	// Compute adresses
	p->ComputeAdresses(Commands, globalOffset);
	
	// Copy adresses to labels
	for(int i = 0; i < Labels.size(); i++)
	{
		if(Labels[i]->NextCommand == NULL)
		{
			Labels[i]->Adress = 0;
		}
		else
		{
			Labels[i]->Adress = ((Command*)Labels[i]->NextCommand)->Adress;
		}
	}
	
	// Update all arguments
	for(int i = 0; i < Commands.size(); i++)
	{
		Commands[i]->UpdateArguments();
	}

	// Print some info in verbose mode
	if(verboseMode)
	{
		printf("Commands addresses calculated.\n");
	}

	// Open file
	ofstream oFile (outputFile);
  	if (!oFile.is_open())
	{
		printf("Cannot open file.");
		return 1;
	}

	// Print some info in verbose mode
	if(verboseMode)
	{
		printf("Writing program to thie file...\n");
	}
	
	// Write program to output file
	if(p->WriteToFile(Commands, Labels, oFile))
	{
		// Error
		printf("Writing failded!\n");
		return 1;
	}

	// Write assembler info
	oFile << "\n===========================================\n               " << InfoVersion;

	// Close file
	oFile.close();
	
	// Print some info in verbose mode
	if(verboseMode)
	{
		printf("Done!\n");
	}
	
	// Clean
	delete p;

	// Good
	return 0;
}

