#include "wx/wx.h"

class CStaticText : public wxPanel
{ 
public:
        CStaticText(wxWindow* apParent/*, wxTextCtrl& arOutput*/);

        virtual void OnPaint(wxPaintEvent& event);

        void SetSum(const wxString& arSumString);

        const wxString& GetSum() const;

        void SetSelection(const size_t aStartIdx, const size_t aEndIdx);

        wxString GetStartString();
        wxString GetSelectedString();
        wxString GetEndString();


        void OnClickEvent(wxMouseEvent& arEvent);
private:

        size_t mSelectionStartIdx;
        size_t mSelectionEndIdx;
        wxString mSum;
        wxString mSumTemp;
        //wxTextCtrl& mrOutput;
        DECLARE_EVENT_TABLE()
};


BEGIN_DECLARE_EVENT_TYPES()
    DECLARE_EVENT_TYPE(wxEVT_MY_CUSTOM_COMMAND, 7777)
END_DECLARE_EVENT_TYPES()

