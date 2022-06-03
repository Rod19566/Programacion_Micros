# Programa base para utilizar interfaz gráfica diseñada en QtDesigner
import sys
from PyQt5 import uic
from PyQt5.QtWidgets import QMainWindow, QApplication
import serial

class App(QMainWindow):
    def __init__(self):
        super().__init__()
        uic.loadUi("4PotControl.ui", self)			# Ingresar nombre de su archivo .ui
        #self.dialeye1.valueChanged.connect( self.getvalueS1 )
        self.ser = serial.Serial(port="COM4", baudrate=9600, timeout = 1.0)
        self.ser.close()
        
        self.dialeye1.valueChanged.connect( self.getvalueS1 )
        self.dialeye2.valueChanged.connect( self.getvalueS2 )
        self.dialeye3.valueChanged.connect( self.getvalueS3 )
        self.dialeye4.valueChanged.connect( self.getvalueS4 )
        self.dialeye11.sliderReleased.connect( self.getvalueS11 )
        self.dialeye22.sliderReleased.connect( self.getvalueS22 )
        self.dialeye33.sliderReleased.connect( self.getvalueS33 )
        self.dialeye44.sliderReleased.connect( self.getvalueS44 )
	
    def getvalueS1(self):
        pot = 121
        self.ser.open()
        self.ser.write( chr(pot).encode() )
        #a = self.ser.readline().decode() 
        self.ser.close()
        
        value = self.dialeye1.value()
        self.ser.open()
        self.ser.write( chr(value).encode() )
        a = self.ser.readline().decode() 
        self.ser.close()
        print(a[0:5] + " " + str(ord(a[6])))
        
        self.labelLEV.setText( str(value) )
	
    def getvalueS2(self):
        pot = 122
        self.ser.open()
        self.ser.write( chr(pot).encode() )
        a = self.ser.readline().decode() 
        self.ser.close()
         
        value = self.dialeye2.value()
        self.ser.open()
        self.ser.write( chr(value).encode() )
        a = self.ser.readline().decode() 
        self.ser.close()
        print(a[0:5] + " " + str(ord(a[6])))
        
        self.labelLEH.setText( str(value) )
	
    def getvalueS3(self):
        pot = 123
        self.ser.open()
        self.ser.write( chr(pot).encode() )
        a = self.ser.readline().decode() 
        self.ser.close()
         
        value = self.dialeye3.value()
        self.ser.open()
        self.ser.write( chr(value).encode() )
        a = self.ser.readline().decode() 
        self.ser.close()
        print(a[0:5] + " " + str(ord(a[6])))
        
        self.labelREV.setText( str(value) )
	
    def getvalueS4(self):
        pot = 124
        self.ser.open()
        self.ser.write( chr(pot).encode() )
        a = self.ser.readline().decode() 
        self.ser.close()
         
        value = self.dialeye4.value()
        self.ser.open()
        self.ser.write( chr(value).encode() )
        a = self.ser.readline().decode() 
        self.ser.close()
        print(a[0:5] + " " + str(ord(a[6])))
        
        self.labelREH.setText( str(value) )
	
	
    def getvalueS11(self):
        self.labelLEV2.setText( str(self.dialeye11.value()) )
	
    def getvalueS22(self):
        self.labelLEH2.setText( str(self.dialeye22.value()) )
	
    def getvalueS33(self):
        self.labelREV2.setText( str(self.dialeye33.value()) )
	
    def getvalueS44(self):
        self.labelREH2.setText( str(self.dialeye44.value()) )

   
if __name__ == '__main__':
	app = QApplication(sys.argv)
	GUI = App()
	GUI.show()
	sys.exit(app.exec_())