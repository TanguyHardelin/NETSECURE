import time
import smbus

bus = smbus.SMBus(1)
data = bus.read_i2c_block_data(0x48, 0)

msb = data[0]
lsb = data[1]

print (((msb << 8) | lsb) >> 4) * 0.0625      #printing the temperature value in Celsius.


