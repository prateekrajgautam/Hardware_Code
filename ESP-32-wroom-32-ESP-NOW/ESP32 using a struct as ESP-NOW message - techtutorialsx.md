https://techtutorialsx.com/2019/10/27/esp32-using-a-struct-as-esp-now-message/

> Using a struct as an ESP-NOW message.

# ESP32: using a struct as ESP-NOW message - techtutorialsx
In this tutorial we will check how to use a struct as an ESP-NOW message. For this tutorial two ESP32 devices were used: a [FireBeetle board](https://www.dfrobot.com/product-1590.html) and a [Beetle board](https://www.dfrobot.com/product-1798.html?tracking=5977b2a20858a), both from DFRobot.

Introduction
------------

In this tutorial we will check how to use a struct as an ESP-NOW message. We will be using the Arduino core to program both the sender and receiver devices.

For this tutorial two ESP32 devices were used: a [FireBeetle board](https://www.dfrobot.com/product-1590.html) and a [Beetle board](https://www.dfrobot.com/product-1798.html?tracking=5977b2a20858a), both from DFRobot.

The sender code
---------------

We will start the code by the library includes. We will need the **esp\_now.h** library, which exposes the ESP-NOW related functions, and the **WiFi.h,** which will allow us to set the WiFi mode of the ESP32.

1

2

`#include <esp_now.h>`

`#include <WiFi.h>`

Then we will declare a variable with the broadcast address. For this simple tutorial, since we are focusing on the data structure used for the message, we will assume that we will broadcast the message.

1

`uint8_t` `broadcastAddress[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};`

Then we will declare a struct to hold our message. We will declare a simple structure with two integer data members, for testing purposes. Note however that the total size of the message should not exceed 250 bytes, so you should take this in consideration when modeling your message.

1

2

3

4

`typedef` `struct` `test_struct {`

 `int` `x;`

 `int` `y;`

`} test_struct;`

Moving on to the setup function, we will first open a serial connection, to be able to output some messages from our program.

After this we will set the ESP32 to work in station mode. Note that setting the mode will initialize the WiFi under the hood, which is needed for the ESP-NOW to work.

1

2

3

`Serial.begin(115200);`

`WiFi.mode(WIFI_STA);`

Then we will initialize ESP-NOW with a call to the **esp\_now\_init** function. We will do an error check to the returned value to confirm the initialization was successful.

1

2

3

4

`if` `(esp_now_init() != ESP_OK) {`

 `Serial.println(``"Error initializing ESP-NOW"``);`

 `return``;`

`}`

After this, to be able to send data to another device, we need to add a peer, like we did on the [previous tutorial](https://techtutorialsx.com/2019/10/20/esp32-getting-started-with-esp-now/). In our case, the MAC address of our peer will correspond to the broadcast address.

1

2

3

4

5

6

7

8

9

10

`esp_now_peer_info_t peerInfo;`

`memcpy``(peerInfo.peer_addr, broadcastAddress, 6);`

`peerInfo.channel = 0;` 

`peerInfo.encrypt =` `false``;`

`if` `(esp_now_add_peer(&peerInfo) != ESP_OK){`

 `Serial.println(``"Failed to add peer"``);`

 `return``;`

`}`

After registering the peer, we will be able to send messages. But, before that, we will declare a variable of type **test\_struct** to represent our message.

We will initialize its data members with some values: x will be equal to 10 and y will be equal to 20. Note that these should be the values that will be obtained by the ESP32 acting as receiver.

1

2

3

`test_struct test;`

`test.x = 10;`

`test.y = 20;`

Once we have our message, we can send it with a call to the **esp\_now\_send** function. As first input, this message receives the address of the device we want to reach. Thus, we will pass the broadcast address.

As second input we need to pass the address of the data to be sent. In our case, we need to pass the address of our testing struct. Note that we need to cast it to **uint8\_t \***, like we did on the previous tutorial.

As third and last parameter, we need to pass the length of the data we want to send. We can obtain it with the **sizeof** function.

To finalize our setup, we will check the result of the previous function call to confirm if the data was sent with success or not.

1

2

3

4

5

6

7

8

9

10

11

`esp_err_t result = esp_now_send(`

 `broadcastAddress,`

 `(``uint8_t` `*) &test,`

 `sizeof``(test_struct));`

`if` `(result == ESP_OK) {`

 `Serial.println(``"Sent with success"``);`

`}`

`else` `{`

 `Serial.println(``"Error sending the data"``);`

`}`

The final code can be seen below.

1

2

3

4

5

6

7

8

9

10

11

12

13

14

15

16

17

18

19

20

21

22

23

24

25

26

27

28

29

30

31

32

33

34

35

36

37

38

39

40

41

42

43

44

45

46

47

48

49

50

51

52

`#include <esp_now.h>`

`#include <WiFi.h>`

`uint8_t` `broadcastAddress[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};`

`typedef` `struct` `test_struct {`

 `int` `x;`

 `int` `y;`

`} test_struct;`

`void` `setup() {`

 `Serial.begin(115200);`

 `WiFi.mode(WIFI_STA);`

 `if` `(esp_now_init() != ESP_OK) {`

 `Serial.println(``"Error initializing ESP-NOW"``);`

 `return``;`

 `}`

 `esp_now_peer_info_t peerInfo;`

 `memcpy``(peerInfo.peer_addr, broadcastAddress, 6);`

 `peerInfo.channel = 0;` 

 `peerInfo.encrypt =` `false``;`

 `if` `(esp_now_add_peer(&peerInfo) != ESP_OK){`

 `Serial.println(``"Failed to add peer"``);`

 `return``;`

 `}`

 `test_struct test;`

 `test.x = 10;`

 `test.y = 20;`

 `esp_err_t result = esp_now_send(`

 `broadcastAddress,`

 `(``uint8_t` `*) &test,`

 `sizeof``(test_struct));`

 `if` `(result == ESP_OK) {`

 `Serial.println(``"Sent with success"``);`

 `}`

 `else` `{`

 `Serial.println(``"Error sending the data"``);`

 `}`

`}`

`void` `loop() {}`

The receiver code
-----------------

Like we did for the sender, we will start with the library includes.

1

2

`#include <esp_now.h>`

`#include <WiFi.h>`

Then we will also declare our message struct, so the receiver can use it. For simplicity, we are declaring it in both the receiver and sender code. Nonetheless, in a real application scenario, the best approach would be declaring it in a header file and import it in both programs.

1

2

3

4

`typedef` `struct` `test_struct {`

 `int` `x;`

 `int` `y;`

`} test_struct;`

Moving on to the Arduino setup, we will start by opening a serial connection and then setting the WiFi operation mode to station. After that, we will initialize ESP-NOW, like we did on the sender’s code.

1

2

3

4

5

6

7

8

`Serial.begin(115200);`

`WiFi.mode(WIFI_STA);`

`if` `(esp_now_init() != ESP_OK) {`

 `Serial.println(``"Error initializing ESP-NOW"``);`

 `return``;`

`}`

To finish the setup function, we will register a function that will be executed when a ESP-NOW message is received. We will call this callback function **onReceiveData** and check its implementation below.

To register it, we simply need to call the **esp\_now\_register\_recv\_cb** function, passing as input our callback.

The full setup function can be seen below and already includes the registering of the callback.

1

2

3

4

5

6

7

8

9

10

11

12

`void` `setup() {`

 `Serial.begin(115200);`

 `WiFi.mode(WIFI_STA);`

 `if` `(esp_now_init() != ESP_OK) {`

 `Serial.println(``"Error initializing ESP-NOW"``);`

 `return``;`

 `}`

 `esp_now_register_recv_cb(OnDataRecv);`

`}`

To finalize, we will check the implementation of the **OnDataRecv** function. Like we have seen in the [previous tutorial](https://techtutorialsx.com/2019/10/20/esp32-getting-started-with-esp-now/), this function follows a pre-defined signature. It returns void and it receives the following 3 arguments: the MAC address of the sender, a pointer to the data received and the length of the data received.

1

2

3

`void` `OnDataRecv(``const` `uint8_t` `* mac,` `const` `uint8_t` `*data,` `int` `len) {`

`}`

Since we receive the pointer to the data with the **uint8\_t\*** data type, we need to cast it to **test\_struct\***.

1

`test_struct* test =(test_struct*) data;`

After that we will print the values of both data members of our message struct. Recall from the sender’s code that **x** should be equal to **10** and **y** should be equal to **20**.

1

2

`Serial.println(test->x);`

`Serial.println(test->y);`

The complete callback code can be seen below.

1

2

3

4

5

6

7

8

`void` `OnDataRecv(``const` `uint8_t` `* mac,` `const` `uint8_t` `*data,` `int` `len) {`

 `test_struct* test =(test_struct*) data;`

 `Serial.println(test->x);`

 `Serial.println(test->y);`

`}`

Note that this callback function runs from the WiFi task, which means we should not do lengthy operations on it. For simplicity, we are going to print the result to the serial port on the callback but, in a real application scenario, we should just signal the reception of the data and process it in another lower priority task.

The complete final code of the receiver can be seen below.

1

2

3

4

5

6

7

8

9

10

11

12

13

14

15

16

17

18

19

20

21

22

23

24

25

26

27

28

29

30

31

`#include <esp_now.h>`

`#include <WiFi.h>`

`typedef` `struct` `test_struct {`

 `int` `x;`

 `int` `y;`

`} test_struct;`

`void` `OnDataRecv(``const` `uint8_t` `* mac,` `const` `uint8_t` `*data,` `int` `len) {`

 `test_struct* test =(test_struct*) data;`

 `Serial.println(test->x);`

 `Serial.println(test->y);`

`}`

`void` `setup() {`

 `Serial.begin(115200);`

 `WiFi.mode(WIFI_STA);`

 `if` `(esp_now_init() != ESP_OK) {`

 `Serial.println(``"Error initializing ESP-NOW"``);`

 `return``;`

 `}`

 `esp_now_register_recv_cb(OnDataRecv);`

`}`

`void` `loop() {}`

Testing the code
----------------

The easiest way to test the code is to open two instances of the Arduino IDE serial monitor: one for the sender and another for the receiver.

First, compile and upload the receiver code. When the procedure finishes, open the serial monitor on the corresponding Arduino IDE instance.

Then, compile and upload the sender code. Like before, when the procedure finishes, open the IDE serial monitor. A message indicating that the data was sent with success should get printed.

After that, go back to the serial monitor instance of the receiver. You should get an output similar to figure 1. As can be seen, the values of both data members of the message struct were received correctly.

![Output of the receiver, showing the data members of the received message.](https://i2.wp.com/techtutorialsx.com/wp-content/uploads/2019/10/image-3.png?resize=697%2C133&ssl=1)

**Figure 1** – Output of the receiver, showing the data members of the received message.
