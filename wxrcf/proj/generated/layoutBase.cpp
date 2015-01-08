///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Jan  8 2015)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "layoutBase.h"

///////////////////////////////////////////////////////////////////////////
using namespace wxRCF;

mainWindowBase::mainWindowBase( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	mainMenuBar = new wxMenuBar( 0 );
	mainServerMenu = new wxMenu();
	serverConnectItem = new wxMenuItem( mainServerMenu, wxID_ANY, wxString( wxT("Connect to server...") ) , wxEmptyString, wxITEM_NORMAL );
	mainServerMenu->Append( serverConnectItem );
	
	mainMenuBar->Append( mainServerMenu, wxT("Servers") ); 
	
	mainHelpMenu = new wxMenu();
	helpAboutItem = new wxMenuItem( mainHelpMenu, wxID_ANY, wxString( wxT("About...") ) , wxEmptyString, wxITEM_NORMAL );
	mainHelpMenu->Append( helpAboutItem );
	
	mainMenuBar->Append( mainHelpMenu, wxT("Help") ); 
	
	this->SetMenuBar( mainMenuBar );
	
	mainSizer = new wxBoxSizer( wxVERTICAL );
	
	mainPanel = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	mainPanelSizer = new wxBoxSizer( wxVERTICAL );
	
	activeConnectionsLabel = new wxStaticText( mainPanel, wxID_ANY, wxT("Active connections:"), wxDefaultPosition, wxDefaultSize, 0 );
	activeConnectionsLabel->Wrap( -1 );
	mainPanelSizer->Add( activeConnectionsLabel, 0, wxALL, 5 );
	
	activeServersList = new wxListBox( mainPanel, wxID_ANY, wxDefaultPosition, wxSize( 800,100 ), 0, NULL, wxLB_MULTIPLE ); 
	mainPanelSizer->Add( activeServersList, 0, wxALL|wxEXPAND, 5 );
	
	actionButtonsSizer = new wxBoxSizer( wxHORIZONTAL );
	
	listActionButton = new wxButton( mainPanel, wxID_ANY, wxT("List available commands"), wxDefaultPosition, wxDefaultSize, 0 );
	actionButtonsSizer->Add( listActionButton, 0, wxALIGN_CENTER|wxALL, 5 );
	
	closeActionButton = new wxButton( mainPanel, wxID_ANY, wxT("Close connection"), wxDefaultPosition, wxDefaultSize, 0 );
	actionButtonsSizer->Add( closeActionButton, 0, wxALIGN_CENTER|wxALL, 5 );
	
	
	mainPanelSizer->Add( actionButtonsSizer, 1, wxEXPAND, 5 );
	
	m_staticline1 = new wxStaticLine( mainPanel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	mainPanelSizer->Add( m_staticline1, 0, wxEXPAND | wxALL, 5 );
	
	lastCommandStatusLabel = new wxStaticText( mainPanel, wxID_ANY, wxT("Last command status: Idle"), wxDefaultPosition, wxDefaultSize, 0 );
	lastCommandStatusLabel->Wrap( -1 );
	mainPanelSizer->Add( lastCommandStatusLabel, 0, wxALL, 5 );
	
	commandExitCodeLabel = new wxStaticText( mainPanel, wxID_ANY, wxT("Command exit status: None"), wxDefaultPosition, wxDefaultSize, 0 );
	commandExitCodeLabel->Wrap( -1 );
	mainPanelSizer->Add( commandExitCodeLabel, 0, wxALL, 5 );
	
	commandOutputLabel = new wxStaticText( mainPanel, wxID_ANY, wxT("Command standard console output:"), wxDefaultPosition, wxDefaultSize, 0 );
	commandOutputLabel->Wrap( -1 );
	mainPanelSizer->Add( commandOutputLabel, 0, wxALL, 5 );
	
	commandOutputText = new wxTextCtrl( mainPanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 800,150 ), wxHSCROLL|wxTE_MULTILINE|wxTE_READONLY );
	mainPanelSizer->Add( commandOutputText, 0, wxALL, 5 );
	
	commandErrorLabel = new wxStaticText( mainPanel, wxID_ANY, wxT("Command standard error output:"), wxDefaultPosition, wxDefaultSize, 0 );
	commandErrorLabel->Wrap( -1 );
	mainPanelSizer->Add( commandErrorLabel, 0, wxALL, 5 );
	
	commandErrorText = new wxTextCtrl( mainPanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 800,150 ), wxHSCROLL|wxTE_MULTILINE|wxTE_READONLY|wxTE_WORDWRAP );
	mainPanelSizer->Add( commandErrorText, 0, wxALL, 5 );
	
	m_staticline2 = new wxStaticLine( mainPanel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	mainPanelSizer->Add( m_staticline2, 0, wxEXPAND | wxALL, 5 );
	
	closeButtonSizer = new wxBoxSizer( wxHORIZONTAL );
	
	
	closeButtonSizer->Add( 0, 0, 1, wxEXPAND, 5 );
	
	closeButton = new wxButton( mainPanel, wxID_ANY, wxT("Close"), wxDefaultPosition, wxDefaultSize, 0 );
	closeButtonSizer->Add( closeButton, 0, wxALIGN_CENTER|wxALL, 5 );
	
	
	mainPanelSizer->Add( closeButtonSizer, 1, wxEXPAND, 5 );
	
	
	mainPanel->SetSizer( mainPanelSizer );
	mainPanel->Layout();
	mainPanelSizer->Fit( mainPanel );
	mainSizer->Add( mainPanel, 1, wxEXPAND | wxALL, 5 );
	
	
	this->SetSizer( mainSizer );
	this->Layout();
	
	this->Centre( wxBOTH );
	
	// Connect Events
	this->Connect( serverConnectItem->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( mainWindowBase::serverConnectItemOnMenuSelection ) );
	this->Connect( helpAboutItem->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( mainWindowBase::helpAboutItemOnMenuSelection ) );
	listActionButton->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( mainWindowBase::listActionButtonOnButtonClick ), NULL, this );
	closeActionButton->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( mainWindowBase::closeActionButtonOnButtonClick ), NULL, this );
	closeButton->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( mainWindowBase::closeButtonOnButtonClick ), NULL, this );
}

mainWindowBase::~mainWindowBase()
{
	// Disconnect Events
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( mainWindowBase::serverConnectItemOnMenuSelection ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( mainWindowBase::helpAboutItemOnMenuSelection ) );
	listActionButton->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( mainWindowBase::listActionButtonOnButtonClick ), NULL, this );
	closeActionButton->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( mainWindowBase::closeActionButtonOnButtonClick ), NULL, this );
	closeButton->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( mainWindowBase::closeButtonOnButtonClick ), NULL, this );
	
}

connectDialogBase::connectDialogBase( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	connectDialogSizer = new wxBoxSizer( wxVERTICAL );
	
	connectToServerLabel = new wxStaticText( this, wxID_ANY, wxT("Connect to server:"), wxDefaultPosition, wxDefaultSize, 0 );
	connectToServerLabel->Wrap( -1 );
	connectDialogSizer->Add( connectToServerLabel, 0, wxALL, 5 );
	
	wxArrayString serversChoiceChoices;
	serversChoice = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxSize( 200,-1 ), serversChoiceChoices, 0 );
	serversChoice->SetSelection( 0 );
	connectDialogSizer->Add( serversChoice, 0, wxALL, 5 );
	
	m_staticline3 = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	connectDialogSizer->Add( m_staticline3, 0, wxEXPAND | wxALL, 5 );
	
	connectAsLabel = new wxStaticText( this, wxID_ANY, wxT("Connect as:"), wxDefaultPosition, wxDefaultSize, 0 );
	connectAsLabel->Wrap( -1 );
	connectDialogSizer->Add( connectAsLabel, 0, wxALL, 5 );
	
	userCredentialsSizer = new wxFlexGridSizer( 2, 2, 0, 0 );
	userCredentialsSizer->SetFlexibleDirection( wxBOTH );
	userCredentialsSizer->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	userEntryLabel = new wxStaticText( this, wxID_ANY, wxT("Username:"), wxDefaultPosition, wxDefaultSize, 0 );
	userEntryLabel->Wrap( -1 );
	userCredentialsSizer->Add( userEntryLabel, 0, wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT|wxALL, 5 );
	
	userEntry = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 200,-1 ), 0 );
	userCredentialsSizer->Add( userEntry, 0, wxALL, 5 );
	
	passwordEntryLabel = new wxStaticText( this, wxID_ANY, wxT("Password:"), wxDefaultPosition, wxDefaultSize, 0 );
	passwordEntryLabel->Wrap( -1 );
	userCredentialsSizer->Add( passwordEntryLabel, 0, wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT|wxALL, 5 );
	
	passwordEntry = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 200,-1 ), wxTE_PASSWORD );
	userCredentialsSizer->Add( passwordEntry, 0, wxALL, 5 );
	
	
	connectDialogSizer->Add( userCredentialsSizer, 1, wxEXPAND, 5 );
	
	m_staticline4 = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	connectDialogSizer->Add( m_staticline4, 0, wxEXPAND | wxALL, 5 );
	
	dialogActionSizer = new wxBoxSizer( wxHORIZONTAL );
	
	
	dialogActionSizer->Add( 0, 0, 1, wxEXPAND, 5 );
	
	cancelButton = new wxButton( this, wxID_ANY, wxT("Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
	dialogActionSizer->Add( cancelButton, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	connectButton = new wxButton( this, wxID_ANY, wxT("Connect"), wxDefaultPosition, wxDefaultSize, 0 );
	dialogActionSizer->Add( connectButton, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	
	connectDialogSizer->Add( dialogActionSizer, 1, wxEXPAND, 5 );
	
	
	this->SetSizer( connectDialogSizer );
	this->Layout();
	connectDialogSizer->Fit( this );
	
	this->Centre( wxBOTH );
	
	// Connect Events
	this->Connect( wxEVT_INIT_DIALOG, wxInitDialogEventHandler( connectDialogBase::connectDialogBaseOnInitDialog ) );
	serversChoice->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( connectDialogBase::serversChoiceOnChoice ), NULL, this );
	cancelButton->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( connectDialogBase::cancelButtonOnButtonClick ), NULL, this );
	connectButton->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( connectDialogBase::connectButtonOnButtonClick ), NULL, this );
}

connectDialogBase::~connectDialogBase()
{
	// Disconnect Events
	this->Disconnect( wxEVT_INIT_DIALOG, wxInitDialogEventHandler( connectDialogBase::connectDialogBaseOnInitDialog ) );
	serversChoice->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( connectDialogBase::serversChoiceOnChoice ), NULL, this );
	cancelButton->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( connectDialogBase::cancelButtonOnButtonClick ), NULL, this );
	connectButton->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( connectDialogBase::connectButtonOnButtonClick ), NULL, this );
	
}

aboutDialogBase::aboutDialogBase( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	aboutSizer = new wxBoxSizer( wxVERTICAL );
	
	aboutNameLabel = new wxStaticText( this, wxID_ANY, wxT("wxRCF v. 0.1\n\nA graphical multi-server client for RemoteControlFramework\nbased on wxWidgets\n\nDeveloped by Phitherek_ in 2015\n\nContact with the author: phitherek@gmail.com"), wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE );
	aboutNameLabel->Wrap( -1 );
	aboutSizer->Add( aboutNameLabel, 0, wxALIGN_CENTER|wxALL, 5 );
	
	m_staticline5 = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	aboutSizer->Add( m_staticline5, 0, wxEXPAND | wxALL, 5 );
	
	aboutCloseSizer = new wxBoxSizer( wxHORIZONTAL );
	
	
	aboutCloseSizer->Add( 0, 0, 1, wxEXPAND, 5 );
	
	aboutCloseButton = new wxButton( this, wxID_ANY, wxT("Close"), wxDefaultPosition, wxDefaultSize, 0 );
	aboutCloseSizer->Add( aboutCloseButton, 0, wxALL, 5 );
	
	
	aboutSizer->Add( aboutCloseSizer, 1, wxEXPAND, 5 );
	
	
	this->SetSizer( aboutSizer );
	this->Layout();
	aboutSizer->Fit( this );
	
	this->Centre( wxBOTH );
	
	// Connect Events
	this->Connect( wxEVT_INIT_DIALOG, wxInitDialogEventHandler( aboutDialogBase::aboutDialogBaseOnInitDialog ) );
	aboutCloseButton->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( aboutDialogBase::aboutCloseButtonOnButtonClick ), NULL, this );
}

aboutDialogBase::~aboutDialogBase()
{
	// Disconnect Events
	this->Disconnect( wxEVT_INIT_DIALOG, wxInitDialogEventHandler( aboutDialogBase::aboutDialogBaseOnInitDialog ) );
	aboutCloseButton->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( aboutDialogBase::aboutCloseButtonOnButtonClick ), NULL, this );
	
}
