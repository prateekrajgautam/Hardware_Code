https://core-electronics.com.au/tutorials/how-to-network-xbee-and-arduino.html

> Want to setup a wireless XBee network between some Arduinos and your PC? We're going to show you how. We will be using Series 1 XBee modules alongside some XBee Explorer USB modules from Sparkfun to set up our own Personal Area Network (PAN). All the parts needed to have two devices talk to each other are in this Kit too, so pick up one and let's get started. We're using a Windows 10 PC, with the XBee configuration application, X-CTU. We're using Series 1 modules as they are the easiest to work with but beware, they aren't compatible with other Series Xbee Modules. Before you kick off, get a better understanding of the direction and language we'll use by reading up these two articles:

What are XBee Modules?
Serial Communications with the Arduino Uno

Configure XBees with X-CTU

A Screenshot of X-CTU software with the important buttons shown
The only way we can configure the network settings of our modules is to download the program X-CTU by Digi (Digi are the XBee manufacturers). Run through the install as you would with any Windows application.
Next grab your XBee Explorer USB modules out, we've got the USB Explorer and the Mini-B Explorer. You'll only really need one for the setup of your modules, though, and it comes in the SparkFun kit so that shouldn't be a problem. With any networking, there are some essential pieces of information devices need to communicate with each other. We will be using X-CTU to make sure that those pieces of information match up and are compatible. By default, all series one modules will have the same communication settings, but we're going to change ours so that we've got a (more) private XBee network!
Orient your XBee Module correctly, there are white outlines on your Explorer Module, and insert it into the pin headers, then plug it into a spare USB port on your PC.
At this point, you should have downloaded and installed X-CTU and have an Explorer with XBee module plugged into your PC. To access the network setting for the module, we need to add the device to our X-CTU application. Following these quick steps, referring to the image, to quickly add your module to X-CTU:

Select the

# How to network XBee and the Arduino - Tutorial
Want to setup a [wireless XBee network](https://core-electronics.com.au/wireless/zigbee-xbee.html) between some Arduinos and your PC? We're going to show you how. We will be using [Series 1 XBee modules](http://core-electronics.com.au/search?q=xbee&fq[category]=General&fq[category_id]=13) alongside some XBee Explorer USB modules from [Sparkfun](http://core-electronics.com.au/brands/sparkfun-australia) to set up our own Personal Area Network (PAN). All the parts needed to have two devices talk to each other are in [this Kit](https://core-electronics.com.au/catalog/product/view/sku/KIT-13197) too, so pick up one and let's get started. We're using a Windows 10 PC, with the XBee configuration application, X-CTU. We're using Series 1 modules as they are the easiest to work with but beware, they aren't compatible with other Series Xbee Modules. Before you kick off, get a better understanding of the direction and language we'll use by reading up these two articles:

*   [What are XBee Modules?](http://core-electronics.com.au/tutorials/what-are-xbee-modules.html)
*   [Serial Communications with the Arduino Uno](http://core-electronics.com.au/tutorials/serial-communications-arduino-uno.html)

Configure XBees with X-CTU
--------------------------

![XCTU XBee Setup Application screenshot](https://core-electronics.com.au/media/wysiwyg/tutorials/aidan/x-ctu-screenshot.png)

A Screenshot of X-CTU software with the important buttons shown

The only way we can configure the network settings of our modules is to [download the program X-CTU by Digi](https://www.digi.com/products/xbee-rf-solutions/xctu-software/xctu) (Digi are the XBee manufacturers). Run through the install as you would with any Windows application.

Next grab your [XBee Explorer USB](https://core-electronics.com.au/catalog/product/view/sku/WRL-11697) modules out, we've got the USB Explorer and the Mini-B Explorer. You'll only really **need one** for the setup of your modules, though, and it comes in [the SparkFun kit](https://core-electronics.com.au/catalog/product/view/sku/KIT-13197) so that shouldn't be a problem. With any networking, there are some essential pieces of information devices need to communicate with each other. We will be using X-CTU to make sure that those pieces of information match up and are compatible. By default, all series one modules will have the same communication settings, but we're going to change ours so that we've got a (more) private XBee network!

Orient your XBee Module correctly, there are white outlines on your Explorer Module, and insert it into the pin headers, then plug it into a spare USB port on your PC.

At this point, you should have downloaded and installed X-CTU and have an Explorer with XBee module plugged into your PC. To access the network setting for the module, we need to add the device to our X-CTU application. Following these quick steps, referring to the image, to quickly add your module to X-CTU:

*   Select the "**Add new device**" button within X-CTU.
*   Select the relevant **COM port** from the window that appears; it will be listed as USB Serial Port
*   Choose the **Finish** option and wait for the following progress bars to complete
*   Your Xbee Module is listed in the left-hand window

XBee configuration: MY and Destination Addresses
------------------------------------------------

Select your XBee board in the left menu (if it appears as a blue box, it is selected). Now click on the **Configuration** icon in the top-right of the application. You'll see a lengthy list of settings you can alter, with a **Refresh** symbol and **Pencil** symbol to the left of them. There's no need to be daunted; it's not as complicated as it looks. Let's just talk about the settings we need to worry about.

**Setting**

**Explanation**

**Range**

Channel (CH)

Uses the channel of wireless network specified (a,b,g,n,etc.) you want this to match for your two modules.

16

PAN ID (ID)

The ID of the network the module will broadcast on. This must match for XBee modules you want to communicate.

0x0 - 0xFFFF

Destination Address High (DH)

This value onway to communicate directly to a particular module (hence destination). We typically set this to 0 for point-to-point communication.

0x0 - 0xFFFF

Destination Address Low (DL)

This is the My Address of the board you are trying to communicate with.

0x0 - 0xFFFF

My Address (MY)

This is the address you pick for the board you are currently programming. If multiple boards have the same MY, they will all receive data sent to that address.

0x0 - 0xFFFF

You might want to take the time to **read that table again**; it will make the entire setup a lot easier if you understand the nature of the DL-MY settings, especially that they will alternate for each module.

Follow the guidance in the above table to pick values for each of those settings, and ignore all the rest! Once you have everything changed on your first module, click the Write icon in the grey bar at the top of the window. Now you will write all these settings to your module. The next step is to remove your XBee Module and insert your other one. Scroll back up to the top of this section and re-do all the steps there. Be aware that you will be prompted to re-add a device (it isn't an error) and select OK.

If you've got two Explorer modules, you can connect both of your XBees up to your PC at the same time. Heck, you can even connect them up to separate PCs if you like. Just follow these steps to test your newly founded wireless data connection:

*   Select your first XBee module from the X-CTU menu; we will just call this little guy A
*   Click on the Serial Monitor Icon in the top right of the screen
*   Select the Connect button
*   Change to your other module, XBee B
*   Repeat steps 2 and 3 for B
*   Type some text into the Console Log; it will appear blue in your console.
*   Swap to the other module, A, and you should see that text in the console as red text.
*   If you don't, you've probably done something wrong (make sure you saved your settings!)

Applying XBee communication to the Arduino
------------------------------------------

So now we have two XBee boards that are configured to communicate with each other, if we want to add more boards to this network (OOOOH MESH NETWORK TIME, Nah that's a future tutorial) we will do it using X-CTU. To manage data being sent over the network we can use a bunch of different tools, we'll use the Arduino IDE.

As with any new tech we pick up, we're going to need to install the library for it in the Arduino IDE. Open your IDE and get ready to send some data! Head to Sketch > Libraries > Manage Libraries and search XBee. Once you've got it installed, there are a few things we need to specify in a sketch to be able to read/write data between Arduinos. Firstly, we're using SoftwareSerial to enable serial communication on any of the pins on our Arduino. Using the XBee Shield, we will need to setup our XBee as communicating on pins 2 and 3. Do this by using the function

SofwareSerial XBee(2,3)

But do it after you include the software serial library in the sketch

Then we can begin the serial data transfer with the XBee module using

Xbee.begin(9600);

Now we can do any number of things remotely, so long as we setup the handling of the data correctly.

We're going to write out pin 13 LED High from our X-CTU console as an example. This example sketch is directly from SparkFun's XBee Remote Control Guide:

#include SoftwareSerial XBee(2, 3); // Arduino RX, TX (XBee Dout, Din) void setup() { // Initialize XBee Software Serial port. Make sure the baud // rate matches your XBee setting (9600 is default). XBee.begin(9600); printMenu(); // Print a helpful menu: } void loop() { // In loop() we continously check to see if a command has been //  received. if (XBee.available()) { char c \= XBee.read(); switch (c) { case 'w': // If received 'w' case 'W': // or 'W' writeAPin(); // Write analog pin break; case 'd': // If received 'd' case 'D': // or 'D' writeDPin(); // Write digital pin break; case 'r': // If received 'r' case 'R': // or 'R' readDPin(); // Read digital pin break; case 'a': // If received 'a' case 'A': // or 'A' readAPin(); // Read analog pin break; } } } // Write Digital Pin // Send a 'd' or 'D' to enter. // Then send a pin # //   Use numbers for 0-9, and hex (a, b, c, or d) for 10-13 // Then send a value for high or low //   Use h, H, or 1 for HIGH. Use l, L, or 0 for LOW void writeDPin() { while (XBee.available() < 2) ; // Wait for pin and value to become available char pin \= XBee.read(); char hl \= ASCIItoHL(XBee.read()); // Print a message to let the control know of our intentions: XBee.print("Setting pin "); XBee.print(pin); XBee.print(" to "); XBee.println(hl ? "HIGH" : "LOW"); pin \= ASCIItoInt(pin); // Convert ASCCI to a 0-13 value pinMode(pin, OUTPUT); // Set pin as an OUTPUT digitalWrite(pin, hl); // Write pin accordingly } // Write Analog Pin // Send 'w' or 'W' to enter // Then send a pin # //   Use numbers for 0-9, and hex (a, b, c, or d) for 10-13 //   (it's not smart enough (but it could be) to error on //    a non-analog output pin) // Then send a 3-digit analog value. //   Must send all 3 digits, so use leading zeros if necessary. void writeAPin() { while (XBee.available() < 4) ; // Wait for pin and three value numbers to be received char pin \= XBee.read(); // Read in the pin number int value \= ASCIItoInt(XBee.read()) \* 100; // Convert next three value += ASCIItoInt(XBee.read()) \* 10; // chars to a 3-digit value += ASCIItoInt(XBee.read()); // number. value \= constrain(value, 0, 255); // Constrain that number. // Print a message to let the control know of our intentions: XBee.print("Setting pin "); XBee.print(pin); XBee.print(" to "); XBee.println(value); pin \= ASCIItoInt(pin); // Convert ASCCI to a 0-13 value pinMode(pin, OUTPUT); // Set pin as an OUTPUT analogWrite(pin, value); // Write pin accordingly } // Read Digital Pin // Send 'r' or 'R' to enter // Then send a digital pin # to be read // The Arduino will print the digital reading of the pin to XBee. void readDPin() { while (XBee.available() < 1) ; // Wait for pin # to be available. char pin \= XBee.read(); // Read in the pin value // Print beggining of message XBee.print("Pin "); XBee.print(pin); pin \= ASCIItoInt(pin); // Convert pin to 0-13 value pinMode(pin, INPUT); // Set as input // Print the rest of the message: XBee.print(" = "); XBee.println(digitalRead(pin)); } // Read Analog Pin // Send 'a' or 'A' to enter // Then send an analog pin # to be read. // The Arduino will print the analog reading of the pin to XBee. void readAPin() { while (XBee.available() < 1) ; // Wait for pin # to be available char pin \= XBee.read(); // read in the pin value // Print beginning of message XBee.print("Pin A"); XBee.print(pin); pin \= ASCIItoInt(pin); // Convert pin to 0-6 value // Printthe rest of the message: XBee.print(" = "); XBee.println(analogRead(pin)); } // ASCIItoHL // Helper function to turn an ASCII value into either HIGH or LOW int ASCIItoHL(char c) { // If received 0, byte value 0, L, or l: return LOW // If received 1, byte value 1, H, or h: return HIGH if ((c \== '0') || (c \== 0) || (c \== 'L') || (c \== 'l')) return LOW; else if ((c \== '1') || (c \== 1) || (c \== 'H') || (c \== 'h')) return HIGH; else return \-1; } // ASCIItoInt // Helper function to turn an ASCII hex value into a 0-15 byte val int ASCIItoInt(char c) { if ((c \>= '0') && (c <= '9')) return c \- 0x30; // Minus 0x30 else if ((c \>= 'A') && (c <= 'F')) return c \- 0x37; // Minus 0x41 plus 0x0A else if ((c \>= 'a') && (c <= 'f')) return c \- 0x57; // Minus 0x61 plus 0x0A else return \-1; } // printMenu // A big ol' string of Serial prints that print a usage menu over // to the other XBee. void printMenu() { // Everything is "F()"'d -- which stores the strings in flash. // That'll free up SRAM for more importanat stuff. XBee.println(); XBee.println(F("Arduino XBee Remote Control!")); XBee.println(F("============================")); XBee.println(F("Usage: ")); XBee.println(F("w#nnn - analog WRITE pin # to nnn")); XBee.println(F("  e.g. w6088 - write pin 6 to 88")); XBee.println(F("d#v   - digital WRITE pin # to v")); XBee.println(F("  e.g. ddh - Write pin 13 High")); XBee.println(F("r#    - digital READ digital pin #")); XBee.println(F("  e.g. r3 - Digital read pin 3")); XBee.println(F("a#    - analog READ analog pin #")); XBee.println(F("  e.g. a0 - Read analog pin 0")); XBee.println(); XBee.println(F("- Use hex values for pins 10-13")); XBee.println(F("- Upper or lowercase works")); XBee.println(F("- Use 0, l, or L to write LOW")); XBee.println(F("- Use 1, h, or H to write HIGH")); XBee.println(F("============================")); XBee.println(); }

To write (w) digital pin 13 (d in hexadecimal) high (1), we just need to send the command wd1 through our X-CTU console.

As you type the third character your Arduino will send back, it's current instruction in the form of  
"Setting Digital Pin d to HIGH."

And coincidentally, the onboard LED will light up. Alternatively, sending a wd0 will turn it off.

![XCTU XBee Setup Application screenshot of console of connected data being displayed](https://core-electronics.com.au/media/wysiwyg/tutorials/aidan/console-log-screenshot_1.png)

You can now read and write any pin of your microcontroller over the air with XBee! The setup is just that smooth, we've just made one up using Arduino boards and a PC! You could do this with multiple microcontrollers just as easily, or add buttons into the microcontroller that allow you to send functions between 2 different microcontrollers. The real power behind XBee networking comes in setting up meshes of modules. Meshes piggy-back off each other to increase the range of the entire network. You can grab new XBee Series 1 modules (like the ones used in this article) or if you don't have any lying around grab the Xbee Kit from Sparkfun. Need any help or want to take your XBee knowledge to the next level and create something truly unique? Let us know in the comments below, and we will do our best to help you out! Thanks for taking the time to checkout how Xbee works.
