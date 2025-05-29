#pragma once

const string opers = "+-*/%";

enum{
    div_zero = 1,
    mod_zero,
    unknown
};

class Task{
public:
    Task(int x, int y, char op):data1_(x), data2_(y), oper_(op), result_(0), exitcode_(0){}

    void run(){
        switch (oper_)
        {
        case '+':
            result_ = data1_ + data2_;
            break;
        case '-':
            result_ = data1_ - data2_;
            break;
        case '*':
            result_ = data1_ * data2_;
            break;
        case '/':
            {
                if(data2_==0){
                    exitcode_ = div_zero;
                }
                else{
                    result_ = data1_ / data2_;
                }
            }
            break;
        case '%':
            {
                if(data2_==0){
                    exitcode_ = mod_zero;
                }
                else{
                    result_ = data1_ % data2_;
                }
            }
            break;
        default:
            exitcode_ = unknown;
            break;
        }
    }
    std::string GetResult(){
        string r = to_string(data1_);
        r += oper_;
        r += to_string(data2_);
        r += "=";
        r += to_string(result_);
        r += "[code: ";
        r += to_string(exitcode_);
        r += "]";
        return r;

    }
    string GetTask(){
        string r = to_string(data1_);
        r += oper_;
        r += to_string(data2_);
        r += "=?";
        return r;
    }
    ~Task(){}
private:
    int data1_;
    int data2_;
    char oper_;

    int result_;
    int exitcode_;
};