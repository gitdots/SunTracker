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
   - [x] WiFi module
   - [x] outlet power source 

2. What to do
   - [x] Wifi connection to a web server for transmit or receive information
   - [x] create a backend web server with database (django and !?)
   - [x] (optional) statistics
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
    - [x] at the end create a user manual
    - [ ] (important) have tracking information to easily write the project documentation



# Preconditions for running the project

The project was built on a Raspberry Pi 2 model B v1.1 and the running of the program is directly impacted by the CPU ping numbering. If the project is tried to be run on other hardware than this it has a high chance of not working.
The instructions for building and running the project are presented below. For a more detailed explanation you can check the Appendix of the documentation which is written in Romanian language for further user guidance.

# Tools to be used

The project comes with a small display screen. If there is no display screen attached to the Raspberry Pi Hardware please take notes on the following:

*Some tools that can be used for accesing the shell are:*
1. Putty
2. MobaXTerm 

The Raspberry Pi linux shells can be then accesed through SSH connection using one of those software tools.

# Building & Running the project

## Building

For building the *appl* application please use do the following commands:
1. navigate to application directory:
        *cd /home/corona/shared/aaa/SunTracker/appl*
2. build the application in the directory:
        *make all*

The second application is the *webserver* which is not required to be built and can be run freely.

## Running

For running the project you will need to run both of the applications:
1. open 2 separate shells
2. in the **1st shell** navigate as above to the first application:
    *cd /home/corona/shared/aaa/SunTracker/appl*
3. in the **2nd shell** navigate to the project directory:
    *cd /home/corona/shared/aaa/SunTracker*
4. activate python virtual environment
    *source env/bin/activate*
5. navigate to the project:
    *cd webserver*

Now, the setup is ready in both shells and execution of program can start. For this, run the next commands:
1. **1st shell**: *./appl*
2. **2nd shell**: *python3 app.py* 

The execution of the program is now started and you can open in the browser the webpage by going to: *raspberrypi.local:5000*.

**NOTE**: If the device is connected to a WLAN, you should be able to acces the website on any device that is connected to the same network.

## *New possible way of running that handles stopping as well:*

Now, the setup is ready in both shells and execution of program can start. For this, run the next commands:
1. **1st shell**: *./appl&* (to run in background)
2. **2nd shell**: *python3 app.py* 

**For stopping**:
In the **1st shell** run: *./stop.sh*