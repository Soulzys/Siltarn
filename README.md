# Siltarn

This project is about the making of a RPG-FPS multiplayer game with Unreal Engine 5 (v5.1.1).

As it is a work in progress, you are strongly advised to only clone the master branch if you wish to try a safe version of the game. Any other branch is susceptible to weird behavior, potentially leading to a crash. 


## Current features

* Aim down sights (ADS) 
* Item "cycle"
    - Picking item
    - Moving item within player's inventory
    - Dropping item (either one by one, either through the "group drop" functionality*)
    - Interacting with external item container (currently only an item bag)
    - Picking item back from either the bag or the world


_Group drop functionality :_
Any item from the player's inventory can be set for group drop by RIGHT MAJ + RIGHT CLICK on it. Once set for group drop, the item will be highlighted. When the player then drags and drops any of the highlighted item, all of the highlighted items will be throwned into an item bag and dropped into the world. 


## Future features

* Equipping items
* Drag and drop items from player inventory to external inventory and vice versa
* Implementation of persistency through a database (probably MySQL) via a web app (using .NET Core)
* Aiming up and down done through static animation poses instead of procedurally dealing with spine rotation


## Controls

* Moving around                               : WSAD
* Aiming                                      : right click
* Interacting with objects (e.g. items, bags) : E
* Open/Close player inventory                 : I
* Switching camera                            : CTRL + Numpad 0, 1, 2, 3, 4 (respectively "default", "right", "left", "top", "front")
* Open/Close escape menu                      : Escape


## To keep in mind

I am a junior developer and this project is a work in progress that I work on during my sparetime. The formatting of the code is not always consistent and the overall architecture of the project is most likely not optimal. Every line of code may be subject to change at some point. 