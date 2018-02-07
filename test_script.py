import serial
import sys

# This is a hacked together script to test Parspammer.
# NOTE: DOES NOT WORK WITH PYTHON 2, USE PYTHON 3 (or port, if you must)
# Change generate_data() (or anything else) to suit your needs.

def generate_data():
  MODE_CONT = 0
  MODE_ONESHOT = 1
  MODE_ONESHOT_REPEAT_1HZ = 2
  MODE_ONESHOT_REPEAT_10HZ = 3
  MODE_ONESHOT_REPEAT_100HZ = 4
  MODE_ONESHOT_REPEAT_1000HZ = 5
  MODE_DEFAULT = 0x64
 
  mode = MODE_ONESHOT_REPEAT_1000HZ
  num_samples = 16384
  data = [i for i in range (num_samples-1, -1, -1)]
  return (mode, num_samples, data)

def write_byte(ser, byte):
  return ser.write(bytes([byte & 0xff]))

def write_halfword(ser, halfword):
  return ser.write(bytes([((halfword >> 8) & 0xff), (halfword & 0xff)]))

def send_data(ser):
  mode, num_samples, data = generate_data()

  print("=== TRANSACTION INITIATED ===")
  print("=== SENDING MODE:", mode)
  write_byte(ser, mode)

  print("=== SENDING NUM SAMPLES:", num_samples)
  write_halfword(ser, num_samples)

  print("=== SENDING DATA:")
  for sample in data:
    sys.stdout.write('.')
    sys.stdout.flush()
    write_halfword(ser, sample)
  sys.stdout.write('\n')
  print("=== DATA SENT ===")

def main():
  with serial.Serial('/dev/ttyUSB0', 115200) as ser:
    print(ser.name)         # check which port was really used
    print("=== OPEN ===")

    READY_CODE = "RDY"
    ready_code_index = 0
    while True:
      byte = ser.read()[0]
      c = chr(byte)
      sys.stdout.write(c)
      sys.stdout.flush()

      if c == READY_CODE[ready_code_index]:
        ready_code_index += 1
      else:
        ready_code_index = 0

      if ready_code_index >= len(READY_CODE):
        sys.stdout.write('\n')
        ready_code_index = 0
        send_data(ser)

if __name__ == '__main__':
  main()