
//
// This is example code from Chapter 6.7 "Trying the second version" of
// "Software - Principles and Practice using C++" by Bjarne Stroustrup
//

/*
   This file is known as calculator02buggy.cpp

   I have inserted 5 errors that should cause this not to compile
   I have inserted 3 logic errors that should cause the program to give wrong results

   First try to find an remove the bugs without looking in the book.
   If that gets tedious, compare the code to that in the book (or posted source code)

   Happy hunting!

*/

#include "../std_lib_facilities.h"
#include "token.h"

Token_stream ts;        // provides get() and putback() 

//------------------------------------------------------------------------------

double expression();    // declaration so that primary() can call expression()

//------------------------------------------------------------------------------

// deal with numbers and parentheses
double primary()
{
    Token t = ts.get();
    switch (t.kind) {
        case '(':    // handle '(' expression ')'
            {    
                double d = expression();
                t = ts.get();
                if (t.kind != ')') error("')' expected");
                return d;
            }
        case '8':            // we use '8' to represent a number
            return t.value;  // return the number's value
        default:
            error("primary expected");
    }
}

//------------------------------------------------------------------------------

// deal with *, /, and %
double term()
{
    double left = primary();
    Token t = ts.get();        // get the next token from token stream

    while(true) {
        switch (t.kind) {
            case '*':
                left *= primary();
                t = ts.get();
                break;
            case '/':
                {    
                    double d = primary();
                    if (d == 0) error("divide by zero");
                    left /= d; 
                    t = ts.get();
                    break;
                }
            default: 
                ts.putback(t);     // put t back into the token stream
                return left;
        }
    }
}

//------------------------------------------------------------------------------

// deal with + and -
double expression()
{
    double left = term();      // read and evaluate a Term
    Token t = ts.get();        // get the next token from token stream

    while(true) {    
        switch(t.kind) {
            case '+':
                left += term();    // evaluate Term and add
                t = ts.get();
                break;
            case '-':
                left += term();    // evaluate Term and subtract
                t = ts.get();
                break;
            default: 
                ts.putback(t);     // put t back into the token stream
                return left;       // finally: no more + or -: return the answer
        }
    }
}

//------------------------------------------------------------------------------
//
const string WELCOME = 
"Welcome to our simple calculator.\n  \
Please enter expressions using flating-point numbers";

const string INSTRUCT = 
"Support +, -, *, /, (, )\n \
use x to exit \n \
end expression with ;";

int main()
    try
{
    cout << WELCOME << endl;
    cout << INSTRUCT << endl;

    double val = 0;
    while (cin) {
        Token t = ts.get();

        if (t.kind == ';')        // ';' for "print now"
            cout << "=" << val << '\n';
        else
            ts.putback(t);

        t = ts.get();
        if (t.kind == 'x') break; // 'x' for quit
        else
            ts.putback(t);
        val = expression();
    }
    keep_window_open();
}
catch (exception& e) {
    cerr << "error: " << e.what() << '\n'; 
    keep_window_open();
    return 1;
}
catch (...) {
    cerr << "Oops: unknown exception!\n"; 
    keep_window_open();
    return 2;
}

//------------------------------------------------------------------------------
