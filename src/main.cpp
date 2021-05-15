#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <cstring>

std::vector<std::string> files, libs, code, flags;
std::map<std::string, std::string> vars;
std::string name;

bool detect() {
#ifdef __GNUC__
	return true;
#else
	return false;
#endif	
}
void readvars()
{
	for(int i = 0; i < code.size(); i++) {
		if (code[i].find("vars") != std::string::npos && (code[i + 1] == "{" || code[i].find("{") != std::string::npos)) {
			for(int j = i + 1; ; j++, i++) {
				if(code[j] == "}") break;
				if(code[j] != "{")
					if(code[j].find(" ") != std::string::npos) vars.emplace(std::make_pair(code[j].substr(0, code[j].find(" ")), code[j].substr(code[j].find(" ") + 1, code[j].size() - 1)));
					else vars.emplace(std::make_pair(code[j], ""));
			}
		}
	}
}
void prepare() {
	for(int i = 0; i < code.size(); i++) {
		for(int i = 0; i < code.size(); i++) {
			for(auto& j : vars)
			{
				if (code[i].find(j.first) != std::string::npos)
				{
						code[i].replace(code[i].find(j.first), j.first.size(), j.second);
				}
			}
		}
		if (code[i].find("files") != std::string::npos && (code[i + 1] == "{" || code[i].find("{") != std::string::npos)) {
			for(int j = i + 1; ; j++, i++) {
				if(code[j] == "}") break;
				if(code[j] != "{") files.push_back(code[j]);
			}
		}
		if (code[i].find("libs") != std::string::npos && (code[i + 1] == "{" || code[i].find("{") != std::string::npos)) {
			for(int j = i + 1; ; j++, i++) {
				if(code[j] == "}") break;
				if(code[j] != "{") libs.push_back(code[j]);
			}
		}
		if (code[i].find("flags") != std::string::npos && (code[i + 1] == "{" || code[i].find("{") != std::string::npos)) {
			for(int j = i + 1; ; j++, i++) {
				if(code[j] == "}") break;
				if(code[j] != "{") flags.push_back(code[j]);
			}
		}
		if (code[i].find("name") != std::string::npos && (code[i + 1] == "{" || code[i].find("{") != std::string::npos)) {
			for(int j = i + 1; ; j++, i++) {
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
	for(int i = 0; i < files.size(); i++)
	{
		if(files[i].find("/") != std::string::npos)
		{
			files[i] = files[i].substr(files[i].find_last_of("/") + 1, files[i].size());
		}
	}
	std::string command = "g++ ";
	command += "-o \"" + name + "\" ";
	for(int i = 0; i < flags.size(); i++) command += "-" + flags[i] + " ";
	for(int i = 0; i < files.size(); i++) {
		if(files[i].find(".h") != std::string::npos)
			command += "\"" + files[i] + ".gch" + "\" ";
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
	if(strcmp(argv[1], "--help") == 0 || strcmp(argv[1], "-h") == 0)
	{
		std::cout << "Usage: ibs [option] build_file_path *instructions file path* -*vars_values*" << std::endl;
		std::cout << "Options:" << std::endl;
        std::cout << "-h  --help - Display this information" << std::endl;
        std::cout << "-v  --version - Display version information" << std::endl;
        exit(0);
	}
	if(strcmp(argv[1], "--version") == 0 || strcmp(argv[1], "-v") == 0)
	{
		std::cout << "ibs 1.1.0\n\nThis is free software. There is NO warranty;\nnot even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE." << std::endl;
		exit(0);
	}
	if(argv[1] == nullptr) { std::cout << "\033[31mError: File path not specified!\033[0m" << std::endl; exit(-1); }
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
		if(argv[2][0] != '-')
		{
			std::cout << "\033[1m\033[35mReading instructions file \"" << argv[2] << "\"...\033[0m" << std::endl;
			if(!readfile(argv[2])) { std::cout << "\033[31mError: Can't open file!\033[0m" << std::endl; exit(-1); }
			std::cout << "\033[1m\033[35mExecuting commands...\033[0m" << std::endl;
			instructions();
		}
	}
	std::cout << "\033[1m\033[35mReading build file \"" << argv[1] << "\"...\033[0m" << std::endl;
	
	if(!readfile(argv[1])) { std::cout << "\033[31mError: Can't open file!\033[0m" << std::endl; exit(-1); }
	else std::cout << "\033[32mSuccess!\033[0m" << std::endl;
	
	std::cout << "\033[1m\033[35mPreparation...\033[0m" << std::endl;
	readvars();
	for(auto& i : vars)
	{
		if(argv[2] != nullptr)
		{
			int notinit = (argv[2][0] == '-') ? 0 : 1;
			if(i.second == "")
			{
				i.second = ((argv[2 + notinit] == nullptr) ? "" : argv[2 + notinit]);
				if(i.second.find("-") != std::string::npos) i.second.erase(i.second.begin());
			}
		}
	}
	prepare();
	
	std::cout << "\033[1m\033[35mCompiling...\033[0m" << std::endl;
	compile();
	build();

	std::cout << "\033[32mCompilation finished!\033[0m" << std::endl;
}
