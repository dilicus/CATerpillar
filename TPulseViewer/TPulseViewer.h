#include <TString.h>
#include <TGClient.h>
#include <TCanvas.h>
#include <TF1.h>
#include <TPad.h>
#include <TEventList.h>
#include <TRandom.h>
#include <stdio.h>
#include <TGButton.h>
#include <TGFrame.h>
#include <TGNumberEntry.h>
#include <TChain.h>
#include <TGraph.h>
#include <TRootEmbeddedCanvas.h>
#include <RQ_OBJECT.h>


class TPulseViewer{
    
    RQ_OBJECT("TPulseViewer")
    
private:
    
    TGMainFrame               *fMain;
    TRootEmbeddedCanvas       *fEcanvas;
    TCanvas                   *fCanvas;
    TChain                    *fMainChain;
    TTree                     *fPulseChain=nullptr;
    TList                     *fList;
    TCut                       fTCutExpression;
    TGTextEdit                *fEntryCutExpression;
    TGMenuBar                 *fMenuBar;
    Bool_t                     fCheckAveragePlot=false;
    Bool_t                     fCheckRegularPlot=true;
    Bool_t                     fCheckDrawFit=false;
    Bool_t                     fCheckRemoveBaseLine=false;
    Bool_t                     fCheckSuperImpose=false;
    TGVButtonGroup            *fDrawFitOption;
    TGCheckButton             *fRegularPlotButton=nullptr;
    TGCheckButton             *fAveragePlotButton=nullptr;
    TGCheckButton             *fRemoveBaselineButton=nullptr;
    TGCheckButton             *fSuperImposeButton=nullptr;
    TGLabel                   *fDisplayListNumber;
    TGLabel                   *fDisplayNumber;
    TGTextEdit                *fTextNameAndValue;
    TGComboBox                *fComboBox;
    TGTextEntry               *fEntryNumberIndex;
    TGTextEntry               *fEntryNumberIndex_tab2;
    TList                     *fFriendListChain=nullptr;
    TTree                     *fInfoTree=nullptr;
    TTree                     *fFitTreeParticle=nullptr;
    TTree                     *fFitTreeTP=nullptr;
    TEventList                *fEventList; //Tevent list con i gli indici che rispettano i tcut expression
    Long64_t                   fEventIndex; //indice evento della chain
    Long64_t                   fEventListIndex;//indice ad elemento lista che contiene info su indice evento chain
    Long64_t                   fEventNumber;//numero totale eventi
    Long64_t                   fEventListNumber;// numero totale eventi lista
    
    
    UInt_t                     fSample = 16384;
    UInt_t                     fInt=7;
    UInt_t                     fUInt=3;
    UInt_t                     fReals=3;
    UInt_t                     fDVM=0;
    Double_t                   fTimeBase=0.04;
    ULong64_t                  fRecordLenght = fSample*sizeof(short)
                                             + fDVM*sizeof(float)
                                             + fInt*sizeof(int)
                                             + fUInt*sizeof(unsigned int)
                                             + fReals*sizeof(float);
    UInt_t                     fNumberAverage = 50;



    //tab3
    TRootEmbeddedCanvas       *fEcanvas_tab3;
    TCanvas                   *fCanvas_tab3;
    TRootEmbeddedCanvas       *fEcanvas2_tab3;
    TCanvas                   *fCanvas2_tab3;
    TGTextEntry               *fEventNumber_tab3;
    TGTextEntry               *fEventChannel_tab3;
    UInt_t                     fChannel_tab3=0;
    Long64_t                   fEventIndex_tab3;
    TGLabel                   *fChannelLabel_tab3;
    TGLabel                   *fEvNumLabel_tab3;
    TTree                     *fPulseChain_tab3=nullptr;
    TFitEditor                *fFitEditor_tab3;
    TGRadioButton             *fChannelPhonon;
    TGRadioButton             *fChannelLight;
    TGRadioButton             *fChannelStick;
    TGButtonGroup             *fChannelButtons_tab3;
    TPad                      *pad1_tab3;
    TPad                      *pad2_tab3;
    TGraph                    *GraphVector_tab3_temp;
    std::vector<TGraph*>       GraphVector_tab3;
    Int_t                      Nfitfun1;
    Int_t                      Nfitfun2;
    


    
    
    virtual void   GetInfoParFile(FILE *fid);
    
        
public:
    TPulseViewer();//default constructor
    TPulseViewer(TChain *input_chain); //finestra con la chain caricata
    
    virtual ~TPulseViewer();
    

    virtual void Init();
    
    //Getter method
    virtual Long64_t GetEventIndex();
    virtual Long64_t GetEventListIndex();
    virtual Long64_t GetEventNumber();
    virtual Long64_t GetEventListNumber();
    virtual TCut     GetCutExpression();
    
    
    //Setter method
    virtual void   SetEventIndex(Long64_t i);
    virtual void   SetEventListIndex(Long64_t i);
    virtual void   SetCutExpression(TCut Cut_expression);
    virtual void   SetPlotRegular(Bool_t);
    virtual void   SetPlotAverage(Bool_t);
    virtual void   SetBaseLineOff(Bool_t);
    virtual void   SetSuperImpose(Bool_t);
    virtual void   SetDrawFit(Bool_t);
    
    
    //Draw method
    virtual void LoadEvent(Long64_t index);
    virtual void DrawEvent();
    virtual void SelectEvent();
    virtual void SelectEvent_tab2();
    virtual void Next();
    virtual void Previous();

    //tab3
    virtual void   DrawSinglePulse();
    virtual void   SelectChannel_tab3();
    virtual void   Apply_tab3();
    virtual void   LoadEvent_tab3(Long64_t index);
    virtual void   SelectEvent_tab3();
    virtual UInt_t GetChannelNumber_tab3();
    virtual void   SetChannelNumber_tab3(UInt_t i);
    virtual void   SelectChannel_gradio(Int_t i);
    virtual void   ShowResiduals(Event_t *ev);

    
    
    //Info method
    virtual void SelectChain(Int_t PulseIdx);
    virtual void InfoEvent();
    virtual void SelectionReturnKeyPressed(Event_t*); //<------------------
    virtual void ApplyCut();
    virtual void GetInfoData();
    
   
};



