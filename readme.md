 Task 1: Finding the Sea Floor

This task ensures Odysseus’ ship does not run aground by monitoring depth sensor data.  
We process sample depth readings recorded every second, clean corrupted values, and visualize them in real time.
The system provides both a clear graph and an animated display to allow timely intervention.


- Used Python (pandas) to read the sample CSV file containing depth values.
- Interpolated missing or corrupted values to maintain continuity.
- Plotted Depth vs Time using matplotlib.
- Animated the graph so a new data point appears every second, simulating real-time monitoring. 
- Added labels and titles for clarity:
  - X-axis: Time (s)  
  - Y-axis: Depth below sea surface (m)  
  - Title: Odysseus Ship Depth Monitoring

  see '![alt text](<odysseus ship depth monitoring.png>)


- Applied Savitzky–Golay filter to smooth noisy sensor data.    
- Added a danger threshold line (e.g., 50 m) to visually warn when the ship is too close to the seabed.


 Code Documentation

  - Data cleaning.
  - Noise reduction using filtering.
  - Real-time animation logic.



Task 2: Keeping watch over Odysseus 

This project simulates Odysseus’ journey using an Arduino Uno, sensors, LCD, LED, buzzer, and a push button.  
The system is implemented as a state machine with the following states:  
- OPEN SEA 
- STORM
- CHARYBDIS
- ANCHOR DROPPED  
- WRECKED

i added a startup grace period to ensure the journey begins in OPEN SEA, and implemented hysteresis thresholds for both light and distance sensors to stabilize transitions.

  Wiring (Tinkercad) :
  
  - Ultrasonic sensor: TRIG → pin 9, ECHO → pin 10, VCC → 5V, GND → GND.  
  - LDR → A0.  
  - LCD wired to pins 7,6,5,4,3,2.  
  - LED → pin 12, Buzzer → pin 11, Button → pin 8.  

 See `screenshots/tinkercad arduino.png`


Code Documentation:

  - Sensor setup  
  - State machine transitions  
  - Startup grace period logic  
  - Hysteresis for light and distance sensor