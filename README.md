# JEC - About
JEC is a Self-Driving Robot. It uses two 28BYJ Stepper Motors to move in different Directions and scans its Surrounding using an Ultrasonic Sensor. <br>
Whenever it can't move foreward, it's looking for another Way to move.
<br><br>
# JEC - Code
JEC is coded in C++ and compiled for Arduino Mega 2560. Using the Arduino IDE is recommended. <br>
All required Libraries are included in the Folder "Code" -> "Libraries".
<br><br>
# JEC - Models
There are STL Files included in the Folder "Models". All Models are sorted by their Generation. Models may vary between different Generations. <br>
Before printing the Models, have a Look at the "REDME" File in the corresponding Model Folder. They can include Printing Tips or Notes about which Material to use for a specific Model.
<br><br>
# JEC - Generations
<h3>Generation 0</h3>
<h4>JEC-0.7G</h4>

JEC Gen0.7 is a Prototype of the JEC Robot. It's based on JEC-0.6G, but now it's stopping whenever the Robot gets stuck. 

<h4>JEC-0.6G</h4>

JEC Gen0.6 is a Prototype of the JEC Robot. It's based on JEC-0G, but now it's logging the previous Route with up to 128 Route Elements. It's also checking the Distance of a Direction before driving, therefore whilst it is in Manual Route Mode it can't bump into Stuff. As a side Improvement, the Source Code has been beautified.

<h4>JEC-0.5G</h4>

JEC Gen0.5 is an alternative JEC Prototype that drives along a Wall with a somewhat constant Distance of about 20cm. It uses the exact same Hardware as JEC Gen0. 

<h4>JEC-0G</h4>

JEC Gen0 is a Prototype of the JEC Robot. It's driving along in it's Enviornment without a Target. Whenever it can't move foreward, it's looking for the most suitable Way to move next. In this case, it is the direction with the longest free Space.
