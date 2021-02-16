https://stackoverflow.com/questions/60633095/how-to-read-xbee-rssi-in-api-mode

> I am trying to test a XBee RSSI in API mode at receiving end, how can I retrieve the RSSI value of the receiving radio in arduino.
I configured both XBee in API-2 mode and are connected to arduino ...

#  How to read XBee RSSI in API mode?  
I am trying to test a XBee RSSI in API mode at receiving end, how can I retrieve the RSSI value of the receiving radio in arduino.  
I configured both XBee in API-2 mode and are connected to arduino by pin 4-5(rxtx & txrx) to Xbee radios.

Sending frame code is like below and there is no problem in transmission at both ends,

      uint8_t data[] = {'H','i'};
      XBeeAddress64 addr64 = XBeeAddress64();
      addr64.setMsb(0x00000000); // Msb address of receiver
      addr64.setLsb(0x00000000); // Lsb address of receiver
      ZBTxRequest zbTx = ZBTxRequest(addr64, data, sizeof(data));
      xbee.send(zbTx); 
      delay(1000);
    

At the receiving end I tried `pulseIn` of arduino and `.getRssi()` of , The former function gives "0" in result while the later gives "102" but remains the same as I move the Xbee radios away from each other. What should I need to do for getting correct RSSI at the receiving end..?

[

![](https://www.gravatar.com/avatar/173e2caa17a8522cae442e23e7879921?s=32&d=identicon&r=PG)

](https://stackoverflow.com/users/266392/tomlogic)

[tomlogic](https://stackoverflow.com/users/266392/tomlogic)

10.8k3 gold badges29 silver badges57 bronze badges

asked Mar 11 '20 at 9:43

[

![](https://www.gravatar.com/avatar/7bbb400069b673676ad062e2132de75c?s=32&d=identicon&r=PG&f=1)

](https://stackoverflow.com/users/7467640/jawad-khan)

2

Hopefully this answer helps you and others.  
Assuming you are using the following lib: [https://github.com/andrewrapp/xbee-arduino](https://github.com/andrewrapp/xbee-arduino)  
and you have a series 1 module you can use the following test code for quick diagnostics. the commented parts can of course also be used if needed

    #include <XBee.h>
    #include <SoftwareSerial.h>
    
    // XBee's DOUT (TX) is connected to pin 8 (Arduino's Software RX)
    // XBee's DIN (RX) is connected to pin 9 (Arduino's Software TX)
    SoftwareSerial serial1(8, 9); // RX, TX
    
    XBee xbee = XBee();
    XBeeResponse response = XBeeResponse();
    Rx16Response rx16 = Rx16Response();
    Rx64Response rx64 = Rx64Response();
    
    // uint8_t xbeeOption = 0;
    // uint8_t xbeeData = 0;
    uint8_t xbeeRssi = 0;
    
    void setup() {
      Serial.begin(9600);
      serial1.begin(9600);
      xbee.setSerial(serial1);
    }
    
    void loop() {
      xbee.readPacket(100);
      if (xbee.getResponse().isAvailable())  {
        Serial.println("Xbee available");
        if (xbee.getResponse().getApiId() == RX_64_RESPONSE || xbee.getResponse().getApiId() == RX_16_RESPONSE)  {
          Serial.println("64 or 16");
          if (xbee.getResponse().getApiId() == RX_16_RESPONSE)  {
            Serial.println("16");
            xbee.getResponse().getRx16Response(rx16);
            // xbeeOption = rx16.getOption();
            //Serial.print("xbeeOption: "); Serial.println(xbeeOption);
            //xbeeData = rx16.getData(0);
            //Serial.print("xbeeData: "); Serial.println(xbeeData);
            xbeeRssi = rx16.getRssi();
            Serial.print("xbeeRssi: "); Serial.println(xbeeRssi);
          }
          else  {
            Serial.println("64");
            xbee.getResponse().getRx64Response(rx64);
            //xbeeOption = rx64.getOption();
            //Serial.print("xbeeOption: "); Serial.println(xbeeOption);
            //xbeeData = rx64.getData(0);
            //Serial.print("xbeeData: "); Serial.println(xbeeData);
            xbeeRssi = rx64.getRssi();
            Serial.print("xbeeRssi: "); Serial.println(xbeeRssi);
          }
        }
      }
    

If you use a series2 module there is only the way using the hardware pwm signal: In order for the RSSI pwm signal to be updated it needs to have received an API packet. Also, for the series 2 Xbee this applies only for the last hop of the packet, so from last router to destination. You need to use the XBee rssi pin and some coding depending on your appliance.  
The rssi for distance is not very reliable and you will see a change perhaps every 10 to 15 meters while sending packets. So just moving a Xbee around on your workplace will not change the values.  
EDIT: When using a series 2 module there is the following possibility: connect the rssi pin of the xbee (6) to an Arduino pwm pin (eg 10) and measure the incoming signal, which could then be mapped to a quality or/and distance range. So writing your own rssi function. The usual xbee libs only support series1 modules.

answered Mar 11 '20 at 20:36

[

![](https://i.stack.imgur.com/hJvgi.png?s=32&g=1)

](https://stackoverflow.com/users/10245581/codebreaker007)

[Codebreaker007](https://stackoverflow.com/users/10245581/codebreaker007)Codebreaker007

2,2101 gold badge4 silver badges19 bronze badges

3

Your Answer
-----------

*   Links
*   Images
*   Styling/Headers
*   Lists
*   Blockquotes
*   Code
*   HTML
*   Tables
*   [Advanced help](https://stackoverflow.com/editing-help)

In most cases, a plain URL will be recognized as such and automatically linked:

Visit https://area51.stackexchange.com/ regularly!
Use angle brackets to force linking: Have you seen <https://superuser.com>?

To create fancier links, use Markdown:

Here's \[a link\](https://www.example.com/)! And a reference-style link to \[a panda\]\[1\].
References don't have to be \[numbers\]\[question\].

 \[1\]: https://notfound.stackexchange.com/
 \[question\]: https://english.stackexchange.com/questions/11481

You can add tooltips to links:

Click \[here\](https://diy.stackexchange.com "this text appears when you mouse over")!
This works with \[reference links\]\[blog\] as well.

 \[blog\]: https://stackoverflow.blog/ "click here for updates"

Images are exactly like links, but they have an exclamation point in front of them:

!\[a busy cat\](https://cdn.sstatic.net/Sites/stackoverflow/Img/error-lolcat-problemz.jpg)
!\[two muppets\]\[1\]

 \[1\]: https://i.imgur.com/I5DFV.jpg "tooltip"

The word in square brackets is the alt text, which gets displayed if the browser can't show the image. Be sure to include meaningful alt text for screen-reading software.

Be sure to use text styling sparingly; only where it helps readability.

\*This is italicized\*, and so
is \_this\_.

\*\*This is bold\*\*, just like \_\_this\_\_.

You can \*\*\*combine\*\*\* them
if you \_\_\_really have to\_\_\_.

To break your text into sections, you can use headers:

A Large Header
==============

Smaller Subheader
-----------------

Use hash marks if you need several levels of headers:

\# Header 1 #
## Header 2 ##
### Header 3 ###

Both bulleted and numbered lists are possible:

\- Use a minus sign for a bullet
+ Or plus sign
\* Or an asterisk

1. Numbered lists are easy
2. Markdown keeps track of
   the numbers for you
7. So this will be item 3.

1. Lists in a list item:
 \- Indented four spaces.
 \* indented eight spaces.
 \- Four spaces again.
2. You can have multiple
 paragraphs in a list items.
 
 Just be sure to indent.

\> Create a blockquote by
> prepending “>” to each line.
>
> Other formatting also works here, e.g.
>
> 1. Lists or
> 2. Headings:
>
> ## Quoted Heading ##

You can even put blockquotes in blockquotes:

\> A standard blockquote is indented
> > A nested blockquote is indented more
> > > > You can nest to any depth.

To create code blocks or other preformatted text, indent by four spaces or surround with groups of backticks:

 This will be displayed in a monospaced font. The first four spaces
 will be stripped off, but all other whitespace will be preserved.

\`\`\`
Markdown and HTML are turned off in code blocks:
<i>This is not italic</i>, and \[this is not a link\](https://example.com)
\`\`\`

To create not a block, but an inline code span, use backticks:

The \`$\` character is just a shortcut for \`window.jQuery\`.

If you want to have a preformatted block within a list, indent by eight spaces:

1\. This is normal text.
2. So is this, but now follows a code block:
 
 Skip a line and indent eight spaces.
 That's four spaces for the list
 and four to trigger the code block.

If you need to do something that Markdown can't handle, use HTML. Note that [we only support a very strict subset of HTML!](https://meta.stackexchange.com/questions/1777/what-html-tags-are-allowed)

Strikethrough humor is <strike>funny</strike>.

Markdown is smart enough not to mangle your span-level HTML:

<b>Markdown works \*fine\* in here.</b>

Block-level HTML elements have a few restrictions:

1.  They must be separated from surrounding text by blank lines.
2.  The begin and end tags of the outermost block element must not be indented.
3.  Markdown can't be used within HTML blocks.

  

<pre>
    You can <em>not</em> use Markdown in here.
</pre>

You can create tables using the [GitHub-flavored markdown format](https://github.github.com/gfm/#tables-extension-).

| A header | Another header |
| -------- | -------------- |
| First    | row            |
| Second   | row            |

*   A header row is required and must be followed by a separator row with the same number of cells
*   Cells are separated by a pipe (`|`) symbol

Set the **alignment** of a table column by placing a `:` on the left, right, or both sides of a separator in the separator line.

| left | center | right |
|:---- |:------:| -----:|
| One  | Two    | Three |

Not the answer you're looking for? Browse other questions tagged [arduino](https://stackoverflow.com/questions/tagged/arduino "show questions tagged 'arduino'") [xbee](https://stackoverflow.com/questions/tagged/xbee "show questions tagged 'xbee'") or [ask your own question](https://stackoverflow.com/questions/ask).
----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
