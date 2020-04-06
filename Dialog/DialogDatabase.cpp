#include "DialogDatabase.h"
#include "Dialog.h"

#include <fstream>

using std::shared_ptr;
using std::make_shared;

#include "../helper.h"

#include "Replica.h"
#include "ReplicaAction.h"

void DialogDatabase::loadDialogs(const string& dialogsFile) {
    nlohmann::json parsedDialogs;
    std::ifstream file(dialogsFile);
    file >> parsedDialogs;
    for(auto dialogInfo : parsedDialogs) {
        Dialog dialog;
        dialog.startText = parseText(dialogInfo["text_data"]);
        for(auto replica : dialogInfo["replicas"]) {
            dialog.replicas.push_back(replicaBuilder(replica));
        }

        dialog.nextDialog = dialogInfo.value("next_default_dialog", 0);
        _dialogs[dialogInfo["id"]] = dialog;
    }

}

Dialog DialogDatabase::getDialog(ID dialogID) {
    return _dialogs[dialogID];
}

shared_ptr<Replica> DialogDatabase::replicaBuilder(nlohmann::json& info) {
    shared_ptr<Replica> replica = make_shared<Replica>(parseText(info["text_data"]));
    if(info.contains("switch_dialog_id")) {
        replica->addReplicaAction(make_shared<SwitchDialog>(info["switch_dialog_id"], parseText(info["text_answer"])));
    }

    if(info.contains("finish")) {
        replica->addReplicaAction(make_shared<CloseDialog>());
    }

    if(info.contains("next_default_dialog")) {
        replica->addReplicaAction(make_shared<SetNextDefaultDialog>(info["next_default_dialog"]));
    }

    return replica;
}

Text DialogDatabase::parseText(nlohmann::json& text) {
    return Text{text["text"], parseColor(text["color"])};
}

cocos2d::Color3B DialogDatabase::parseColor(vector<int> color) {
    return Color3B(color[0], color[1], color[2]);
}
