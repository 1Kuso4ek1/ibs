#include <iostream>
#include <fstream>
#include <vector>

std::vector<std::string> files, libs, code;
std::string name;

bool detect() {
#ifdef __GNUC__
	return true;
#else
	return false;
#endif	
}

void prepare() {
	for(int i = 0; i < code.size(); i++) {
		if (code[i].find("files") != std::string::npos && (code[i + 1] == "{" || code[i].find("{") != std::string::npos)) {
			for(int j = i + 1; ; j++) {
				if(code[j] == "}") { i = j; break; }
				if(code[j] != "{") files.push_back(code[j]);
			}
		}
		if (code[i].find("libs") != std::string::npos && (code[i + 1] == "{" || code[i].find("{") != std::string::npos)) {
			for(int j = i + 1; ; j++) {
				if(code[j] == "}") { i = j; break; }
				if(code[j] != "{") libs.push_back(code[j]);
			}
		}
		if (code[i].find("name") != std::string::npos && (code[i + 1] == "{" || code[i].find("{") != std::string::npos)) {
			for(int j = i + 1; ; j++) {
				if(code[j] == "}") { i = j; break; }
				if(code[j] != "{") name = code[j];
			}
		}
	}
}

void instructions() {
	for(int i = 0; i < code.size(); i++) {
		system(code[i].c_str());
	}
}

void compile() {
	std::string command = "g++ ";
	for(int i = 0; i < files.size(); i++) command += "\"" + files[i] + "\" ";
	command += " -o \"" + name + "\"";
	for(int i = 0; i < libs.size(); i++) command += " -l" + libs[i];
	system(command.c_str());
}

bool readfile(std::string filename) {
	std::ifstream file(filename);
	if(file.is_open()) {
		code.clear();
		while(!file.eof()) {
			std::string temp;
			std::getline(file, temp);
			code.push_back(temp);
		}
		file.close();
	}
	else return false;
	return true;
}

int main(int argc, char* argv[]) {
	if(argv[1] == nullptr) { std::cout << "Error: Empty arguments!" << std::endl; exit(-1); }
	std::cout << "Detecting GCC compiler..." << std::endl;
	
	if(!detect()) {
		std::cout << "Can't detect GCC compiler! Installing..." << std::endl;
		system("sudo apt install g++");
	}
	else std::cout << "GCC compiler detected!" << std::endl;
	
	if(argv[2] != nullptr) {
		std::cout << "Reading instructions file \"" << argv[2] << "\"..." << std::endl;
		if(!readfile(argv[2])) { std::cout << "Error: Can't open file!" << std::endl; exit(-1); }
		std::cout << "Executing commands..." << std::endl;
		instructions();
	}
	std::cout << "Reading build file \"" << argv[1] << "\"..." << std::endl;
	
	if(!readfile(argv[1])) { std::cout << "Error: Can't open file!" << std::endl; exit(-1); }
	else std::cout << "Success!" << std::endl;
	
	std::cout << "Preparation..." << std::endl;
	prepare();
	
	std::cout << "Compiling..." << std::endl;
	compile();
}
