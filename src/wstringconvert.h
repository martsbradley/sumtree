#ifndef WSTRING_CONV
#define WSTRING_CONV
#include <string>
#include <vector>

//  Convert a wide string to a basic string
std::string ws2s(const std::wstring& arWideString);

//  Split a string into a vector
int splitString(const std::string& arString,
                std::vector<std::string>& arParts,
                const std::string arDelimiters /*= " \t"*/);



std::string trimString(const std::string& arString,
                 const std::string& arCharsToTrim /* = " \t"*/);

void ConvertDoubles(int aNumberToRead,
                    const std::vector<std::string>& arParts,
                    std::vector<double>& arValues);
#endif // WSTRING_CONV
