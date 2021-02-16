https://spin.atomicobject.com/2016/07/18/xbee-tutorial/

> The setup (and some background knowledge) needed for a basic XBee setup—tutorial includes step-by-step instructions, screen grabs, and sample code.

# Get Started with XBee - A Beginner's Tutorial
You may have heard the news: Atomic Object is moving to a [new building](https://spin.atomicobject.com/2015/09/16/new-building/) later this summer.  We’re excited about designing the new space.  For my part, I found an opportunity to try out a technology that I have not worked with before, the wireless networking system [XBee](http://www.digi.com/lp/xbee).

In addition to migrating furniture, computers, and people down the street, we will also be moving an updated version of our “Callaloo” [bathroom monitor system](https://spin.atomicobject.com/2014/05/16/radio-system-from-scratch/) to our new home. This system, built by a former Atom, works as an occupancy sensor for each of our bathrooms, illuminating a green or red light on the wall to signify which bathrooms are available. Bathroom availability should not be a significant issue in the new space, but the monitor has become a useful and familiar part of our office. And looking forward, installing wireless mesh-network infrastructure will be a good experience: developing our “intranet of places” using current internet of things technology.

This blog post will cover the steps (and some background knowledge) needed for a basic XBee setup. The goal is to get two XBee modules to communicate. We’ll have a basic input (a button) on one side, which will remotely trigger an output (an LED) on the other side. This models the basic functionality of the overall Callaloo system: proximity sensors on the door (input) are transmitted to a receiving module that changes an output (indicator lights, server information) based on the data it receives.

Get to Know XBee
----------------

First, let’s talk about [ZigBee](http://www.digi.com/products/xbee-rf-solutions/rf-modules/xbee-zigbee)\*, a low-power wireless mesh network standard operating in the 2.4 GHz range. XBee is not the same as ZigBee—instead, XBee is a brand of radio communication modules (made by [Digi](http://www.digi.com/)) that can support a number of protocols, including ZigBee, 802.15.4, WiFi, etc. Its range is 10 to 30 meters. ZigBee is often used in home automation products, though it is not the only option.

_\*Bonus fact: ZigBee is named after the “[waggle dance](https://www.youtube.com/watch?v=-7ijI-g4jHg)” that bees use to point other bees to food sources._

Gather Your Supplies
--------------------

For this tutorial, I used:

*   Two (2) Series 1 XBee Antenna Modules, available [here](https://www.sparkfun.com/products/11215)
*   Two (2) XBee Explorer USB Modules, available [here](https://www.sparkfun.com/products/11812), and USB cables
*   Two (2) Arduino Unos and USB cables
*   Two small breadboards (or one larger breadboard)
*   Jumper wires
*   Male [headers](https://www.sparkfun.com/products/10112) (0.1”) to be soldered onto the Explorer modules
*   One (1) 100 ohm resistor
*   One (1) 5.1k ohm resistor
*   One (1) button
*   One (1) LED
*   XCTU, available for download [here](http://www.digi.com/support/productdetail?pid=3352)

I chose Arduinos because I happened to have two on hand. You could use any dev boards that have a serial port.

Note: there are kits available for the XBee antenna and Explorer USB modules, such as [this one](http://www.trossenrobotics.com/p/Xbee-Communication-Starter-Kit.aspx) from Trossen Robotics.

Configure Your XBees and Get ‘Em Talking
----------------------------------------

You will need to configure your XBee modules so they can communicate.  The first part of this configuration involves setting the Channel, PAN ID, and Address values.

*   **Channel**: The channel calibrates the operating frequency within the 2.4GHz 802.15.4 band. Your XBees must be on the same channel to communicate with one another.
*   **PAN ID** (Personal Area Network ID): Your XBees must share the same PAN ID to communicate with one another. You can choose a value between 0 and 0xFFFF.
*   **Addressing**: Each XBee has a source address (referred to as “MY address”) and a destination address (which has an upper half, Destination High or DH, and a lower half, Destination Low or DL).  An XBee’s destination address specifies to which source address it can send data. You can specify a universally unique address by using the 64-bit address printed on the back of the module, use a shorter 16-bit address (unique within a network), or use a string of text (e.g., “Alice’s radio”).

Additionally, each XBee in a network plays a role. The three role options are Coordinator, End Device, and Router. Each network has exactly one Coordinator, which serves as the root of the network tree. A network can have multiple Routers; these can forward information to end devices and also run application functions. Lastly, End Devices cannot relay data, but only talk to a parent node (either a Coordinator or Router). A network can have multiple End Devices.

With that, let’s configure our XBees.

1.  Download and install [XCTU](http://www.digi.com/support/productdetail?pid=3352). Available for both Windows and Mac.
2.  Plug your first XBee into an Explorer module, and connect to your computer’s USB port via a USB cable.
3.  Open XCTU and click “Discover devices.”  
    ![XBee_discover_devices](https://spin.atomicobject.com/wp-content/uploads/20160628125532/XBee_discover_devices-590x542.png)
4.  Select the port to be scanned. Then on the next page, select the settings as shown below. Click “Finish.”  
    ![XBee_search_settings](https://spin.atomicobject.com/wp-content/uploads/20160628125700/XBee_search_settings-590x678.png)
5.  Your device should appear on the “Devices discovered” list. Click “Add selected devices” for your module.![XBee_device_found](https://spin.atomicobject.com/wp-content/uploads/20160628125934/XBee_device_found-590x673.png)
6.  With the Gear icon selected, click the radio module in the left-hand menu. This should open up a long list of settings.  
    ![XBee_settings](https://spin.atomicobject.com/wp-content/uploads/20160628130100/XBee_settings.png)
7.  Pick a channel (between 0x0 and 0xFFFF). Remember, each XBee must share the same channel number. I chose an arbitrary value of “0x0C.”
8.  Pick a PAN ID. As before, both modules must have the same number.
9.  For this simple demo, I went with the addressing configuration shown below (the 16-bit option from the address options previously discussed).

XBee Module #1

XBee Module #2

Destination High (DH)

0

0

Destination Low (DL)

2

1

MY Address

1

2

11.  Lastly, select “Coordinator” from the Coordinator Enable (CE) dropdown list. After you’ve made all your changes, click “Write.”
12.  Now unplug the first XBee, and repeat steps 2 through 10 with the second XBee, but with the addresses as shown in the second column of the table above. On this second XBee, you can leave the Coordinator Enable (CE) set as 0, or EndUser. I recommend you mark each module so you can easily tell them apart.

Run a Communication Test
------------------------

1.  Now it’s time to see if they’ll talk. Plug one XBee (on its Explorer module) into a USB port (via USB cable), and plug the other XBee (on its Explorer module) into another USB port. In XCTU, scan for devices as before. You should see both devices available. Select both of them and click “Add selected devices.”  
    ![Select both XBees](https://spin.atomicobject.com/wp-content/uploads/20160628125251/XBee_select_both-590x674.png)
2.  Click one of the modules in the left-hand column. Now select the Console icon to view the console. Click “Open.”  
    ![XBee_console](https://spin.atomicobject.com/wp-content/uploads/20160628125254/XBee_console-590x354.png)
3.  Repeat for the other module, opening up a console.
4.  Type into one console. You should see the result echoed back in the other console. If so, congrats!

![XBee Demo!](https://spin.atomicobject.com/wp-content/uploads/20160628125043/XBee_demo.gif)

Set Up Your Circuits
--------------------

The next step is to connect each XBee module to an Arduino, so we can connect our input (button) and output (LED).  Again, this mimics the future bathroom monitor system, where inputs will be processed by a microcontroller and transmitted to a receiving microcontroller, which will change its outputs accordingly.

At this point, you will need to solder the headers on your Explorer boards. Connect the ground and 3.3V from the Arduino to the ground and power rails of your breadboard. Connect the power and ground rails of the breadboard to the 3.3V and GND pins on your Explorer module.

![XBee_InputSideCircuit](https://spin.atomicobject.com/wp-content/uploads/20160628191043/XBee_InputSideCircuit-590x424.png)

Input-Side (Button) Circuit

Next, connect the XBee Explorer pin OUT to Arduino pin 2 (RX), and XBee pin IN to Arduino pin 3 (TX). I chose to use the SoftwareSerial library and Arduino pins 2 and 3 (rather than the default pin 0 and 1 for serial communication), so that I could use the default serial pins for printing debug statements to a serial monitor.

Connect your button to the Arduino as shown in the diagram above. I wanted the button to default to logic low when not pressed, so I added a pull-down resistor to my circuit. If you don’t have a button, you can just use a piece of wire as your “switch,” connecting it to either power or ground.

Your first Arduino is now set up! To set up the other Arduino, repeat the connections for OUT, IN, 3.3V and GND. Connect the LED by connecting a wire from Pin 9 to the 100 ohm resistor, and then put in the LED. Make sure that the LED is in the right orientation. Remember that the shorter “leg” of the LED (the cathode) must be on the “ground” side.

![XBee_OutputSideCircuit](https://spin.atomicobject.com/wp-content/uploads/20160628191039/XBee_OutputSideCircuit-590x415.png)

Output-Side (LED) Circuit

At this point, I like to debug my circuit by writing a quick Arduino sketch to make sure that my LED and button are functioning as expected. Otherwise, I might conflate a communication issue with a simple wiring or port setting issue.

Write the Code
--------------

Using Arduinos makes the demo code fairly simple, although the final Callaloo will be a custom PCB design, not a pre-made development board (i.e., not using Arduino).

On the input (button) side, we are checking for a button input and writing a character to the XBee via a serial connection.

    
    #include "SoftwareSerial.h"
    
    SoftwareSerial XBee(2, 3);
    int BUTTON = 8;
    
    void setup()
    {
      
      pinMode(BUTTON, INPUT);
      XBee.begin(9600);
    }
    
    void loop()
    {
      if (digitalRead(BUTTON) == HIGH)
      {
        XBee.write('H');
        delay(50);
      }
    }
    

On the output side, we are checking if we received a certain character, and if so, turning on the LED by setting that bit high.

    
    #include "SoftwareSerial.h"
    
    SoftwareSerial XBee(2, 3);
    int LED = 9;
    
    void setup()
    {
      
      XBee.begin(9600);
      pinMode(LED, OUTPUT);
    }
    
    void loop()
    {
      if (XBee.available())  
      { 
        char c = XBee.read();
        if (c == 'H')
        {
          digitalWrite(LED, HIGH);
          delay(50);
        }
        else
        {
          digitalWrite(LED, LOW);
        }
      }
      else
      {
        digitalWrite(LED, LOW);
      }
    }
    

Now you should have a basic XBee network up and running! The next step for the Callaloo system is to expand the XBee network with more modules and prototype the bathroom door sensors.
