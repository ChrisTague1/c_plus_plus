#include <string>
#include <sstream>

using namespace std;

string number_to_word(int h) {
    if (h == 1) return "one";
    if (h == 2) return "two";
    if (h == 3) return "three";
    if (h == 4) return "four";
    if (h == 5) return "five";
    if (h == 6) return "six";
    if (h == 7) return "seven";
    if (h == 8) return "eight";
    if (h == 9) return "nine";
    if (h == 10) return "ten";
    if (h == 11) return "eleven";
    if (h == 12) return "twelve";
    if (h == 13) return "thirteen";
    if (h == 14) return "fourteen";
    if (h == 15) return "fifteen";
    if (h == 16) return "sixteen";
    if (h == 17) return "seventeen";
    if (h == 18) return "eighteen";
    if (h == 19) return "nineteen";
    if (h == 20) return "twenty";
    if (h == 21) return "twenty one";
    if (h == 22) return "twenty two";
    if (h == 23) return "twenty three";
    if (h == 24) return "twenty four";
    if (h == 25) return "twenty five";
    if (h == 26) return "twenty six";
    if (h == 27) return "twenty seven";
    if (h == 28) return "twenty eight";
    
    return "twenty nine";
}

string timeInWords(int h, int m) {
    stringstream output;
    string hour = number_to_word(h);
    
    if (m == 0) {
        output << hour << " o' clock";
    } else if (m == 1) {
        output << number_to_word(m) << " minute past " << hour;
    } else if (m == 15) {
        output << "quarter past " << hour;
    } else if (m == 30) {
        output << "half past " << hour;
    } else if (m == 45) {
        output << "quarter to " << number_to_word(h + 1);
    } else if (m < 30) {
        output << number_to_word(m) << " minutes past " << hour;
    } else {
        output << number_to_word(60 - m) << " minutes to " << number_to_word(h + 1);
    }
    
    return output.str();
}