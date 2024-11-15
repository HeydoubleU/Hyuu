#include "Hyuu.h"

#include <fstream>
#include <sstream>


void Hyuu::File::read_text_file(const Amino::String& filename, Amino::String& text) {
	std::ifstream file(filename.c_str());

	if (!file.is_open()) {
		text = "";
		return;
	}

	std::stringstream buffer;
	buffer << file.rdbuf();
	text = buffer.str();

	file.close();
}