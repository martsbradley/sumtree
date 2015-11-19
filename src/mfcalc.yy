%{  #include <math.h>  /* For math functions, cos(), sin(), etc.  */
    #include <stdio.h>
    #include <ctype.h>
    #include "SumNode.h"
    #include <wx/log.h>
    void yyerror(CSumNode**, char const *);
    int yylex();
    extern int PreviousValueOfSumPtr;
    extern int SumPtr;
    extern char SumBuffer[];
%}

%parse-param {CSumNode** pNode}

%union {
    double      val;   /* For returning numbers.  */
    CSumNode    *node;   /* For returning nodes */
}
%token <val>  NUM       /* Simple double precision number.  */
%type  <node>  exp

%right '='
%left '-' '+'
%left '*' '/'
%left NEG     /* negation--unary minus */
%right '^'    /* exponentiation */

%% /* The grammar follows.  */

input:   /* empty */
     | input line
;

line:
       '\n'
     | exp '\n'   { *pNode = $1;  YYACCEPT;   }
     | error '\n' { yyerrok;                  }
;

exp:      NUM               { $$ = CreateNewNode($1);                          }
        | exp '+' exp       { $$ = CreateNewNode(EAdditionNode,       $1, $3); }
        | exp '-' exp       { $$ = CreateNewNode(ESubtractionNode,    $1, $3); }
        | exp '*' exp       { $$ = CreateNewNode(EMultiplicationNode, $1, $3); }
        | exp '/' exp       { $$ = CreateNewNode(EDivisionNode,       $1, $3); }
        | exp '+' exp '%'   { $$ = CreateNewNode(EAddPercent,         $1, $3);
                                   IsPercentage($3);                           }

        | exp '-' exp '%'   { $$ = CreateNewNode(ESubtractPercent,    $1, $3);
                                   IsPercentage($3);                           }

        | exp '*' exp '%'   { $$ = CreateNewNode(EMultiplyPercent,    $1, $3);
                                   IsPercentage($3);                           }

        | exp '/' exp '%'   { $$ = CreateNewNode(EDividePercent,      $1, $3);
                                   IsPercentage($3);                           }

        | '-' NUM %prec NEG { $$ = CreateNewNode(-1 * $2);                     }

        | exp '^' exp       { $$ = CreateNewNode(EExponential,        $1, $3); }

        | '(' exp ')'       { $$ = HasBrackets($2);                            }
;


/* End of grammar.  */
%%
static int GetACharFromBuffer(void)
{

        int c = '\0';

        if (SumPtr < 1024 && SumBuffer[SumPtr])
        {
                c = SumBuffer[SumPtr++];
        }

        if (c == '\0')
        {
                c = EOF;
        }

        return c;
}

static void PutCharBack()
{
        if (SumPtr > 0)
        {
                --SumPtr;
        }
}

static int isDigitOrDot(char c)
{
        return (c == '.' || isdigit (c));
}

static double GetNumber(bool* apResult)
{
        char Store[1024];
        memset(&Store[0], 0, 1024);

        int c;
        while ((c = GetACharFromBuffer()) != EOF && isDigitOrDot(c))
        {
                char AChar[2] = {'?', '\0'};
                AChar[0] = (char)c;

                strcat(Store, &AChar[0]);
        }
        PutCharBack();

        //  Adding on this bit of text so that we are sure that there 
        //  was not extra '.' characters.
        strcat(Store, ",23");

        double Result = 0;
        int Barrier;

        int ScannedItems = sscanf(&Store[0], "%lf,%d", &Result, &Barrier);

        //wxLogError(_T("ScannedItems %d"), ScannedItems);
        
        if (ScannedItems != 2)
        { 
                *apResult = false;
        }

        return Result;
}


int yylex (void)
{
        int c;

        /* Ignore white space, get first nonwhite character.  */
        
        do {
                c = GetACharFromBuffer();

        } while (c == ' ' || c == '\t');

        if (c != EOF)
        {
                // Char starts a number => parse the number.         
                if (isDigitOrDot(c))
                {
                        PutCharBack();
                        PreviousValueOfSumPtr = SumPtr;
                        bool Result = true;

                        double Value = GetNumber(&Result);

                        if (Result)
                        {
                                yylval.val = Value;
                                c = NUM;
                        }
                }
        }

        return c;
}

     
/* Called by yyparse on error.  */
void yyerror (CSumNode** , char const *)
{
        //wxLogError(_T("yyerror hit"));
}
