#include "SumParser.h"
#include "SumNode.h"
//#include "mfcalc.tab.h"
#include <string.h>
#include <wx/log.h>

int yyparse(CSumNode**);
extern char SumBuffer[];
extern int SumPtr;
extern int PreviousValueOfSumPtr;

CSumNode* CSumParser::Parse(const char* apSumString)
{
        // copy the string into the buffer
    CSumNode* pResult = 0;

    if (apSumString)
    {
            //wxLogMessage("apSumString not null");
            SumPtr = 0;
            PreviousValueOfSumPtr = 0;

            memset(&SumBuffer[0], 0, 1024);
            strncpy(&SumBuffer[0], apSumString, 1023);
            strncat(&SumBuffer[0], "\n", 1);

            //wxString Message("Message is '");
            //Message << &SumBuffer[0] << "'**";

            //wxLogMessage(Message);
            yyparse(&pResult);
    }

    return pResult;
}
