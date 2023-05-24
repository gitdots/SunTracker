# SunTracker
Diploma project

## Roadmap

1. Components
   - 2 axis tower that accomplishes the following:
     - [x] supports the solar panel
     - [x] supports light sensors
     - [x] supports rotation horizontally
     - [x] supports rotation vertically
   - [x] temperature sensors
   - [x] humidity sensors
   - [ ] (optional) athmospheric pressure sensor
   - [ ] (optional) wind speed sensor
   - [x] graphic LCD screen
   - [x] 2 DC motors for turret rotation
   - [ ] WiFi module
   - [x] outlet power source 

2. What to do
   - [x] Wifi connection to a web server for transmit or receive information
   - [x] create a backend web server with database (django and !?)
   - [ ] (optional) statistics
   - [ ] (optional) adjust parameters based on location and season
   - [x] 3D print the turret - have a good design
     - [x] incorporates the rotating mechanism
     - [x] easily attachable sonar panel and sensors
     - [x] can be opened to check insides
     - [x] robust
   - [x] graphic LCD screen to display system status
     - [x] temperature
     - [x] humidity
     - [x] Wi-Fi connection
     - [ ] (optional) atmospheric pressure
     - [ ] (optional) wind speed
   - figure out how to move the solar panel ( choose from below )
     - use an api with locates the sun and move the solar panel as best as you can to point at that position
     - create a custom object to allign with incomming sun light such that it has the least projection
    - [ ] at the end create a user manual
    - [ ] (important) have tracking information to easily write the project documentation