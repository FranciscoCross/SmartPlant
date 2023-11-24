from ctypes import *

def mock_function():
    a = 0
    b = 60000
    while(a < b): #posible loop infinito
        a = a + 1
