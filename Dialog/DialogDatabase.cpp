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
    vector<shared_ptr<Replica>> replicas;
    if(info.contains("switch_dialog_id")) {
        replicas.push_back(make_shared<SwitchDialogReplica>(info["switch_dialog_id"],
                                                            parseText(info["text_answer"]),
                                                            parseText(info["text_data"])));
    }

    if(info.contains("finish")) {
        replicas.push_back(make_shared<CloseDialogReplica>(parseText(info["text_data"])));
    }

    if(info.contains("next_default_dialog")) {
        replicas.push_back(make_shared<SetNextDefaultDialogReplica>(info["next_default_dialog"],
                                                                    parseText(info["text_data"])));
    }

    if(replicas.size() > 1) {
        auto result = make_shared<MultipleReplica>();
        for(auto replica: replicas)
            result->addReplica(replica);

        result->setReplicaText(parseText(info["text_data"]));
        return result;
    } else if(replicas.size() == 1) return replicas.front();

    return nullptr;
}

Text DialogDatabase::parseText(nlohmann::json& text) {
    return Text{text["text"], parseColor(text["color"])};
}

cocos2d::Color3B DialogDatabase::parseColor(vector<int> color) {
    return Color3B(color[0], color[1], color[2]);
}
