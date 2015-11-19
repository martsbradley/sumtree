#include "wx/wx.h"
#include "CalculatorDialog.h"
#include <wx/gbsizer.h>

#include "SumParser.h"
#include "SumNode.h"
#include "StaticText.h"

BEGIN_EVENT_TABLE(CCalculatorDialog, wxDialog)
        EVT_BUTTON(WXBUTTON_ZERO,  CCalculatorDialog::OnNumber)
        EVT_BUTTON(WXBUTTON_ONE,   CCalculatorDialog::OnNumber)
        EVT_BUTTON(WXBUTTON_TWO,   CCalculatorDialog::OnNumber)
        EVT_BUTTON(WXBUTTON_THREE, CCalculatorDialog::OnNumber)
        EVT_BUTTON(WXBUTTON_FOUR,  CCalculatorDialog::OnNumber)
        EVT_BUTTON(WXBUTTON_FIVE,  CCalculatorDialog::OnNumber)
        EVT_BUTTON(WXBUTTON_SIX,   CCalculatorDialog::OnNumber)
        EVT_BUTTON(WXBUTTON_SEVEN, CCalculatorDialog::OnNumber)
        EVT_BUTTON(WXBUTTON_EIGHT, CCalculatorDialog::OnNumber)
        EVT_BUTTON(WXBUTTON_NINE,  CCalculatorDialog::OnNumber)

        EVT_BUTTON(WXBUTTON_DOT,          CCalculatorDialog::OnOperator)
        EVT_BUTTON(WXBUTTON_ADD,          CCalculatorDialog::OnOperator)
        EVT_BUTTON(WXBUTTON_SUBTRACT,     CCalculatorDialog::OnOperator)
        EVT_BUTTON(WXBUTTON_MULTIPLY,     CCalculatorDialog::OnOperator)
        EVT_BUTTON(WXBUTTON_DIVIDE,       CCalculatorDialog::OnOperator)
        EVT_BUTTON(WXBUTTON_OPENBRACKET,  CCalculatorDialog::OnOperator)
        EVT_BUTTON(WXBUTTON_CLOSEBRACKET, CCalculatorDialog::OnOperator)
        EVT_BUTTON(WXBUTTON_PERCENT,      CCalculatorDialog::OnOperator)
        EVT_BUTTON(WXBUTTON_EXPONENTIAL,  CCalculatorDialog::OnOperator)

        EVT_BUTTON(WXBUTTON_CLEAR,        CCalculatorDialog::OnClear)
        EVT_BUTTON(WXBUTTON_BACKSPACE,    CCalculatorDialog::OnBackspace)
        EVT_BUTTON(WXBUTTON_DELETEPRESS,  CCalculatorDialog::OnDeletePress)
        EVT_BUTTON(WXBUTTON_ENTER,        CCalculatorDialog::OnEnter)
END_EVENT_TABLE()



static int DisplayFontSize = 24;
static int CalculatorFontSize = 20;

static void DrawBorder(wxBitmap& arBitmap,
                       int aBorderWidth,
                       int aBorderHeight,
                       const wxColor& arBkgColor,
                       const wxColor& arBorderColor,
                       wxMemoryDC& arDC)
{
        arDC.SelectObject(arBitmap);
        arDC.SetBackground(arBkgColor);
        arDC.Clear();
        arDC.SetBrush(arBkgColor);
        arDC.SetPen(wxPen(arBorderColor,5));
        arDC.DrawRoundedRectangle(5,5,aBorderWidth,aBorderHeight,9.0);
}

static wxBitmap GetBitMap(const wxString& arString, const wxColor& arBkgColor, const wxColor& arBorderColor)
{
        wxBitmap Bitmap(60, 60);
        wxMemoryDC Dc;
        DrawBorder(Bitmap, 50, 50, arBkgColor,arBorderColor, Dc);
        int Width;
        int Height;
        Dc.GetTextExtent(arString,&Width, &Height);

        wxFont Font(CalculatorFontSize,
                    wxFONTFAMILY_DEFAULT,
                    wxFONTSTYLE_NORMAL,
                    wxFONTWEIGHT_BOLD);
        Dc.SetFont(Font);

        int XPos = (50 -Width)/2;

        Dc.DrawText(arString, XPos, 14);

        Dc.SelectObject( wxNullBitmap );
        return Bitmap;
}
static wxBitmap GetAddBitmap(const wxColor& arBkgColor,
                               const wxColor& arBorderColor)
{
        wxBitmap Bitmap(60, 140);
        wxMemoryDC Dc;
        DrawBorder(Bitmap, 50, 130, arBkgColor,arBorderColor, Dc);

        Dc.SetTextForeground(*wxBLACK);
        wxFont Font(CalculatorFontSize,
                    wxFONTFAMILY_DEFAULT,
                    wxFONTSTYLE_NORMAL,
                    wxFONTWEIGHT_BOLD);
        Dc.SetFont(Font);
        Dc.DrawText(_T("+"), 22, 54);

        Dc.SelectObject( wxNullBitmap );
        return Bitmap;
}
static wxBitmap GetDivideBitmap(const wxColor& arBkgColor,
                                const wxColor& arBorderColor)
{
        wxBitmap Bitmap(60, 60);
        wxMemoryDC Dc;
        DrawBorder(Bitmap, 50, 50, arBkgColor,arBorderColor, Dc);

        Dc.SetPen(wxPen(*wxBLACK,3));
        Dc.DrawLine(20,30, 40,30);

        Dc.DrawRoundedRectangle(30, 23, 2, 2, 1);
        Dc.DrawRoundedRectangle(30, 37, 2, 2, 1);



        Dc.SelectObject( wxNullBitmap );
        return Bitmap;
}
static wxBitmap GetClearBitmap(const wxColor& arBkgColor,
                               const wxColor& arBorderColor)
{
        wxBitmap Bitmap(140, 60);
        wxMemoryDC Dc;
        DrawBorder(Bitmap, 130, 50, arBkgColor,arBorderColor, Dc);

        wxFont Font(CalculatorFontSize,
                    wxFONTFAMILY_DEFAULT,
                    wxFONTSTYLE_NORMAL,
                    wxFONTWEIGHT_BOLD);
        Dc.SetFont(Font);
        Dc.DrawText(_T("Clear"), 22, 14);

        Dc.SelectObject( wxNullBitmap );
        return Bitmap;
}

static wxBitmap GetBackspaceBitmap(const wxColor& arBkgColor,
                                   const wxColor& arBorderColor)
{
        wxBitmap Bitmap(140, 60);
        wxMemoryDC Dc;
        DrawBorder(Bitmap, 130, 50, arBkgColor,arBorderColor, Dc);

        wxFont Font(CalculatorFontSize,
                    wxFONTFAMILY_DEFAULT,
                    wxFONTSTYLE_NORMAL,
                    wxFONTWEIGHT_BOLD);
        Dc.SetFont(Font);
        Dc.DrawText(_T("Back"), 22, 14);

        Dc.SelectObject( wxNullBitmap );
        return Bitmap;
}


CCalculatorDialog::CCalculatorDialog(wxWindow *parent, const wxString& arInitialSum)
             : wxDialog(parent, wxID_ANY, wxString(_T("SumTree Calculator Input Dialog")))
{

        wxTextValidator InputValidator(wxFILTER_INCLUDE_CHAR_LIST);

        wxArrayString AcceptableCharacters;

        const wxString AllowedChars= _T("0123456789.+-*/()%^");

        for (size_t Idx = 0; Idx < AllowedChars.Length(); ++Idx)
        {
                AcceptableCharacters.Add(AllowedChars[Idx]);
        }

        InputValidator.SetIncludes(AcceptableCharacters);
        
        mpTextBox = new wxTextCtrl(this,
                                   wxID_ANY,
                                   _T(""),
                                   wxDefaultPosition,
                                   wxSize(100,40),
                                   wxTE_PROCESS_ENTER | wxTE_RICH2 |wxTE_RIGHT,
                                   InputValidator);


        mpTextBox->SetEditable(false);

        wxFont Font(DisplayFontSize,
                    wxFONTFAMILY_DEFAULT,
                    wxFONTSTYLE_NORMAL,
                    wxFONTWEIGHT_NORMAL);

        mpTextBox->SetDefaultStyle(wxTextAttr(*wxBLACK, wxNullColour, Font));
        mpTextBox->AppendText(arInitialSum);

        wxColor NumberColor(156,254,132);
        wxColor ControlColor(*wxBLUE);
        wxColor OperatorColor(188,222,236);


        wxGridBagSizer* pGrid = new wxGridBagSizer(10,10);

        wxGBPosition Position(0,0);
        wxGBSpan Span(1,2);

        wxBitmap bmp = GetClearBitmap(GetBackgroundColour(), ControlColor);
         
        wxBitmapButton* pClearButton= new wxBitmapButton(this,WXBUTTON_CLEAR ,bmp,wxPoint(10,10),wxSize(-1,-1));

        pGrid->Add(pClearButton,Position, Span, wxEXPAND);

        Position.SetCol(2);
        Span.SetColspan(2);

         
        wxAcceleratorEntry Accelerators[39];

        bmp = GetBackspaceBitmap(GetBackgroundColour(), ControlColor);
        wxBitmapButton* pBackspaceButton= new wxBitmapButton(this,WXBUTTON_BACKSPACE ,bmp,wxPoint(10,10),wxSize(-1,-1));
        pGrid->Add(pBackspaceButton,Position, Span, wxEXPAND);
        Accelerators[0].Set(wxACCEL_NORMAL,  WXK_BACK,    WXBUTTON_BACKSPACE);
        //   The delete button.
        //Accelerators[??].Set(wxACCEL_NORMAL,  WXK_NUMPAD_DELETE,  WXBUTTON_DELETEPRESS);

        Position.SetCol(4);
        Span.SetColspan(1);
        bmp = GetDivideBitmap(GetBackgroundColour(), OperatorColor);
        wxBitmapButton* pDivideButton= new wxBitmapButton(this,WXBUTTON_DIVIDE ,bmp,wxPoint(10,10),wxSize(-1,-1));
        pGrid->Add(pDivideButton, Position, Span, wxEXPAND);
        Accelerators[1].Set(wxACCEL_NORMAL,  WXK_NUMPAD_DIVIDE, WXBUTTON_DIVIDE);
        Accelerators[2].Set(wxACCEL_NORMAL,  (int)'/', WXBUTTON_DIVIDE);


        ////////////////////////////////////////////////////////////////////////////////
        //  Row 1
        ////////////////////////////////////////////////////////////////////////////////
        Position.SetCol(0);
        Position.SetRow(1);
        Span.SetColspan(1);
        AddButton(*pGrid, Position, Span, _T("("), WXBUTTON_OPENBRACKET, OperatorColor);
        Accelerators[3].Set(wxACCEL_SHIFT,  (int) '(', WXBUTTON_OPENBRACKET);

        Position.SetCol(1);
        AddButton(*pGrid, Position, Span, _T("7"), WXBUTTON_SEVEN, NumberColor);
        Accelerators[4].Set(wxACCEL_NORMAL,  (int) '7',   WXBUTTON_SEVEN);
        Accelerators[5].Set(wxACCEL_NORMAL,  WXK_NUMPAD7, WXBUTTON_SEVEN);

        Position.SetCol(2);
        AddButton(*pGrid, Position, Span, _T("8"), WXBUTTON_EIGHT, NumberColor);
        Accelerators[6].Set(wxACCEL_NORMAL,  (int) '8', WXBUTTON_EIGHT);
        Accelerators[7].Set(wxACCEL_NORMAL,  WXK_NUMPAD8, WXBUTTON_EIGHT);

        Position.SetCol(3);
        AddButton(*pGrid, Position, Span, _T("9"), WXBUTTON_NINE, NumberColor);
        Accelerators[8].Set(wxACCEL_NORMAL,  (int) '9', WXBUTTON_NINE);
        Accelerators[9].Set(wxACCEL_NORMAL,  WXK_NUMPAD9, WXBUTTON_NINE);

        Position.SetCol(4);
        AddButton(*pGrid, Position, Span, _T("*"), WXBUTTON_MULTIPLY, OperatorColor);
        Accelerators[10].Set(wxACCEL_SHIFT,  (int)'*', WXBUTTON_MULTIPLY);
        Accelerators[11].Set(wxACCEL_NORMAL,  WXK_NUMPAD_MULTIPLY, WXBUTTON_MULTIPLY);

        ////////////////////////////////////////////////////////////////////////////////
        //  Row 2
        ////////////////////////////////////////////////////////////////////////////////

        Position.SetRow(2);
        Position.SetCol(0);
        AddButton(*pGrid, Position, Span, _T(")"), WXBUTTON_CLOSEBRACKET, OperatorColor);
        Accelerators[12].Set(wxACCEL_SHIFT,  (int) ')',     WXBUTTON_CLOSEBRACKET);

        Position.SetCol(1);
        AddButton(*pGrid, Position, Span, _T("4"), WXBUTTON_FOUR, NumberColor);
        Accelerators[13].Set(wxACCEL_NORMAL,  (int) '4',     WXBUTTON_FOUR);
        Accelerators[14].Set(wxACCEL_NORMAL,  WXK_NUMPAD4,  WXBUTTON_FOUR);

        Position.SetCol(2);
        AddButton(*pGrid, Position, Span, _T("5"), WXBUTTON_FIVE, NumberColor);
        Accelerators[15].Set(wxACCEL_NORMAL,  (int) '5',     WXBUTTON_FIVE);
        Accelerators[16].Set(wxACCEL_NORMAL,  WXK_NUMPAD5,   WXBUTTON_FIVE);

        Position.SetCol(3);
        AddButton(*pGrid, Position, Span, _T("6"), WXBUTTON_SIX, NumberColor);
        Accelerators[17].Set(wxACCEL_NORMAL,  (int) '6',    WXBUTTON_SIX);
        Accelerators[18].Set(wxACCEL_NORMAL,  WXK_NUMPAD6,  WXBUTTON_SIX);

        Position.SetCol(4);
        AddButton(*pGrid, Position, Span, _T("-"), WXBUTTON_SUBTRACT, OperatorColor);
        Accelerators[19].Set(wxACCEL_NORMAL,  WXK_NUMPAD_SUBTRACT, WXBUTTON_SUBTRACT);
        Accelerators[20].Set(wxACCEL_NORMAL,  (int) '-',            WXBUTTON_SUBTRACT);

        ////////////////////////////////////////////////////////////////////////////////
        //  Row 3
        ////////////////////////////////////////////////////////////////////////////////
        Position.SetRow(3);
        Position.SetCol(0);

        AddButton(*pGrid, Position, Span, _T("%"), WXBUTTON_PERCENT, OperatorColor);
        Accelerators[21].Set(wxACCEL_SHIFT,  (int) '%',  WXBUTTON_PERCENT);

        Position.SetCol(1);
        AddButton(*pGrid, Position, Span, _T("1"), WXBUTTON_ONE, NumberColor);
        Accelerators[22].Set(wxACCEL_NORMAL,  (int) '1',     WXBUTTON_ONE);
        Accelerators[23].Set(wxACCEL_NORMAL,  WXK_NUMPAD1,     WXBUTTON_ONE);

        Position.SetCol(2);
        AddButton(*pGrid, Position, Span, _T("2"), WXBUTTON_TWO, NumberColor);
        Accelerators[24].Set(wxACCEL_NORMAL,  (int) '2',     WXBUTTON_TWO);
        Accelerators[25].Set(wxACCEL_NORMAL,  WXK_NUMPAD2,     WXBUTTON_TWO);

        Position.SetCol(3);
        AddButton(*pGrid, Position, Span, _T("3"), WXBUTTON_THREE, NumberColor);
        Accelerators[26].Set(wxACCEL_NORMAL,  (int) '3',     WXBUTTON_THREE);
        Accelerators[27].Set(wxACCEL_NORMAL,  WXK_NUMPAD3,     WXBUTTON_THREE);


        Position.SetCol(4);
        Span.SetRowspan(2);

        bmp = GetAddBitmap(GetBackgroundColour(), OperatorColor);
        wxBitmapButton* pAddButton= new wxBitmapButton(this,WXBUTTON_ADD ,bmp,wxPoint(10,10),wxSize(-1,-1));

        pGrid->Add(pAddButton,Position, Span, wxEXPAND);
        Accelerators[28].Set(wxACCEL_NORMAL,WXK_NUMPAD_ADD, WXBUTTON_ADD);
        Accelerators[29].Set(wxACCEL_SHIFT, (int)'+',       WXBUTTON_ADD);

    

        ////////////////////////////////////////////////////////////////////////////////
        //  Row 3
        ////////////////////////////////////////////////////////////////////////////////
        Position.SetRow(4);
        Position.SetCol(0);
        Span.SetRowspan(1);
        AddButton(*pGrid, Position, Span, _T("^"), WXBUTTON_EXPONENTIAL, OperatorColor);
        Accelerators[30].Set(wxACCEL_SHIFT,  (int) '^',  WXBUTTON_EXPONENTIAL);

        Position.SetCol(1);
        AddButton(*pGrid, Position, Span, _T("0"), WXBUTTON_ZERO, NumberColor);
        Accelerators[31].Set(wxACCEL_NORMAL,  (int) '0', WXBUTTON_ZERO);
        Accelerators[32].Set(wxACCEL_NORMAL,  WXK_NUMPAD0, WXBUTTON_ZERO);

        Position.SetCol(2);
        AddButton(*pGrid, Position, Span, _T("."), WXBUTTON_DOT, NumberColor);
        Accelerators[33].Set(wxACCEL_NORMAL, (int) '.'       , WXBUTTON_DOT);
        Accelerators[34].Set(wxACCEL_NORMAL, WXK_NUMPAD_DECIMAL, WXBUTTON_DOT);

        Position.SetCol(3);
        AddButton(*pGrid, Position, Span, _T("="), WXBUTTON_ENTER, OperatorColor);
        Accelerators[35].Set(wxACCEL_NORMAL,  WXK_NUMPAD_ENTER, WXBUTTON_ENTER);
        Accelerators[36].Set(wxACCEL_NORMAL,  WXK_RETURN      , WXBUTTON_ENTER);
        Accelerators[37].Set(wxACCEL_NORMAL,  WXK_NUMPAD_EQUAL, WXBUTTON_ENTER);
        Accelerators[38].Set(wxACCEL_NORMAL,  (int) '=', WXBUTTON_ENTER);



        wxBoxSizer *pVerticalBox = new wxBoxSizer(wxVERTICAL);
        pVerticalBox->Add(mpTextBox,1, wxEXPAND,5);
        pVerticalBox->Add(pGrid);

        SetSizer(pVerticalBox);

        pVerticalBox->SetSizeHints(this);
        pVerticalBox->Fit(this);
  
        wxAcceleratorTable accel(39, Accelerators);
        this->SetAcceleratorTable(accel);
}

void CCalculatorDialog::AddButton(wxGridBagSizer& arSizer, 
                                  const wxGBPosition& arPositon,
                                  const wxGBSpan& arSpan,
                                  const wxString& arString, 
                                  long aButtonId,
                                  wxColor& arOutlineColor)
{
        wxColor rBkgColor = GetBackgroundColour();

        wxBitmap Bitmap = GetBitMap(arString,
                                    rBkgColor,
                                    arOutlineColor);

        CCalcButton* pButton = new CCalcButton(this, aButtonId, Bitmap);
        arSizer.Add(pButton,arPositon, arSpan, wxEXPAND);
}

void CCalculatorDialog::OnNumber(wxCommandEvent& arEvent)
{
        wxString Value = wxString::Format(_T("%d"), arEvent.GetId() - WXBUTTON_ZERO);
        InsertCharacter(Value);
}

void  CCalculatorDialog::InsertCharacter(const wxString& arString)
{
        if (GetSum() == _T("0") && arString != _T("."))
        {
                mpTextBox->Clear();
        }

        mpTextBox->WriteText(arString);
        mpTextBox->SetFocus();
}

void CCalculatorDialog::OnOperator(wxCommandEvent& arEvent)
{
        switch(arEvent.GetId())
        {
                case WXBUTTON_ADD:
                        InsertCharacter(_T("+"));

                        break;
                case WXBUTTON_SUBTRACT:
                        InsertCharacter(_T("-"));
                        break;
                case WXBUTTON_EXPONENTIAL:
                        InsertCharacter(_T("^"));
                        break;
                case WXBUTTON_MULTIPLY:
                        InsertCharacter(_T("*"));
                        break;
                case WXBUTTON_DIVIDE:
                        InsertCharacter(_T("/"));
                        break;
                case WXBUTTON_OPENBRACKET:
                        InsertCharacter(_T("("));
                        break;
                case WXBUTTON_CLOSEBRACKET:
                        InsertCharacter(_T(")"));
                        break;
                case WXBUTTON_DOT:
                        InsertCharacter(_T("."));
                        break;
                case WXBUTTON_PERCENT:
                        InsertCharacter(_T("%"));
                        break;
                default:
                        InsertCharacter(_T("WTF"));
        }
}
void CCalculatorDialog::OnEnter(wxCommandEvent& /*arEvent*/)
{
        CSumParser SumParser;
        CSumNode* pNewTree = SumParser.Parse(GetSum().mb_str());
        if (pNewTree)
        {
                delete pNewTree;
                EndModal(wxID_OK);
        }
        else
        {
                wxMessageDialog Message(this, 
                                        _T("That expression is not valid.\n")
                                        _T("Press Ok to fix the expression or press Cancel to close the calculator."),
                                        _T("SumTree Input Dialog"), wxOK|wxCANCEL);

                if (Message.ShowModal() == wxID_CANCEL)
                {
                        EndModal(wxID_CANCEL);
                }
        }
}

void CCalculatorDialog::OnClear(wxCommandEvent& /*arEvent*/)
{
        Clear();
}

void CCalculatorDialog::Clear()
{
        mpTextBox->Clear();
        InsertCharacter(_T("0"));
}


void CCalculatorDialog::OnDeletePress(wxCommandEvent& /*arEvent*/)
{
        wxLogError(_T("OnDeletePress"));
        wxTextPos InsertPoint = mpTextBox->GetInsertionPoint();

        if (InsertPoint < mpTextBox->GetLastPosition())
        {
                mpTextBox->Remove(InsertPoint,InsertPoint+1);
                mpTextBox->SetFocus();
        }
}

void CCalculatorDialog::OnBackspace(wxCommandEvent& /*arEvent*/)
{
        wxTextPos InsertPoint = mpTextBox->GetInsertionPoint();
        if (InsertPoint > 1)
        {
                mpTextBox->Remove(InsertPoint-1,InsertPoint);
                mpTextBox->SetFocus();
        }
        else
        {
                if (GetSum() != _T("0"))
                {
                        mpTextBox->Clear();
                        InsertCharacter(_T("0"));
                }
        }
}

wxString CCalculatorDialog::GetSum()
{
        return mpTextBox->GetLineText(0);
}


IMPLEMENT_DYNAMIC_CLASS (CCalcButton, wxBitmapButton)

CCalcButton::CCalcButton()
        :wxBitmapButton()
{
}
CCalcButton::CCalcButton(wxWindow* parent,
                         wxWindowID id,
                         wxBitmap aBitmap,
                         const wxPoint& pos,
                         const wxSize& /*size*/)
        :wxBitmapButton(parent,id, aBitmap, pos, wxSize(-1,-1), wxBU_AUTODRAW|wxBORDER_SUNKEN)
{
}

bool CCalcButton::Create(wxWindow* parent,
            wxWindowID id,
            const wxBitmap& bitmap,
            const wxPoint& pos,
            const wxSize& size,
            long style,
            const wxValidator& validator,
            const wxString& name)
{
    bool bRetVal = wxBitmapButton::Create(parent, id, bitmap, pos, size, style, validator, name);

    SetBackgroundColour(parent->GetBackgroundColour());
    SetBackgroundStyle(wxBG_STYLE_COLOUR);
    SetForegroundColour(parent->GetForegroundColour());

    return bRetVal;
}

bool CCalcButton::HasTransparentBackground()
{
        return true;
}

