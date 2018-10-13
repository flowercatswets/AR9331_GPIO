#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <string.h>
#include <time.h>


#define GPIO_ADDR 0x18040000 // base address
#define GPIO_BLOCK 48     // memory block size

volatile unsigned long *gpioAddress;
//Read GPIO Status
//return 0 -->Low
//return >0 -->High
int gpioRead(int gpio)
{
    unsigned long value = *(gpioAddress + 1);
    return (value & (1 << gpio));
}

// "1" (output) or "0" (input) 
void gpioDirection(int gpio, int direction)
{
    // obtain current GPIO settings
    unsigned long value = *(gpioAddress + 0); 
    if (direction == 1)
    {
        value |= (1 << gpio); // set bit to 1 (output)
    }
    else
    {
        value &= ~(1 << gpio); // clear bit (input)
    }
    *(gpioAddress + 0) = value;
}
//Set GPIO
void gpioSet(int gpio, int data)
{
    // obtain current GPIO settings
    unsigned long value = *(gpioAddress + 2); 
    if (data == 1)
    {
        value |= (1 << gpio); // set bit to 1 (High)
    }
    else
    {
        value &= ~(1 << gpio); // clear bit  (Low)
    }
    *(gpioAddress + 2) = value;
}
//Init GPIO Address, if /dev/mem not foud, you need to rebuild Image and add this module
int gpioSetup()
{
    int  m_mfd;
    if ((m_mfd = open("/dev/mem", O_RDWR)) < 0)
    {
        return -1;
    }    
    gpioAddress = (unsigned long*)mmap(NULL, GPIO_BLOCK, PROT_READ|PROT_WRITE, MAP_SHARED, m_mfd, GPIO_ADDR);
    close(m_mfd);

    if (gpioAddress == MAP_FAILED)
    {
        return -2;
    }
    return 0;
}

int main(int argc, char **argv)
{
    int pin=27;
    
    gpioSetup();
        
	sscanf(argv[1], "%d", &pin);

    gpioDirection(pin,1);//set pin output    

    if (argv[2][0]=='1')  //set pin high
    {
        //printf("set GPIO %d High\n",pin);           
        gpioSet(pin,1);     
        printf("GPIO %d==%d\n",pin,gpioRead(pin)==0?0:1);
    }
    

    else if (argv[2][0]=='0') //set pin low
    {
        //printf("set GPIO %d Low\n",pin);
        gpioSet(pin,0);
        printf("GPIO %d==%d\n",pin,gpioRead(pin)==0?0:1);
    }

    else if (argv[2][0]=='3')  //blink 100 times
    {
        int ms=0;
        sscanf(argv[3], "%d", &ms);
        printf("GPIO %d Blink , delay %d ms\n",pin,ms);
        for (int i=0;i<100;i++)
        {
            gpioSet(pin,1);
            usleep(1000*ms);
            gpioSet(pin,0);
            usleep(1000*ms);
        }        
    }     
    else if (argv[2][0]=='6') //read GPIO Status 5 times ,every 2 sec
    {
        printf("Set GPIO %d to Input \n",pin);
        gpioDirection(pin,0);//set pin output    
        for (int i=0;i<5;i++)
        {
            printf("GPIO %d==%d\n",pin,gpioRead(pin)==0?0:1);
            usleep(1000*2000);
        }
    }
    else if (argv[2][0]=='9')
    {
        printf("GPIO %d==%d\n",pin,gpioRead(pin)==0?0:1);
    }
}
