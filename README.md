**RVR C++**

![Talking Rvr](https://sdk.sphero.com/img/RVR_SDK_Icons/RVR_SDK_icons_RVR-Talking.png)

[TOC]
***
# Introduction

This repository is work in progress on a C++ implementation of the Rvr serial API communications protocol with an end goal of using the [Robot Operating System](https://www.ros.org/) (ROS). The library is usable for general robotics work without ROS. With the goal of ROS in mind note that some of the more *pure* educational portions probably will not be implemented, e.g color sensing. 

In addition to `rvr++` there are few utility and general testing programs in the repository. See the [development environment](Development Environment.md) page for details.  

***
# References

Here are the resource used to decipher the protocol.

* [RVR SDK](https://sdk.sphero.com/)
* [RVR Sensor & Colors Cards](https://sdk.sphero.com/docs/general_documentation/sensors/)
* [RVR Docs](https://sdk.sphero.com/docs/sdk_documentation/connection/) - reference to API 

The [Sphero Community](https://community.sphero.com/) is another useful resource. 

The [Wiki](https://bitbucket.org/rmerriam/rvr-cpp/wiki/Home) explains the details of the protocol. The Sphero staff and other Rvr owners have supplied details and experience that isn't otherwise documented. The Rvr is a brand new product and Sphero is adding to their documentation. 



# Code Issues

The **[Issues](https://bitbucket.org/rmerriam/rvr-cpp/issues?status=new&status=open)** capability is open. Feel free to enter problems or questions.

# C++ Version

Code development is with GCC 9.2.1 with C++17. The exception is the use of the `-fconcepts` flag that provides some early access to C++20 capabilities. 

# License

This is totally free to anyone who wants to risk using it for personal or commercial use. I don't issue any assurances that any of it works properly so all liability of use falls on you. 

