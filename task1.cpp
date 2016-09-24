#include <iostream>
#include <set>
#include <cstdlib>
#include <fstream>
#include <string>
#include <cstdio>
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
    friend std::ostream& operator << (std::ostream& _out, const State& st)
    {
        _out << st.c_f << ", " << st.c_g << ", "; 
        if (st.c_f == 0) {
            _out << "-, -, ";
        } else {
            if(st.und_f_x){
                _out << "#, " ;
            } else {
                _out << st.f_x << ", "; 
            }
            if(st.und_f_y){
                _out << "#, " ;
            } else {
                _out << st.f_y << ", "; 
            }
        }
        if (st.c_g == 0) {
            _out << "-, -, ";
        } else {
            if(st.und_g_x){
                _out << "#, " ;
            } else {
                _out << st.g_x << ", "; 
            }
            if(st.und_g_y){
                _out << "#, " ;
            } else {
                _out << st.g_y << ", "; 
            }
        }
        if(st.und_h){
            _out << "#, " ;
        } else {
            _out << st.h;
        }
        return _out;
    }
    
    bool operator == (const State o) const {
        return c_f == o.c_f && c_g == o.c_g && 
               ((und_f_x && o.und_f_x) || f_x == o.f_x) &&
               ((und_f_y && o.und_f_y) || f_y == o.f_y) &&
               ((und_g_x && o.und_g_x) || g_x == o.g_x) &&
               ((und_g_y && o.und_g_y) || g_y == o.g_y) &&
               ((und_h && o.und_h) || h == o.h);
    }

    bool operator < (const State o) const {
        if (operator==(o)) return false; 
        if (c_f != o.c_f) return c_f < o.c_f;
        if (c_g != o.c_g) return c_g < o.c_g;
        if (!((und_f_x && o.und_f_x) || f_x == o.f_x)){
            if (und_f_x) return true;
            else if(o.und_f_x) return false;
            else return f_x < o.f_x;
        }
        if (!((und_f_x && o.und_f_y) || f_y == o.f_y)){
            if (und_f_y) return true;
            else if(o.und_f_y) return false;
            else return f_y < o.f_y;
        }
        if (!((und_g_x && o.und_g_x) || f_x == o.g_x)){
            if (und_g_x) return true;
            else if(o.und_g_x) return false;
            else return g_x < o.g_x;
        }
        if (!((und_g_y && o.und_g_y) || g_y == o.g_y)){
            if (und_g_y) return true;
            else if(o.und_g_y) return false;
            else return g_y < o.g_y;
        }
        if (!((und_h && o.und_h) || h == o.h)){
            if (und_h) return true;
            else if(o.und_h) return false;
            else return h < o.h;
        }
        return false; 
    }

};

std::set<State> states;

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
        st.und_f_x = false;
        break;
    case 2 :
        st.c_f = 3;
        st.f_y = 4;
        st.und_f_y = false;
        break;
    case 3 :
        st.c_f = 4;
        st.h = 1;
        st.und_h =  false;
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
        st.und_f_y = false;
        break;
    case 8 : 
        st.c_f = 12;
        st.f_y = 4;
        st.und_f_y = false;
        break;
    case 9 :
        st.c_f = st.f_y > 7 ? 10 : 11;
        break;
    case 10 :
        st.c_f = 11;
        st.f_x = 5;
        st.und_f_x = false;
        break;
    case 11 :
        st.c_f = 12;
        st.f_y = 1;
        st.und_f_y = false;
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
            st.und_g_x = true;
            st.und_g_y = true;
        break;
        case 1:
            st.c_g = 2;
            st.g_x = 2;
            st.und_g_x = false;
        break;
        case 2:
            st.c_g = 3;
            st.g_y = 2;
            st.und_g_y = false;
        break;
        case 3:
            st.c_g = 4;
            st.h = 2;
            st.und_h = false;
        break;
        case 4:
            st.c_g = st.h < param.g_a ? 5 : 7;
        break;
        case 5:
            st.c_g = 6;
            st.h = param.g_a;
            st.und_h = false;
        break;
        case 6:
            st.c_g = 7;
            st.g_y = 5;
            st.und_g_y = false;
        break;
        case 7:
            st.c_g = 8;
            st.h = param.g_a - st.g_x;
            st.und_h = false;
        break;
        case 8:
            st.c_g = 9;
            st.h = st.g_x - param.g_a;
            st.und_h = false;
        break;
        case 9:
            st.c_g = st.g_x < 5 ? 10 : 16;
        break;
        case 10:
            st.c_g = st.h > 0 ? 11 : 12;
        break;
        case 11:
            st.c_g = 16;
        break;
        case 12:
            st.c_g = 13;
            st.g_y = 6;
            st.und_g_y = false;
        break;
        case 13:
            st.c_g = st.h > st.g_y - st.g_x;
        break;
        case 14:
            st.c_g = 15;
            st.h = st.g_x - st.g_y;
            st.und_h = false;
        break;
        case 15:
            st.c_g = 9;
            st.h = st.g_y;
            st.und_h = false;
        break;
        case 16: break;
        default : throw "undefined instruction in g function";
    }
    return st;
}

void execution(State st)
{
    if (states.find(st) != states.end()){
        return;
    }
    states.insert(st);
    if (st.c_f != 12 ){
        execution(exec_f(st));
    }
    if (st.c_g != 16) {
        execution(exec_g(st));
    }
}
int main(int argc, char ** argv)
{
    if (!parse(argc, argv)){
        about();
    }
    State st;
    st.c_f = 0;
    st.c_g = 0;
    execution(st);
    OUT << states.size();
    return 0;
}
