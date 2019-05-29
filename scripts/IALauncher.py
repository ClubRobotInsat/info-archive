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

left="yellow"
right="purple"
env=os.environ
env['LD_LIBRARY_PATH']="/home/pi/Desktop"

# This variable defines where the AI are living
ia_directory="/home/pi/Desktop/"

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
        os.system('xterm -into '+ str(self.c_term.winfo_id()) +' -geometry 97x37 -sb -e tail -f ' + ia_directory + 'tmp&')

        # ---- Color internal variable
        self.color = left

        # ---- IA Processus
        self.ia=""

        # ---- IA Picker
        self.ia_picker_option = ('IAPrimary', 'IAWiimote')
        self.ia_map = {'IAPrimary' : 'IAPrimary', 'IAWiimote' : 'IAWiimote'}
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

        self.ia_picker.place(relx=0,rely=0.3,relwidth=0.25,relheight=0.2222)
        self.available_connection=None

        # ---- Run
        self.b_run = tkinter.Button(self.root,text="",command=self.callback_run,state="normal")
        self.b_run.place(relx=0,rely=0.74444,relwidth=0.25,relheight=0.2222)

       # ---- Kill
        self.b_kill=tkinter.Button(self.root,text="KILL IA",state="disabled",command=self.callback_kill_ia)
        self.b_kill.place(relx=0,rely=0.5222,relwidth=0.25,relheight=0.2222)

        # ---- Color
        self.b_color=tkinter.Button(self.root,text=left.capitalize(),bg=left,activebackground=left,fg="black",command=self.callback_color_change)
        self.b_color.place(relx=0,rely=0,relwidth=0.25,relheight=0.3)


        # ---- Kill Launcher
        #self.b_kill_launcher=tkinter.Button(self.root,text="Kill Interface",command=self.callback_kill_interface)
        #self.b_kill_launcher.place(relx=0,rely=0.25,relwidth=0.125,relheight=0.125)

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
        #print("Status du processus : " + process_status)
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
            self.b_run.config(state='normal')
            self.b_run.config(text="Launch IA")

    # Kill the worker thread, then exit.
    def callback_kill_interface(self):
        global keep_running
        keep_running= False
        self.root.quit()

    #Callback functin for changing color
    def callback_color_change(self) :
        if self.color == left :
            self.color = right
            self.b_color.configure(bg=right)
            self.b_color.configure(activebackground=right)
            self.b_color.configure(text=right.capitalize())
        elif self.color == right:
            self.color=left
            self.b_color.configure(bg=left)
            self.b_color.configure(activebackground=left)
            self.b_color.configure(text=left.capitalize())
        else :
            exit

    #Callback for running the AI
    def callback_run(self) :
        #ia_arguments = " RS232 " + self.available_connection
        if self.ia_picker_value == "IAWiimote" :
            self.ia = subprocess.Popen("exec "+ ia_directory +[self.ia_map[self.ia_picker_value.get()] + ia_arguments + " > " + ia_directory + "tmp 2>&1"],shell=True,env=env,preexec_fn=os.setsid,cwd=ia_directory)
        else :
            ia_invocation = "exec " + ia_directory + self.ia_map[self.ia_picker_value.get()]+ ia_arguments + " -c " + self.color + " >" + ia_directory + "tmp 2>&1"
            print(ia_invocation)
            self.ia = subprocess.Popen([ia_invocation],shell=True,env=env,preexec_fn=os.setsid,cwd=ia_directory)

    #Callback for killing the AI
    def callback_kill_ia(self) :
        # Killink the AI and the subprocess, so we have to reset self.ia
        os.killpg(os.getpgid(self.ia.pid), signal.SIGKILL)
        self.ia=""

    def get_color(self) :
        return self.color

def check_pid(pid):
    try:
        os.kill(pid, 0)
    except OSError:
        return False
    else:
        return True

# Signal Handler for SIGINT,SIGSTOP,SIGTERM
def signal_handler(signal,frame):
    global theLauncher
    global thread
    global keep_running
    keep_running=False
    thread.join()
    theLauncher.root.quit()

def should_i_keep_running():
    global keep_running
    return keep_running

# This function run on a separate thread and update variables for button management.
# The data is put in a thread_safe queue that is read in the GUI thread to avoid problems.
def send_data_to_queue(queue,launcher):
    while should_i_keep_running() :
        ia_process = get_ia_status()
        connection_status = monitor_connections()
        if ia_process == "":
            process_status="Killed"
        elif ia_process.returncode == None :
            process_status="Running"
        else :
            process_status="Killed"
        if queue.qsize() < 2 :
            queue.put((connection_status,process_status))
        time.sleep(0.005)


theLauncher = app()

def get_ia_status() :
    global theLauncher
    return theLauncher.ia


thread=threading.Thread(target=send_data_to_queue,args=(theLauncher.queue,theLauncher))
thread.start()

signal.signal(signal.SIGINT,signal_handler)
signal.signal(signal.SIGTERM,signal_handler)

theLauncher.root.mainloop()

