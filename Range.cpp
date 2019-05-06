//
// Created by George on 3/29/2019.
//

#include <iostream>
#include "Expr.hpp"
#include "SymTab.hpp"

class Range
{
public:
    Range(ExprNode* stop): exp_start{nullptr}, exp_stop{stop}, exp_step{nullptr} {}
    Range(ExprNode* start, ExprNode* stop): exp_start{start}, exp_stop{stop}, exp_step{nullptr} {}
    Range(ExprNode* start, ExprNode* stop, ExprNode* step): exp_start{start}, exp_stop{stop}, exp_step{step} {}

    void begin(SymTab& symTab)
    {
        // Get Start first
        if (exp_start)
        {
            _start = exp_start->evaluate(symTab).get_ivalue();
        }
        else
        {
            _start = 0;
        }
        // Get stop after determining start value
        _stop = exp_stop->evaluate(symTab).get_ivalue();

        // Get the step if there is one, else set to default value of 1
        if (exp_step)
        {
            _step = exp_step->evaluate(symTab).get_ivalue();
        }
        else
        {
            _step = 1;
        }

        // Set to start so we can modify value
        value = _start;
    }

    int next()
    {
        int n = value;
        value += _step;
        return n;
    }

    bool done() { return value >= _stop; }

    void print()
    {
        std::cout << "range(";
        if (exp_start)
        {
            exp_start->print();
        }
        else
        {
            std::cout << "0";
        }
        std::cout << ", ";
        exp_stop->print();
        std::cout << ", ";
        if (exp_step)
        {
            exp_step->print();
        }
        else
        {
            std::cout << "1";
        }
        std::cout << ")";
    }

private:
    ExprNode* exp_start, *exp_stop, *exp_step;
    int _start = 0, _stop = 0, _step = 0;
    int value = 0;
};
