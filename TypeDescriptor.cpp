//
// Created by George on 4/5/2019.
//
#ifndef TYPEDESCRIPTOR
#define TYPEDESCRIPTOR

#include <string>
#include <iostream>
#include <vector>

class TypeDescriptor
{
public:
    enum type {NONE, INTEGER, STRING, BOOLEAN, ARRAY};

    TypeDescriptor()
    {
        _type = NONE;
    }
    TypeDescriptor(int value)
    {
        _type = INTEGER;
        i_value = value;
    }
    TypeDescriptor(std::string value)
    {
        _type = STRING;
        s_value = value;
    }
    TypeDescriptor(bool value)
    {
        _type = BOOLEAN;
        b_value = value;
    }
    TypeDescriptor(std::vector<TypeDescriptor> value)
    {
        _type = ARRAY;
        a_value = value;
    }

    int get_ivalue()
    {
        if (_type != INTEGER)
        {
            std::cout << "Incorrect type used somewhere: Expected int but got: " << _type << std::endl;
            exit(3);
        }
        return i_value;
    }
    std::string get_svalue()
    {
        if (_type != STRING)
        {
            std::cout << "Incorrect type used somewhere: Expected string but got: " << _type << std::endl;
            exit(3);
        }
        return s_value;
    }

    bool get_bvalue()
    {
        if (_type != BOOLEAN)
        {
            std::cout << "Incorrect type used somewhere: Expected boolean but got: " << _type << std::endl;
            exit(3);
        }
        return b_value;
    }

    int get_size()
    {
        if (_type != ARRAY)
        {
            std::cout << "Tried to get size of array on a non array variable: Type is: " << _type << std::endl;
            exit(3);
        }
        return a_value.size();
    }

    void push(TypeDescriptor value) {
        a_value.emplace_back(value);
    }

    void atSet(TypeDescriptor index, TypeDescriptor value) {
        if (a_value.size() < index.get_ivalue())
        {
            std::cout << "You are out of bounds! Exiting...";
            exit(8);
        }
        else
            a_value.at(index.get_ivalue()) = value;
    }

    TypeDescriptor at(TypeDescriptor index) {
        if (a_value.size() < index.get_ivalue())
        {
            std::cout << "You are out of bounds! Exiting...";
            exit(8);
        }
        else
            return a_value.at(index.get_ivalue());
    }

    void pop() {
        a_value.pop_back();
    }

    bool empty(){
        if (a_value.empty())
            return true;
        else
            return false;
    }

    std::vector<TypeDescriptor> returnArray() {
        return a_value;
    }

    void print()
    {
        if (_type == INTEGER) {std::cout << i_value;}
        else if (_type == STRING) {std::cout << s_value;}
        else if (_type == BOOLEAN)
        {
            if (b_value) {std::cout << "True";}
            else {std::cout << "False";}
        }
        else if (_type == ARRAY) {
            std::cout << "[";
            if (!a_value.empty()) {
                for (auto it = a_value.begin(); it != a_value.end() - 1; ++it) {
                    it->print();
                    std::cout << ", ";
                }
                a_value.back().print();
            }
            std::cout << "]";
        }
        else {std::cout << "None";}
    }



    type get_type() { return _type; }

private:
    type _type;
    int i_value;
    std::string s_value;
    bool b_value;
    std::vector <TypeDescriptor> a_value;
};

#endif