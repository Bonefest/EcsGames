# About

Roguelike classic turn-based game.

Key features:

Complex classes and races\
Huge number of skills\
Advanced system of abilities\
Agile crafting system\
Tons of unique dungeons\
Endless game session\
Follows canon roguelike rules


# TODO-lists

### Major

[__Status__: *active*] Flexible and extensible menu system\
[__Status__: *active*] Data-driven items system

[__Status__: *unactive*] Migrating to SDL\
[__Status__: *unactive*] Skills and abilities\
[__Status__: *unactive*] Fighting system\
[__Status__: *unactive*] Procedural dungeon generation\
[__Status__: *unactive*] Data-driven entities storage\
[__Status__: *unactive*] Classes and races\
[__Status__: *unactive*] User-friendly crafting system


[__Status__: *done*] Data-driven dialog system\
[__Status__: *done*] Vision and lighting system\
[__Status__: *done*] Hints system

### Minor

[__Status__: *unactive*] Multitext class - creating html-like text patterns\
[__Status__: *unactive*] Text scripting parser\
[__Status__: *unactive*] System pre-defined and user extensible game configuration

# Systems overview

### Dialog System

Consists of 5 main parts

*DialogDatabase* - Dialogs container. Main purposes are: loading dialogs from JSON and sharing them to other systems;\
*Dialog* - Container of replicas, greeting text and (optional) next default dialog id.\
*DialogInfo* - Structure for communicating between logic and view. Contains information about active dialog, dialog member, current dialog text.\
*ReplicaAction* - Represent an action of replica on activating.\
*Replica* - Represents replica itself. Consists of list of ReplicaAction.

Problems:

-Now it's impossible to create a dynamic dialog. We cannot build a dialog based on some state of game and are limiting ourself only to static built-in dialog.\
-Dynamic dialogs require some sort of scripting language and that's overkill.

Usage:

*TalkingState* on activating obtain information from dialog member about dialog id, asks *DialogDatabase* to create the dialog and initialize *DialogInfo* structure. Creates dialog view.\
*DialogView* obtain current active dialog and initialize a list of possible replicas.\
Whenever dialog changes or player selects other replica, *TalkingState* informs the *DialogView* about that.
