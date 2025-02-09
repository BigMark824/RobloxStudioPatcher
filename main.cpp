#include <iostream>
#include <fstream>
#include <vector>

const char* instruction = "\x75"; // jne

void Patch(std::fstream &file, std::uintptr_t pos)
{
	// overrides the "je" with a "jne" instruction
	file.seekg(pos, std::ios::beg);
	file.write(instruction, strlen(instruction));
}

int main(int argc, char* argv[])
{
	if (argc != 2)
		std::cerr << "Please provide RobloxStudioBeta.exe";
		
	std::fstream file( argv[1], std::ios::in | std::ios::out | std::ios::binary );

	if ( !file.good() )
	{
		std::cerr << "File unreadable";
		std::cin.get();
	}

	Patch(file, 0x4FB8AE7);

	file.close();

	std::cout << "Patched RobloxStudioBeta";
	return 0;
}