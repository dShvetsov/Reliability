#include <iostream>
#include <cstdlib>
#include <fstream>
#include <string>
#include <vector>
#include <iterator>

const std::ostream *out = &std::cout;
#define OUT (*out)

struct __param__ {
    int f_a;
    int f_b;
    int g_a;
    int g_b;
} param;

bool isCount = false;
/** 
 * print information about program
 */
void about()
{
    std::cerr << "This Programm counts possibly state of another programm\n";
    std::cerr << "Author : Shvetsov Denis\n";
    std::cerr << "Year   : 2016\n";
    std::cerr << "Usage : program_name <f_a> <f_b> <g_a> <g_b> [-file filename] [-count]\n"; 
}

/**
 *  parsing argumetns
 *  @return args ok or not
 */
bool parse(int argc, char **_argv_)
{
    typedef std::vector<std::string> Argv;
    Argv argv;
    /* lets make list of argv */
    for (int i = 1; i < argc; i++){
        argv.push_back(std::string(_argv_[i]));
    }
    // parsing "-count" and "-file"
    for (std::vector<std::string>::iterator i = argv.begin() ; i != argv.end(); i++){
       // if file
       if (*i  == "-file"){
            // if after file doensnt exist element then error
            if ((i + 1) != argv.end()){
                out = new std::fstream( (i+1)->c_str(), std::fstream::in);
            } else {
                return false;
            }
            // delete handled elements
            argv.erase(i, i + 2);
            i --;
       } else if (*i == "-count"){ // if count
            isCount = true;
            // delete handled element
            argv.erase(i);
            i--;
       }
    }
    if (argv.size() != 4){
        return false;
    }
    try{
    //std::stoi is more pretty
         param.f_a = std::atoi(argv[0].c_str());
         param.f_b = std::atoi(argv[1].c_str());
         param.g_a = std::atoi(argv[2].c_str());
         param.g_a = std::atoi(argv[3].c_str());
    }
    catch(...){
    // if std::atoi change on std::stoi this will useful :)
        return false;
    }
    return true;
    
}
int main(int argc, char ** argv)
{
    if (!parse(argc, argv)){
        about();
    }
    return 0;
}
