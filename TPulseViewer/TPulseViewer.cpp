#include "TPulseViewer.h"

#define MIRIAM 0


TPulseViewer::~TPulseViewer()
{
    
    fMain->SetCleanup(kDeepCleanup);
    fMain->Cleanup();
    delete fMain;
}

void TPulseViewer::Init()
{
    
    fMain = new TGMainFrame(gClient->GetRoot(),1800,1200);
    
    fMainChain = nullptr;
    fEventList = nullptr;
    fEntryCutExpression = nullptr;
    
    fTCutExpression = "";
    fEventIndex = 0;
    fEventListIndex = 0;
    fEventNumber = 0;
    fEventListNumber = 0;

    //tab3
    fChannel_tab3=1;
    fEventIndex_tab3=0;

    
    //Add bar option
    fMenuBar = new TGMenuBar(fMain);
    
    fMain->AddFrame(fMenuBar, new TGLayoutHints(kLHintsExpandX, 0, 0, 0, 3));
    
    //Creates menu bar  and added to the MainFrame
    
    fMenuBar->AddPopup("&File");
    fMenuBar->AddPopup("&Help");
    
    
    //create the tab handler
    TGTab *fTab = new TGTab(fMain, 300, 300);
    
    //creation of the tab for the pulseviewer
    TGCompositeFrame *fTab1 = fTab->AddTab("Pulse viewer");
    
    //Creation of Horizontal frame for canvas and button group
    TGHorizontalFrame *hFrameTab1_1 = new TGHorizontalFrame(fTab1,600,400);
    
    fTab1->AddFrame(hFrameTab1_1, new TGLayoutHints(kLHintsNormal| kLHintsExpandX |kLHintsExpandY));
    
    
    //Canvas creation
    fEcanvas = new TRootEmbeddedCanvas("Ecanvas",hFrameTab1_1,500,300);
    //adding of the canvas to the frame
    hFrameTab1_1->AddFrame(fEcanvas,new TGLayoutHints(kLHintsNormal |kLHintsExpandX | kLHintsExpandY ,5,5,5,5));
    
    //Creation of Vertical Frame for buttons, options and other stuff
    TGVerticalFrame *vFrame_1 = new TGVerticalFrame(hFrameTab1_1,200,100,kFixedWidth);
    
    //Add the vertical frame to the first horizontal frame of tab1
    hFrameTab1_1->AddFrame(vFrame_1,new TGLayoutHints(kLHintsExpandY,0,0,0,0));
    
    
    //Creation of the Group frame for index and total event of the chain and adding of the info line to the group
    TGGroupFrame *InfoDisplayChain = new TGGroupFrame(vFrame_1,"Event in Chain");
    vFrame_1->AddFrame(InfoDisplayChain, new TGLayoutHints(kLHintsTop | kLHintsExpandX , 5,5,3,3));
    
    //info line created and added to the respecive group frame
    fDisplayNumber=new TGLabel(InfoDisplayChain,Form("%lld of %lld",fEventIndex,fEventNumber));
    fDisplayNumber->SetTextJustify(kTextRight);
    InfoDisplayChain->AddFrame(fDisplayNumber, new TGLayoutHints(kLHintsExpandX,1,1,1,1));
    
    
    //Creation of the Group frame for index and total event of the event selected and adding of the info line to the group
    TGGroupFrame *InfoDisplaySelection = new TGGroupFrame(vFrame_1,"Event in Selection");
    vFrame_1->AddFrame(InfoDisplaySelection, new TGLayoutHints(kLHintsTop | kLHintsExpandX , 5,5,3,3));
    
    //info line created and added to the respecive group frame
    fDisplayListNumber=new TGLabel(InfoDisplaySelection,Form("%lld of %lld",fEventListIndex,fEventListNumber));
    fDisplayListNumber->SetTextJustify(kTextRight);
    InfoDisplaySelection->AddFrame(fDisplayListNumber, new TGLayoutHints(kLHintsExpandX,1,1,1,1));
    
    
    //Creation of the Group frame for the Draw Buttons
    TGGroupFrame *DrawButtonGroup = new TGGroupFrame(vFrame_1,"Draw Button");
    vFrame_1->AddFrame(DrawButtonGroup, new TGLayoutHints(kLHintsTop | kLHintsExpandX , 5,5,5,5));
    
    
    //Three different horizontal frame for the alignement
    TGHorizontalFrame *DrawButtonHoriz_1 = new TGHorizontalFrame(DrawButtonGroup);
    TGHorizontalFrame *DrawButtonHoriz_2 = new TGHorizontalFrame(DrawButtonGroup);
    TGHorizontalFrame *DrawButtonHoriz_3 = new TGHorizontalFrame(DrawButtonGroup);
    //Line for separation
    TGHorizontal3DLine *hline = new TGHorizontal3DLine(DrawButtonGroup);
    
    DrawButtonGroup->AddFrame(DrawButtonHoriz_1, new TGLayoutHints(kLHintsCenterX | kLHintsCenterY| kLHintsExpandX,0,0,5,2));
    DrawButtonGroup->AddFrame(DrawButtonHoriz_2, new TGLayoutHints(kLHintsExpandX,0,0,2,2));
    DrawButtonGroup->AddFrame(hline, new TGLayoutHints(kLHintsExpandX,4,4,5,4));
    DrawButtonGroup->AddFrame(DrawButtonHoriz_3, new TGLayoutHints(kLHintsExpandX,0,0,2,3));
    
    
    //Creation of the previous and next buttons...
    
    /*--previous--*/
    TGTextButton *previous = new TGTextButton(DrawButtonHoriz_1,"Previous");
    previous->Connect("Clicked()","TPulseViewer",this,"Previous()");
    
    
    /*--next--*/
    TGTextButton *next = new TGTextButton(DrawButtonHoriz_1,"Next");
    next->Connect("Clicked()","TPulseViewer",this,"Next()");
    
    //...and addition to the First horizontal frame
    DrawButtonHoriz_1->AddFrame(previous, new TGLayoutHints(kLHintsCenterX | kLHintsCenterY,-1,2,2,2));
    DrawButtonHoriz_1->AddFrame(next, new TGLayoutHints(kLHintsCenterX | kLHintsCenterY,2,-1,2,2));
    
    previous->ChangeOptions(kFixedWidth | kFixedHeight | kRaisedFrame);
    next->ChangeOptions(kFixedWidth| kFixedHeight | kRaisedFrame);
    previous->Resize(75,24);
    next->Resize(75,24);
    
    
    //Creation of the textentry and button for the selection of any event in the chain
    fEntryNumberIndex = new TGTextEntry(DrawButtonHoriz_2,"");
    fEntryNumberIndex->SetAlignment(kTextRight);
    fEntryNumberIndex->SetMaxLength(10);
    fEntryNumberIndex->Connect("ReturnPressed()","TPulseViewer",this,"SelectEvent()");
    
    TGTextButton *setIndex = new TGTextButton(DrawButtonHoriz_2,"Load Event");
    setIndex->Connect("Clicked()","TGTextEntry",fEntryNumberIndex,"ReturnPressed()");
    
    DrawButtonHoriz_2->AddFrame(fEntryNumberIndex, new TGLayoutHints(kLHintsCenterX | kLHintsCenterY,-1,2,2,2));
    DrawButtonHoriz_2->AddFrame(setIndex, new TGLayoutHints(kLHintsCenterX | kLHintsCenterY,2,-1,2,2));
    
    
    
    setIndex->ChangeOptions(kFixedWidth | kFixedHeight | kRaisedFrame);
    fEntryNumberIndex->ChangeOptions(kFixedWidth | kFixedHeight | kSunkenFrame);
    setIndex->Resize(75,24);
    fEntryNumberIndex->Resize(75,24);
    
    
    //Creation of the refresh button
    TGTextButton *draw = new TGTextButton(DrawButtonHoriz_3,"Refresh");
    draw->Connect("Clicked()","TPulseViewer",this,"DrawEvent()");
    
    
    DrawButtonHoriz_3->AddFrame(draw, new TGLayoutHints(kLHintsExpandX| kLHintsCenterX | kLHintsCenterY,1,1,2,2) );
    draw->ChangeOptions( kFixedHeight | kRaisedFrame);
    draw->SetHeight(24);
    
    //Creation of the button frame  and adding to the parent vertical frame
    TGGroupFrame *DrawOptionButtons = new TGGroupFrame(vFrame_1, "Draw Option");
    vFrame_1->AddFrame(DrawOptionButtons, new TGLayoutHints(kLHintsTop | kLHintsExpandX , 5,5,5,5));
    
    
    
    //Creation of the check button
    fRegularPlotButton = new TGCheckButton(DrawOptionButtons,"&Regular Plot");
    fAveragePlotButton = new TGCheckButton(DrawOptionButtons,"&Moving Average Plot");
    fRemoveBaselineButton = new TGCheckButton(DrawOptionButtons,"&Subtract Baseline");
    TGHorizontal3DLine *hline_drawButton = new TGHorizontal3DLine(DrawOptionButtons);
    fSuperImposeButton = new TGCheckButton(DrawOptionButtons,"&SuperImpose Plot");
    
    //Button added to the frame container
    DrawOptionButtons->AddFrame(fRegularPlotButton,new TGLayoutHints(kLHintsNormal,1, 1, 5, 1));
    DrawOptionButtons->AddFrame(fAveragePlotButton,new TGLayoutHints(kLHintsNormal,1, 1, 1, 1));
    DrawOptionButtons->AddFrame(fRemoveBaselineButton,new TGLayoutHints(kLHintsNormal,1, 1, 1, 1));
    DrawOptionButtons->AddFrame(hline_drawButton,new TGLayoutHints(kLHintsExpandX,4,4,5,4));
    DrawOptionButtons->AddFrame(fSuperImposeButton,new TGLayoutHints(kLHintsNormal,1, 1, 1, 1));
    
    
    
    //Connect action to the button
    fRegularPlotButton->Connect("Toggled(Bool_t)", "TPulseViewer", this,"SetPlotRegular(Bool_t)");
    fRegularPlotButton->SetState(kButtonDown);
    fRegularPlotButton->SetName("RegularPlotButton");
    
    fAveragePlotButton->Connect("Toggled(Bool_t)", "TPulseViewer", this,"SetPlotAverage(Bool_t)");
    fAveragePlotButton->SetName("AveragePlotButton");
    
    fRemoveBaselineButton->Connect("Toggled(Bool_t)", "TPulseViewer", this,"SetBaseLineOff(Bool_t)");
    fRemoveBaselineButton->SetName("RemoveBaselineButton");
    
    fSuperImposeButton->Connect("Toggled(Bool_t)", "TPulseViewer", this,"SetSuperImpose(Bool_t)");
    
    
    //Creation of the button frame  and adding to the parent vertical frame
    fDrawFitOption = new TGVButtonGroup(vFrame_1, "Fit Option");
    vFrame_1->AddFrame(fDrawFitOption, new TGLayoutHints(kLHintsTop | kLHintsExpandX , 5,5,5,5));
    TGCheckButton *ShowFit = new TGCheckButton(fDrawFitOption,"&Draw Fit");
    
    //adding the button to show the fit
    ShowFit->Connect("Toggled(Bool_t)", "TPulseViewer", this,"SetDrawFit(Bool_t)");
    fDrawFitOption->SetState(false);
    
    
    //End of the horizontal frame for the cnavas and buttons
    
    //Creation of the group frame for the cut selection and the button
    TGGroupFrame *frameCutSelection = new TGGroupFrame(fTab1,"TCut Expression",kHorizontalFrame);
    fTab1->AddFrame(frameCutSelection, new TGLayoutHints(kLHintsCenterX | kLHintsCenterX | kLHintsTop |kLHintsExpandX,5,5,4,4));

    //OLD VERSION--------------------------------------------------------------
    // fEntryCutExpression = new TGTextEntry(frameCutSelection,"");
    // fEntryCutExpression->SetMaxLength(2000);    
    // frameCutSelection->AddFrame(fEntryCutExpression, new TGLayoutHints(kLHintsExpandX,-2,5,2,2));
    // fEntryCutExpression->Connect("ReturnPressed()","TPulseViewer",this,"ApplyCut()");
    //NEW ONE
    fEntryCutExpression = new TGTextEntry(frameCutSelection);
    frameCutSelection->AddFrame(fEntryCutExpression, new TGLayoutHints(kLHintsExpandX|kLHintsExpandY,-2,5,2,2));
    fEntryCutExpression->GetCanvas()->Connect("ProcessedEvent(Event_t*)", "TPulseViewer", this, "SelectionReturnKeyPressed(Event_t*)");
    fEntryCutExpression->EnableCursorWithoutFocus(false);
    //---------------------------------------------------------------------------
    
    
    TGTextButton *apply = new TGTextButton(frameCutSelection,"Apply Cut");
    apply->Connect("Clicked()","TGTextEntry",fEntryCutExpression,"ReturnPressed()");
    frameCutSelection->AddFrame(apply, new TGLayoutHints(kLHintsRight ,5,-2,2,2));
    frameCutSelection->ChangeOptions( kFixedHeight);
    frameCutSelection->Resize(500,75);//OLD:47);
    
    //******************************************************************************************//
    //******************************************************************************************//
    //******************************************************************************************//
    
    //Second tab creation
    TGCompositeFrame *fTab2 = fTab->AddTab("Pulse Information");
    TGHorizontalFrame *hFrameTab2_1 = new TGHorizontalFrame(fTab2,600,400);
    fTab2->AddFrame(hFrameTab2_1,new TGLayoutHints(kLHintsExpandX | kLHintsExpandY,5,5,5,5));
    
    //Adding vframe for multi line label
    
    TGVerticalFrame *vFrame_2_1 = new TGVerticalFrame(hFrameTab2_1);
    //OLD:TGVerticalFrame *vFrame_2_2= new TGVerticalFrame(hFrameTab2_1);
    
    
    hFrameTab2_1->AddFrame(vFrame_2_1, new TGLayoutHints(kLHintsExpandY |kLHintsLeft,5,5,5,5));
    //OLD:hFrameTab2_1->AddFrame(vFrame_2_2, new TGLayoutHints(kLHintsExpandY |kLHintsLeft,5,5,5,5));
    
    
    vFrame_2_1->ChangeOptions(kFixedWidth);
    //OLD:vFrame_2_2->ChangeOptions(kFixedWidth);
    
    vFrame_2_1->SetWidth(380);
    //OLD:vFrame_2_2->SetWidth(380);
    
    //OLD:fLabelBranchName = new TGLabel(vFrame_2_1,"");
    //OLD:fLabelBranchValue = new TGLabel(vFrame_2_2,"");
    //OLD:fLabelBranchName->SetTextJustify(kTextLeft|kTextTop);
    //OLD:fLabelBranchValue->SetTextJustify(kTextRight|kTextTop);

    //NEW:
    fTextNameAndValue = new TGTextEdit(vFrame_2_1);
    

    vFrame_2_1->AddFrame(fLabelBranchName, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY,2,2,2,2));
    //OLD:vFrame_2_2->AddFrame(fLabelBranchValue,new TGLayoutHints(kLHintsExpandX | kLHintsExpandY,2,2,2,2));
    
    //Combo box
    fComboBox = new TGComboBox(hFrameTab2_1);
    hFrameTab2_1->AddFrame(fComboBox,new TGLayoutHints(kLHintsTop| kLHintsRight,2,2,2,2));
    fComboBox->ChangeOptions(kFixedWidth);
    fComboBox->SetWidth(120);
    fComboBox->SetHeight(20);
    fComboBox->Connect("Selected(Int_t)", "TPulseViewer", this, "SelectChain(Int_t)");
    
    
    
    //******************************************************************************************//
    //******************************************************************************************//
    //******************************************************************************************//

    //TFrame(parent,w,h);
    //TGLayoutHints(,left,right,top,bottom)
    
    //Third tab creation
    TGCompositeFrame *fTab3 = fTab->AddTab("Interactive fitter");
    TGHorizontalFrame *hFrameTab3_1 = new TGHorizontalFrame(fTab3,600,400);
    fTab3->AddFrame(hFrameTab3_1,new TGLayoutHints(kLHintsExpandX | kLHintsExpandY,0,0,0,0));

    TGVerticalFrame *vFrameTab3_2a = new TGVerticalFrame(hFrameTab3_1,800,700); //drawing and buttons
    TGCompositeFrame *vFrameTab3_2b = new TGCompositeFrame(hFrameTab3_1,350,830); //fitpanel 
    TGHorizontalFrame *hFrameTab3_2aa = new TGHorizontalFrame(vFrameTab3_2a); //drawings
    TGHorizontalFrame *hFrameTab3_2ab = new TGHorizontalFrame(vFrameTab3_2a,830,50); //group of buttons
    TGHorizontalFrame *hFrameTab3_2aba = new TGHorizontalFrame(hFrameTab3_2ab); //event selection + apply
    TGVerticalFrame *vFrameTab3_2abb = new TGVerticalFrame(hFrameTab3_2ab); //channels tgradiobuttons

    //adding to parent object
    hFrameTab3_1->AddFrame(vFrameTab3_2a, new TGLayoutHints(kLHintsExpandX|kLHintsExpandY, 0,0,0,0));
    hFrameTab3_1->AddFrame(vFrameTab3_2b, new TGLayoutHints(kLHintsExpandY, 0,0,0,0));
    vFrameTab3_2a->AddFrame(hFrameTab3_2aa, new TGLayoutHints(kLHintsExpandX|kLHintsExpandY, 0,0,0,0));
    vFrameTab3_2a->AddFrame(hFrameTab3_2ab);
    hFrameTab3_2ab->AddFrame(hFrameTab3_2aba);
    hFrameTab3_2ab->AddFrame(vFrameTab3_2abb);

    
    vFrameTab3_2b->SetEditable(kTRUE);
    fFitEditor_tab3 = new TFitEditor(NULL,NULL);
    vFrameTab3_2b->SetEditable(kFALSE);

    fFitEditor_tab3->Connect("ProcessedEvent(Event_t*)","TPulseViewer",this,"ShowResiduals(Event_t*)");

    // Create canvas widget
    fEcanvas_tab3 = new TRootEmbeddedCanvas("Ecanvas",hFrameTab3_2aa,800,700);
    hFrameTab3_2aa->AddFrame(fEcanvas_tab3,new TGLayoutHints(kLHintsExpandX|kLHintsExpandY, 5,5,5,5));

    
    //---- Create buttons widget ----------------------------------------------------------------------
    //event selection
    fEvNumLabel_tab3=new TGLabel(hFrameTab3_2aba,"Event number: ");
    fEvNumLabel_tab3->SetTextJustify(kTextCenterX);
    hFrameTab3_2aba->AddFrame(fEvNumLabel_tab3,new TGLayoutHints(kLHintsCenterY,50,5,60,10));
    fEventNumber_tab3 = new TGTextEntry(hFrameTab3_2aba,"");
    fEventNumber_tab3->SetAlignment(kTextCenterX);
    fEventNumber_tab3->SetMaxLength(30);
    fEventNumber_tab3->Connect("ReturnPressed()","TPulseViewer",this,"SelectEvent_tab3()");
    hFrameTab3_2aba->AddFrame(fEventNumber_tab3,new TGLayoutHints(kLHintsCenterY,5,30,60,10));
    fEventNumber_tab3->ChangeOptions(kFixedWidth | kFixedHeight | kSunkenFrame);
    fEventNumber_tab3->Resize(80,30);

    TGTextButton *apply_tab3 = new TGTextButton(hFrameTab3_2aba,"&Apply");
    apply_tab3->Connect("Clicked()","TGTextEntry",fEventNumber_tab3,"ReturnPressed()");
    apply_tab3->ChangeOptions(kFixedWidth | kFixedHeight | kRaisedFrame);
    apply_tab3->Resize(80,30);
    hFrameTab3_2aba->AddFrame(apply_tab3,new TGLayoutHints(kLHintsCenterY,10,0,60,10));

    //channel selection
    fChannelButtons_tab3 = new TGButtonGroup(vFrameTab3_2abb,"Channel signal");
    new TGRadioButton(fChannelButtons_tab3,"&Phonon channel",0);
    new TGRadioButton(fChannelButtons_tab3,"&Light channel",1);
    new TGRadioButton(fChannelButtons_tab3,"&iStick channel",2);
    fChannelButtons_tab3->SetButton(0);
    fChannelButtons_tab3->Connect("Pressed(Int_t)","TPulseViewer",this,"SelectChannel_gradio(Int_t)");
    vFrameTab3_2abb->AddFrame(fChannelButtons_tab3, new TGLayoutHints(kLHintsRight | kLHintsTop, 200,0,10,10));
    

    fMain->Connect("CloseWindow()", "TPulseViewer", this, "~TPulseViewer()");
    fMain->AddFrame(fTab, new TGLayoutHints(kLHintsNormal| kLHintsExpandX |kLHintsExpandY));

    // Set a name to the main frame
    fMain->SetWindowName("Pulse Viewer CRESST - 3");
    // Map all subwindows of main frame
    fMain->MapSubwindows();
    
    // Initialize the layout algorithm
    fMain->Resize(1200,900);
    
    // Map main frame
    fMain->MapWindow();
    
}

TPulseViewer::TPulseViewer()
{
    Init();
    
}

TPulseViewer::TPulseViewer(TChain *input_chain)
{
    Init();
    fMainChain=input_chain;
    
    fInfoTree=fMainChain;
    
    fMainChain->Draw(">>list",fTCutExpression);
    fEventList=(TEventList*)gROOT->FindObject("list");
    fEventIndex = 0;
    fEventListIndex = 0;
    fEventNumber = fMainChain->GetEntries();
    
    
    if(fEventList!=nullptr)
        fEventListNumber = fEventList->GetN();
    else
        fEventListNumber = 0;
    
    
    fDisplayNumber->ChangeText(Form("%lld of %lld",fEventIndex,fEventNumber));
    fDisplayListNumber->ChangeText(Form("%lld of %lld",fEventListIndex,fEventListNumber));
    
    fFriendListChain = fMainChain->GetListOfFriends();
    
    
    fComboBox->AddEntry(fMainChain->GetName(),0);
    
    if(fFriendListChain!=nullptr)
        for(int i=0;i<fFriendListChain->GetEntries();i++)
        {
            fComboBox->AddEntry(fFriendListChain->At(i)->GetName(),i+1);
            if(strcmp(fFriendListChain->At(i)->GetName(),"ParticleFit")==0 || strcmp(fFriendListChain->At(i)->GetName(),"TestPulseFit")==0 )
            {
                fDrawFitOption->SetState(true);
                TGString s=new TGString();
                if(strcmp(fFriendListChain->At(i)->GetName(),"ParticleFit")==0)
                    fFitTreeParticle=((TFriendElement *)fFriendListChain->FindObject( "ParticleFit" ))->GetTree();
                if(strcmp(fFriendListChain->At(i)->GetName(),"TestPulseFit")==0)
                    fFitTreeTP=((TFriendElement *)fFriendListChain->FindObject( "TestPulseFit" ))->GetTree();
                
            }
            if(strcmp(fFriendListChain->At(i)->GetName(),"PulseData")==0)
                fPulseChain=((TFriendElement *)fFriendListChain->FindObject("PulseData"))->GetTree();
            
        }
    
    
    if(fPulseChain==nullptr)
        if(strcmp(fMainChain->GetName(),"PulseData")==0)
	  fPulseChain=fMainChain;

    //NEW: ----------------------------------------------------------------
    if(fFitTreeParticle==nullptr) //<------------------ form here
      if(strcmp(fMainChain->GetName(),"ParticleFit")==0)
	fFitTreeParticle=fMainChain;
    if(fFitTreeTP==nullptr)
      if(strcmp(fMainChain->GetName(),"TestPulseFit")==0)
	fFitTreeTP=fMainChain;  //<------------------ to here
    //---------------------------------------------------------------------
    
    
    fComboBox->Select(0);

    //NEW:
    DrawEvent();
    
}








//Getter method

Long64_t TPulseViewer::GetEventIndex(){
    return fEventIndex;
}

Long64_t TPulseViewer::GetEventListIndex(){
    return fEventListIndex;
}

Long64_t TPulseViewer::GetEventNumber(){
    return fEventNumber;
}

Long64_t TPulseViewer::GetEventListNumber(){
    return fEventListNumber;
}

TCut  TPulseViewer::GetCutExpression(){
    return fTCutExpression;
}


//Setter method

void TPulseViewer::SetEventIndex(Long64_t i){
    
    if(i>=0 && i< fEventNumber)
        fEventIndex=i;
    
}

void TPulseViewer::SetEventListIndex(Long64_t i){
    if(i>=0 && i<fEventListNumber)
        fEventListIndex=i;
    
}

void TPulseViewer::SetCutExpression(TCut cut_expression){
    fTCutExpression=cut_expression;
}

void TPulseViewer::SetPlotAverage(Bool_t on)
{
    fCheckAveragePlot=on;
}

void TPulseViewer::SetPlotRegular(Bool_t on)
{
    fCheckRegularPlot=on;
}

void TPulseViewer::SetDrawFit(Bool_t on)
{
    fCheckDrawFit=on;
    
    if(on)
    {
        fCheckSuperImpose=!on;
        fCheckRemoveBaseLine=!on;
        
    }
    
    fSuperImposeButton->SetEnabled(!on);
    fRemoveBaselineButton->SetEnabled(!on);
    
    
}

void TPulseViewer::SetBaseLineOff(Bool_t on)
{
    fCheckRemoveBaseLine=on;
}


void TPulseViewer::SetSuperImpose(Bool_t on)
{
        if(on)
        {
            fAveragePlotButton->SetDown(!on);
            fRegularPlotButton->SetDown(on);
            fRemoveBaselineButton->SetDown(on);
            
            fCheckRegularPlot=on;
            fCheckAveragePlot=!on;
            fCheckRemoveBaseLine=on;
            fCheckDrawFit=!on;
        }
    
    fRegularPlotButton->SetEnabled(!on);
    fAveragePlotButton->SetEnabled(!on);
    fRemoveBaselineButton->SetEnabled(!on);
    
    fDrawFitOption->SetState(!on);
    
    fCheckSuperImpose=on;
    

}


void TPulseViewer::SelectionReturnKeyPressed(Event_t *event)    //<------------------ new method
{
    
    if (event->fType == kGKeyPress)
    {
        UInt_t keysym;
        char input[10];
        gVirtualX->LookupString(event, input, sizeof(input), keysym);
        
        if((EKeySym)keysym == kKey_Return || (EKeySym)keysym == kKey_Enter)
        {
            fEntryCutExpression->DelChar();
            ApplyCut();
        }
    }
    
}




//Apply cut
void TPulseViewer::ApplyCut()
{
    
    fTCutExpression =fEntryCutExpression->GetText();
    
    if(fPulseChain==nullptr)
    {
        printf("No Pulse Chain, impossible to draw pulse\n");
        return;
    }
    
    
    Long64_t check=fMainChain->Draw(">>list",Form("%s",fTCutExpression.GetTitle()));
    fEventList=(TEventList*)gDirectory->FindObject("list");
    
    gDirectory->Clear();
    
    if(check>=0)
    {
        fEventListIndex = 0;
        fEntryCutExpression->SetBackgroundColor(0x00ff00);
        fEventListNumber = fEventList->GetN();
    }
    else
    {
        fEntryCutExpression->SetBackgroundColor(0xff0000);
        return;
    }

    
    DrawEvent();
    return;
    
}


//Draw method

void TPulseViewer::SelectEvent()
{
    TString s = fEntryNumberIndex->GetText();
        
    if(s.IsDigit())
        LoadEvent(s.Atoll());
    return;
}

void TPulseViewer::LoadEvent(Long64_t idx)
{
    if(idx>-1 && idx<fEventListNumber)
        fEventListIndex=idx;
    
    else
        return;
    
    DrawEvent();
    return;
}

void TPulseViewer::Previous()
{
    if(fEventListIndex-1<0)
        fEventListIndex=fEventListNumber-1;
    else
        fEventListIndex--;
    
    DrawEvent();
    return;
}


void TPulseViewer::Next()
{
    
    if(fEventListIndex+1>=fEventListNumber)
        fEventListIndex=0;
    else
        fEventListIndex++;
    
    DrawEvent();
    return;
}


void TPulseViewer::DrawEvent()
{
    
    if(fPulseChain==nullptr)
    {
        printf("No Pulse Chain, impossible to draw pulse\n");
        return;
    }
    
   
    
    if(fEventListNumber==0)
    {
        printf("No Event in the Cut\n");
        return;
    }
    
    fEventIndex=fEventList->GetEntry(fEventListIndex);
    fDisplayNumber->SetText(Form("%lld of %lld",fEventIndex,fEventNumber));
    fDisplayListNumber->SetText(Form("%lld of %lld",fEventListIndex,fEventListNumber));
    
    
    //for pulse drawing
    std::vector<ULong64_t> *RecordNumber = {nullptr};
    std::vector<Int_t> *ChannelID = {nullptr};
    std::vector<Double_t> *TestPulseAmplitude = {nullptr};
    std::string *FileName = {nullptr};
    std::vector<Double_t> *BaseLineOffset = {nullptr};
    
    UInt_t ModuleID = 0;
    
    
    
    fPulseChain->SetBranchAddress("RecordNumber",&RecordNumber);
    fPulseChain->SetBranchAddress("FileName",&FileName);
    fPulseChain->SetBranchAddress("ChannelID",&ChannelID);
    fPulseChain->SetBranchAddress("TestPulseAmplitude",&TestPulseAmplitude);
    fPulseChain->SetBranchAddress("BaseLineOffset",&BaseLineOffset);
    fPulseChain->SetBranchAddress("ModuleID",&ModuleID);
    
    
    
    fPulseChain->GetEntry(fEventIndex);
    
    short volt=0;
    int skip_int=0;
    struct tm * timeinfo;
    char buffer[80]={0};
    time_t time=0;
    int time_us=0;
    
    Int_t ChannelNumber = RecordNumber->size();
    
    std::vector<TGraph*>    GraphVector;
    std::vector<TGraph*>    GraphVectorAverage;
    std::vector<TF1*>       GraphFit;
    
    std::vector<TLegend*>   LegendVector;
    
    
    //for fit drawing
    
    std::vector< vector<Double_t>> *Scales = {nullptr};
    std::vector<Int_t> *Badness = {nullptr};
    
    
    GraphVector.resize(ChannelNumber);
    GraphVectorAverage.resize(ChannelNumber);
    GraphFit.resize(ChannelNumber);
    
    LegendVector.resize(ChannelNumber);
    
    
    FILE *file_rdt=fopen(FileName[0].c_str(),"rb");
    TString FileNamePar(FileName[0].c_str());
    FileNamePar.Replace(FileNamePar.Index(".rdt",4,0,TString::kExact),4,".par");
    
    FILE *file_par=fopen(FileNamePar.Data(),"r");
    
    GetInfoParFile(file_par);
        
    for(Int_t k=0;k<ChannelNumber;k++)
    {
        
        if(!fCheckRemoveBaseLine)
            BaseLineOffset->at(k)=0;

            
        GraphVector[k]=new TGraph();
        GraphVector[k]->SetEditable(false);
        
        GraphVectorAverage[k]=new TGraph();
        GraphVectorAverage[k]->SetEditable(false);
        
        fseek(file_rdt,fRecordLenght*RecordNumber->at(k),SEEK_SET);
        
        for(Int_t i=0;i<4;i++)
            fread(&skip_int,sizeof(int),1,file_rdt);
        
        
        fread(&time,sizeof(int),1,file_rdt);
        timeinfo = gmtime(&time);
        
        fread(&time_us,sizeof(int),1,file_rdt);
        
        strftime(buffer,80,"%a %d %b %G %T",timeinfo);
        
        
        if(!fCheckSuperImpose)
        GraphVector[k]->SetTitle(Form("Event %lld, ChID %d , Time: %s.%3d (UTC);Time [ms];Amplitude [V]   ",fEventIndex,ChannelID->at(k), buffer, time_us/1000 ));
        else
            GraphVector[k]->SetTitle(Form("Event %lld, ModuleID %d,Time: %s.%3d (UTC);Time [ms];Amplitude [V]   ",fEventIndex, ModuleID, buffer, time_us/1000 ));
        GraphVectorAverage[k]->SetTitle(Form("Event %lld, ChID %d, Time: %s.%3d (UTC);Time [ms];Amplitude [V]   ",fEventIndex,ChannelID->at(k), buffer, time_us/1000));
        
        for(Int_t i=0;i<1;i++)
            fread(&skip_int,sizeof(int),1,file_rdt);
        
        for(Int_t i=0;i<fUInt;i++)
            fread(&skip_int,sizeof(unsigned int ),1,file_rdt);
        
        for(Int_t i=0;i<fReals;i++)
            fread(&skip_int,sizeof(float ),1,file_rdt);
        
        
        
        for(Int_t j=0;j<fSample;j++)
        {
            fread(&volt,sizeof(short),1,file_rdt);
            GraphVector[k]->SetPoint(j,j*fTimeBase,-10.  +double( volt +32768 )*20./65536. - BaseLineOffset->at(k)) ;
        }
        
        
        Double_t mean=0;
        Double_t x,y;
        GraphVector[k]->GetPoint(0,x,y);
	
        
        mean=y * fNumberAverage/2;
        for(Int_t j=0;j<fNumberAverage/2;j++)
        {
            GraphVector[k]->GetPoint(j,x,y);
            mean+=y;
        }
        
        mean=mean/fNumberAverage;
        
        GraphVectorAverage[k]->SetPoint(0,0,mean);
        
        
        
        for(Int_t j=1;j<fSample;j++)
        {
            if(j<fNumberAverage/2)
            {
                GraphVector[k]->GetPoint(j+fNumberAverage/2 -1 ,x,y);
                mean+=y/fNumberAverage;
                GraphVector[k]->GetPoint(0,x,y);
                mean+=-y/fNumberAverage;
                GraphVectorAverage[k]->SetPoint(j,j*fTimeBase,mean);
            }
            
            if(j>=fNumberAverage/2 &&  j<=(fSample  - 1 - fNumberAverage/2  + 1   ))
            {
                GraphVector[k]->GetPoint(j+fNumberAverage/2 -1,x,y);
                mean+=y/fNumberAverage;
                GraphVector[k]->GetPoint(j-fNumberAverage/2 ,x,y);
                mean+=-y/fNumberAverage;
                GraphVectorAverage[k]->SetPoint(j,j*fTimeBase,mean);
            }
                
            if(j>(fSample - 1 - fNumberAverage/2 +1 ))
            {
                GraphVector[k]->GetPoint(fSample-1,x,y);
                mean+=y/fNumberAverage;
                GraphVector[k]->GetPoint(j-fNumberAverage/2,x,y);
                mean+= -y/fNumberAverage;
                GraphVectorAverage[k]->SetPoint(j,j*fTimeBase,mean);
            }
               
        }
        
    }
    
    TTree *auxTree=nullptr;
    
    if(TestPulseAmplitude->at(0)==0. && fFitTreeParticle!=nullptr)
        auxTree =  fFitTreeParticle;
    if(TestPulseAmplitude->at(0)>0.  && fFitTreeTP!=nullptr)
        auxTree = fFitTreeTP;
    
    if(fCheckDrawFit && auxTree!=nullptr)
    {
        
        auxTree->SetBranchAddress("Badness",&Badness);
        auxTree->SetBranchAddress("Scales",&Scales);
        auxTree->GetEntry(fEventIndex);
        
        for(Int_t k=0;k<ChannelNumber;k++)
        {
            if(Badness->at(k)==0)
            {
                GraphFit[k]=new TF1(Form("baseline_%d",k),Form("pol%lu",Scales->at(k).size()-2),0,fTimeBase*fSample);
                
                for(Int_t kk=1;kk<Scales->at(k).size();kk++)
                    GraphFit[k]->SetParameter(kk-1,Scales->at(k).at(kk));
            }
        }
    }
    
    
    fCanvas = fEcanvas->GetCanvas();
    fCanvas->cd();
    fCanvas->Clear();
    
    
    Double_t Xmin,Xmax;
    
    if(!fCheckSuperImpose)
    {
    if(ChannelNumber==1)
        fCanvas->Divide(1,1);
    
    if(ChannelNumber==2)
        fCanvas->Divide(2,1);
    
    if(ChannelNumber==3)
        fCanvas->Divide(2,2);
    }
    
    if(fCheckSuperImpose)
        fCanvas->Divide(1,1);
    
    
        
    for(Int_t k=0;k<ChannelNumber;k++)
    {
      
        if(!fCheckSuperImpose)
            fCanvas->cd(k+1);
        
        Xmin = GraphVector[k]->GetYaxis()->GetXmin();
        Xmax = GraphVector[k]->GetYaxis()->GetXmax();
        GraphVector[k]->GetYaxis()->SetRangeUser(Xmin,(Xmax-Xmin)*1.25 + Xmin);
        GraphVector[k]->GetYaxis()->SetTitleOffset(1.35);
        GraphVector[k]->GetXaxis()->SetTitleSize(0.036);
        GraphVector[k]->GetYaxis()->SetTitleSize(0.036);
        GraphVector[k]->GetXaxis()->SetLabelSize(0.034);
        GraphVector[k]->GetYaxis()->SetLabelSize(0.034);
        GraphVector[k]->GetXaxis()->SetRangeUser(0,fTimeBase*fSample);
        
        GraphVectorAverage[k]->GetYaxis()->SetRangeUser(Xmin,(Xmax-Xmin)*1.25 + Xmin);
        GraphVectorAverage[k]->GetYaxis()->SetTitleOffset(1.35);
        GraphVectorAverage[k]->GetXaxis()->SetTitleSize(0.036);
        GraphVectorAverage[k]->GetYaxis()->SetTitleSize(0.036);
        GraphVectorAverage[k]->GetYaxis()->SetLabelSize(0.034);
        GraphVectorAverage[k]->GetXaxis()->SetLabelSize(0.034);
        GraphVectorAverage[k]->GetXaxis()->SetRangeUser(0,fTimeBase*fSample);
        
        GraphVector[k]->SetFillColor(kWhite);
        GraphVectorAverage[k]->SetLineColor(kAzure+7);
        GraphVectorAverage[k]->SetFillColor(kWhite);
        
        
        
        
        if(auxTree!=nullptr)
            if(fCheckDrawFit && Badness->at(k)==0)
            {
                GraphFit[k]->SetLineColor(kRed);
                GraphFit[k]->SetFillColor(kWhite);
            }
        
        
        
        if(!fCheckSuperImpose)
        {
	  fCanvas->cd(k+1);//NEW: va levato?!
            
            if(fCheckRegularPlot)
	      GraphVector[k]->Draw("AL");
            
            if(fCheckAveragePlot && fCheckRegularPlot)
	      GraphVectorAverage[k]->Draw("LC");
            
            if(fCheckAveragePlot && !fCheckRegularPlot)
	      GraphVectorAverage[k]->Draw("AL");
            
            if(auxTree!=nullptr)
                if(fCheckDrawFit && Badness->at(k)==0 )
		  GraphFit[k]->Draw("C");
            
            
            if(fCheckAveragePlot|| fCheckRegularPlot)
                LegendVector[k]=new TLegend(0.65,0.78,0.93,0.93);
            if(fCheckRegularPlot)
                LegendVector[k]->AddEntry(GraphVector[k],Form("Regular Plot"),"l");
            if(fCheckAveragePlot)
                LegendVector[k]->AddEntry(GraphVectorAverage[k],Form("Average Plot"),"l");
            if(auxTree!=nullptr)
                if(fCheckDrawFit && Badness->at(k)==0 )
                    LegendVector[k]->AddEntry(GraphFit[k],Form("Fit"),"l");
            
            if(fCheckAveragePlot|| fCheckRegularPlot)
	      LegendVector[k]->Draw("same");
            
        }
        
    }
    
    
    if(fCheckSuperImpose)
    {
        if(ChannelNumber==2)
        {
            GraphVector[1]->SetLineColor(kRed);
            GraphVector[1]->SetFillColor(kWhite);        
        }
        
        LegendVector[0]=new TLegend(0.65,0.78,0.93,0.93);
        
        if(ChannelNumber==3)
        {
            GraphVector[1]->SetLineColor(kRed);
            GraphVector[1]->SetFillColor(kWhite);
            GraphVector[2]->SetLineColor(kSpring+7);
            GraphVector[2]->SetFillColor(kWhite);
        }
        
        
        for(Int_t k=0;k<ChannelNumber;k++)
        {
            
            if(k==0)
                GraphVector[k]->Draw("AL");
            if(k>0)
                GraphVector[k]->Draw("LC");
            
            if(k==0)
                LegendVector[0]->AddEntry(GraphVector[k],"Phonon");
            
            if(k==1)
                LegendVector[0]->AddEntry(GraphVector[k],"Light");
            
            if(k==2)
                LegendVector[0]->AddEntry(GraphVector[k],"Stick");
            
        }
        
        LegendVector[0]->Draw("same");
    }
    
    
    if(auxTree!=nullptr)
        auxTree->ResetBranchAddresses();
    
    fPulseChain->ResetBranchAddresses();
    
    fCanvas->Modified();//NEW:va levato?!
    fCanvas->Update();
    fclose(file_rdt);
    fclose(file_par);
    
    
}



//--------- tab 3 ------------------------------------------------------------

UInt_t TPulseViewer::GetChannelNumber_tab3(){
    return fEventIndex;
}

void TPulseViewer::SetChannelNumber_tab3(UInt_t i){
        fChannel_tab3=i;
    
}


void TPulseViewer::SelectChannel_tab3()
{
  // TString s = fEventChannel_tab3->GetText();

  // if(s.IsDigit()){
  //   //    if(s.Atoll()<0 || s.Atoll()>2) casellina rossa
  //   if(s.Atoll()<3)fChannel_tab3=s.Atoll();
  // }
  // return;
}


void TPulseViewer::SelectChannel_gradio(Int_t id)
{
  fChannel_tab3=id;
  //SelectEvent_tab3();
  DrawSinglePulse();
  //return;
}


void TPulseViewer::SelectEvent_tab3()
{
  TString s = fEventNumber_tab3->GetText();
  
  if(s.IsDigit())
    LoadEvent_tab3(s.Atoll());
  //return;
}

void TPulseViewer::Apply_tab3(){
  // SelectEvent_tab3();
  
  // SelectChannel_tab3();
  // return;
};

void TPulseViewer::LoadEvent_tab3(Long64_t idx)
{    
    if(idx>-1 && idx<fEventNumber)
      fEventIndex_tab3=idx;
    else
      return;


    if(fPulseChain==nullptr)
      {
        printf("No Pulse Chain, impossible to draw pulse\n");
        return;
      }
    
    fPulseChain_tab3=fPulseChain;
    
    
    std::vector<Int_t> *ChannelID = {nullptr};
    fPulseChain_tab3->SetBranchAddress("ChannelID",&ChannelID);
    fPulseChain_tab3->GetEntry(fEventIndex_tab3);
    
    Int_t ChannelNumber = ChannelID->size();

    fChannelButtons_tab3->SetState(true);

    
    if(ChannelNumber==2){//only phonon+light
      ((TGButton*)fChannelButtons_tab3->Find(2))->SetState(kButtonDisabled);

    }
    
    if(ChannelNumber==1){//only phonon
      ((TGButton*)fChannelButtons_tab3->Find(2))->SetState(kButtonDisabled);
      ((TGButton*)fChannelButtons_tab3->Find(1))->SetState(kButtonDisabled);

      
    }
    
    fChannelButtons_tab3->SetButton(0);    
    DrawSinglePulse();
    //return;
}



void TPulseViewer::DrawSinglePulse()
{    
    if(fPulseChain==nullptr)
    {
        printf("No Pulse Chain, impossible to draw pulse\n");
        return;
    }

    fPulseChain_tab3=fPulseChain;    
    
    //for pulse drawing
    std::vector<ULong64_t> *RecordNumber = {nullptr};
    std::vector<Int_t> *ChannelID = {nullptr};
    std::vector<Double_t> *TestPulseAmplitude = {nullptr};
    std::string *FileName = {nullptr};
    std::vector<Double_t> *BaseLineOffset = {nullptr};
    
    UInt_t ModuleID = 0;   
    
    fPulseChain_tab3->SetBranchAddress("RecordNumber",&RecordNumber);
    fPulseChain_tab3->SetBranchAddress("FileName",&FileName);
    fPulseChain_tab3->SetBranchAddress("ChannelID",&ChannelID);
    fPulseChain_tab3->SetBranchAddress("TestPulseAmplitude",&TestPulseAmplitude);
    fPulseChain_tab3->SetBranchAddress("BaseLineOffset",&BaseLineOffset);
    fPulseChain_tab3->SetBranchAddress("ModuleID",&ModuleID);

    
    fPulseChain_tab3->GetEntry(fEventIndex_tab3);
    
    short volt=0;
    int skip_int=0;
    struct tm * timeinfo;
    char buffer[80]={0};
    time_t time=0;
    int time_us=0;
    
    Int_t ChannelNumber = RecordNumber->size();
       
    GraphVector_tab3.resize(ChannelNumber);
    
    FILE *file_rdt=fopen(FileName[0].c_str(),"rb");
    TString FileNamePar(FileName[0].c_str());
    FileNamePar.Replace(FileNamePar.Index(".rdt",4,0,TString::kExact),4,".par");
    
    FILE *file_par=fopen(FileNamePar.Data(),"r");
    
    GetInfoParFile(file_par);
        
    
    for(Int_t k=0;k<ChannelNumber;k++)
    {
      
         GraphVector_tab3[k]=new TGraph();
         GraphVector_tab3[k]->SetEditable(false);
               
        fseek(file_rdt,fRecordLenght*RecordNumber->at(k),SEEK_SET);
        
        for(Int_t i=0;i<4;i++)
            fread(&skip_int,sizeof(int),1,file_rdt);
        
        
        fread(&time,sizeof(int),1,file_rdt);
        timeinfo = gmtime(&time);
        
        fread(&time_us,sizeof(int),1,file_rdt);
        
        strftime(buffer,80,"%a %d %b %G %T",timeinfo);  
      
         GraphVector_tab3[k]->SetTitle(Form("Event %lld, ChID %d , Time: %s.%3d (UTC);Time [ms];Amplitude [V]   ",fEventIndex_tab3,ChannelID->at(k), buffer, time_us/1000 ));
      

        
        for(Int_t i=0;i<1;i++)
            fread(&skip_int,sizeof(int),1,file_rdt);
        
        for(Int_t i=0;i<fUInt;i++)
            fread(&skip_int,sizeof(unsigned int ),1,file_rdt);
        
        for(Int_t i=0;i<fReals;i++)
            fread(&skip_int,sizeof(float ),1,file_rdt);
        
        
        
        for(Int_t j=0;j<fSample;j++)
        {
            fread(&volt,sizeof(short),1,file_rdt);
            GraphVector_tab3[k]->SetPoint(j,j*fTimeBase,-10.  +double( volt +32768 )*20./65536. - BaseLineOffset->at(k)) ;
        }
        
    }        
    
    fCanvas_tab3=fEcanvas_tab3->GetCanvas();
    pad1_tab3 = new TPad("pad1_tab3", "pad1_tab3", 0, 0.3, 1, 1.0);
    pad2_tab3 = new TPad("pad2_tab3", "pad2_tab3", 0, 0.001, 1, 0.3);
    fCanvas_tab3->cd();
    fCanvas_tab3->Clear();
    
    Double_t Xmin,Xmax;
            
    for(Int_t k=0;k<ChannelNumber;k++)
    {        
        Xmin = GraphVector_tab3[k]->GetYaxis()->GetXmin();
        Xmax = GraphVector_tab3[k]->GetYaxis()->GetXmax();
        GraphVector_tab3[k]->GetYaxis()->SetRangeUser(Xmin,(Xmax-Xmin)*1.25 + Xmin);
        GraphVector_tab3[k]->GetXaxis()->SetTitleOffset(0.95);
        GraphVector_tab3[k]->GetYaxis()->SetTitleOffset(1.05);
        GraphVector_tab3[k]->GetXaxis()->SetTitleSize(0.045);
        GraphVector_tab3[k]->GetYaxis()->SetTitleSize(0.045);
        GraphVector_tab3[k]->GetXaxis()->SetLabelSize(0.037);
        GraphVector_tab3[k]->GetYaxis()->SetLabelSize(0.037);
        GraphVector_tab3[k]->GetXaxis()->SetRangeUser(0,fTimeBase*fSample);
                
        GraphVector_tab3[k]->SetFillColor(kWhite);
    }

    pad1_tab3->SetTicks();
    pad1_tab3->SetRightMargin(0.03);
    pad1_tab3->Draw();             // Draw the upper pad: pad1
    pad1_tab3->cd();               // pad1 becomes the current pad
    GraphVector_tab3[fChannel_tab3]->Draw("AL");
    fCanvas_tab3->cd();          // Go back to the main canvas before defining pad2

    fFitEditor_tab3->SetFitObject((TVirtualPad*)pad1_tab3,GraphVector_tab3[fChannel_tab3],kButton1Down);
   
    fPulseChain_tab3->ResetBranchAddresses();
    
    fCanvas_tab3->Update();
    fclose(file_rdt);
    fclose(file_par);

    //needed by ShowResiduals
    Nfitfun2 = 0;

    return;
}

void TPulseViewer::ShowResiduals(Event_t *ev)
{

  if(ev->fType==14){

      Int_t nFunction = GraphVector_tab3[fChannel_tab3]->GetListOfFunctions()->GetEntries();

      if(nFunction>0){
		
	Int_t LastUsedFunction = nFunction-1;
	
	TF1 *f_fitted_temp = (TF1*)GraphVector_tab3[fChannel_tab3]->GetListOfFunctions()->At(GraphVector_tab3[fChannel_tab3]->GetListOfFunctions()->GetEntries()-1);
	TF1 *f_fitted = new TF1(*f_fitted_temp);
	

	Nfitfun1 = fFitEditor_tab3->GetListOfFittingFunctions()->GetEntries();
	if(Nfitfun1!=Nfitfun2){
	  cout << endl;
	  cout << "#######################################################################################" << endl;
	  fFitEditor_tab3->GetListOfFittingFunctions()->At(Nfitfun1-1)->Print();
	  cout << "#######################################################################################" << endl;
	  cout << endl;
	  Nfitfun2=Nfitfun1;
	}
	

	TGraph *residuals = new TGraph(fSample);
	Double_t XmaxRange,XminRange;
	f_fitted->GetRange(XminRange,XmaxRange);

	
	for(Int_t i=0;i<fSample;i++){
	  residuals->SetPoint(i,i*fTimeBase,GraphVector_tab3[fChannel_tab3]->Eval(i*fTimeBase)-f_fitted->Eval(i*fTimeBase));
	  if(i*fTimeBase<XminRange || i*fTimeBase>XmaxRange)residuals->SetPoint(i,i*fTimeBase,0.);
	}
       
	fCanvas_tab3->cd();
	pad2_tab3->SetPad(0,0.001,1,0.37);//xlow,ylow,xup,yup
	pad2_tab3->SetTicks();
	pad2_tab3->SetTopMargin(0);
	pad2_tab3->SetBottomMargin(0.2);
	pad2_tab3->SetRightMargin(0.03);
	pad2_tab3->SetLeftMargin(0.1);
	pad2_tab3->Draw();
	pad2_tab3->cd();

	
	Double_t Xmin,Xmax;
	Xmin = residuals->GetYaxis()->GetXmin();
        Xmax = residuals->GetYaxis()->GetXmax();
	residuals->SetTitle("");
	residuals->GetXaxis()->SetTickLength(0.06);
	residuals->GetYaxis()->SetTickLength(0.03);
	residuals->GetXaxis()->SetTitle("Time [ms]");
	residuals->GetYaxis()->SetTitle("Data - Fit [V]");
	residuals->GetYaxis()->CenterTitle();
        residuals->GetXaxis()->SetTitleSize(0.08);
        residuals->GetYaxis()->SetTitleSize(0.09);
        residuals->GetXaxis()->SetTitleOffset(0.95);
        residuals->GetYaxis()->SetTitleOffset(0.53);
        residuals->GetXaxis()->SetLabelSize(0.07);
        residuals->GetYaxis()->SetLabelSize(0.07);
        residuals->GetXaxis()->SetRangeUser(0,fTimeBase*fSample);
	residuals->SetFillColor(kWhite);
	residuals->Draw("AL+");
	residuals->SetEditable(kFALSE);
	TF1 *zeroLine = new TF1("zeroLine","0",0,fTimeBase*fSample);
	zeroLine->SetLineWidth(1);
	zeroLine->SetLineColor(kRed);
	zeroLine->Draw("same");
	pad2_tab3->Update();
	
      }
      
    }
}

//----------------------------------------------------------------------------


//Info method
void TPulseViewer::SelectChain(Int_t PulseIdx)
{
    fComboBox->Select(PulseIdx);
    
    if(PulseIdx!=0)
        fInfoTree=((TFriendElement *)fFriendListChain->FindObject( fComboBox->GetSelectedEntry()->GetTitle() ))->GetTree();
    else
        fInfoTree=fMainChain;
        
    InfoEvent();
    
}

void TPulseViewer::InfoEvent()
{
    //NEW:
    fTextNameAndValue->Clear();
    
    //OLD:TGString s_name=new TGString();
    TGString s_value=new TGString();
    Int_t scan_field = fInfoTree->GetScanField();
    char buffer[80];
    tmpnam (buffer);
    FILE *pFile;
    
    char buf[20];
    int saved_stdout = dup(1);
    
    freopen("/dev/null", "w", stdout);
    
    sprintf(buf, "/dev/fd/%d", saved_stdout);
    
    TTreePlayer *player= (TTreePlayer*)fInfoTree->GetPlayer();
    ((TTreePlayer*)(fInfoTree->GetPlayer()))->SetScanRedirect(true);
    for(Int_t i=0;i<fInfoTree->GetListOfBranches()->GetEntries();i++)
    {
        //OLD: ---------------------------------------------------------------
        // s_name.Append(fInfoTree->GetListOfBranches()->At(i)->GetName());
        // s_name.Append("\n");
        //NEW:
        s_value.Append(fInfoTree->GetListOfBranches()->At(i)->GetName());
        s_value.Append("\n");
	//-------------------------------------------------------------------
        
        TString aux_string;
        
        pFile=fopen(buffer,"w");
        
        ((TTreePlayer*)(fInfoTree->GetPlayer()))->SetScanFileName(buffer);

	//OLD: -------------------------------------------------------------------------
        ((TTreePlayer*)(fInfoTree->GetPlayer()))->Scan(fInfoTree->GetListOfBranches()->At(i)->GetName(),"","colsize=40",1,fEventIndex);
	//NEW:
	if(strcmp(fInfoTree->GetListOfBranches()->At(i)->GetName(),"FileName")==0 )
	  ((TTreePlayer*)(fInfoTree->GetPlayer()))->Scan(fInfoTree->GetListOfBranches()->At(i)->GetName(),"","colsize=55",1,fEventIndex);
        else
	  ((TTreePlayer*)(fInfoTree->GetPlayer()))->Scan(fInfoTree->GetListOfBranches()->At(i)->GetName(),"","colsize=8",1,fEventIndex);
        //-------------------------------------------------------------------------------
        
        fclose(pFile);
        pFile=fopen(buffer,"r");
        
        
        
        for(Int_t i=0;i<3;i++)
            aux_string.Gets(pFile);
        
        //OLD: s_value.Append("\n");
        
        while(aux_string.Gets(pFile))
        {
            //aux_string.Tokenize("*")->Print();
            if(aux_string.Tokenize("*")->GetEntries()>0)
            s_value.Append(aux_string.Tokenize("*")->At(aux_string.Tokenize("*")->GetEntries()-1)->GetName());
            s_value.Append("\n");
            //OLD:s_name.Append("\n");
        }
        
        fclose(pFile);
        pFile=nullptr;
        remove(buffer);
        
        
    }

    //OLD: -----------------------------------------------
    //fLabelBranchName->SetText(s_name.Data());
    //fLabelBranchValue->SetText(s_value.Data());
    //NEW:
    TGText text(s_value.Data());
    fTextNameAndValue->AddText(&text);
    //---------------------------------------------------- 
       
    ((TTreePlayer*)(fInfoTree->GetPlayer()))->SetScanRedirect(false);
    fInfoTree->SetScanField(scan_field);
    
    freopen(buf, "w", stdout);
    
}



void TPulseViewer::GetInfoParFile(FILE *fid)
{


    TString s;
    for(int i=0;i<25;i++)
    {

       s.Gets(fid);

       //searching for the number of integer
        if(s.Contains("Integers in header             :"))
        {
            s.Remove(0,s.First(":")+1);
            fInt=s.Atoi();
        }
        
        //searching for the number of unsigned integer
        if(s.Contains("Unsigned longs in header       :"))
        {
            s.Remove(0,s.First(":")+1);
            fUInt=s.Atoi();
        }
        
        //searching for the number of real
        if(s.Contains("Reals in header                :"))
        {
            s.Remove(0,s.First(":")+1);
            fReals=s.Atoi();
        }
        
        //searching for the numebr of DVM channels
        if(s.Contains("DVM channels                   :"))
        {
            s.Remove(0,s.First(":")+1);
            fDVM=s.Atoi();
        }
        
        //searching for the number of sample
        if(s.Contains("Record length                  :"))
        {
            s.Remove(0,s.First(":")+1);
            fSample=s.Atoi();
        }
        
        //searching for the time base
        if(s.Contains("Time base [ms]                 :"))
        {
            s.Remove(0,s.First(":")+1);
            fTimeBase=s.Atof()/1000.;
        }
        
        
        
    }


    
    fRecordLenght= fSample*sizeof(short) + fDVM*sizeof(float) + fInt*sizeof(int) + fUInt*sizeof(unsigned int) +fReals*sizeof(float);
    
    
    
}

void TPulseViewer::GetInfoData()
{
    cout<<"**********************************************"<<endl;
    cout<<"*  Info for the pulse from the .pad file"<<endl;
    cout<<"*  Integers in header             : "<<fInt<<endl;
    cout<<"*  Unsigned longs in header       : "<<fUInt<<endl;
    cout<<"*  Reals in header                : "<<fReals<<endl;
    cout<<"*  DVM channels                   : "<<fDVM<<endl;
    cout<<"*  Record length                  : "<<fSample<<endl;
    cout<<"*  Record data                    : "<<fRecordLenght<<endl;
    cout<<"*  Time Base [ms]                 : "<<fTimeBase<<endl;
    cout<<"*  Time Window [ms]               : "<<fSample*fTimeBase<<endl;
    cout<<"**********************************************"<<endl;
    
}




////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
TChain *ch = nullptr;
TChain *ch_friend_ptc = nullptr;
TChain *ch_friend_tp = nullptr;

TPulseViewer *p=nullptr;
void example() {
    
    //TChain *ch=nullptr;
    ch=new TChain("PulseData");
    ch->Add("cal3_001.root");
    ch_friend_ptc =new TChain("ParticleFit");
    ch_friend_tp =new TChain("TestPulseFit");

#if MIRIAM
    ch_friend_ptc->Add("cal3_001_fit.root");
    ch_friend_tp->Add("cal3_001_fit.root");
#else
    ch_friend_ptc->Add("~/ncal1_001_fit.root");
    ch_friend_tp->Add("~/ncal1_001_fit.root");
#endif
    ch->AddFriend(ch_friend_ptc);
    ch->AddFriend(ch_friend_tp);
    
    p=new TPulseViewer(ch);
    
}
