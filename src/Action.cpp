#include "../include/Action.h"
#include "../include/Session.h"
#include <iostream>
#include <utility>
#include "../include/User.h"
using namespace std;


BaseAction::BaseAction() :errorMsg(""),status(PENDING) {}

ActionStatus BaseAction::getStatus() const {
    return status;
}

void BaseAction::complete() {
    status = COMPLETED;
}

void BaseAction::error(const string &errorMsg1) {
    this->errorMsg = errorMsg1;
    status = ERROR;
}

string BaseAction::getErrorMsg() const {
    return errorMsg;
}

string BaseAction::printMsg() {
    return getErrorMsg();
}


CreateUser::CreateUser(string inputname, const string &inputalgo) :BaseAction(), name(move(inputname)),algorithm( inputalgo ) {
}


void CreateUser::act(Session &sess) {

    string result = sess.makeUser(name, algorithm);
    // update action status
    if (result == "userError") {
        error("there is already a user with that name!");
        cout << "Error -" << getErrorMsg() << endl;
    } else if (result == "algoError") {
        error("recommendation algorithm is invalid");
        cout << "Error -" << getErrorMsg() << endl;
    }
    if (result.empty())
        complete();
}

string CreateUser::toString() const {
    string st = "COMPLETED";
    if (getStatus() == PENDING)
        st = "PENDING";
    if (getStatus() == ERROR)
        st = "ERROR";
    return "Create user " + st;
}

BaseAction *CreateUser::clone() {
    return new CreateUser(*this);
}



ChangeActiveUser::ChangeActiveUser(const string &inputname):BaseAction(),name(inputname) {
}

void ChangeActiveUser::act(Session &sess) {
    string result = sess.changeUser(name);
    // update action status
    if (result == "error") {
        error("there is no user with that name!");
        cout << "Error -" << getErrorMsg() << endl;
    } else complete();

}

string ChangeActiveUser::toString() const {
    string st = "COMPLETED";
    if (getStatus() == PENDING)
        st = "PENDING";
    if (getStatus() == ERROR)
        st = "ERROR";
    return "Change active user " + st;
}

BaseAction *ChangeActiveUser::clone() {
    return new ChangeActiveUser(*this);
}

DeleteUser::DeleteUser(const string &inputname):BaseAction(),name(inputname) {
}

void DeleteUser::act(Session &sess) {
    string result = sess.deleteUser(name);
    // update action status
    if (result == "error") {
        error("there is no user with that name!");
        cout << "Error -" << getErrorMsg() << endl;
    } else complete();

}

string DeleteUser::toString() const {
    string st = "COMPLETED";
    if (getStatus() == PENDING)
        st = "PENDING";
    if (getStatus() == ERROR)
        st = "ERROR";
    return "delete user " + st;
}

BaseAction *DeleteUser::clone() {
    return new DeleteUser(*this);
}

DuplicateUser::DuplicateUser(const string &originalName, const string &newName):BaseAction(),originalName(originalName),newName(newName) {
}

void DuplicateUser::act(Session &sess) {
    string result = sess.duplicateUser(originalName, newName);

    if (result == "originalUser") {
        error("original username does not exist");
        cout << "Error -" << getErrorMsg() << endl;
    } else if (result == "newUser") {
        error("new username is already in use");
        cout << "Error -" << getErrorMsg() << endl;
    } else if (result.empty())
        complete();
}

string DuplicateUser::toString() const {
    string st = "COMPLETED";
    if (getStatus() == PENDING)
        st = "PENDING";
    if (getStatus() == ERROR)
        st = "ERROR";
    return "duplicate user " + st;
}

BaseAction *DuplicateUser::clone() {
    return new DuplicateUser(*this);
}

void PrintContentList::act(Session &sess) {
    sess.printContent();
    complete();
}

string PrintContentList::toString() const {
    return "print content list COMPLETED";
}

BaseAction *PrintContentList::clone() {
    return new PrintContentList(*this);

}

void PrintWatchHistory::act(Session &sess) {
    User *u=sess.getActiveUser();
    u->printHistory();
    complete();
}

string PrintWatchHistory::toString() const {
    return "print watch history COMPLETED";
}

BaseAction *PrintWatchHistory::clone() {
    return new PrintWatchHistory(*this);
}

void PrintActionsLog::act(Session &sess) {
    sess.printLog();
    complete();
}

string PrintActionsLog::toString() const {
    return "print action log COMPLETED";
}

BaseAction *PrintActionsLog::clone() {
    return new PrintActionsLog(*this);
}

void Watch::act(Session &sess) {
    sess.watch(id);
    complete();
}

string Watch::toString() const {
    return "watch COMPLETED";
}

Watch::Watch(const string &inputId):BaseAction(),id(inputId) {
}

BaseAction *Watch::clone() {
    return new Watch(*this);
}

void Exit::act(Session &sess) {
    complete();
}

string Exit::toString() const {
    return "exit COMPLETED";
}

BaseAction *Exit::clone() {
    return new Exit(*this);
}