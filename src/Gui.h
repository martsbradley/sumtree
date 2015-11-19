#if defined(__GNUG__) && !defined(__APPLE__)
#pragma interface
#endif
#include <memory>
#include "Layout.h"
#include "CurrentSumVisitor.h"

class CStaticText;

class MyApp: public wxApp
{
  public:
    MyApp(){};
    bool OnInit();
    int OnExit();

};

DECLARE_APP(MyApp)

class MyCanvas;
//class CS#umNode;
#include "SumNode.h"
#include "NodeLocations.h"

// Define a new canvas and frame
class MyFrame: public wxFrame
{
  public:
    MyFrame(const wxString& title, const wxPoint& pos, const wxSize& size);

    void OnExit(wxCommandEvent& event);

    void OnParseSum(wxCommandEvent& event);
    void OnShowCalculatorDialog(wxCommandEvent& event);
    void ParseSum();

    void SetSumSelection(int StartIdx, int EndIdx);


    void ShowAboutDialog();
    void ShowTrialOverDialog();

    void ShowFancyAboutDialog(wxCommandEvent& WXUNUSED(event));


    void FlipTreeAround(wxCommandEvent& /*arEvent*/);

    void OnEquals(wxCommandEvent& /*arEvent*/);

    void OnSaveImage(wxCommandEvent& /*arEvent*/);

    void SetDisplayedSum(const wxString& arString);


  private:

        void PopulateToolbar(wxToolBarBase* toolBar);
        //  Remove all highlighting from the sum

        void UnHighLightSum();


        void CreateMenu();
        MyCanvas* mpCanvas;
        wxTextCtrl* mpSumText;
        wxString mSumEntered;

        CStaticText* mpSumDisplay;

        wxTextCtrl* mpSelectedNode;// Remove this.

        int mSumFontSize;
DECLARE_EVENT_TABLE()
};

// Define a new canvas which can receive some events
class MyCanvas: public wxScrolledWindow
{
public:
        MyCanvas(MyFrame *frame,
                 /*wxTextCtrl& arSelectedNode,*/
                 const wxPoint& pos,
                 const wxSize& size);

        ~MyCanvas(void){};

        void OnPaint(wxPaintEvent &event);

        void OnSize(wxSizeEvent& event);

        void UpdateSelectedNode(const wxString& arSelectedNodeName,
                                bool aJustFindTheNode = false);
        void OnDoubleClickEvent(wxMouseEvent& event);
        void OnMouseMoveEvent(wxMouseEvent& event);
        void OnMouseWheel(wxMouseEvent& arEvent);

        void HandleNodeExpandState(CSumNode& rNode);
        void HandleMouseOverNode(CSumNode& rNode);

        wxString GetSumNodeNameUnderCursor(wxMouseEvent& event);

        void ParseSum(const wxString& arSum);

        void FlipTreeAround();

        void OnEquals();

        void SaveImage();

        void UpdateDiagram();
private:
        bool IsTrialOver();
        void UpdatePictureCentering();

        MyFrame* mpFrame;
        wxBitmap m_bitmap;

        CSumNode* mpSumTree;
        wxString mPreviouslySelectedNodeName;
        int mImageCenteringAdjustment;

        double mScale;

        /* The following is useful for debugging without using message boxes.
        wxTextCtrl& mrSelectedNode;*/

        // knows where the nodes are on the screen
        CNodeLocations mNodeLocations;
        
        // paint to this then blit over to the screen
        wxBitmap mBitmap;
        int mXShift;
        int mMaxX;
        int mMaxY;
        int mClientWidth;
        EOrientation mTreeOrientation;
        bool mDrawingChanged; 

        CCurrentSumVisitor mCurrentSumVisitor;
        DECLARE_EVENT_TABLE()
};

#define WXPRINT_QUIT            100
#define WXPRINT_PREVIEW         101
#define WXPRINT_ABOUT           102
#define WXCALCULATOR            103
#define WXSUMTEXT_ENTER         104
#define WXTREE_FLIPTREE         105
#define WXTREE_EQUALS           106
#define WXSAVE_IMAGE            107
#define WXSHOW_LICENCE          108
