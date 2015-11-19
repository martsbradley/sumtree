#include "StaticText.h"

BEGIN_EVENT_TABLE(CStaticText, wxPanel)
        EVT_PAINT(CStaticText::OnPaint)
        EVT_LEFT_DOWN(CStaticText::OnClickEvent)
END_EVENT_TABLE()

CStaticText::CStaticText(wxWindow *parent /*,   wxTextCtrl& arOutput*/)
:   wxPanel (parent),
    mSelectionStartIdx(0),
    mSelectionEndIdx(0)
    //,  mrOutput(arOutput)
{
}

static int DrawText(wxPaintDC& arDC, const wxString& arString, int aStartPosition, wxFont& arFont)
{
        int Width = 0;
        int Height = 0;

        if (arString.Length() > 0)
        {
                arDC.SetFont(arFont);
                arDC.GetTextExtent(arString, &Width, &Height);
                arDC.DrawText(arString, aStartPosition, 0);
        }
        
        return aStartPosition+Width;
}

void CStaticText::OnPaint(wxPaintEvent& /*event*/) 
{
        wxPaintDC dc(this);
        wxFont SelectedFont(24,
                    wxFONTFAMILY_DEFAULT,
                    wxFONTSTYLE_NORMAL,
                    wxFONTWEIGHT_NORMAL);

        wxFont UnSelectedFont(24,
                    wxFONTFAMILY_DEFAULT,
                    wxFONTSTYLE_NORMAL,
                    wxFONTWEIGHT_NORMAL);

        int StartPosition = 0;
        dc.SetTextForeground(*wxBLACK);
        StartPosition = DrawText(dc, GetStartString(), StartPosition, UnSelectedFont);

        dc.SetTextForeground(*wxRED);
        StartPosition = DrawText(dc, GetSelectedString(), StartPosition, SelectedFont);

        dc.SetTextForeground(*wxBLACK);
        /*StartPosition = */DrawText(dc, GetEndString(), StartPosition, UnSelectedFont);

}


void CStaticText::SetSum(const wxString& arSumString)
{
        mSum = arSumString;
        mSelectionStartIdx = 0;
        mSelectionEndIdx = 0;
        Refresh();
}

const wxString& CStaticText::GetSum() const
{
        return mSum;
}

void CStaticText::SetSelection(const size_t aStartIdx, const size_t aEndIdx)
{
        assert(aStartIdx <= mSum.Length());
        assert(aEndIdx   <= mSum.Length());
        assert(aStartIdx <= aEndIdx);

        mSelectionStartIdx = aStartIdx;
        mSelectionEndIdx = aEndIdx;

        Refresh();

        if (aEndIdx != aStartIdx)
        {
                wxString Start    = GetStartString();
                wxString Selected = GetSelectedString();
                wxString End      = GetEndString();

/*                mrOutput.SetValue(wxString::Format(_T("%d,%d Start[%s], Selected[%s] End[%s]"),
                                                   mSelectionStartIdx,
                                                   mSelectionEndIdx,
                                                   Start.c_str(),
                                                   Selected.c_str(),
                                                   End.c_str()));*/
        }
}



// This method will remove the unselected text from mSumTemp
wxString CStaticText::GetStartString()
{
        mSumTemp = mSum;
        wxString Result;

        Result = mSumTemp.Mid(0, mSelectionStartIdx);

        mSumTemp = mSumTemp.Mid(mSelectionStartIdx);

        return Result;
}

wxString CStaticText::GetSelectedString()
{
        wxString Result;

        Result = mSumTemp.Mid(0, mSelectionEndIdx - mSelectionStartIdx);
        

        wxString Was = mSumTemp;
        mSumTemp =  mSumTemp.Mid(mSelectionEndIdx-mSelectionStartIdx);

        /*mrOutput.SetValue(wxString::Format(_T("%d %d (%s) '%s'"), 
                                           mSelectionStartIdx,
                                           mSelectionEndIdx,
                                           Was.c_str(),
                                           mSumTemp.c_str()));*/


        return Result;
}

wxString CStaticText::GetEndString()
{
        wxString Result = mSumTemp;
        mSumTemp.Clear();
        return Result;
}


void CStaticText::OnClickEvent(wxMouseEvent& /*arEvent*/)
{
        wxCommandEvent eventCustom(wxEVT_MY_CUSTOM_COMMAND);

        wxPostEvent(this, eventCustom);
}
