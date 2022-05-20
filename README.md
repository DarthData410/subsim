[![pipeline status](https://gitlab.com/chris_nada/subsim/badges/master/pipeline.svg)](https://gitlab.com/chris_nada/subsim/-/commits/master) 

[//]: # ([![coverage report]&#40;https://gitlab.com/chris_nada/subsim/badges/master/coverage.svg&#41;]&#40;https://gitlab.com/chris_nada/subsim/-/commits/master&#41; )

# Submarine Simulation
+ w/Singleplayer/Multiplayer (including computer controlled opponents)
+ Immersive simulation-like submarine handling 
    + Active + Passive Sonar
    + Noise simulation model
    + Manual Torpedo firing resolution
    + no arcade-style HUD
+ combined with adapted Deatchmatch/TDM/Domination (as in UT) and Warzone (as in ARMA) - like gameplay modes.
    + Team-play
    + Capturing of zones for points
    + Research & Upgrade your team's submarines + armaments
+ *no fixed theme (WWI/WWII/cold war/modern/sci-fi) yet, engine is kept flexible to support them all, but focus is on modern with fictional upgrade system*
+ *(unnamed as of now, ideas welcome)*

After above goals are implemented, future plans include:
+ More Object types (Surface ships and aircraft)

# TODOs
**Bold** = High Priority<br/>
(?) = Idea / Unsure if necessary<br/>
<font color='grey'>Grey Text</font> = Probably Later

## Gameplay / Meta / Strategy
+ [ ] Teambase: Repair/Resupply/Change sub
+ [ ] Replace Player's sub on death
+ [ ] Spawn new AI subs (-timer? -points?)
+ [ ] Research
    + [ ] Torpedo models
    + [ ] Passive Sonar models
    + [ ] Motor models
    + [ ] Sub models
    + [ ] Decoys
+ [ ] (?) Zones spawn Ships; Have them reach home for Points
+ [ ] Mainmenu - game setup
    + [ ] Nicknames for players
    + [ ] New Game / Team setup
    + [ ] (?) Attack / Defend gameplay instead of 'Equal Encounter'
    + [ ] (?) Deathmatch (FFA / no Teams)
+ [ ] <font color='grey'>Surface Ships</font>
+ [ ] <font color='grey'>Aircraft</font>
+ [ ] Save/Load Game

## AI
+ [x] **Evade**
    + [x] Use Decoys
    + [ ] Hide
+ [x] **fire Torpedos**

## Armament
+ [ ] **Multiple Launch Tubes**
+ [ ] Fix+Test Sonar Blindspots
+ [ ] **AS Pings temporär skalieren**
+ [x] Decoys
    + [ ] **Kommando**
    + [ ] Decoy effectiveness balancing
+ [x] Active Sonar detects depths
+ [X] Torpedos kill
+ [x] Torpedo Targeting choice test + improve
+ [ ] Torpedo Activate to change course / Waypoints

## UI
+ [X] Improve / Make Map Screen less rough
    + [X] Show scale px : km/~~sm~~
    + [x] Terrain Renderer (VertexArray based for now)
    + [ ] Clean up Map Settings Window
    + [ ] (?) Bug: Possible Map Depth discrepancy view/model
+ [x] Passive Sonar: Bearing Markers
+ [x] Passive Sonar: Bug: vector::_M_range_check: __n (which is 432) >= this->size() (which is 432)
+ [x] Active Sonar: Bearing Markers
+ [x] Active Sonar: Range Markers
+ [X] New Screen: Navigation / Controls
  + [ ] O2 Ventilation
  + [ ] Emergency Surface
+ [ ] (?) Speed -> knots
+ [ ] Weapons Screen

## Simulation
+ [X] Basic Physics Simulation
    + [X] Object Movement
    + [X] Distance and bearing calculations
+ [ ] Damage model
+ [X] Terrain
    + [X] Terrain from ~~Height Map~~ procedural Simplex Noise
    + [x] Synchronize with Host
    + [ ] Land: Collision
      + [x] Adjust sub's start position if collision detected
      + [ ] Adjust zone position to sea only
    + [ ] Land: AI Pathfinding around)
+ [x] Depth-rudder
    + [ ] **Depth influences stealth**

## Code
+ [ ] Convert all stdouts / error messages to english
+ [ ] Read Subs from xml/yaml
