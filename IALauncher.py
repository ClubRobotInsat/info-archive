#!/usr/bin/python3

# ---
# ---
# --- /!\ Before running the script, be sure to give the rights to the user to access /dev/ttyUS0
# --- You can use : sudo usermod -a -G dialout NOM_UTILISATEUR
# ---

import tkinter,os,subprocess,signal,sys
import tkinter.ttk as ttk

left="vert"
right="purple"
ia_invocation="sudo ./IAWiimote RS232 /dev/ttyUSB0"
env=os.environ
env['LD_LIBRARY_PATH']="/home/pi/Desktop"
def test() :
    print("ohoh")



# Return a list of all acceptable USB connection
def scan_usb_connection() :
    files = os.listdir("/dev")
    result = list()
    for file in files :
        if file[0:6] == "ttyUSB" :
            result.append("/dev/"+file)
            print(file)
        elif file[0:6] == "ttyACM":
            result.append("/dev/"+file)
            print(file)
    return result


scan_usb_connection()

class app :
    def __init__(self) :
        
        # ---- Tk top level 
        self.root = tkinter.Tk()
        self.root.resizable(width=False,height=False)
        self.root.wm_title(string="Lanceur d'IA Club Robot INSA Toulouse")
        self.root.wm_minsize(800,480)
        self.root.attributes('-fullscreen', True)
        
        # ---- Canvas
        self.c_term=tkinter.Frame(self.root)
        self.c_term.place(relx=0.25,rely=0,relwidth=0.75,relheight=1)

        # ---- Terminal
        os.system('xterm -into %d -geometry 97x37 -sb -e tail -f tmp&' % self.c_term.winfo_id())
        
        # ---- Color internal variable
        self.color = "blue"

        # ---- IA Processus
        self.ia="" 

        # ---- IA Picker
        self.ia_picker_option = ('IAWiimote','IAPrincipal')
        self.ia_map = {'IAWiimote' : 'sudo ./IAWiimote RS232 /dev/ttyUSB0', 'IAPrincipal' : 'sudo ./IAPrincipal RS232'}
        self.ia_picker_value = tkinter.StringVar()
        self.ia_picker_value.set(self.ia_picker_option[0])

        # Dropdown menu text style
        # text_font = ('Arial', '15')
        # self.root.option_add('*TCombobox*Listbox.justify','center')
        # self.root.option_add('*TCombobox*Listbox.font', text_font)

        #self.ia_picker = ttk.Combobox(self.root,textvariable=self.ia_picker_value,values=self.ia_picker_option,state='readonly',justify='center',height=30)
        self.ia_picker = tkinter.OptionMenu(self.root,self.ia_picker_value,*self.ia_picker_option)
        
        # Get the menu because OptionMenu is a shitty widget
        menu = self.ia_picker.nametowidget(self.ia_picker.menuname) 
        menu.configure(font=('Arial', 70)) 
        
        self.ia_picker.place(relx=0,rely=0.5,relwidth=0.25,relheight=0.16667)
        self.available_connection=list()
        # ---- Refresh Connection
        self.b_refresh = tkinter.Button(self.root,text="Refresh Connections",command=self.callback_refresh,state="normal")

        # ---- Run
        self.b_run = tkinter.Button(self.root,text="GO !",command=self.callback_run,state="normal")
        self.b_run.place(relx=0,rely=0.83333,relwidth=0.25,relheight=0.16667)
        # ---- Kill
        self.b_kill=tkinter.Button(self.root,text="KILL IA",state="disabled",command=self.callback_kill)
        self.b_kill.place(relx=0,rely=0.6667,relwidth=0.25,relheight=0.16667)

        # ---- Color 
        self.b_color=tkinter.Button(self.root,text="Bleu",bg="blue",activebackground="blue",fg="black",command=self.callback_color_change)
        #self.b_color.config(activebackground=self.get_color())
        self.b_color.place(relx=0,rely=0,relwidth=0.25,relheight=0.25)


        # ---- Kill Launcher
        self.b_kill_launcher=tkinter.Button(self.root,text="Kill Interface",command=self.callback_kill_interface)
        self.b_kill_launcher.place(relx=0,rely=0.25,relwidth=0.25,relheight=0.25)

    def callback_refresh(self):
        self.available_connection=scan_usb_connection


    def callback_kill_interface(self):
        sys.exit(0)

    #Callback functin for changing color
    def callback_color_change(self) :
        if self.color == "blue"  :
            self.color = "yellow"
            self.b_color.configure(bg="yellow")
            self.b_color.configure(activebackground="yellow")
            self.b_color.configure(text="Jaune")
        elif self.color == "yellow" :
            self.color="blue"
            self.b_color.configure(bg="blue")
            self.b_color.configure(activebackground="blue")
            self.b_color.configure(text="Bleu")
        else : 
            exit

    #Callback for running the AI
    def callback_run(self) :
        self.b_run.config(state="disabled")
        self.b_kill.config(state="normal")

        if self.ia_picker_value == "IAWiimote" :
            self.ia = subprocess.Popen([self.ia_map[self.ia_picker_value.get()] + " > tmp 2>&1"],shell=True,env=env)
        else :
            ia_invocation = self.ia_map[self.ia_picker_value.get()] + "-c " + self.color + " > tmp 2>&1"
            self.ia = subprocess.Popen([ia_invocation],shell=True,env=env)

    #Callback for killing the AI
    def callback_kill(self) :
        self.b_run.config(state="normal")
        self.b_kill.config(state="disabled")
        os.killpg(self.ia.pid,signal.SIGKILL)


    def get_color(self) :
        return self.color

theLauncher = app()
theLauncher.root.mainloop()
