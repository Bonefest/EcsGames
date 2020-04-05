#ifndef DIALOGDATABASE_H_INCLUDED
#define DIALOGDATABASE_H_INCLUDED

#include <string>
#include <memory>
#include <map>

using std::shared_ptr;
using std::string;
using std::map;

#include "../Dependencies/entt.hpp"
#include "../Dependencies/json.hpp"
#include "../common.h"

#include "Dialog.h"

class DialogDatabase {
public:
    void loadDialogs(const string& fileName);
    Dialog getDialog(ID dialogID);
private:
    shared_ptr<Replica> replicasFactory(nlohmann::json& info);
    Text parseText(nlohmann::json& stateText);

    map<ID, Dialog> _dialogs;
};


#endif // DIALOGDATABASE_H_INCLUDED
