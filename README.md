# OBDII
OBDII
This is the ReadMe file for the Software Engineering OBDII project.

# Goals
This software should be able to connect with a USB or Bluetooth OBDII dongles.  These dongles are pretty dumb, but they are great serial terminals for using the OBDII protocol.  Once connected this software should discover the available sensors and attempt to read them all if data conversion routines have been written for the sensors.

# Parser/Generator
Along with connecting to the device and probing sensors a description file will be written for all types of OBDII codes the software can handle.  This file will likely be in JSON or YAML.  From these descriptions a parser/generator will create a series of objects or functions that will allow programmatic access to probe and clear individual sensors on a car.  This file will also allow for noting data conversion routines as well as good operating values for sensors where this information is readily available.

# Library Requirements
* Qt 5.4  https://www.qt.io/download-open-source/
** http://doc.qt.io/qt-5/modules-cpp.html
