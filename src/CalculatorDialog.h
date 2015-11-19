#ifndef CALCULATOR_DIALOG_H
#define CALCULATOR_DIALOG_H
#include <wx/dialog.h>

class wxButton;
class wxTextCtrl;
class CStaticText;
class wxColor;
class wxGBSpan;
class wxGBPosition;
class wxGridBagSizer;

class CCalculatorDialog : public wxDialog
{
public:
        CCalculatorDialog(wxWindow *parent, const wxString& arInitialSum);


        wxString GetSum();

        void OnNumber(wxCommandEvent& event);
        void OnOperator(wxCommandEvent& arEvent);
        void OnEnter(wxCommandEvent& arEvent);
        void OnClear(wxCommandEvent& arEvent);
        void OnBackspace(wxCommandEvent& arEvent);
        void OnDeletePress(wxCommandEvent& arEvent);
private:
        void InsertCharacter(const wxString& arString);

        void Clear();

        // Avoid repeating myself adding buttons.
        void AddButton(wxGridBagSizer& arSizer, 
                       const wxGBPosition& arPositon,
                       const wxGBSpan& arSpan,
                       const wxString& arString, 
                       long aButtonId,
                       wxColor& arOutlineColor);

        //CStaticText* mpTextBox;
        wxTextCtrl* mpTextBox;

        DECLARE_EVENT_TABLE()
};

class CCalcButton: public wxBitmapButton
{
        DECLARE_DYNAMIC_CLASS (CCalcButton)
public:
        CCalcButton();
        CCalcButton(wxWindow* parent,
                    wxWindowID id,
                    wxBitmap aBitmap,
                    const wxPoint& pos = wxDefaultPosition,
                    const wxSize& size = wxDefaultSize);

        bool Create(wxWindow* parent,
                    wxWindowID id,
                    const wxBitmap& bitmap,
                    const wxPoint& pos,
                    const wxSize& size = wxDefaultSize,
                    long style = 0,
                    const wxValidator& validator = wxDefaultValidator,
                    const wxString& name = _T("button"));

        virtual bool HasTransparentBackground();
};
 
#define WXBUTTON_ZERO         120
#define WXBUTTON_ONE          121
#define WXBUTTON_TWO          122
#define WXBUTTON_THREE        123
#define WXBUTTON_FOUR         124
#define WXBUTTON_FIVE         125
#define WXBUTTON_SIX          126
#define WXBUTTON_SEVEN        127
#define WXBUTTON_EIGHT        128
#define WXBUTTON_NINE         129
#define WXBUTTON_DOT          130
#define WXBUTTON_ADD          131
#define WXBUTTON_SUBTRACT     132
#define WXBUTTON_MULTIPLY     133
#define WXBUTTON_DIVIDE       134
#define WXBUTTON_EXPONENTIAL  135
#define WXBUTTON_OPENBRACKET  136
#define WXBUTTON_CLOSEBRACKET 137
#define WXBUTTON_CLEAR        138
#define WXBUTTON_BACKSPACE    139
#define WXBUTTON_ENTER        140
#define WXBUTTON_PERCENT      141
#define WXBUTTON_DELETEPRESS  142
#endif // CALCULATOR_DIALOG_H
