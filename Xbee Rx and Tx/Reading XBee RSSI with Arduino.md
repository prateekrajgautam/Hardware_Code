https://davidbeath.com/posts/reading-xbee-rssi-with-arduino.html

> A tutorial on how to read RSSI from an XBee on Arduino.

# Reading XBee RSSI with Arduino
Last year I spent quite a lot of time trying to read the [Recieved Signal Strength Indicator](https://en.wikipedia.org/wiki/Received_signal_strength_indication) from an XBee for a project I was working on. I had planned to blog about the process, but never got round to writing any more than two posts. However, I had a look at the nascent blog for the first time in a while, and it surprised me that it was getting a respectable number of views, especially for something that wasn't particularly informative and hadn't been updated in so long. Having a look at the analytics, I realised that like I been doing, there are still a lot of people trying to figure out how to read RSSI from an XBee to an Arduino. So here's a tutorial and some pointers for getting XBees set up for signal strength reading.

The first thing you're going to need is of course a couple of [XBee](https://en.wikipedia.org/wiki/XBee) units. You'll also need an XBee usb connector like [this](https://www.sparkfun.com/products/8687) to configure them from your computer. For this tutorial I'll explain how to read the signal strength with an [Arduino](https://www.arduino.cc/), though you can just use a PC.

_PLEASE NOTE_: Unless you want to set up a [Wireless mesh network](https://en.wikipedia.org/wiki/Wireless_mesh_network), you should use the [XBee 802.15.4](http://www.digi.com/products/wireless-wired-embedded-solutions/zigbee-rf-modules/point-multipoint-rfmodules/xbee-series1-module) modules, also known as the Series 1 modules. The Series 1 operates a standard point to point network. The Series 2 modules, the [XBee ZB](http://www.digi.com/products/wireless-wired-embedded-solutions/zigbee-rf-modules/zigbee-mesh-module/xbee-zb-module), use the ZigBee mesh protocol. Because of this, they do not include RSSI information in the packet, and anyway, the RSSI is only good for the last hop. Make sure you know your required use-case before ordering these modules. _You have been warned._

### XBee Configuration

Before doing anything with your XBees, read this list of [common XBee mistakes](http://www.faludi.com/projects/common-xbee-mistakes/). It will save you a lot of time wondering why things aren't working as they should. I also recommend you read the official [Getting Started Guide](http://ftp1.digi.com/support/documentation/90002160_A.pdf), and consult the [Product Manual](http://ftp1.digi.com/support/documentation/90000982_M.pdf).

In order to configure your XBees, by far the easiest way is to use [X-CTU](http://www.digi.com/support/productdetail?pid=3352). Unfortunately this only runs on Windows, though it may be possible to get it working with WINE in Linux. For very basic instructions on how to use X-CTU, look [here](http://examples.digi.com/get-started/configuring-xbee-radios-with-x-ctu/). Alternatively, you check out the instructions [here](http://examples.digi.com/get-started/basic-xbee-802-15-4-chat/3/) on how to configure them from a terminal.

Getting the XBees talking to each other should be a fairly simple matter. You'll want the _PAN ID_ and _Channel_ settings to be the same on each XBee you're using, and of course the _MY_ address should be unique to each XBee. If you're using only two XBees to talk to each other, then set the _Destination address high_ to `0` and the _Destination low_ address to the _MY_ address of the other XBee. Otherwise, if you're wanting to broadcast to all XBees listening on the same _Channel_ and _PAN ID_, then set the _Destinaton high_ address to `0`, and the _Destination low_ address to `FFFF` to enable broadcast mode.

The final setting is to change the _API Enable_ setting to "2", which will allow the Arduino to control the XBee using API commands.

### Arduino Configuration

I'm going to assume that if you're reading this tutorial, then you already have a basic understanding of Arduino code, and how to use them. You'll need to download the [xbee-arduino](https://code.google.com/p/xbee-arduino/) library and put it in your Arduino library folder.

In your Arduino code include and intitialise the xbee-arduino library.

    #include <XBee.h>
    
    XBee xbee = XBee()
    

Then the start the serial connection to the XBee and computer in setup.

    void setup()
    {
        // XBee serial connection
        xbee.begin(9600);
    
        // Computer serial connection
        Serial.begin(9600);
    }
    

For the purposes of this tutorial, we're going to have one XBee send a packet, and the other read the packet and output the RSSI value. You can have the sending XBee just send from a terminal, and use only one Arduino for receiving, but I'm going to show the code for having both XBees attached to an Arduino. The code about applies to both sender and receiver.

#### Sending Packets

On your sending XBee/Arduino, before the setup, initialise the payload. You can of course change the payload anywhere within the loop. I'm going to set the payload to "Hi".

    uint8_t payload[] = { 'H', 'i' };
    

Then specify the destination address that you're going to send to. This is the _Serial High_ and _Serial Low_ address of the recieving XBee. If you're broadcasting, then you can skip this step.

    XBeeAddress64 addr64 = XBeeAddress64(0x0013a200, 0x403e0f30);
    

In this example, because we're not dealing with sending updated data, we can create the transmit request in setup. The _Tx16Request_ takes three parameters; the address it's being sent to, the payload, and the size of the payload. If you're broadcasting, then change the address parameter to `0xFFFF`.

    Tx16Request tx16 = Tx16Request(addr64, payload, sizeof(payload));
    

Finally, in your loop, send the packet. I'm going to set it to send every 50 milliseconds.

    void loop()
    {
        xbee.send(tx16);
        delay(50);
    }
    

#### Receiving Packets and reading RSSI

On your receiver, before setup, you'll first need to initialise a Response object.

    Rx16Response rx16 = Rx16Response();
    

In the loop, your XBee will first wait for an incoming packet. The value within the brackets specifies how long it shoud continue waiting before continuing through the loop. For this example, because we're only doing the one thing, listening for and reading the signal strength of a packet, we can afford to wait a while.

    xbee.readPacket(100);
    

Once a packet is received, in order to prevent errors we first check if the response is available, then check if it matches the response type we want. Only then do we actually read the packet.

    // Check if available
    if (xbee.getResponse().isAvailable())
    {
        // Check if packet is correct type
        if (xbee.getResponse.getApiId() == RX_16_RESPONSE)
        {
            // Read the packet
            xbee.getResponse().getRx16Response(rx16);
        }
    }
    

Now you can read the signal strength from the packet and send it to the computer. If you don't want to print out the previous RSSI value every time through the loop whenever a packet isn't received (which you shouldn't), then this code should go within your _if_ statements immediately after you've read the packet.

    Serial.print( rx16.getRssi() );
    

And if you want to print out the payload:

    for (int i = 0; i < rx16.getDataLength(); i++)
    {
        Serial.print( rx16.getData(i), HEX );
    }
    

#### Final Code

Your code for the sender should look something like this:

    #include <XBee.h>
    
    XBee xbee = XBee()
    
    uint8_t payload[] = { 'H', 'i' };
    
    XBeeAddress64 addr64 = XBeeAddress64(0x0013a200, 0x403e0f30);
    
    Tx16Request tx16 = Tx16Request(addr64, payload, sizeof(payload));
    
    void setup()
    {
        xbee.begin(9600);
    }
    
    void loop()
    {
        xbee.send( tx16 );
        delay(50);
    }
    

And the code for the receiver something like this:

    #include <XBee.h>
    
    XBee xbee = XBee()
    
    Rx16Response rx16 = Rx16Response();
    
    void setup()
    {
        xbee.begin(9600);
        Serial.begin(9600);
    }
    
    void loop()
    {
        xbee.readPacket(100);
        if (xbee.getResponse().isAvailable())
        {
            if (xbee.getResponse().getApiId() == RX_16_RESPONSE)
            {
                xbee.getResponse().getRx16Response(rx16);
                Serial.print( rx16.getRssi() );
            }
        }
    }
    

You should now have a couple of XBees that are talking to each other, and can read the signal strength of the packets. For a full example of working code, where a whole bunch of senders broadcast to multiple receivers, which then send the data to an aggregator, check out my [Github repository](https://github.com/DBeath/rssi-aggregator).
