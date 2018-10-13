# AR9331_GPIO
Direct Access AR9331 GPIO

# Usage

- Set GPIO High/Low
    ./gpio [pin] [1/0]
    ex:
    ./gpio 27 1    
- Set GPIO blink
    ./gpio [pin] [3] [delay_ms]
    ex:
    ./gpio 27 3 20
- Set GPIO Input And Read (5 times , delay 2 seconds)
    ./gpio [pin] [6]
    ./gpio 27 6
- Get GPIO Status (output)
    ./gpio [pin] [9]
    ./gpio 27 9

[id]: https://www.unwireddevices.com/wiki/index.php/Working_with_GPIOs_(C/C%2B%2B)  "Working with GPIOs (C/C++)"