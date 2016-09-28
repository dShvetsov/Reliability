/***************************** 
 * author : DENIS SHVETSOV
 * year : 2016
 ****************************/

#include <iostream>
#include <set>
#include <cstdlib>
#include <fstream>
#include <string>
#include <cstdio>
#include <vector>
#include <iterator>
#include <sstream>

std::fstream out;
std::fstream ltsout;

// params of program
struct __param__ {
    int f_a;
    int f_b;
    int g_a;
    int g_b;
} param;

bool isCount = false;
bool ltsOn = false;

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
    std::string filename = "states.txt";
    // parsing "-count" and "-file"
    for (std::vector<std::string>::iterator i = argv.begin() ; i != argv.end(); i++){
       // if file
       if (*i  == "-file"){
            // if after file doensnt exist element then error
            if ((i + 1) != argv.end()){
                filename = *(i + 1);
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
       } else if (*i == "-lts"){ // if lts
             ltsOn = true;
             // if after file doensnt exist element then error
            if ((i + 1) != argv.end()){
                ltsout.open(std::string("./") + *(i + 1),
                            std::fstream::out | std::fstream::trunc);
            } else {
                return false;
            }
            // delete handled elements
            argv.erase(i, i + 2);
            i --;
       }
    }
    out.open(std::string("./") + filename, std::fstream::out | std::fstream::trunc);
    if (argv.size() != 4){
        return false;
    }
    try{
         // convertion string to integer
         param.f_a = std::stoi(argv[0].c_str());
         param.f_b = std::stoi(argv[1].c_str());
         param.g_a = std::stoi(argv[2].c_str());
         param.g_a = std::stoi(argv[3].c_str());
    }
    catch(...){
        // If parametrs convertion failed
        return false;
    }
    return true;
    
}

// State of programm
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
    unsigned long id;
    unsigned long from; // if 0 then initial
    std::string link;
    std::string colour; // ref if f, blue if g

    State() : c_f(0), c_g(0), und_f_x(true), und_f_y(true),
              und_g_x(true), und_g_y(true), und_h(true), id(0), from(0)
    { }

    std::string dotName()
    {
        if (id == 0) throw "unexpected state";
        std::stringstream ret;
        ret << id << "[ label = \"";
        ret << *this;
        ret << "\"]\n";
        return ret.str();
    }

    std::string dotLink()
    {
        if (id == 0) throw "unexpected state";
        if (from == 0) return std::string();
        std::stringstream ret;
        ret << from << " -> " << id << "[label=\" " << link << " \" color=\"" <<
            colour << "\"]\n";
        return ret.str();
    }

    /**
     * print information about state
     */
    friend std::ostream& operator << (std::ostream& _out, const State& st)
    {
        _out << st.c_f << ", " << st.c_g << ", "; 
        if(st.und_h){
            _out << "#, " ;
        } else {
            _out << st.h << ", ";
        }
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
        return _out;
   }
    
    /**
     * equal operator
     */
    bool operator == (const State o) const {
        return c_f == o.c_f && c_g == o.c_g && 
               ((und_f_x && o.und_f_x) || (f_x == o.f_x && !und_f_x && !o.und_f_x)) &&
               ((und_f_y && o.und_f_y) || (f_y == o.f_y && !und_f_y && !o.und_f_y)) &&
               ((und_g_x && o.und_g_x) || (g_x == o.g_x && !und_g_x && !o.und_g_x)) &&
               ((und_g_y && o.und_g_y) || (g_y == o.g_y && !und_g_y && !o.und_g_y)) &&
               ((und_h && o.und_h) || (h == o.h && !und_h && !o.und_h));
    }

    /** 
     * This operator is needed for std::set
     * compare of state do by comparing of members of object
     */
    bool operator < (const State o) const {
       // if (operator==(o)) return false; 
        if (c_f != o.c_f) return c_f < o.c_f;
        if (c_g != o.c_g) return c_g < o.c_g;

        if (!((und_h && o.und_h) || (h == o.h && !und_h && !o.und_h))){
            if (und_h) return true;
            else if(o.und_h) return false;
            else return h < o.h;
        }
 
        if (!((und_f_x && o.und_f_x) || (f_x == o.f_x && !und_f_x && !o.und_f_x))){
            if (und_f_x) return true;
            else if(o.und_f_x) return false;
            else return f_x < o.f_x;
        }
        if (!((und_f_y && o.und_f_y) || (f_y == o.f_y && !und_f_y && !o.und_f_y))){
            if (und_f_y) return true;
            else if(o.und_f_y) return false;
            else return f_y < o.f_y;
        }
        if (!((und_g_x && o.und_g_x) || (g_x == o.g_x && !und_g_x && !o.und_g_x))){
            if (und_g_x) return true;
            else if(o.und_g_x) return false;
            else return g_x < o.g_x;
        }
        if (!((und_g_y && o.und_g_y) || (g_y == o.g_y && !und_g_y && !o.und_g_y))){
            if (und_g_y) return true;
            else if(o.und_g_y) return false;
            else return g_y < o.g_y;
        }
       return false; 
    }

};

// states of program
std::set<State> states;

// models of f function
// exec_f take State of program and return state after execution of single instruction
State exec_f(State st)
{
    st.from = st.id;
    st.colour = "red";
    switch (st.c_f){
    case 0 :  // int x, y
        st.c_f = 1;
        st.und_f_x = true;
        st.und_f_y = true;
        st.link = "int x, y";
        break;
    case 1 : // x = 1
        st.c_f = 2;
        st.f_x = 1;
        st.und_f_x = false;
        st.link = "x = 1";
        break;
    case 2 : // y = 4
        st.c_f = 3;
        st.f_y = 4;
        st.und_f_y = false;
        st.link = "y = 4";
        break;
    case 3 : // h = 1
        st.c_f = 4;
        st.h = 1;
        st.und_h =  false;
        st.link = "h = 1";
        break;
    case 4 : // if ( y > 5 )
        st.c_f = st.f_y > 5 ? 5 : 12;
        st.link = st.f_y > 5 ? "y > 5" : "!(y > 5)";
        break;
    case 5 : // if (y > 1)
        st.c_f = st.f_y > 1 ? 6 : 9;
        st.link = st.f_y > 1 ? "y > 1" : "!(y > 1)";
        break;
    case 6 : // if (h > a)
        st.c_f = st.h > param.f_a ? 7 : 8;
        st.link = st.h > param.f_a ? "h > a" : "!(h > a)";
        break;
    case 7 : // y = 1
        st.c_f = 12;
        st.f_y = 1;
        st.und_f_y = false;
        st.link = "y = 1";
        break;
    case 8 : // y = 4
        st.c_f = 12;
        st.f_y = 4;
        st.und_f_y = false;
        st.link = "y = 4";
        break;
    case 9 : // if (y > 7)
        st.c_f = st.f_y > 7 ? 10 : 11;
        st.link = st.f_y > 7 ? "y > 7" : "!(y < 7)";
        break;
    case 10 : // x = 5
        st.c_f = 11;
        st.f_x = 5;
        st.und_f_x = false;
        st.link = "x = 5";
        break;
    case 11 : // y = 1
        st.c_f = 12;
        st.f_y = 1;
        st.und_f_y = false;
        st.link = "y = 1";
        break;
    case 12 : break; //return
    default : throw "undefined instruction in f function";
    }
    return st;
}

// models of g function
// exec_g take State of program and return state after execution of single instruction
State exec_g(State st)
{
    st.from = st.id;
    st.colour = "blue";
    switch(st.c_g) {
        case 0: // int x, y
            st.c_g = 1;
            st.und_g_x = true;
            st.und_g_y = true;
            st.link = "int x, y";
        break;
        case 1: // x = 2
            st.c_g = 2;
            st.g_x = 2;
            st.und_g_x = false;
            st.link = "x = 2";
        break;
        case 2: // y = 2
            st.c_g = 3;
            st.g_y = 2;
            st.und_g_y = false;
            st.link = "y = 2";
        break;
        case 3: // h = 2
            st.c_g = 4;
            st.h = 2;
            st.und_h = false;
            st.link = "h = 2";
        break;
        case 4: // if (h < a)
            st.c_g = st.h < param.g_a ? 5 : 7;
            st.link = st.h < param.g_a ? "h < a" : "!(h < a)";
        break;
        case 5: // h = a
            st.c_g = 6;
            st.h = param.g_a;
            st.und_h = false;
            st.link = "h = a";
        break;
        case 6: // y = 5
            st.c_g = 7;
            st.g_y = 5;
            st.und_g_y = false;
            st.link = "y = 5";
        break;
        case 7: // h = a - x
            st.c_g = 8;
            st.h = param.g_a - st.g_x;
            st.und_h = false;
            st.link = "h = a - x";
        break;
        case 8: // h = x - a
            st.c_g = 9;
            st.h = st.g_x - param.g_a;
            st.und_h = false;
            st.link = "h = x - a";
        break;
        case 9: // while ( x < 5 )
            st.c_g = st.g_x < 5 ? 10 : 16;
            st.link = st.g_x < 5 ? "x < 5" : "!(x < 5)";
        break;
        case 10: // if (h > 0)
            st.c_g = st.h > 0 ? 11 : 12;
            st.link = st.h > 0 ? "h > 0" : "!(h > 0)";
        break;
        case 11: // break
            st.c_g = 16;
            st.link = "break";
        break;
        case 12: // y = 6
            st.c_g = 13;
            st.g_y = 6;
            st.und_g_y = false;
            st.link = "y = 6";
        break;
        case 13: // if (h > y - x)
            st.c_g = st.h > st.g_y - st.g_x ? 14 : 15;
            st.link = st.h > st.g_y ? "h > y - x" : "!(h > y - x)";
        break;
        case 14: // h = x - y
            st.c_g = 15;
            st.h = st.g_x - st.g_y;
            st.und_h = false;
            st.link = "h = x - y";
        break;
        case 15: // h = y
            st.c_g = 9;
            st.h = st.g_y;
            st.und_h = false;
            st.link = "h = y";
        break;
        case 16: break; // return
        default : throw "undefined instruction in g function";
    }
    return st;
}

/* main function of alghorithm
 * this function recursive call itself with exec_f and then exec_g
 */
void execution(State st)
{
    // we assign each new state own id
    static long next_id = 1;
    // if this state already was
    auto it = states.find(st);
    if ( it != states.end()){
        if (ltsOn){
            st.id = it->id;
            ltsout << st.dotLink();
        }
         return;
    }
    st.id = next_id++;
    states.insert(st);
    if (ltsOn){
        ltsout << st.dotName();
        ltsout << st.dotLink();
    }
    // if function f ended
    if (st.c_f != 12 ){
        // execute instruction of f
        execution(exec_f(st));
    }
    // if function g ended
    if (st.c_g != 16) {
        // execute instriction of g
        execution(exec_g(st));
    }
}

int main(int argc, char ** argv)
{
    if (!parse(argc, argv)){
        about();
        return 1;
    }
    State st;
    st.c_f = 0;
    st.c_g = 0;
    if (ltsOn){
        ltsout << "digraph G{\n";
    }
    execution(st);
    out << "c_f, c_g, h, f.x, f.y, g.x, g.y\n";
    for (auto i : states){
        out << i << std::endl;
    }
    if(isCount){
         std::cout << states.size() << std::endl;
    }
    if (ltsOn)
        ltsout << "}\n";
        ltsout.close();
    return 0;
}
