#include "layout.h"
#include <iostream>
#include <exception>
#include <sstream>
using namespace wxRCF;

wxDEFINE_EVENT(wxEVT_COMMAND_EXECUTETHREAD_NEEDS_PARAMETER, wxThreadEvent);

ServerData::ServerData() {
    cli = NULL;
    sdef = NULL;
    lastcode = -1;
    lastout = "";
    lasterr = "";
    username = "";
    status = "Idle";
}

void ServerData::close() {
    if(cli != NULL) {
        try {
            cli->close();
        } catch(std::exception& e) {
        }
        delete cli;
        cli = NULL;
        lastcode = -1;
        lastout = "";
        lasterr = "";
        username = "";
        status = "Idle";
    }
}

void CommandGroup::appendByQuery(std::string query) {
    int queryLength = query.length();
    std::vector<std::string> splitQuery;
    std::string queryPart = "";
    for(int i = 0; i < queryLength; i++) {
        if(query[i] == ':') {
            if(queryPart != "") {
                splitQuery.push_back(queryPart);
            }
            queryPart = "";
        } else {
            queryPart += query[i];
        }
    }
    if(queryPart != "") {
        splitQuery.push_back(queryPart);
    }
    queryPart = "";
    if(splitQuery.size() == 1) {
        Command c;
        c.name = splitQuery[0];
        commands.push_back(c);
        return;
    } else {
        int groupsSize = groups.size();
        if(groupsSize > 0) {
            for(int i = 0; i < groupsSize; i++) {
                if(groups[i].name == splitQuery[0]) {
                    int splitQuerySize = splitQuery.size();
                    std::string subquery = "";
                    for(int j = 1; j < splitQuerySize; j++) {
                        subquery += splitQuery[j];
                        if(i != splitQuerySize-1) {
                            subquery += ':';
                        }
                    }
                    groups[i].appendByQuery(subquery);
                    return;
                }
            }
        }
        CommandGroup cg;
        cg.name = splitQuery[0];
        int splitQuerySize = splitQuery.size();
        std::string subquery = "";
        for(int i = 1; i < splitQuerySize; i++) {
            subquery += splitQuery[i];
            if(i != splitQuerySize-1) {
                subquery += ':';
            }
        }
        cg.appendByQuery(subquery);
        groups.push_back(cg);
        return;
    }
}

void Command::addToTreeCtrl(wxTreeCtrl* control, wxTreeItemId parentId) {
    control->AppendItem(parentId, (wxString)name);
    return;
}

void CommandGroup::addToTreeCtrl(wxTreeCtrl* control, wxTreeItemId parentId) {
    int commandsSize = commands.size();
    int groupsSize = groups.size();
    if(name == "") {
        for(int i = 0; i < commandsSize; i++) {
            commands[i].addToTreeCtrl(control, parentId);
        }
        for(int i = 0; i < groupsSize; i++) {
            groups[i].addToTreeCtrl(control, parentId);
        }
    } else {
        wxTreeItemId myId = control->AppendItem(parentId, (wxString)name);
        for(int i = 0; i < commandsSize; i++) {
            commands[i].addToTreeCtrl(control, myId);
        }
        for(int i = 0; i < groupsSize; i++) {
            groups[i].addToTreeCtrl(control, myId);
        }
    }
}

ExecuteThread::ExecuteThread(int id, std::string query, ServerData* sd, void (*handler)(int, std::string, ServerData*, mainWindow*), mainWindow* mw) {
    _id = id;
    _query = query;
    _sd = sd;
    _handler = handler;
    _mw = mw;
}

ExecuteThread::~ExecuteThread() {}

wxThread::ExitCode ExecuteThread::Entry()  {
    _handler(_id, _query, _sd, _mw);
    return (wxThread::ExitCode)0;
}

std::string ExecuteThread::getServerName() {
    return _sd->sdef->getName();
}

std::string ExecuteThread::getLoggedUsername() {
    return _sd->username;
}

// Complete refactor of the execute for wxWidgets threading model.
void wxRCF::execute(int threadId, std::string query, ServerData* sd, mainWindow* mw) {
    sd->lastcode = -1;
    sd->lastout = "";
    sd->lasterr = "";
    sd->status = "Processing";
    try {
        std::string msg = "";
        std::string resp = "";
        int code;
        std::string ps = "toplevel";
        msg += "EXEC ";
        msg += query;
        sd->cli->write(msg);
        do {
            resp = sd->cli->read();
            if(ps == "toplevel") {
                if(resp == "PARAM") {
                    std::string param;
                    wxThreadEvent* event = new wxThreadEvent(wxEVT_COMMAND_EXECUTETHREAD_NEEDS_PARAMETER);
                    event->SetInt(threadId);
                    wxQueueEvent(mw, event);
                    bool cont = false;
                    do {
                        mw->sharedParameterCS.Enter();
                        if(mw->sharedParameterModified == true && mw->sharedParameterForThreadId == threadId) {
                            cont = true;
                        } else {
                            mw->sharedParameterCS.Leave();
                        }
                    } while(!cont);
                    param = mw->sharedParameter;
                    mw->sharedParameter = "";
                    mw->sharedParameterModified = false;
                    mw->sharedParameterForThreadId = -1;
                    mw->sharedParameterCS.Leave();
                    sd->cli->write(param);
                } else if(resp == "EXECBEGIN") {
                    ps = "inexec";
                } else if(resp == "OUTBEGIN") {
                    ps = "out";
                } else if(resp == "ERRBEGIN") {
                    ps = "err";
                } else if(resp.substr(0, 5) == "ERROR") {
                    std::string err = msg.substr(6, std::string::npos);
                    throw RCF::Common::ProtocolException(err);
                } else if(resp.substr(0, 7) == "NCERROR") {
                    std::string err = msg.substr(8, std::string::npos);
                    throw RCF::Common::NotFoundException("From server", err);
                } else {
                    throw RCF::Common::ProtocolException("Unexpected protocol message!");
                }
            } else if(ps == "inexec") {
                if(resp.substr(0, 7) == "EXECEND") {
                    std::string scode = resp.substr(8, std::string::npos);
                    code = atoi(scode.c_str());
                    ps = "toplevel";
                } else {
                    if(resp.substr(0, 5) == "ERROR") {
                        std::string err = msg.substr(6, std::string::npos);
                        throw RCF::Common::ProtocolException(err);
                    } else {
                        throw RCF::Common::ProtocolException("Unexpected protocol message!");
                    }
                }
            } else if(ps == "out") {
                if(resp == "OUTEND") {
                    ps = "toplevel";
                } else {
                    sd->lastout += resp;
                }
            } else if(ps == "err") {
                if(resp == "ERREND") {
                    ps = "toplevel";
                } else {
                    sd->lasterr += resp;
                }
            }
        } while(resp != "ERREND");
        sd->lastcode = code;
        sd->status = "Idle";
    } catch(std::exception& e) {
        sd->status = "Exception: ";
        sd->status += e.what();
    }
}

mainWindow::mainWindow(wxWindow* parent): mainWindowBase(parent) {
    sharedParameter = "";
    sharedParameterModified = false;
    sharedParameterForThreadId = -1;
    this->Connect(wxEVT_COMMAND_EXECUTETHREAD_NEEDS_PARAMETER, wxThreadEventHandler(mainWindow::executeThreadNeedsParameterHandler));
}

void mainWindow::serverConnectItemOnMenuSelection( wxCommandEvent& event ) {
    wxRCF::ServerData* sd = NULL;
    connectDialog* cd = new connectDialog(this, &sd);
    cd->ShowModal();
    if(sd != NULL) {
        vsd.push_back(sd);
    }
   wxArrayString activeConnections;
   int vsdSize = vsd.size();
   if(vsdSize > 0) {
       for(int i = 0; i < vsdSize; i++) {
           std::string desc = vsd[i]->sdef->getName();
           desc  += ": ";
           desc += vsd[i]->username;
           activeConnections.Add((wxString)desc);
       }
       wxListBox* newActiveServersList = new wxListBox( mainPanel, wxID_ANY, wxDefaultPosition, wxSize( 800,100 ), activeConnections, wxLB_MULTIPLE );
       wxListBox* oldActiveServersList = activeServersList;
       mainPanelSizer->Replace((wxWindow*)oldActiveServersList, (wxWindow*)newActiveServersList);
       activeServersList = newActiveServersList;
       delete oldActiveServersList;
   } else {
       wxListBox* newActiveServersList = new wxListBox( mainPanel, wxID_ANY, wxDefaultPosition, wxSize( 800,100 ), 0, NULL, wxLB_MULTIPLE );
       wxListBox* oldActiveServersList = activeServersList;
       mainPanelSizer->Replace((wxWindow*)oldActiveServersList, (wxWindow*)newActiveServersList);
       activeServersList = newActiveServersList;
       delete oldActiveServersList;
   }
   this->Layout();
}

void mainWindow::helpAboutItemOnMenuSelection( wxCommandEvent& event ) {
    aboutDialog* ad = new aboutDialog(this);
    ad->ShowModal();
}

void mainWindow::activeServersListOnListBox( wxCommandEvent& event ) {
    wxArrayInt selections;
    int selectionsLength;
    selectionsLength = activeServersList->GetSelections(selections);
    if(selectionsLength == 1) {
        int idx = selections.Item(0);
        std::string statusLabel = "Last command status: ";
        statusLabel += vsd[idx]->status;
        std::string exitCodeLabel = "";
        std::stringstream ssecl;
        ssecl.str("");
        ssecl << "Command exit status: ";
        if(vsd[idx]->lastcode == -1) {
            ssecl << "None";
        } else {
            ssecl << vsd[idx]->lastcode;
        }
        exitCodeLabel = ssecl.str();
        wxStaticText* newLastCommandStatusLabel = new wxStaticText(mainPanel, wxID_ANY, (wxString)statusLabel);
        wxStaticText* oldLastCommandStatusLabel = lastCommandStatusLabel;
        mainPanelSizer->Replace((wxWindow*)oldLastCommandStatusLabel, (wxWindow*)newLastCommandStatusLabel);
        delete oldLastCommandStatusLabel;
        lastCommandStatusLabel = newLastCommandStatusLabel;
        wxStaticText* newCommandExitCodeLabel = new wxStaticText(mainPanel, wxID_ANY, (wxString)exitCodeLabel);
        wxStaticText* oldCommandExitCodeLabel = commandExitCodeLabel;
        mainPanelSizer->Replace((wxWindow*)oldCommandExitCodeLabel, (wxWindow*)newCommandExitCodeLabel);
        delete oldCommandExitCodeLabel;
        commandExitCodeLabel = newCommandExitCodeLabel;
        wxTextCtrl* newCommandOutputText = new wxTextCtrl(mainPanel, wxID_ANY, (wxString)(vsd[idx]->lastout), wxDefaultPosition, wxSize( 800,150 ), wxHSCROLL|wxTE_MULTILINE|wxTE_READONLY);
        wxTextCtrl* oldCommandOutputText = commandOutputText;
        mainPanelSizer->Replace((wxWindow*)oldCommandOutputText, (wxWindow*)newCommandOutputText);
        delete oldCommandOutputText;
        commandOutputText = newCommandOutputText;
        wxTextCtrl* newCommandErrorText = new wxTextCtrl(mainPanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 800,150 ), wxHSCROLL|wxTE_MULTILINE|wxTE_READONLY|wxTE_WORDWRAP);
        wxTextCtrl* oldCommandErrorText = commandErrorText;
        mainPanelSizer->Replace((wxWindow*)oldCommandErrorText, (wxWindow*)newCommandErrorText);
        delete oldCommandErrorText;
        commandErrorText = newCommandErrorText;
        this->Layout();
    }
}

void mainWindow::listActionButtonOnButtonClick( wxCommandEvent& event ) {
    wxArrayInt selections;
    int selectionsLength;
    selectionsLength = activeServersList->GetSelections(selections);
    std::vector<ServerData*> processedServers;
    for(int i = 0; i < selectionsLength; i++) {
        int idx = selections.Item(i);
        if(vsd[idx]->status == "Processing") {
            wxMessageDialog errorDialog(this, wxT("At least one of the selected servers is still executing a command! Please wait until that command finishes!"), wxT("Error occured"));
            errorDialog.ShowModal();
            return;
        }
        processedServers.push_back(vsd[idx]);
    }
    if(processedServers.size() > 0) {
        listDialog* ld = new listDialog(this, processedServers);
        ld->ShowModal();
    } else {
        wxMessageDialog errorDialog(this, wxT("You must select at least one server to use this functionality!"), wxT("Error occured"));
        errorDialog.ShowModal();
    }
}

void mainWindow::closeActionButtonOnButtonClick( wxCommandEvent& event ) {
    wxArrayInt selections;
    int selectionsLength;
    selectionsLength = activeServersList->GetSelections(selections);
    for(int i = 0; i < selectionsLength; i++) {
        int idx = selections.Item(i);
        vsd[idx]->close();
        delete vsd[idx];
        vsd[idx] = NULL;
    }
    int vsdSize = vsd.size();
    for(int i = 0; i < vsdSize; i++) {
        if(vsd[i] == NULL) {
            vsd.erase(vsd.begin()+i);
            i--;
            vsdSize--;
        }
    }
    wxArrayString activeConnections;
    vsdSize = vsd.size();
    if(vsdSize > 0) {
        for(int i = 0; i < vsdSize; i++) {
            std::string desc = vsd[i]->sdef->getName();
            desc  += ": ";
            desc += vsd[i]->username;
            activeConnections.Add((wxString)desc);
        }
        wxListBox* newActiveServersList = new wxListBox( mainPanel, wxID_ANY, wxDefaultPosition, wxSize( 800,100 ), activeConnections, wxLB_MULTIPLE );
        wxListBox* oldActiveServersList = activeServersList;
        mainPanelSizer->Replace((wxWindow*)oldActiveServersList, (wxWindow*)newActiveServersList);
        activeServersList = newActiveServersList;
        delete oldActiveServersList;
        activeServersList->Connect( wxEVT_COMMAND_LISTBOX_SELECTED, wxCommandEventHandler( mainWindow::activeServersListOnListBox ), NULL, this );
    } else {
        wxListBox* newActiveServersList = new wxListBox( mainPanel, wxID_ANY, wxDefaultPosition, wxSize( 800,100 ), 0, NULL, wxLB_MULTIPLE );
        wxListBox* oldActiveServersList = activeServersList;
        mainPanelSizer->Replace((wxWindow*)oldActiveServersList, (wxWindow*)newActiveServersList);
        activeServersList = newActiveServersList;
        delete oldActiveServersList;
    }
    this->Layout();
}

void mainWindow::refreshStatusButtonOnButtonClick( wxCommandEvent& event ) {
    wxArrayInt selections;
    int selectionsLength;
    selectionsLength = activeServersList->GetSelections(selections);
    if(selectionsLength == 1) {
        int idx = selections.Item(0);
        std::string statusLabel = "Last command status: ";
        statusLabel += vsd[idx]->status;
        std::string exitCodeLabel = "";
        std::stringstream ssecl;
        ssecl.str("");
        ssecl << "Command exit status: ";
        if(vsd[idx]->lastcode == -1) {
            ssecl << "None";
        } else {
            ssecl << vsd[idx]->lastcode;
        }
        exitCodeLabel = ssecl.str();
        wxStaticText* newLastCommandStatusLabel = new wxStaticText(mainPanel, wxID_ANY, (wxString)statusLabel);
        wxStaticText* oldLastCommandStatusLabel = lastCommandStatusLabel;
        mainPanelSizer->Replace((wxWindow*)oldLastCommandStatusLabel, (wxWindow*)newLastCommandStatusLabel);
        delete oldLastCommandStatusLabel;
        lastCommandStatusLabel = newLastCommandStatusLabel;
        wxStaticText* newCommandExitCodeLabel = new wxStaticText(mainPanel, wxID_ANY, (wxString)exitCodeLabel);
        wxStaticText* oldCommandExitCodeLabel = commandExitCodeLabel;
        mainPanelSizer->Replace((wxWindow*)oldCommandExitCodeLabel, (wxWindow*)newCommandExitCodeLabel);
        delete oldCommandExitCodeLabel;
        commandExitCodeLabel = newCommandExitCodeLabel;
        wxTextCtrl* newCommandOutputText = new wxTextCtrl(mainPanel, wxID_ANY, (wxString)(vsd[idx]->lastout), wxDefaultPosition, wxSize( 800,150 ), wxHSCROLL|wxTE_MULTILINE|wxTE_READONLY);
        wxTextCtrl* oldCommandOutputText = commandOutputText;
        mainPanelSizer->Replace((wxWindow*)oldCommandOutputText, (wxWindow*)newCommandOutputText);
        delete oldCommandOutputText;
        commandOutputText = newCommandOutputText;
        wxTextCtrl* newCommandErrorText = new wxTextCtrl(mainPanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 800,150 ), wxHSCROLL|wxTE_MULTILINE|wxTE_READONLY|wxTE_WORDWRAP);
        wxTextCtrl* oldCommandErrorText = commandErrorText;
        mainPanelSizer->Replace((wxWindow*)oldCommandErrorText, (wxWindow*)newCommandErrorText);
        delete oldCommandErrorText;
        commandErrorText = newCommandErrorText;
        this->Layout();
    }
}

void mainWindow::closeButtonOnButtonClick( wxCommandEvent& event ) {
    int vsdSize = vsd.size();
    for(int i = 0; i < vsdSize; i++) {
        vsd[i]->close();
        delete vsd[i];
    }
    RCF::Client::ServerDefinition::free();
    this->Close();
}

void mainWindow::executeThreadNeedsParameterHandler( wxThreadEvent& event) {
    int threadId = event.GetInt();
    bool cont = false;
    do {
        sharedParameterCS.Enter();
        if(!sharedParameterModified) {
            cont = true;
        } else {
            sharedParameterCS.Leave();
        }
    } while(!cont);
    std::string title = "";
    ExecuteThread* thread = tv[threadId];
    title += "[";
    title += thread->getServerName();
    title += ": ";
    title += thread->getLoggedUsername();
    title += "] Enter command parameter - wxRCF v. 0.1 (C) 2015 by Phitherek_";
    wxString wxtitle = title;
    paramQueryDialog pqdialog(this, wxtitle, &sharedParameter);
    pqdialog.ShowModal();
    sharedParameterModified = true;
    sharedParameterForThreadId = threadId;
    sharedParameterCS.Leave();
}

connectDialog::connectDialog(wxWindow* parent, ServerData** target_sd): connectDialogBase(parent) {
    _target_sd = target_sd;
}

void connectDialog::connectDialogBaseOnInitDialog( wxInitDialogEvent& event ) {
    RCF::Common::HelperFunctions hf;
    boost::filesystem::path sdpath = hf.getHomeDirectory();
    sdpath += "/.rcfclient/sdefs";
    wxArrayString sdsChoices;
    bool enableChoice = false;
    if(boost::filesystem::exists(sdpath)) {
        boost::filesystem::directory_iterator sditer(sdpath);
        boost::filesystem::directory_iterator enditer;
        std::vector<std::string> sds;
        while(sditer != enditer) {
            if((*sditer).path().extension().string() == ".rcfsdef") {
                sds.push_back((*sditer).path().stem().string());
            }
            sditer++;
        }
        int sdsSize = sds.size();
        if(sdsSize == 0) {
            sdsChoices.Add(wxT("No defined servers"));
        } else {
            enableChoice = true;
            sdsChoices.Add("Choose a server...");
            for(int i = 0; i < sdsSize; i++) {
                sdsChoices.Add((wxString)sds[i]);
            }
        }
    } else {
        sdsChoices.Add(wxT("No defined servers"));
    }
    wxChoice* newServersChoice = new wxChoice(this, wxID_ANY, wxDefaultPosition, wxSize( 200,-1 ), sdsChoices, 0);
    wxChoice* oldServersChoice = serversChoice;
    connectDialogSizer->Replace((wxWindow*)oldServersChoice, (wxWindow*)newServersChoice);
    serversChoice = newServersChoice;
    delete oldServersChoice;
    serversChoice->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( connectDialog::serversChoiceOnChoice ), NULL, this );
    serversChoice->SetSelection(0);
    serversChoice->Enable(enableChoice);
    userEntry->Enable(false);
    passwordEntry->Enable(false);
    connectButton->Enable(false);
    this->Layout();
}

void connectDialog::serversChoiceOnChoice( wxCommandEvent& event ) {
    if(serversChoice->GetSelection() == 0) {
        userEntry->Enable(false);
        passwordEntry->Enable(false);
        connectButton->Enable(false);
    } else {
        userEntry->Enable(true);
        passwordEntry->Enable(true);
        connectButton->Enable(true);
    }
}

void connectDialog::cancelButtonOnButtonClick( wxCommandEvent& event ) {
    this->Close();
}

void connectDialog::connectButtonOnButtonClick( wxCommandEvent& event ) {
    try {
        std::string choice = serversChoice->GetString(serversChoice->GetSelection()).ToStdString();
        ServerData* sdata = new ServerData();
        sdata->sdef = RCF::Client::ServerDefinition::load(choice);
        boost::asio::io_service service;
        sdata->cli = new RCF::Client::Client(service, sdata->sdef->getName());
        sdata->cli->handshake();
        std::string username = userEntry->GetLineText(0).ToStdString();
        std::string password = passwordEntry->GetLineText(0).ToStdString();
        sdata->cli->authorize(username, password);
        sdata->username = username;
        *_target_sd = sdata;
        this->Close();
    } catch(std::exception& e) {
        std::string msg = "Caught exception: ";
        msg += e.what();
        wxMessageDialog errorDialog(this, (wxString)msg, wxT("Exception occured"));
        errorDialog.ShowModal();
    }
}

aboutDialog::aboutDialog(wxWindow* parent): aboutDialogBase(parent) {}

void aboutDialog::aboutDialogBaseOnInitDialog( wxInitDialogEvent& event ) {
    event.Skip();
}

void aboutDialog::aboutCloseButtonOnButtonClick( wxCommandEvent& event ) {
    this->Close();
}

listDialog::listDialog(wxWindow* parent, std::vector<ServerData*> processedServers): listDialogBase(parent) {
    _processedServers = processedServers;
}

void listDialog::listDialogBaseOnInitDialog( wxInitDialogEvent& event ) {
    std::vector<std::vector<std::string> > queryLists;
    int processedServersSize = _processedServers.size();
    for(int i = 0; i < processedServersSize; i++) {
        std::string rawlist = _processedServers[i]->cli->list();
        std::vector<std::string> queryList;
        int rawlistLength = rawlist.length();
        std::string line = "";
        std::vector<std::string> queryParts;
        for(int j = 0; j < rawlistLength; j++) {
            if(rawlist[j] == '\n' || rawlist[j] == '\r') {
                if(line != "") {
                    if(line[0] == '[' && line[line.length()-1] == ']') {
                        if(line.substr(1, 3) == "end") {
                            queryParts.pop_back();
                        } else {
                            queryParts.push_back(line.substr(1, line.length()-2));
                        }
                    } else {
                        std::string query = "";
                        int queryPartsSize = queryParts.size();
                        for(int k = 0; k < queryPartsSize; k++) {
                            query += queryParts[k];
                            query += ':';
                        }
                        query += line;
                        queryList.push_back(query);
                    }
                }
                line = "";
            } else {
                line += rawlist[j];
            }
        }
        queryLists.push_back(queryList);
    }
    std::vector<std::string> finalQueryList;
    int queryListsSize = queryLists.size();
    std::vector<std::string> firstQueryList = queryLists[0];
    int firstQueryListSize = firstQueryList.size();
    for(int i = 0; i < firstQueryListSize; i++) {
        bool include = true;
        for(int j = 1; j < queryListsSize; j++) {
            bool inlist = false;
            int queryListSize = queryLists[j].size();
            for(int k = 0; k < queryListSize; k++) {
                if(queryLists[j][k] == firstQueryList[i]) {
                    inlist = true;
                    break;
                }
            }
            if(!inlist) {
                include = false;
                break;
            }
        }
        if(include) {
            finalQueryList.push_back(firstQueryList[i]);
        }
    }
    CommandGroup list;
    list.name = "";
    int finalQueryListSize = finalQueryList.size();
    if(finalQueryListSize > 0) {
        for(int i = 0; i < finalQueryListSize; i++) {
            list.appendByQuery(finalQueryList[i]);
        }
        wxTreeItemId rootTreeItem = commandListTree->AddRoot("Command list");
        list.addToTreeCtrl(commandListTree, rootTreeItem);
        commandListTree->ExpandAll();
    } else {
        wxMessageDialog errorDialog(this, wxT("No commands available!"), wxT("Error occured"));
        errorDialog.ShowModal();
        this->Close();
    }
}

void listDialog::listCancelButtonOnButtonClick( wxCommandEvent& event ) {
    this->Close();
}

void listDialog::listExecuteButtonOnButtonClick( wxCommandEvent& event ) {
    wxTreeItemId selection = commandListTree->GetSelection();
    if(!commandListTree->ItemHasChildren(selection)) {
        std::string query = "";
        wxTreeItemId iter = selection;
        while(iter != commandListTree->GetRootItem()) {
            query.insert(0, commandListTree->GetItemText(iter).ToStdString());
            iter = commandListTree->GetItemParent(iter);
            if(iter != commandListTree->GetRootItem()) {
                query.insert(0, ":");
            }
        }
        int processedServersSize = _processedServers.size();
        for(int i = 0; i < processedServersSize; i++) {
            ExecuteThread* thrd = new ExecuteThread(dynamic_cast<mainWindow*>(this->GetParent())->tv.size(), query, _processedServers[i], &wxRCF::execute, dynamic_cast<mainWindow*>(this->GetParent()));
            thrd->Run();
            dynamic_cast<mainWindow*>(this->GetParent())->tv.push_back(thrd);
        }
        this->Close();
    } else {
        wxMessageDialog errorDialog(this, wxT("Cannot execute a category!"), wxT("Error occured"));
        errorDialog.ShowModal();
    }
}

paramQueryDialog::paramQueryDialog(wxWindow* parent, wxString& title, std::string* paramTarget): paramQueryDialogBase(parent, wxID_ANY, title) {
    _paramTarget = paramTarget;
}

void paramQueryDialog::paramQueryDialogBaseOnInitDialog( wxInitDialogEvent& event ) {
    event.Skip();
}

void paramQueryDialog::paramContinueButtonOnButtonClick( wxCommandEvent& event ) {
    *_paramTarget = paramInput->GetLineText(0).ToStdString();
    this->Close();
}
