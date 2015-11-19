#ifdef __GNUG__
#pragma implementation
#endif

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"
#include "wx/filename.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#if !wxUSE_PRINTING_ARCHITECTURE
#error "You must set wxUSE_PRINTING_ARCHITECTURE to 1 in setup.h, and recompile the library."
#endif

// Set this to 1 if you want to test PostScript printing under MSW.
// However, you'll also need to edit src/msw/makefile.nt.
#define wxTEST_POSTSCRIPT_IN_MSW 0

#include <ctype.h>
#include "wx/metafile.h"
#include "wx/print.h"
#include "wx/printdlg.h"
#include "wx/image.h"
#include "wx/accel.h"

#if wxTEST_POSTSCRIPT_IN_MSW
#include "wx/generic/printps.h"
#include "wx/generic/prntdlgg.h"
#endif

#include "Gui.h"
//#include "mfcalc.tab.h"

#include <stdlib.h>
#include "SumParser.h"
#include "SumNodeExpanderVisitor.h"
#include "wstringconvert.h"
#include "wx/textfile.h"
#include "wx/file.h"
#include "NodeDiminsionVisitor.h"
#include "CurrentSumVisitor.h"
#include "CalculatorDialog.h"
#include "StaticText.h"
#include "wx/aboutdlg.h"
#include "wx/statline.h"
#include "wx/generic/aboutdlgg.h"
#include "wx/stdpaths.h"
//#include "wx/msw/registry.h"
#include "wx/config.h"
IMPLEMENT_APP(MyApp)


void MyFrame::ShowFancyAboutDialog(wxCommandEvent& WXUNUSED(event))
{
    wxAboutDialogInfo info;

    info.SetName(_T("SumTree Desktop Calculator"));
    info.SetVersion(wxVERSION_NUM_DOT_STRING_T);
    //info.SetDescription(_T("SumTree Desktop Calculator"));

    info.SetVersion(_T("1.0"));

    wxString Author = _T("Martin Bradley");
    info.AddDeveloper(Author);
    info.SetLicence(_T("Sumtree is free software; you can redistribute it and/or modify it under the terms of the GNU General Public Licence as published by the Free Software Foundation; either version 3 of the Licence, or (at your option) any later version.\n")
_T("Sumtree is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public Licence for more details.\n")
_T("You should have received a copy of the GNU General Public Licence along with Sumtree; if not, write to the Free Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301 USA\n"));
    info.SetCopyright(_T("This software is released under the terms of the GPL.\n"));

    wxAboutBox(info);
}

// The `main program' equivalent, creating the windows and returning the
// main frame

bool MyApp::OnInit(void)
{
        wxInitAllImageHandlers();
        SetAppName(wxT("SumTree"));
        SetVendorName(wxT("MartinBradley"));
        MyFrame* pFrame = new MyFrame(_T("SumTree Calculator"), wxPoint(-1, -1), wxSize(-1, -1));
        SetTopWindow(pFrame);

        pFrame->ParseSum();
        pFrame->Show(true);


        return true;
}

int MyApp::OnExit()
{
        return 1;
}

DEFINE_EVENT_TYPE(wxEVT_MY_CUSTOM_COMMAND)

// it may also be convenient to define an event table macro for this event type
#define EVT_MY_CUSTOM_COMMAND(id, fn) \
    DECLARE_EVENT_TABLE_ENTRY( \
        wxEVT_MY_CUSTOM_COMMAND, id, wxID_ANY, \
        (wxObjectEventFunction)(wxEventFunction) wxStaticCastEvent( wxCommandEventFunction, &fn ), \
        (wxObject *) NULL \
    ),

BEGIN_EVENT_TABLE(MyFrame, wxFrame)
        EVT_MENU(WXPRINT_QUIT,          MyFrame::OnExit)
        EVT_TEXT_ENTER(WXSUMTEXT_ENTER, MyFrame::OnParseSum)
        EVT_MENU(WXCALCULATOR,          MyFrame::OnShowCalculatorDialog)

        EVT_MY_CUSTOM_COMMAND(wxID_ANY, MyFrame::OnShowCalculatorDialog)

        EVT_MENU(WXPRINT_ABOUT,         MyFrame::ShowFancyAboutDialog)
        EVT_MENU(WXTREE_FLIPTREE,       MyFrame::FlipTreeAround)
        EVT_MENU(WXTREE_EQUALS,         MyFrame::OnEquals)
        EVT_MENU(WXSAVE_IMAGE,          MyFrame::OnSaveImage)

END_EVENT_TABLE()

// Define my frame constructor
MyFrame::MyFrame(const wxString& title, const wxPoint& pos, const wxSize& size)
        :wxFrame(NULL, wxID_ANY, title, pos, size),
        mSumFontSize(20),
        mSumEntered(_T("0"))
{
        CreateStatusBar(2);

        //SetIcon(wxICON(Sumtree));

        CreateMenu();
        wxToolBar* pToolBar = CreateToolBar(wxNO_BORDER | wxTB_HORIZONTAL);
        PopulateToolbar(pToolBar);
        SetToolBar(pToolBar);


        wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);  
        wxBoxSizer* sumSizer = new wxBoxSizer(wxHORIZONTAL);  


        /*mpSelectedNode = new wxTextCtrl(this,
                                        WXSUMTEXT_ENTER ,
                                        _T(""), 
                                        wxDefaultPosition,
                                        wxSize(100,30),
                                        wxTE_PROCESS_ENTER );*/

        mpSumDisplay = new CStaticText(this/*,*mpSelectedNode*/);

        wxStaticBox *item14 = new wxStaticBox( this, -1, wxT("") );
        wxStaticBoxSizer *item13 = new wxStaticBoxSizer( item14, wxVERTICAL );

        item13->Add( mpSumDisplay, 1, wxGROW|wxALIGN_CENTER_VERTICAL);

        mpSumDisplay->SetMinSize(wxSize(100,35));

        SetDisplayedSum(mSumEntered);

        sumSizer->Add(item13, 1);//, wxALL, 10); 

        mpCanvas = new MyCanvas(this,
                                /**mpSelectedNode,*/
                                wxPoint(0, 0),
                                wxSize(-1, -1));
        sizer->Add(sumSizer, 
                   0,                              // Vertically unstrechable.
                   wxEXPAND | wxALL,              //  wxExpand just means fill avaiable space.
                   5);                            // pixels of border. 

        sizer->Add(mpCanvas,
                   1,
                   wxEXPAND);
        /*sizer->Add(mpSelectedNode,
                   0,                              // Vertically strechable.
                   wxEXPAND);*/

        SetSizer(sizer);  

        Centre(wxBOTH);
        sizer->SetSizeHints(this);

        SetStatusText(_T("Sum Graph"));

        SetBackgroundColour(* wxWHITE);
}

static wxBitmap getBitmap(const wxString& arPath){
    wxBitmap bmp;
    bmp.LoadFile(arPath, wxBITMAP_TYPE_BMP);
    return bmp;
}

void MyFrame::PopulateToolbar(wxToolBarBase* toolBar)
{
    // Set up toolbar
    enum
    {
        Tool_CalcDialog,
        Tool_FlipTree,
        Tool_Equals,
        Tool_Max
    };

    wxBitmap toolBarBitmaps[Tool_Max];

    wxString dataPath = wxT(PKG_DATA_DIR); 
    dataPath.append(wxFileName::GetPathSeparator());

    toolBarBitmaps[Tool_CalcDialog]  = getBitmap(dataPath + wxT("calc.bmp"));
    toolBarBitmaps[Tool_FlipTree]  = getBitmap(dataPath + wxT("fliptree.bmp"));
    toolBarBitmaps[Tool_Equals]  = getBitmap(dataPath +   wxT("equals.bmp"));

    int w = toolBarBitmaps[Tool_CalcDialog].GetWidth();
    int h = toolBarBitmaps[Tool_CalcDialog].GetHeight();

    /*if ( !m_smallToolbar )
    {
        w *= 2;
        h *= 2;

        for ( size_t n = Tool_new; n < WXSIZEOF(toolBarBitmaps); n++ )
        {
            toolBarBitmaps[n] =
                wxBitmap(toolBarBitmaps[n].ConvertToImage().Scale(w, h));
        }
    }*/


    toolBar->SetToolBitmapSize(wxSize(w, h));


    toolBar->AddTool(WXCALCULATOR, _T("Input"),
                     toolBarBitmaps[Tool_CalcDialog], wxNullBitmap, wxITEM_NORMAL,
                     _T("Change Sum"), _T("Click here to change the sum."));

    toolBar->AddTool(WXTREE_FLIPTREE, _T("Flip tree"),
                     toolBarBitmaps[Tool_FlipTree], wxNullBitmap, wxITEM_NORMAL,
                     _T("Turn tree around"), _T("Turn the tree the other way around."));

    toolBar->AddTool(WXTREE_EQUALS, _T("Show Answer"),
                     toolBarBitmaps[Tool_Equals], wxNullBitmap, wxITEM_NORMAL,
                     _T("Equals"), _T("The Equals button will take you to the answer directly."));


    toolBar->AddSeparator();

    // after adding the buttons to the toolbar, must call Realize() to refle
    // the changes
    toolBar->Realize();

    //toolBar->SetRows(!(toolBar->IsVertical()) ? m_rows : 10 / m_rows);
}


void MyFrame::OnExit(wxCommandEvent& WXUNUSED(event))
{
        Close(true /*force closing*/);
}

void MyFrame::CreateMenu()
{
        wxMenu *file_menu = new wxMenu;

        file_menu->AppendSeparator();

        wxMenuItem* pSave = new wxMenuItem(file_menu, WXSAVE_IMAGE, _T("S&ave Image"),      _T("Save Image"));


        file_menu->Append(pSave);

        file_menu->Append(WXPRINT_QUIT, _T("E&xit"),      _T("Exit program"));

        wxMenu *help_menu = new wxMenu;
        help_menu->Append(WXPRINT_ABOUT, _T("&About"),    _T("Application Information"));

        wxMenuBar *menu_bar = new wxMenuBar;

        menu_bar->Append(file_menu, _T("&File"));
        menu_bar->Append(help_menu, _T("&Help"));

        SetMenuBar(menu_bar);
}

void MyFrame::OnParseSum(wxCommandEvent& WXUNUSED(event))
{
        ParseSum();
}

void MyFrame::OnShowCalculatorDialog(wxCommandEvent& WXUNUSED(event))
{
    CCalculatorDialog Dlg(this, mSumEntered);

    if (Dlg.ShowModal() == wxID_OK)
    {
            mSumEntered = Dlg.GetSum();
            ParseSum();
    }
}

void MyFrame::ParseSum()
{
        mpCanvas->ParseSum(mSumEntered);
        UnHighLightSum();
        mpCanvas->SetFocus();
}

void MyFrame::FlipTreeAround(wxCommandEvent& /*arEvent*/)
{
        mpCanvas->FlipTreeAround();
}

void MyFrame::OnEquals(wxCommandEvent& /*arEvent*/)
{
        mpCanvas->OnEquals();
}

void MyFrame::OnSaveImage(wxCommandEvent& /*arEvent*/)
{
        mpCanvas->SaveImage();
}


void MyFrame::SetSumSelection(int aStartIdx, int aEndIdx)
{

                /*wxFont Font(mSumFontSize,
                              wxFONTFAMILY_DEFAULT,
                              wxFONTSTYLE_NORMAL,
                              wxFONTWEIGHT_NORMAL);

                mpSumText->SetStyle(aStartIdx, aEndIdx, wxTextAttr(*wxRED, wxNullColour, Font));

                mpSumText->SetSelection(SumTextLength-1 ,SumTextLength-1 );*/
                //s += wxString::Format("%d %d", aStartIdx, aEndIdx);


        mpSumDisplay->SetSelection(aStartIdx, aEndIdx);
}

void MyFrame::SetDisplayedSum(const wxString& arString)
{
    mpSumDisplay->SetSum(arString);
}

void MyFrame::UnHighLightSum()
{
        //wxString SumText = mpSumText->GetLineText(0);
        /*int SumTextLength = SumText.Len();

        wxFont font(mSumFontSize,
                    wxFONTFAMILY_DEFAULT,
                    wxFONTSTYLE_NORMAL,
                    wxFONTWEIGHT_NORMAL);

        mpSumText->SetStyle(0, SumTextLength, wxTextAttr(*wxBLACK, wxNullColour, font));
        */
}





////////////////////////////////////////////////////////////////////////////////
//                  MyCanvas
////////////////////////////////////////////////////////////////////////////////

BEGIN_EVENT_TABLE(MyCanvas, wxScrolledWindow)
        EVT_LEFT_DOWN(MyCanvas::OnDoubleClickEvent)
        EVT_MOTION(MyCanvas::OnMouseMoveEvent)
        //EVT_MOUSEWHEEL(MyCanvas::OnMouseWheel)
        EVT_PAINT(MyCanvas::OnPaint)
        EVT_SIZE(MyCanvas::OnSize)

END_EVENT_TABLE()


// Constructor for my canvas
MyCanvas::MyCanvas(MyFrame* apFrame,
                   /*wxTextCtrl& arSelectedNode,*/
                   const wxPoint& pos,
                   const wxSize& size)
        : wxScrolledWindow(apFrame, -1,pos, size),
          mpFrame(apFrame),
          mBitmap(2000,2000),
          mpSumTree(0),
          /*mrSelectedNode(arSelectedNode),*/
          mImageCenteringAdjustment(0),
          mScale(1.0),
          mXShift(0),
          mMaxX(0),
          mMaxY(0),
          mClientWidth(0),
          mTreeOrientation(ETreeUp)
{
        SetMinSize(wxSize(400, 400));
        SetScrollRate( 10, 10 );
        SetBackgroundColour(* wxWHITE);
}

wxString MyCanvas::GetSumNodeNameUnderCursor(wxMouseEvent& arEvent)
{
        int X = 0, Y = 0;

        CalcUnscrolledPosition(arEvent.m_x, arEvent.m_y, &X, &Y);

        wxPoint Position(X - mXShift,Y);
        const void* pObject = mNodeLocations.GetObjectAtLocation(Position);

        wxString Name;

        if (pObject)
        {
                const CSumNode* pNode =  reinterpret_cast<const CSumNode*>(pObject);

                Name = GetSumNodeUniqueName(*pNode);
        }

        return Name;
}
//  Finds the node by the name and will alter the expanded/compressed state of the node
//  if the argument aUpdateTree is true.
//  if found it will call the passed member funciton.
void MyCanvas::UpdateSelectedNode(const wxString& arSelectedNodeName,
                                  bool aJustHighlightText)
{
        wxString SelectedNodeName = arSelectedNodeName;

        CSumNodeExpanderVisitor ExpanderVisitor(SelectedNodeName, aJustHighlightText);
        mpSumTree->Visit(ExpanderVisitor);

        if (ExpanderVisitor.SelectedNodeWasFound())
        {
                if (aJustHighlightText)
                {
                        CSumNode& rNode = ExpanderVisitor.GetSelectedNode();

                        size_t StartBufferIdx;
                        size_t EndBufferIdx;

                        if (mCurrentSumVisitor.GetNodeLimits(rNode, StartBufferIdx, EndBufferIdx))
                        {
                                //wxLogError(_T("GetNodeLimits worked %d %d"), StartBufferIdx, EndBufferIdx);
                                mpFrame->SetSumSelection(StartBufferIdx, EndBufferIdx);
                        }
                        else
                        {
                        //        wxLogError(_T("GetNodeLimits failed"));
                        }

                }
                else
                {
                        UpdateDiagram();
                        mDrawingChanged  = true;
                }
        }
}

void MyCanvas::HandleNodeExpandState(CSumNode& )
{
}

void MyCanvas::HandleMouseOverNode(CSumNode& )
{
}

// Alter the diagram by expanding/contracting the nodes if necessary.
void MyCanvas::OnDoubleClickEvent(wxMouseEvent& arEvent)
{
        arEvent.Skip(false);
        try{
                wxString SelectedNodeName = GetSumNodeNameUnderCursor(arEvent);

                if (SelectedNodeName.length() != 0)
                {
                        UpdateSelectedNode(SelectedNodeName, false);
                }
        }
        catch(CDivideByZeroException& e)
        {
                wxString Title = _T("Arithmetic Error");
                wxString Message = _T("Division by Zero, error.");
                wxMessageDialog MessageBox(this, Message, Title, wxOK|wxICON_EXCLAMATION);
                if (mpSumTree)
                {
                        wxString Name = GetSumNodeUniqueName(*mpSumTree);
                        UpdateSelectedNode(Name, false);
                }
                arEvent.Skip();// Might not do any harm.
                MessageBox.ShowModal();
        }
        catch(...)
        {
                wxLogError(_T("OnDoubleClickEvent caught exception"));
        }
}

//  Highlight the node that the cursor is over and the corrosponding text in the sum
void MyCanvas::OnMouseMoveEvent(wxMouseEvent& arEvent)
{
        wxString SelectedNodeName = GetSumNodeNameUnderCursor(arEvent);

        if (SelectedNodeName.size() != 0 &&
            (mDrawingChanged || SelectedNodeName != mPreviouslySelectedNodeName) )
        {
                UpdateSelectedNode(SelectedNodeName, true);

                //  FIXME SHOULD BE SETTING THE STATUS BARHERE.
                wxString Selected = SelectedNodeName.c_str();
        }
        else if (SelectedNodeName.size() == 0 && mPreviouslySelectedNodeName.size() != 0)
        {
                mpFrame->SetSumSelection(0, 0);
        }

        mPreviouslySelectedNodeName = SelectedNodeName;
        mDrawingChanged = false;
}

void MyCanvas::OnMouseWheel(wxMouseEvent& /*arEvent*/)
{
}

void MyCanvas::FlipTreeAround()
{
        if (mTreeOrientation == ETreeUp)
                mTreeOrientation = ETreeDown;
        else
                mTreeOrientation = ETreeUp;

        UpdateDiagram();
}

void MyCanvas::OnEquals()
{
        //wxLogError(_T("OnEquals"));

        if (mpSumTree)
        {
                wxString Name = GetSumNodeUniqueName(*mpSumTree);
                UpdateSelectedNode(Name, false);

        }
}

void MyCanvas::SaveImage()
{
        wxBitmap DrawnOnBitmap = mBitmap.GetSubBitmap(wxRect(0,0,mMaxX+5, mMaxY+5));

        wxImage Image = DrawnOnBitmap.ConvertToImage();

        wxFileDialog dialog(this,
                        _T("Save Image"),
                        wxEmptyString,
                        _T("test.bmp"),
                        _T("Bitmap files (*.bmp)|*.bmp"),
                        wxFD_SAVE|wxFD_OVERWRITE_PROMPT);

        dialog.SetFilterIndex(0);

        if (dialog.ShowModal() == wxID_OK)
        {
                //wxLogMessage(_T("%s, filter %d"),
                //              dialog.GetPath().c_str(), dialog.GetFilterIndex());

                Image.SaveFile(dialog.GetPath(), wxBITMAP_TYPE_BMP);
        }
}
void MyCanvas::ParseSum(const wxString& arSum)
{
        CSumParser SumParser;
        CSumNode* pNewTree = SumParser.Parse(arSum.mb_str());

        if (pNewTree == 0)
        {
                wxString Message = _T("A valid Sum is required");
                wxMessageDialog MessageBox(this, Message);
                MessageBox.ShowModal();
        }
        else
        {
                if (mpSumTree)
                        delete mpSumTree;

                mpSumTree = pNewTree;
                UpdateDiagram();
        }
} 

void MyCanvas::UpdateDiagram()
{
        if (mpSumTree)
        {
                wxMemoryDC DC;
                DC.SelectObject(mBitmap);
                wxFont Font(24,
                                wxFONTFAMILY_DEFAULT,
                                wxFONTSTYLE_NORMAL,
                                wxFONTWEIGHT_NORMAL);
                DC.SetFont(Font);

                DC.Clear();

                CLayout LayoutPlanner(mTreeOrientation);
                CNodeSizeVisitor SizeVisitor(DC, LayoutPlanner);

                mpSumTree->Visit(SizeVisitor);

                mCurrentSumVisitor.Clear();
                mpSumTree->Visit(mCurrentSumVisitor);

                wxString DisplaySum = mCurrentSumVisitor.GetSum();

                mpFrame->SetDisplayedSum(DisplaySum);

                LayoutPlanner.Layout();

                mNodeLocations.Clear();

                LayoutPlanner.Draw(DC, mNodeLocations);
                mMaxX = DC.MaxX();
                mMaxY = DC.MaxY();

                SetVirtualSize(mMaxX, mMaxY);

                UpdatePictureCentering();
        }

        Refresh();
}

void MyCanvas::OnPaint(wxPaintEvent& WXUNUSED(event))
{
        wxPaintDC DC(this);
        PrepareDC(DC);
        DC.Clear();

        wxMemoryDC OtherDC;
        OtherDC.SelectObject(mBitmap);

        DC.Blit(mXShift, 0, mMaxX,mMaxY, &OtherDC, 0, 0);
}

// Define the repainting behaviour
void MyCanvas::OnSize(wxSizeEvent& arSizeEvent)
{
        mClientWidth = arSizeEvent.GetSize().GetWidth();

        UpdatePictureCentering();

        Refresh();
}
void MyCanvas::UpdatePictureCentering()
{
        if (mMaxX < mClientWidth)
        {
                mXShift =  (mClientWidth - mMaxX )/2;
                //wxLogError("mMaxX %d XShift %d", mMaxX, mXShift);
        }
}
