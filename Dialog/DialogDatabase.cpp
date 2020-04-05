#include "DialogDatabase.h"
#include "Dialog.h"

#include <fstream>

using std::shared_ptr;
using std::make_shared;

#include "../helper.h"

void DialogDatabase::loadDialogs(const string& dialogsFile) {
    nlohmann::json parsedDialogs;
    std::ifstream file(dialogsFile);
    file >> parsedDialogs;
    for(auto dialogInfo : parsedDialogs) {
        Dialog dialog;
        dialog.startText = parseText(dialogInfo["text_data"]);
        for(auto replica : dialogInfo["replicas"]) {
            dialog.replicas.push_back(replicasFactory(replica));
        }

        dialog.nextDialog = dialogInfo.value("next_dialog", 0);
        _dialogs[dialogInfo["id"]] = dialog;
    }

}

Dialog DialogDatabase::getDialog(ID dialogID) {
    return _dialogs[dialogID];
}

shared_ptr<Replica> DialogDatabase::replicasFactory(nlohmann::json& info) {
    if(info["type"] == "finish") {
        return make_shared<CloseDialogReplica>(parseText(info["text_data"]));
    } else if(info["type"] == "switch_dialog") {
        return make_shared<SwitchDialogReplica>(info["dialog_id"],
                                                parseText(info["text_data"]),
                                                parseText(info["text_answer"]));
    }

    return nullptr;
}

Text DialogDatabase::parseText(nlohmann::json& stateText) {
    return Text{stateText["text"], integerToColor(stateText["color"])};
}
