#ifndef _LAYOUT_H
#define _LAYOUT_H
#include <boost/predef.h>
#if BOOST_OS_WINDOWS
    #define _WIN32_WINNT 0x0501
    #define WINVER 0x0501
    #define WIN32_LEAN_AND_MEAN
    #define WX_LEAN_AND_MEAN
#endif
#include "layoutBase.h"
#include <boost/asio.hpp>
#include <wx/wx.h>
#include <librcfcommon/librcfcommon.h>
#include <librcfclient/librcfclient.h>
#include <boost/thread.hpp>
#include <boost/filesystem.hpp>
#include <vector>
wxDECLARE_EVENT(wxEVT_COMMAND_EXECUTETHREAD_NEEDS_PARAMETER, wxThreadEvent);
namespace wxRCF {
    struct ServerData {
        RCF::Client::Client* cli;
        RCF::Client::ServerDefinition* sdef;
        int lastcode;
        std::string lastout;
        std::string lasterr;
        std::string username;
        std::string status;
        ServerData();
        void close();
    };

    struct Command {
        std::string name;
        void addToTreeCtrl(wxTreeCtrl* control, wxTreeItemId parentId);
    };

    struct CommandGroup {
        std::string name;
        std::vector<Command> commands;
        std::vector<CommandGroup> groups;
        void appendByQuery(std::string query);
        void addToTreeCtrl(wxTreeCtrl* control, wxTreeItemId parentId);
    };

    class ExecuteThread;

    class mainWindow : public mainWindowBase {
    private:
        std::vector<ServerData*> vsd;
        void serverConnectItemOnMenuSelection( wxCommandEvent& event );
        void helpAboutItemOnMenuSelection( wxCommandEvent& event );
        void activeServersListOnListBox( wxCommandEvent& event );
        void listActionButtonOnButtonClick( wxCommandEvent& event );
        void closeActionButtonOnButtonClick( wxCommandEvent& event );
        void refreshStatusButtonOnButtonClick( wxCommandEvent& event );
        void closeButtonOnButtonClick( wxCommandEvent& event );
        void executeThreadNeedsParameterHandler( wxThreadEvent& event );
    public:
        mainWindow(wxWindow* parent);
        std::vector<ExecuteThread*> tv;
        std::string sharedParameter;
        bool sharedParameterModified;
        int sharedParameterForThreadId;
        wxCriticalSection sharedParameterCS;
    };

    void execute(int threadId, std::string query, ServerData* sd, mainWindow* mw);

    class ExecuteThread : public wxThread {
    public:
        ExecuteThread(int id, std::string query, ServerData* sd, void (*handler)(int, std::string, ServerData*, mainWindow*), mainWindow* mw);
        ~ExecuteThread();
        std::string getServerName();
        std::string getLoggedUsername();
    private:
        int _id;
        std::string _query;
        ServerData* _sd;
        void (*_handler)(int, std::string, ServerData*, mainWindow*);
        mainWindow* _mw;
        wxThread::ExitCode Entry();
    };

    class connectDialog : public connectDialogBase {
    private:
        void connectDialogBaseOnInitDialog( wxInitDialogEvent& event );
        void serversChoiceOnChoice( wxCommandEvent& event );
        void cancelButtonOnButtonClick( wxCommandEvent& event );
        void connectButtonOnButtonClick( wxCommandEvent& event );
        ServerData** _target_sd;
    public:
       connectDialog(wxWindow* parent, ServerData** target_sd);
    };

    class aboutDialog : public aboutDialogBase {
    private:
        void aboutDialogBaseOnInitDialog( wxInitDialogEvent& event );
        void aboutCloseButtonOnButtonClick( wxCommandEvent& event );
    public:
        aboutDialog(wxWindow* parent);
    };

    class listDialog : public listDialogBase {
    private:
        std::vector<ServerData*> _processedServers;
        void listDialogBaseOnInitDialog( wxInitDialogEvent& event );
        void listCancelButtonOnButtonClick( wxCommandEvent& event );
        void listExecuteButtonOnButtonClick( wxCommandEvent& event );
    public:
        listDialog(wxWindow* parent, std::vector<ServerData*> processedServers);
    };

    class paramQueryDialog : public paramQueryDialogBase {
    private:
        void paramQueryDialogBaseOnInitDialog( wxInitDialogEvent& event );
        void paramContinueButtonOnButtonClick( wxCommandEvent& event );
        std::string* _paramTarget;
    public:
        paramQueryDialog(wxWindow* parent, wxString& title, std::string* paramTarget);
    };
}
#endif
