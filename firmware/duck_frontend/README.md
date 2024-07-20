# Duck Frontend
The frontend is a little web app that is served by the Microcontroller (ESP8266) acting as a server.

## Application Constraints
There are two main constraints for our web app: limited storage and no internet. A modern web application with a nice UI can be pretty big and often uses thrid party ressources (e.g. icons) that have to be fetched from other websites, on the internet. Did I mention that our duck probably wont have internet access ... yeah that's a bummer but also a nice challenge.
Luckily modern web development seems to be going into the direction of making production grade websites as efficient as possible and offline capable. This is achieved by introducing an extra development step, the "build" step. 

### How does the process work?
With this build pipeline paradigm all external resources are first installed into the project using a package manager. These packages are most often just a bunch of javaScript files. In the build process these files are simply transferred to a final dist folder. What is important here, is that most build systems try to minimize the file size by combining files, shortening variable names, leaving out code that is never used and so on.
The result should be a minimal set of files, that can be served by the microcontroller and that are as small as possible.

## Build Pipeline
This web frontend as of now, is written without a framework like vue or react. It is just pure vanilla JavaScript.
To build the web frontend I use vite with multiple plugins that can be found in the vite.config.js file.
To install packages, I use Node Package Manager npm. The packages are listed in the package.json file.
Last but not least, I have a custom build step that takes the generated files and puts them into c++ files that can be included in the firmware.
The script is written in python and called encodeInArduino.py.

### Custom Build Step for Arduino
The custom build step is necessary because the ESP8266 can only serve files that are stored in its flash memory. It could also be stored on the microcontrollers flash file system using SPIFFS or tinyFS. However, this makes setting up the website extra cumbersome, as you first have to flush the microcontroller and then upload the website files using additional tools and the Arduino IDE. This leaves a lot of room for error and is just generally annoying, when you just want to build a cool project.
That's why I have decided to say good by to my old ways and ditch the fie system all together. Instead, I just embed the website into the firmware as discussed. This means more complexity on my end but ease of use for anyone who wants to build a duck or a Mini Kenterprise.

## ToDo
Fix issue with automatic reset of joysticks. Probably caused by reconnection timer.