#include <iostream>
 
#define OUT 0
#define IN 1
 
int main(int argc, char** argv)
{
    
    char* str = argv[1];
    int state = OUT;
    unsigned wc = 0; 
    
    while (*str)
    {
        if (*str == ' ' || *str == '\n' || *str == '\t')
            state = OUT;
 
        else if (state == OUT)
        {
            state = IN;
            ++wc;
        }
         ++str;
    }
 
    std::cout << "Number of words: " << wc << std::endl;

}
 