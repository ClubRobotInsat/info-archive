#!/usr/bin/python3

# ---
# ---
# --- /!\ Before running the script, be sure to give the rights to the user to access /dev/ttyUS0
# --- You can use : sudo usermod -a -G dialout NOM_UTILISATEUR
# ---

import tkinter,os,subprocess,signal,sys
import tkinter.ttk as ttk
import threading
import queue
import time

left="vert"
right="purple"
env=os.environ
env['LD_LIBRARY_PATH']="/home/pi/Desktop"

# Shared variables between threads
keep_running=True

def test() :
    print("ohoh")


# Return a list of all acceptable USB connection
def scan_usb_connection() :
    files = os.listdir("/dev")
    result = list()
    for file in files :
        if file[0:6] == "ttyUSB" :
            result.append("/dev/"+file)
        elif file[0:6] == "ttyACM":
            result.append("/dev/"+file)
    return result

# Return either a string with the path to a valid serial connection, or NONE.
def monitor_connections() :
    connection_list = scan_usb_connection()
    if len(connection_list) != 0:
        return connection_list[0]
    else :
        return None


def update_button_label(mutex) :
    while true :
        connection_list = scan_usb_connection
        # Sleep de 5 ms
        mutex.acquire(1)
        time.sleep(0.005)

class app :
    def __init__(self) :
       
        # ---- Threading stuff
        self.queue = queue.Queue(maxsize=0)

        # ---- Tk top level 
        self.root = tkinter.Tk()
        self.root.resizable(width=False,height=False)
        self.root.wm_title(string="Lanceur d'IA Club Robot INSA Toulouse")
        self.root.wm_minsize(800,480)
        self.root.attributes('-fullscreen', True)
        
        # Catching alt+f4:
        self.root.protocol("WM_DELETE_WINDOW",self.callback_kill_interface)

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
        menu.configure(font=('Arial', 30)) 
        
        self.ia_picker.place(relx=0,rely=0.5,relwidth=0.25,relheight=0.16667)
        self.available_connection=None
        
        # ---- Run
        self.b_run = tkinter.Button(self.root,text="",command=self.callback_run,state="normal")
        self.b_run.place(relx=0,rely=0.83333,relwidth=0.25,relheight=0.16667)
       
       # ---- Kill
        self.b_kill=tkinter.Button(self.root,text="KILL IA",state="disabled",command=self.callback_kill_ia)
        self.b_kill.place(relx=0,rely=0.6667,relwidth=0.25,relheight=0.16667)

        # ---- Color 
        self.b_color=tkinter.Button(self.root,text="Bleu",bg="blue",activebackground="blue",fg="black",command=self.callback_color_change)
        self.b_color.place(relx=0,rely=0,relwidth=0.25,relheight=0.25)


        # ---- Kill Launcher
        self.b_kill_launcher=tkinter.Button(self.root,text="Kill Interface",command=self.callback_kill_interface)
        self.b_kill_launcher.place(relx=0,rely=0.25,relwidth=0.125,relheight=0.125)

        self.root.after(50,self.read_queue)


    # This function is called in the GUI Thread to update the button status
    def read_queue(self) :
        try :
            connection_status, process_status = self.queue.get_nowait()
            #process_status = self.queue.get_nowait()
        except queue.Empty :
            pass
        else :
            self.update_button(connection_status,process_status)
        self.root.after(50,self.read_queue)
    
            

    # Button management logic
    def update_button(self,connection_status,process_status):
        if process_status == "Running"  :
            self.b_kill.config(state='normal')
            self.b_run.config(state='disabled')
        elif connection_status != None :
            self.available_connection=connection_status
            self.b_kill.config(state='disabled')
            self.b_run.config(state='normal')
            self.b_run.config(text="Launch IA on : "+connection_status)
        else :
            self.b_kill.config(state='disabled')
            self.b_run.config(state='disabled')
            self.b_run.config(text="No connection available ")

    # Kill the worker thread, then exit.
    def callback_kill_interface(self):
        global keep_running 
        keep_running= False
        self.root.quit()

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
        connection_suffix = " RS232 " + self.available_connection
        if self.ia_picker_value == "IAWiimote" :
            self.ia = subprocess.Popen([self.ia_map[self.ia_picker_value.get()] +connection_suffix + " > tmp 2>&1"],shell=True,env=env)
        else :
            ia_invocation = self.ia_map[self.ia_picker_value.get()]+connection_suffix + "-c " + self.color + " > tmp 2>&1"
            self.ia = subprocess.Popen([ia_invocation],shell=True,env=env)

    #Callback for killing the AI
    def callback_kill_ia(self) :
        self.b_run.config(state="normal")
        self.b_kill.config(state="disabled")
        os.killpg(self.ia.pid,signal.SIGKILL)


    def get_color(self) :
        return self.color


# Signal Handler for SIGINT,SIGSTOP,SIGTERM
def signal_handler(signal,frame):
    global theLauncher
    global thread
    global keep_running
    keep_running=False
    thread.join()
    theLauncher.root.quit()

def should_i_keep_running() :
    global keep_running
    return keep_running

# This function run on a separate thread and update variables for button management.
# The data is put in a thread_safe queue that is read in the GUI thread to avoid problems.
def send_data_to_queue(queue,process):
    while should_i_keep_running() :
        connection_status = monitor_connections()
        if process == "":
            process_status="Killed"
        elif process.returncode == None :
            process_status="Running"
        else :
            process_status="Killed"
        if queue.qsize() < 2 :
            queue.put((connection_status,process_status))
        time.sleep(0.005)


theLauncher = app()

thread=threading.Thread(target=send_data_to_queue,args=(theLauncher.queue,theLauncher.ia))
thread.start()

#signal.signal(signal.SIGINT,signal_handler)
signal.signal(signal.SIGINT,signal_handler)
signal.signal(signal.SIGTERM,signal_handler)

theLauncher.root.mainloop()
