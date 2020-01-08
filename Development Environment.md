**Development**

[TOC]
***

# Organization (or lack thereof) 

It is being done using an RVR connected to my desktop Ubuntu Mate Linux using an [Adafruit USB to TTL Serial Cable - Debug / Console Cable for Raspberry Pi](https://www.adafruit.com/product/954). The cable works with 3.3v signals so is not actually TTL since that implies 5v signals.

> Note: The document shipped with the RVR has the pinout for the UART incorrect. Sphero has a [note](https://sdk.sphero.com/docs/getting_started/before_you_start/uart_disclaimer/) about this.The pinout on the RVR is correct. 

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

# Cross Compiling and Running

My plan is to use an [Up Board](https://up-board.org/up/specifications/) to control the Rvr. From the web site:

> UP is a credit-card sized board with the high performance and low power consumption features of the latest tablet technology: the Intel® ATOM™ x5-Z8350 Processors 64 bits up to 1.92GHz. The internal GPU is the new Intel Gen 8 HD 400 with 12 Execution Units up to 500MHz to deliver extremely high 3D graphic performance. UP is equipped with 2GB/4GB DDR3L RAM and 16GB/32GB eMMC.

Since this is an Atom the code written on my Linux desktop will also run on the Up Board loaded with the same OS. It could even handle development environment but I prefer to stay on the desktop. 

The Up is now mounted and can be connected to the serial port on the Rvr. Before I go into the details for the Up, you can setup ]cross compiling C++ for the Raspberry Pi](https://hackaday.com/2016/02/03/code-craft-cross-compiling-for-the-raspberry-pi/). Read the comments because there are some additional tips and updates from readers that may help.  

Here is how I setup the cross compile capability for the Up Board.

## Remote System

On the remote system install a debug server:

```
sudo apt-get install gdbserver
```

Create a directory where the executables will reside. I used ~/Rvr.  

When ready to run tests access the remote through SSH or a terminal, possibly using VNC. First change to the directory  you created and start the `gdbserver` (*responses are in []*):

```
cd ~/Rvr
gdbserver --multi mysticlakelinux.local:2001
[Listening on port 2001]
```
The name of my desktop is `mysticlakelinux`. Use the network name for your desktop in the command. That's all for now on the remote system.

## Desktop System

The desktop system needs a version of the Gnu debugger. It is probably already loaded but if not load it with:

``` 
sudo apt install gdb-multiarch
```

Change to the directory where the executable reside or something above it so you don't have to type a long path name. Start a seesion by entering the next command. The `-q` option turns off some text output at startup. You'll get a prompt `(gdb)` where you enter the following commands:
 

```
gdb-multiarch -q

target extended-remote mystic-rvr.local:2001
[Remote debugging using mystic-rvr.local:2001]

remote put rvrLeds rvrLeds
set remote exec-file rvrLeds
run
[bunch of text if it works]
```
You should also see information on the remote system either output from the executable or error messages that may help determine what is not working. 

There is a greaat deal more to working with remote debugging. Do a search for these two versions of GDB for more information. 




