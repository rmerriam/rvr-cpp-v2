**RVR C++**

[TOC]
***
# Introduction

This repository is work in progress on a C++ implementation of the serial API communications protocol for the RVR robot. The end goal is a library `rvr++` that accesses the Rvr capabilities necessary for robotics work. Some of the more *pure* educational portions probably never will be implemented, e.g color sensing. 

In addition to `rvr++` there are few utility and general testing programs in the repository. See the [development environment](Development Environment.md) page for details.  

***
# References

Here are the resource used to decipher the protocol.

* [RVR SDK](https://sdk.sphero.com/)
* [RVR Sensor & Colors Cards](https://sdk.sphero.com/docs/general_documentation/sensors/)
* [RVR Docs](https://sdk.sphero.com/docs/sdk_documentation/connection/) - reference to API 

The [Sphero Community](https://community.sphero.com/) is another useful resource. The Sphero staff and other Rvr owners provided details that aren't otherwise documented. 

The [Wiki](https://bitbucket.org/rmerriam/rvr-cpp/wiki/Home) explains the details of the protocol and some details of the Rvr gleaned from experiements and community discussions. 


# Code Issues

The **[Issues](https://bitbucket.org/rmerriam/rvr-cpp/issues?status=new&status=open)** capability is open. Feel free to enter problems or questions.

# C++ Version

I'm using GCC 9.2.1 and using this to learn more about current C++, especially the 2020 release. If you are using older compilers there may be issues. Post them as an Issue and I'll consider reverting but not to any version older than C++ 17, please. I don't use modern C++ techniques just to obscure the code, only to make the code more effective. 

# License

This is totally free to anyone who wants to risk using it for personal or commercial use. I don't issue any assurances that any of it works properly so all liability of use falls on you. 

