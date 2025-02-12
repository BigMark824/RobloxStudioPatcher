#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>

const char* instruction = "\x75"; // jne

std::vector<unsigned char> HexToInt(std::string &pattern) {
    std::vector<unsigned char> result;
    std::stringstream buffer(pattern);
    std::string word;

    while (buffer >> word) {
        int r = std::stoi(word, nullptr, 16);
        result.push_back(r); 
    }
    
    return result;
}

std::uintptr_t FindOffset(std::string pattern, std::vector<unsigned char>& bytes) {
    std::vector<unsigned char> new_pattern = HexToInt(pattern);

    for (std::uintptr_t address = 0; address + new_pattern.size() <= bytes.size(); address++) { 
        bool match = true;
        for (int i = 0; i < new_pattern.size(); i++) {
            if (bytes[address + i] != new_pattern[i]) {
                match = false;
                break;
            }
        }
        if (match) {
            return address;
        }
    }

    return 0x0; //pattern not found
}

void Patch(std::fstream &file, std::uintptr_t offset)
{
	// overrides the "je" with a "jne" instruction
	file.seekg(offset, std::ios::beg);
	file.write(instruction, strlen(instruction));
}

int main(int argc, char* argv[])
{
	if (argc != 2)
		std::cerr << "Please provide RobloxStudioBeta.exe";
		
	std::fstream file( argv[1], std::ios::in | std::ios::out | std::ios::binary | std::ios::ate);

	if ( !file.good() )
	{
		std::cerr << "File unreadable";
		std::cin.get();
	}

	std::streamsize size = file.tellg();
    	file.seekg(0, std::ios::beg);

    	std::vector<unsigned char> buffer(size);
	std::uintptr_t offset = 0;

	std::string Pattern = "74 05 E8 12 85 4A FE 48 8D";
	
	if (file.read(reinterpret_cast<char*>(buffer.data()), size)) { 
        	offset = FindOffset(pattern, buffer);
    	}
	
	Patch(file, offset);

	file.close();

	std::cout << "Patched RobloxStudioBeta";
	return 0;
}
