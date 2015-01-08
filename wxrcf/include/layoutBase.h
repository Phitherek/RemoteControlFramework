///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Jan  8 2015)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __LAYOUTBASE_H__
#define __LAYOUTBASE_H__

#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include <wx/string.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/menu.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/stattext.h>
#include <wx/listbox.h>
#include <wx/button.h>
#include <wx/sizer.h>
#include <wx/statline.h>
#include <wx/textctrl.h>
#include <wx/panel.h>
#include <wx/frame.h>
#include <wx/choice.h>
#include <wx/dialog.h>

///////////////////////////////////////////////////////////////////////////

namespace wxRCF
{
	
	///////////////////////////////////////////////////////////////////////////////
	/// Class mainWindowBase
	///////////////////////////////////////////////////////////////////////////////
	class mainWindowBase : public wxFrame 
	{
		private:
		
		protected:
			wxMenuBar* mainMenuBar;
			wxMenu* mainServerMenu;
			wxMenuItem* serverConnectItem;
			wxMenu* mainHelpMenu;
			wxMenuItem* helpAboutItem;
			wxBoxSizer* mainSizer;
			wxPanel* mainPanel;
			wxBoxSizer* mainPanelSizer;
			wxStaticText* activeConnectionsLabel;
			wxListBox* activeServersList;
			wxBoxSizer* actionButtonsSizer;
			wxButton* listActionButton;
			wxButton* closeActionButton;
			wxStaticLine* m_staticline1;
			wxStaticText* lastCommandStatusLabel;
			wxStaticText* commandExitCodeLabel;
			wxStaticText* commandOutputLabel;
			wxTextCtrl* commandOutputText;
			wxStaticText* commandErrorLabel;
			wxTextCtrl* commandErrorText;
			wxStaticLine* m_staticline2;
			wxBoxSizer* closeButtonSizer;
			wxButton* closeButton;
			
			// Virtual event handlers, overide them in your derived class
			virtual void serverConnectItemOnMenuSelection( wxCommandEvent& event ) { event.Skip(); }
			virtual void helpAboutItemOnMenuSelection( wxCommandEvent& event ) { event.Skip(); }
			virtual void listActionButtonOnButtonClick( wxCommandEvent& event ) { event.Skip(); }
			virtual void closeActionButtonOnButtonClick( wxCommandEvent& event ) { event.Skip(); }
			virtual void closeButtonOnButtonClick( wxCommandEvent& event ) { event.Skip(); }
			
		
		public:
			
			mainWindowBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("wxRCF v. 1.0 (C) 2015 by Phitherek_"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 800,-1 ), long style = wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL );
			
			~mainWindowBase();
		
	};
	
	///////////////////////////////////////////////////////////////////////////////
	/// Class connectDialogBase
	///////////////////////////////////////////////////////////////////////////////
	class connectDialogBase : public wxDialog 
	{
		private:
		
		protected:
			wxBoxSizer* connectDialogSizer;
			wxStaticText* connectToServerLabel;
			wxChoice* serversChoice;
			wxStaticLine* m_staticline3;
			wxStaticText* connectAsLabel;
			wxFlexGridSizer* userCredentialsSizer;
			wxStaticText* userEntryLabel;
			wxTextCtrl* userEntry;
			wxStaticText* passwordEntryLabel;
			wxTextCtrl* passwordEntry;
			wxStaticLine* m_staticline4;
			wxBoxSizer* dialogActionSizer;
			wxButton* cancelButton;
			wxButton* connectButton;
			
			// Virtual event handlers, overide them in your derived class
			virtual void connectDialogBaseOnInitDialog( wxInitDialogEvent& event ) { event.Skip(); }
			virtual void serversChoiceOnChoice( wxCommandEvent& event ) { event.Skip(); }
			virtual void cancelButtonOnButtonClick( wxCommandEvent& event ) { event.Skip(); }
			virtual void connectButtonOnButtonClick( wxCommandEvent& event ) { event.Skip(); }
			
		
		public:
			
			connectDialogBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Connect to server - wxRCF v. 0.1 (C) 2015 by Phitherek_"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxDEFAULT_DIALOG_STYLE ); 
			~connectDialogBase();
		
	};
	
	///////////////////////////////////////////////////////////////////////////////
	/// Class aboutDialogBase
	///////////////////////////////////////////////////////////////////////////////
	class aboutDialogBase : public wxDialog 
	{
		private:
		
		protected:
			wxBoxSizer* aboutSizer;
			wxStaticText* aboutNameLabel;
			wxStaticLine* m_staticline5;
			wxBoxSizer* aboutCloseSizer;
			wxButton* aboutCloseButton;
			
			// Virtual event handlers, overide them in your derived class
			virtual void aboutDialogBaseOnInitDialog( wxInitDialogEvent& event ) { event.Skip(); }
			virtual void aboutCloseButtonOnButtonClick( wxCommandEvent& event ) { event.Skip(); }
			
		
		public:
			
			aboutDialogBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("About wxRCF"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxDEFAULT_DIALOG_STYLE ); 
			~aboutDialogBase();
		
	};
	
} // namespace wxRCF

#endif //__LAYOUTBASE_H__
