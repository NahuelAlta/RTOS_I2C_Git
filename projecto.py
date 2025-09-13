import serial
import struct
import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation
from scipy import signal

# abrir COM correcto de Windows
ser = serial.Serial('COM5', 115200)  # timeout corto

x_vals, y1_vals, y2_vals = [], [], []

fig, ax = plt.subplots()

def update(frame):
    # enviar handshake
    ser.write(bytes([0xAA]))  # 1 byte de inicio

    # leer 4 bytes de golpe
    raw = ser.read(4)
    if len(raw) == 4:
        s1, s2 = struct.unpack('<HH', raw)  # 2 uint16

        x_vals.append(frame)
        y1_vals.append(s1)
        y2_vals.append(s2)

        # mantener últimas 200 muestras
        if len(x_vals) > 200:
            x_vals[:] = x_vals[-200:]
            y1_vals[:] = y1_vals[-200:]
            y2_vals[:] = y2_vals[-200:]

        ax.clear()
        ax.plot(x_vals, y1_vals, label="Sensor 1")
        ax.plot(x_vals, y2_vals, label="Sensor 2")
        ax.set_xlabel("Muestras")
        ax.set_ylabel("Valor")
        ax.legend()

ani = FuncAnimation(fig, update, interval=1)
plt.show()