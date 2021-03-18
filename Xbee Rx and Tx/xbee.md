[robolab.in](https://www.robolab.in/zigbee-xbee-s2c-how-to-configure-as-coordinator-router-end-device/)

# ZIGBEE XBEE S2C–How to configure as Coordinator , Router / End Device - Robolab Technologies Pvt. Ltd.

8-10 minutes

---

By Robolab Technologies In [Robotics](https://www.robolab.in/category/robotics/)

DIGI International has recently introduced the new ZIGBEE S2C Module .The previous S2 & the traditional S1 both are discontinued.

The new module is powerful with both UART & SPI communication .

Operating frequency band is The industrial, scientific and medical (ISM) radio band ISM 2.4 – 2.5 GHz

It utilizes Silicon labs EM357 transceiver

The Module is faster and has more RAM & flash memory with much reduced power consumption.

Increases the mesh capabilities & consumes under 1 micro amp sleep current.

Interface options : UART 1 Mb/s maximum (burst) , SPI 5 Mb/s maximum (burst)

Here is the comparison chart of S1 , S2 & S2C .The Indoor/Outdoor range of S2C has also increased much more as the transmit output power is double.Also it works on supply voltage from 2.1v to 3.6v DC.

![XBee](https://www.robolab.in/wp-content/uploads/2018/04/xbee.jpg)

Here are the device types of XBEE :

### Device types

ZigBee defines three different device types: coordinator, router, and end device.

**Coordinator:** ZigBee networks always have a single coordinator device. This device Starts the network, selecting the channel and PAN ID.Distributes addresses, allowing routers and end devices to join the network.Buffers wireless data packets for sleeping end device children.The coordinator manages the other functions that define the network, secure it, and keep it healthy. This device cannot sleep and must be powered on at all times.

**Router:** A router is a full-featured ZigBee node. This device can join existing networks and send, receive, and route information. Routing involves acting as a messenger for communications between other devices that are too far apart to convey information on their own.Can buffer wireless data packets for sleeping end device children. Can allow other routers and end devices to join the network.Cannot sleep and must be powered on at all times.May have multiple router devices in a network

**End device:** An end device is essentially a reduced version of a router. This device can join existing networks and send and receive information, but cannot act as messenger between any other devices.Cannot allow other devices to join the network.Uses less expensive hardware and can power itself down intermittently, saving energy by temporarily entering a nonresponsive sleep mode.Always needs a router or the coordinator to be its parent device. The parent helps end devices join the network and stores messages for them when they are asleep.

ZigBee networks may have any number of end devices. In fact, a network can be composed of one coordinator, multiple end devices, and zero routers.

### Modes of operation

The XBee ZigBee RF Module is in Receive Mode when it is not transmitting data. The device shifts into the other modes of operation under the following conditions:

— Transmit Mode (Serial data in the serial receive buffer is ready to be packetized)

— Sleep Mode

— Command Mode (Command Mode Sequence is issued, not available when using the SPI port)

Now let us practically see how to configure the Radio S2C. For communication one of the Radios to be configured as COORDINATOR & the other one as Router.

The Product family of S2C is XB24C (the previous S2 module belongs to XB24-ZB)

These S2C modules should be able to communicate with older modules, so long as they are all running compatible firmware (i.e. DigiMesh, ZB, etc..)

Following is the pin configuration of S2C.

![XBee Configuration](https://www.robolab.in/wp-content/uploads/2018/04/XBee_configuration.jpg)

To configure the XBEE you need an USB XBEE Adapter which can be easily connected to the USB port of your PC.

![](https://www.robolab.in/wp-content/uploads/2018/04/usb.jpg)

In case you do not have this USB board , you can use your Arduino board as USB-UART. For this you need to connect RST of Arduino to GND.This bypasses the bootloader of Arduino & the board can be used for Serial communication.Remember to connect Rx to Rx , Tx to Tx (straight & NOT reverse connection) between Arduino & XBEE while configuring.

You can watch this video on how to use Arduino to configure XBEE

### Let us start with the configuration.

1\. Plug the S2C modules on to the USB adapters & connect to USB ports of your PC.

2\. Open the new XCTU Software .S2C can be configured with this new XCT. The classic old XCtu does not support the S2C module.

3\. Click on the SEARCH icon on top to detect the USB ports.

4\. A list of active USB COM ports will be displayed.

5\. Select the COM ports where you’ve connected the USB adapters.To confirm you can verify your DEVICE MANAGER for the proper COM ports. In my case one of the USB adapter is allotted COM3 & the other one COM31.

![](https://www.robolab.in/wp-content/uploads/2018/04/image-1_thumb.png)

6\. Click on NEXT & accept the default PORT PARAMETERS. 96008N1 is the default. 9600 is the BAUD RATE, 8 Data Bits, No Parity & 1 Stop bit.

![](https://www.robolab.in/wp-content/uploads/2018/04/image-2.jpg)

7\. Click on FINISH. The XCTU scans the USB ports selected & lists the RADIOs found with their unique 64 bit address.

![](https://www.robolab.in/wp-content/uploads/2018/04/image-3.jpg)

8\. Select both the devices & click ADD SELECTED DEVICES. Now both the Radios appear in the left pane.

![](https://www.robolab.in/wp-content/uploads/2018/04/image-4.jpg)

### Let us configure the RADIO at COM3 as COORDINATOR first.

For this click on the COM3 RADIO to load the module settings.

![](https://www.robolab.in/wp-content/uploads/2018/04/image-5.jpg)

Once the parameter settings are loaded you can see that the product family is XB24C (in case of old S2 it is XB24-ZB & of S1 is 802.15.4)

The function set of Firmware is ZIGBEE TH Reg , the Reg stands for Regular & not PRO. TH stands for THROUGH HOLE & not SMD.

![](https://www.robolab.in/wp-content/uploads/2018/04/image-6.jpg)

First thing is to set the PAN ID of the Network. This can be from 0 to FFFF hex.In my case I’m setting it to 1234.The other Radios also to be set in the same PAN ID.

Scroll down further & Enable the CE (Coordinator Enable)

![](https://www.robolab.in/wp-content/uploads/2018/04/image-7.jpg)

The Destination address DH is left to default 0.

The Destination Address DL is set to hex FFFF which makes the Radio work on BROADCAST mode, so that it can communicate with all Radios in the same PANID.

The Node Identifier can be given any name like “Coordinator”.This naming is optional.

![](https://www.robolab.in/wp-content/uploads/2018/04/image-11_1.png)

Click on the PENCIL icon on top to WRITE the changes made.

![](https://www.robolab.in/wp-content/uploads/2018/04/image-8.jpg)

### Now let us configure the second Radio as ROUTER.

Click on the second Radio on the left pane to load the settings.

![](https://www.robolab.in/wp-content/uploads/2018/04/image-12.jpg)

The Router setting is quite simple.

Enter the PANID as 1234, same as that of Coordinator.

![](https://www.robolab.in/wp-content/uploads/2018/04/image-13.jpg)

JV CHANNEL VERIFICATION is Enabled

CE Coordinator is DISABLED

Destination Address DL is left to default 0. (0 is the default address of Coordinator)

![](https://www.robolab.in/wp-content/uploads/2018/04/rout1.jpg)

![](https://www.robolab.in/wp-content/uploads/2018/04/rout2.jpg)

Click on WRITE button to save the changes made.

![](https://www.robolab.in/wp-content/uploads/2018/04/image-14_thumb.jpg)

The modules are paired & ready for communication.

### Now let us test the communication.

On the XCTU window delete the second Radio.Click on the first Radio to load the settings.

Leaving the XCTU window open, start another instance of XCTU &position it to the left of the previous window.

![](https://www.robolab.in/wp-content/uploads/2018/04/image-15_thumb.jpg)

Click on the Search button on the new instance of XCTU & select the second Radio .

![](https://www.robolab.in/wp-content/uploads/2018/04/image-16_thumb.jpg)

Click on the Radio selected to load the settings.

![](https://www.robolab.in/wp-content/uploads/2018/04/image-17_thumb.jpg)

Now the ROUTER Radio is on the left side & the COORDINATOR Radio to the Right.

![](https://www.robolab.in/wp-content/uploads/2018/04/image-18_thumb.jpg)

Click the TERMINAL icon on both the windows to enter Terminal mode.

![](https://www.robolab.in/wp-content/uploads/2018/04/image-19_thumb.jpg)

Click on the SERIAL CONNECTION icon on both the windows to enter the serial connection mode.

![](https://www.robolab.in/wp-content/uploads/2018/04/image-20_thumb.jpg)

You can see the SERIAL Icon in LOCK mode & the AT CONSOLE Status changes to CONNECTED.

Now you can type any message inside console log window & see that received on the other Radio.The transmit message is in BLUE & received message in RED.

![](https://www.robolab.in/wp-content/uploads/2018/04/image-21_thumb.jpg)

![](https://www.robolab.in/wp-content/uploads/2018/04/802_1_thumb.jpg)

Once the Function set is selected , click on FINISH to load the firmware.

802.15.4 firmware is like that of our classic S1 module.But setting is slightly different.

CH channel can be left to default C

PANID to be selected , say 1234 ( the other Radio to be on same ID)

DL address is FFFF

CE coordinator enable for first Radio

![](https://www.robolab.in/wp-content/uploads/2018/04/802_2_thumb.jpg)

For the second Radio , to be set as END device

CH Channel C

PAN ID 1234 , same as Coordinator

MY address is 1 ( the coordinator MY is 0)

CE is set to END device

![](https://www.robolab.in/wp-content/uploads/2018/04/802_3_thumb.jpg)

Click on WRITE button & test the modules for communication.

In this 802.15.4 Firmware POINT TO POINT communication only is possible & NO MESH Networking.

In a nutshell, the new X2C module is powerful & can work as S2 or the old S1 module.

Watch this support video :

Source: [https://alselectro.wordpress.com/2017/01/23/zigbee-xbee-s2c-how-to-configure-as-coordinator-router-end-device/](https://alselectro.wordpress.com/2017/01/23/zigbee-xbee-s2c-how-to-configure-as-coordinator-router-end-device/)