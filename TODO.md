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
+ [ ] (?) Attack / Defend gameplay instead of 'Equal Encounter'
+ [ ] <font color='grey'>Surface Ships</font>
+ [ ] <font color='grey'>Aircraft</font>

## AI
+ [ ] **Evade** 
  + [ ] Use Decoys
  + [ ] Hide
+ [ ] **fire Torpedos**

## Armament
+ [ ] **Multiple Launch Tubes**
+ [ ] Fix+Test Sonar Blindspots
+ [ ] **AS Pings temporär skalieren** 
+ [x] Decoys 
  + [ ] **Kommando**
  + [ ] Decoy effectiveness balancing
+ [x] Active Sonar detects depths
+ [X] Torpedos kill 
  + [ ] Damage model
+ [x] Torpedo Targeting choice test + improve

## UI
+ [X] Improve / Make Map Screen less rough
  + [X] Show scale px : km/~~sm~~
  + [X] Terrain Renderer (VertexArray based for now)
  + [ ] Clean up Map Settings Window
+ [x] Passive Sonar: Bearing Markers
+ [x] Passive Sonar: Bug: vector::_M_range_check: __n (which is 432) >= this->size() (which is 432)
+ [x] Active Sonar: Bearing Markers
+ [x] Active Sonar: Range Markers
+ [ ] New Screen: Navigation / Controls
+ [ ] (?) Speed -> knots

## Simulation
+ [X] Basic Physics Simulation
  + [X] Object Movement 
  + [X] Distance and bearing calculations
+ [X] Terrain
  + [X] Depth from ~~Height Map~~ Simplex Noise
  + [ ] Synchronize with Host
  + [ ] Land: Collision
  + [ ] Land: AI Pathfinding around)
+ [x] Depth-rudder
  + [ ] **Depth influences stealth**

## Code
+ [ ] Convert all stdouts / error messages to english
+ [ ] Read Subs from xml/yaml
