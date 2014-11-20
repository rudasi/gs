class FixedNeuron:
   def __init__(self):
      self.voltage = 0
      self.spike = False
      self.reset = 0
   
   def tick(self, current):
      self.spike = False
      if self.reset > 0:
         self.reset -= 1
         return

      rc_bits = 3
      dv = (current - self.voltage) >> rc_bits
 
      self.voltage += dv
      if self.voltage < 0:
         self.voltage = 0

      if self.voltage > 255:
         self.voltage -= 256
         self.reset = 3
         self.spike = True



