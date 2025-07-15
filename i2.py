import tkinter as tk
import serial

SERIAL_PORT = "COM7"
BAUD_RATE = 9600


class ArduinoGUI:
    def __init__(self, master):
        self.master = master
        self.master.title("LedReflexTest-joc")
        self.master.geometry("400x400")  # dimensiunea ferestrei
        self.master.configure(bg="#ffd9eb")  #culoare background

        # incercam sa deschidem conexiunea Serial
        try:
            self.ser = serial.Serial(SERIAL_PORT, BAUD_RATE, timeout=1)
        except:
            print("Eroare: Port Serial")
            self.ser = None

        self.title_label = tk.Label(self.master,
                                    text="LedReflexTest",
                                    font=("Helvetica", 24, "bold"),
                                    fg="white", bg="#ffd9eb")
        self.title_label.pack(pady=20)

        # ------------ MOD 1 --------------------------------------
        mod1_frame = tk.Frame(self.master, bg="#ffd9eb")
        mod1_frame.pack(pady=10)
        mod1_label = tk.Label(mod1_frame,
                              text="Mod 1 - dificultate scazuta",
                              font=("Helvetica", 16),
                              fg="green", bg="#ffd9eb")
        mod1_label.pack(side="left", padx=10)
        mod1_btn = tk.Button(mod1_frame,
                             text="1",
                             font=("Helvetica", 14, "bold"),
                             bg="white",
                             command=lambda: self.send_mode(1))
        mod1_btn.pack(side="right", padx=10)

        # ------------ MOD 2 ------------------------------------
        mod2_frame = tk.Frame(self.master, bg="#ffd9eb")
        mod2_frame.pack(pady=10)
        mod2_label = tk.Label(mod2_frame,
                              text="Mod 2 - dificultate medie",
                              font=("Helvetica", 16),
                              fg="goldenrod", bg="#ffd9eb")
        mod2_label.pack(side="left", padx=10)
        mod2_btn = tk.Button(mod2_frame,
                             text="2",
                             font=("Helvetica", 14, "bold"),
                             bg="white",
                             command=lambda: self.send_mode(2))
        mod2_btn.pack(side="right", padx=10)

        # ------------ MOD 3 --------------------------------------
        mod3_frame = tk.Frame(self.master, bg="#ffd9eb")
        mod3_frame.pack(pady=10)
        mod3_label = tk.Label(mod3_frame,
                              text="Mod 3 - dificultate crescuta",
                              font=("Helvetica", 16),
                              fg="red", bg="#ffd9eb")
        mod3_label.pack(side="left", padx=10)
        mod3_btn = tk.Button(mod3_frame,
                             text="3",
                             font=("Helvetica", 14, "bold"),
                             bg="white",
                             command=lambda: self.send_mode(3))
        mod3_btn.pack(side="right", padx=10)
        # ------------ MOD 4 ------------
        mod4_frame = tk.Frame(self.master, bg="#ffd9eb")
        mod4_frame.pack(pady=10)
        mod4_label = tk.Label(mod4_frame,
                              text="Mod 4 - forte rapid",
                              font=("Helvetica", 16),
                              fg="red", bg="#ffd9eb")
        mod4_label.pack(side="left", padx=10)
        mod4_btn = tk.Button(mod4_frame,
                             text="4",
                             font=("Helvetica", 14, "bold"),
                             bg="white",
                             command=lambda: self.send_mode(4))
        mod4_btn.pack(side="right", padx=10)

        # inchidere corecta cand fereastra se inchide
        self.master.protocol("WM_DELETE_WINDOW", self.close_app)

    def send_mode(self, mode):
        if self.ser and self.ser.is_open:
            self.ser.write(str(mode).encode('utf-8'))
            print(f"Am trimis modul {mode} spre placa Arduino.")
        else:
            print("Eroare: Port Serial")

    def close_app(self):
        if self.ser and self.ser.is_open:
            self.ser.close()
        self.master.destroy()


if __name__ == "__main__":
    root = tk.Tk()
    gui = ArduinoGUI(root)
    root.mainloop()
