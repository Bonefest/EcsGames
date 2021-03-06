#ifndef DIALOG_H_INCLUDED
#define DIALOG_H_INCLUDED

#include <map>
#include <string>
#include <vector>

using std::map;
using std::string;
using std::vector;
using std::shared_ptr;

#include "Replica.h"

struct Dialog {
    vector<shared_ptr<Replica>> replicas;
    Text startText;
    ID nextDialog;  //Next dialog after finishing this one
};

struct DialogInfo {
    explicit DialogInfo() : currentIndex(0) { }

    Text answer;
    Dialog dialog;
    std::size_t currentIndex;
    entt::entity dialogMember;
};

#endif // DIALOG_H_INCLUDED
