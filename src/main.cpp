#include <iostream>
#include <fstream>
#include <vector>

std::vector<std::string> files, libs, code, flags;
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
			for(int j = i + 1; ; j++, i++) {
				if(code[j] == "}") break;
				if(code[j] != "{") files.push_back(code[j]);
			}
		}
		if (code[i].find("libs") != std::string::npos && (code[i + 1] == "{" || code[i].find("{") != std::string::npos)) {
			for(int j = i + 1; ; j++) {
				if(code[j] == "}") break;
				if(code[j] != "{") libs.push_back(code[j]);
			}
		}
		if (code[i].find("flags") != std::string::npos && (code[i + 1] == "{" || code[i].find("{") != std::string::npos)) {
			for(int j = i + 1; ; j++) {
				if(code[j] == "}") break;
				if(code[j] != "{") flags.push_back(code[j]);
			}
		}
		if (code[i].find("name") != std::string::npos && (code[i + 1] == "{" || code[i].find("{") != std::string::npos)) {
			for(int j = i + 1; ; j++) {
				if(code[j] == "}") break;
				if(code[j] != "{") name = code[j];
			}
		}
	}
}

void instructions() {
	for(int i = 0; i < code.size(); i++) {
#ifdef _WIN32
		if (code[i].find("windows") != std::string::npos && (code[i + 1] == "{" || code[i].find("{") != std::string::npos)) {
			for(int j = i + 1; ; j++) {
				if(code[j] == "}") { i = j; break; }
				if(code[j] != "{") system(code[j].c_str());
			}
		}
#else
		if (code[i].find("linux") != std::string::npos && (code[i + 1] == "{" || code[i].find("{") != std::string::npos)) {
			for(int j = i + 1; ; j++) {
				if(code[j] == "}") { i = j; break; }
				if(code[j] != "{") system(code[j].c_str());
			}
		}
#endif
	}
}

void compile() {
	std::string command = "g++ -c ";
	for(int i = 0; i < flags.size(); i++) command += "-" + flags[i] + " ";
	for(int i = 0; i < files.size(); i++) command += "\"" + files[i] + "\" ";
	for(int i = 0; i < libs.size(); i++) command += " -l" + libs[i];
	system(command.c_str());
}

void build() {
	std::string command = "g++ ";
	command += "-o \"" + name + "\" ";
	for(int i = 0; i < flags.size(); i++) command += "-" + flags[i] + " ";
	for(int i = 0; i < files.size(); i++) {
		if(files[i].find(".h") != std::string::npos)
			if(files[i].find("/") != std::string::npos)
				command += "\"" + files[i].substr(files[i].find("/") + 1, files[i].size()) + ".gch" + "\" ";
			else
				command += "\"" + files[i] + ".gch" + "\" ";
		else if(files[i].find("/") != std::string::npos)
			command += "\"" + (files[i].substr(files[i].find("/") + 1, files[i].find(".") - 3) + "o") + "\" ";
		else 
			command += "\"" + (files[i].substr(0, files[i].find(".") + 1) + "o") + "\" ";
	}
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
	if(argv[1] == nullptr) { std::cout << "\033[31mError: Empty arguments!\033[0m" << std::endl; exit(-1); }
	std::cout << "\033[1m\033[35mDetecting GCC compiler...\033[0m" << std::endl;
	
	if(!detect()) {
		std::cout << "\033[31mCan't detect GCC compiler!\033[0m \033[32mInstalling...\033[0m" << std::endl;
#ifdef _WIN64
		std::cout << "To install gcc download this file, follow the instructions, then reboot. Link - ftp://ftp.equation.com/gcc/gcc-10.2.0-64.exe" << std::endl;
#elifdef _WIN32 
		std::cout << "To install gcc download this file, follow the instructions, then reboot. Link - ftp://ftp.equation.com/gcc/gcc-10.2.0-32.exe" << std::endl;
#else
		system("sudo apt install g++");
#endif
	}
	else std::cout << "\033[32mGCC compiler detected!\033[0m" << std::endl;
	
	if(argv[2] != nullptr) {
		std::cout << "\033[1m\033[35mReading instructions file \"" << argv[2] << "\"...\033[0m" << std::endl;
		if(!readfile(argv[2])) { std::cout << "\033[31mError: Can't open file!\033[0m" << std::endl; exit(-1); }
		std::cout << "\033[1m\033[35mExecuting commands...\033[0m" << std::endl;
		instructions();
	}
	std::cout << "\033[1m\033[35mReading build file \"" << argv[1] << "\"...\033[0m" << std::endl;
	
	if(!readfile(argv[1])) { std::cout << "\033[31mError: Can't open file!\033[0m" << std::endl; exit(-1); }
	else std::cout << "\033[32mSuccess!\033[0m" << std::endl;
	
	std::cout << "\033[1m\033[35mPreparation...\033[0m" << std::endl;
	prepare();
	
	std::cout << "\033[1m\033[35mCompiling...\033[0m" << std::endl;
	compile();
	build();

	std::cout << "\033[32mCompilation finished!\033[0m" << std::endl;
}
