# SunTracker
Diploma project

## Roadmap

1. Components
   - 2 axis tower that accomplishes the following:
     - [ ] supports the solar panel
     - [ ] supports light sensors
     - [ ] supports rotation horizontally
     - [ ] supports rotation vertically
   - [ ] temperature sensors
   - [ ] humidity sensors
   - [ ] athmospheric pressure sensor
   - [ ] wind speed sensor
   - [ ] graphic LCD screen
   - [ ] 2 DC motors for turret rotation
   - [ ] WiFi module
   - [x] outlet power source 

2. What to do
   - [ ] Wifi connection to a web server for transmit or receive information
   - [ ] create a backend web server with database (django and !?)
   - [ ] (optional) statistics
   - [ ] (optional) adjust parameters based on location and season
   - [ ] 3D print the turret - have a good design
     - [ ] incorporates the rotating mechanism
     - [ ] easily attachable sonar panel and sensors
     - [ ] can be opened to check insides
     - [ ] robust
   - [ ] graphic LCD screen to display system status
     - [ ] temperature
     - [ ] humidity
     - [ ] Wi-Fi connection
     - [ ] (optional) atmospheric pressure
     - [ ] (optional) wind speed
   - figure out how to move the solar panel ( choose from below )
     - use an api with locates the sun and move the solar panel as best as you can to point at that position
     - create a custom object to allign with incomming sun light such that it has the least projection
    - [ ] at the end create a user manual
    - [ ] (important) have tracking information to easily write the project documentation