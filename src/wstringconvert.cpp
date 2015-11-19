#include "wstringconvert.h"
#include <iostream>
#include <string>
#include <sstream>
#include <stdexcept>
#include <vector>
#include <wx/string.h>
#include <wx/log.h>

std::string ws2s(const std::wstring& arWideString)
{
        std::string s(arWideString.begin(), arWideString.end());
        s.assign(arWideString.begin(), arWideString.end()); 
        return s;
}

int splitString(const std::string& arString,
                std::vector<std::string>& arParts,
                const std::string arDelimiters /*= " \t"*/)
{
        size_t Idx = 0;

        /*std::cout << "0123456789abcdef" << std::endl;
        std::cout << arString << std::endl;*/

        while(Idx != std::string::npos)
        {
                Idx = arString.find_first_not_of(arDelimiters.c_str(), Idx);

                size_t NextSpaceIdx = arString.find_first_of(arDelimiters.c_str(), Idx);

                if (Idx != std::string::npos)
                {
                        std::string token;

                        if (NextSpaceIdx != std::string::npos)
                        {
                                token = std::string(arString, Idx, NextSpaceIdx - Idx);
                        }
                        else
                        {
                                token = std::string(arString, Idx);
                        }

                        /*wxString Message = "adding token ";
                        Message << NextSpaceIdx << " " << Idx << " -> '";
                        Message << token.c_str() << "'";
                        wxLogMessage(Message);*/

                        arParts.push_back(token);
                }

                Idx = NextSpaceIdx;
        }

        return arParts.size();
}

// General tool to strip spaces from both ends:
std::string trimString(const std::string& arString,
                       const std::string& arCharsToTrim /* = " \t" */)
{
        if(arString.length() == 0)
        {
                return arString;
        }

        int FirstNonTrimCharIdx = arString.find_first_not_of(arCharsToTrim);
        int LastNonTrimCharIdx = arString.find_last_not_of(arCharsToTrim);

        if(FirstNonTrimCharIdx == -1) // No non-spaces
        {
                return "";
        }

        return std::string(arString, FirstNonTrimCharIdx, LastNonTrimCharIdx - FirstNonTrimCharIdx + 1);
}

void ConvertDoubles(int /*aNumberToRead*/,
                    const std::vector<std::string>& /*arParts*/,
                    std::vector<double>& /*arValues*/)
{
        /*arValues.reserve(aNumberToRead);

        for (int Idx = 0; Idx < aNumberToRead; ++Idx)
        {
                std::istringstream iss(arParts[Idx]);
                double Value;

                iss >> Value;

                if (!iss)
                {
                        wxString Message = _T("Read failed");
                        Message << arParts[Idx];
                        ::wxLogMessage(Message);
                        throw std::invalid_argument("Read failed");
                }
                else
                {
                        //wxString Message = "Read ok";
                        Message << arParts[Idx];
                        //Message << "ConvertDoubles should read";
                        //Message << aNumberToRead;
                        //::wxLogMessage(Message);
                }

                arValues.push_back(Value);
        }*/
}
