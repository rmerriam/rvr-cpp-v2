**RVR C++**

[TOC]
***
# Introduction
This repository is work in progress on a C++ implementation of the  serial communications protocol for the RVR robot. It is being done using an RVR connected to my desktop Ubuntu Mate Linux using an [Adafruit USB to TTL Serial Cable - Debug / Console Cable for Raspberry Pi](https://www.adafruit.com/product/954). The cable works with 3.3v signals so is not actually TTL since that implies 5v signals.

Note: The document shipped with the RVR has the pinout for the UART incorrect. Sphero has a [note](https://sdk.sphero.com/docs/getting_started/before_you_start/uart_disclaimer/) about this.The pinout on the RVR is correct. 

***
# References

Here is what I'm using to decipher the protocol.

* [RVR SDK](https://sdk.sphero.com/)
* [RVR Sensor & Colors Cards](https://sdk.sphero.com/docs/general_documentation/sensors/)
* [RVR Docs](https://sdk.sphero.com/docs/sdk_documentation/connection/) - reference to API 


The [Wiki](https://bitbucket.org/rmerriam/rvr-cpp/wiki/Home) I'm writing as I go explains the details of the protocol with some code snippets.

# Organization (or lack thereof) 

This is a work in progress so the organization of the repository is not as clean as one might like. That's how I work when tackling a new project. Eventually it gets better organized. 

## Eclipse

A factor that may cause confusion is the use of Eclipse as the development environment. That leads to an organization that is different from other open source projects. Again, eventually it will get better. There are also Eclipse artifacts in the repository like the `.systems` directory and `.cproject` and `.project` files. 

## src Directory

The `src` directory contains the meat of the protocol work. It should eventually become a shared library.

## api, sysinfo, leds, power Directories

These directories contain a single file that exercises / demonstrates using the code fromm the `src` directory. They send a set of requests to the RVR and read the responses. There is output of the requests and response packets in hex. Look for additional similar directories as I tackle the sensors and multi-threading for response processing. 

Note: There isn't much error checking on the inputs for requests. Nor much explanation on what goes into a request. This is especially tricky for LEDs. Right now I'm more interested in completing as much of the protocol as possible. Will return for cleanup and documentation. 

## Directories with 'test' in the name

Here there be dragons. These are where I'm working out details. You probably don't want to look at them. They'll change quickly and unexpectedly.  

# Issues

The **[Issues](https://bitbucket.org/rmerriam/rvr-cpp/issues?status=new&status=open)** capability is open. Feel free to enter problems or questions. I'll get to them when I can.

# C++ Version

I'm using GCC 9.2.1 and using this to learn more about current C++, especially the 2020 release. If you are using older compilers there may be issues. Post them as an Issue and I'll consider reverting but not to any version older than C++ 17, please. I don't use modern C++ techniques just to obscure the code, only to make the code more effective. 

# License

This is totally free to anyone who wants to risk using it for personal or commercial use. I don't issue any assurances that any of it works properly so all liability of use falls on you. 
