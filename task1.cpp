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
         param.f_a = std::stoi(argv[0].c_str());
         param.f_b = std::stoi(argv[1].c_str());
         param.g_a = std::stoi(argv[2].c_str());
         param.g_a = std::stoi(argv[3].c_str());
    }
    catch(...){
        return false;
    }
    return true;
    
}

class State{
public:
    int c_f;
    int c_g;
    int f_x;
    bool und_f_x;
    int f_y;
    bool und_f_y;
    int g_x;
    bool und_g_x;
    int g_y;
    bool und_g_y;
    int h;
    bool und_h;
}

State exec_f(State st)
{
    switch (st.c_f){
    case 0 : 
        st.c_f = 1;
        st.und_f_x = true;
        st.und_f_y = true;
        break;
    case 1 :
        st.c_f = 2;
        st.f_x = 1;
        break;
    case 2 :
        st.c_f = 3;
        st.f_y = 4;
        break;
    case 3 :
        st.c_f = 4;
        st.h = 1;
        break;
    case 4 : 
        st.c_f = st.f_y > 5 ? 5 : 12;
        break;
    case 5 :
        st.c_f = st.f_y > 1 ? 6 : 9;
        break;
    case 6 :
        st.c_f = st.h > param.f_a ? 7 : 8;
        break;
    case 7 :
        st.c_f = 12;
        st.f_y = 1;
        break;
    case 8 : 
        st.c_f = 12;
        st.f_y = 4;
        break;
    case 9 :
        st.c_f = st.c_y > 7 ? 10 : 11;
        break;
    case 10 :
        st.c_f = 11;
        st.f_x = 5;
        break;
    case 11 :
        st.c_f = 12;
        st.f_y = 1;
        break;
    case 12 : break;
    default : throw "undefined instruction in f function";
    }
    return st;
}

State exec_g(State st)
{
    switch(st.c_g) {
        case 0:
            st.c_g = 1;
            st.und_c_x = true;
            st.und_c_y = true;
        break;
        case 1:
            st.c_g = 2;
            st.g_x = 2;
        break;
        case 2:
            st.c_g = 3;
            st.g_y = 2;
        break;
        case 3:
            st.c_g = 4;
            st.h = 2;
        break;
        case 4:
            st.c_g = st.h < param.g_a ? 5 : 7;
        break;
        case 5:
            st.c_g = 6;
            st.h = param.g_a;
        break;
        case 6:
            st.c_g = 7;
            st.g_y = 5;
        break;
        case 7:
            st.c_g = 8;
            st.h = param.g_a - st.g_x;
        break;
        case 8:
            st.c_g = 9;
            st.h = st.g_x - param.g_a;
        break;
        case 9:
            st.c_g = st.g_x < 5 ? 10 : 16;
        break;
        case 10:
            st.c_g = st.h > 0 > 11 : 12;
        break;
        case 11:
            st.c_g = 16;
        break;
        case 12:
            st.c_g = 13;
            st.g_y = 6;
        break;
        case 13:
            st.c_g = st.h > st.g_y - st.g_x;
        break;
        case 14:
            st.c_g = 15;
            st.h = st.g_x - st.g_y;
        break;
        case 15:
            st.h = st.g_y;
        break;
        case 16: break;
        default : throw "undefined instruction in g function";
    }
    return st;
}
int main(int argc, char ** argv)
{
    if (!parse(argc, argv)){
        about();
    }
    return 0;
}
