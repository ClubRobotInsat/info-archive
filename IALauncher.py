#!/usr/bin/python3

# ---
# ---
# --- /!\ Before running the script, be sure to give the rights to the user to access /dev/ttyUS0
# --- You can use : sudo usermod -a -G dialout NOM_UTILISATEUR
# ---

import tkinter,os,subprocess,signal

left="vert"
right="purple"
ia_invocation="sudo ./IAWiimote RS232 /dev/ttyUSB0"
env=os.environ
env['LD_LIBRARY_PATH']="/home/pi/Desktop"
def test() :
    print("ohoh")

class app :
    def __init__(self) :
        
        #Tk top level 
        self.root = tkinter.Tk()
        self.root.resizable(width=False,height=False)
        self.root.wm_title(string="Lanceur d'IA Club Robot INSA Toulouse")
        self.root.wm_minsize(800,400)
        
        #Canvas
        self.c_term=tkinter.Frame(self.root)
        self.c_term.place(relx=0.25,rely=0,relwidth=0.75,relheight=1)

        #Terminal
        os.system('xterm -into %d -geometry 80x30 -sb -e tail -f tmp&' % self.c_term.winfo_id())
        
        #Color internal variable
        self.color = "blue"

        #IA
        self.ia="" 

        #Run
        self.b_run = tkinter.Button(self.root,text="GO !",command=self.callback_run,state="normal")
        self.b_run.place(relx=0,rely=0.75,relwidth=0.25,relheight=0.25)
        #Kill
        self.b_kill=tkinter.Button(self.root,text="KILL IA",state="disabled",command=self.callback_kill)
        self.b_kill.place(relx=0,rely=0.5,relwidth=0.25,relheight=0.25)

        #Color 
        self.b_color=tkinter.Button(self.root,text="Bleu",height=15,width=60,bg="blue",activebackground="blue",fg="black",command=self.callback_color_change)
        #self.b_color.config(activebackground=self.get_color())
        self.b_color.place(relx=0,rely=0,relwidth=0.25,relheight=0.5)

    #Callback functin for changing color
    def callback_color_change(self) :
        if self.color == "blue"  :
            self.color = "yellow"
            self.b_color.configure(bg="yellow")
            self.b_color.configure(activebackground="yellow")
            self.b_color.configure(text="Jaune")
            print("blue to yellow")
        elif self.color == "yellow" :
            self.color="blue"
            self.b_color.configure(bg="blue")
            self.b_color.configure(activebackground="blue")
            self.b_color.configure(text="Bleu")
            print("yellow to blue")
        else : 
            exit

    #Callback for running the AI
    def callback_run(self) :
        self.b_run.config(state="disabled")
        self.b_kill.config(state="normal")
        self.ia = subprocess.Popen([ia_invocation + " > tmp 2>&1"],shell=True,env=env)

    #Callback for killing the AI
    def callback_kill(self) :
        self.b_run.config(state="normal")
        self.b_kill.config(state="disabled")
        os.killpg(self.ia.pid,signal.SIGKILL)


    def get_color(self) :
        return self.color

theLauncher = app()
theLauncher.root.mainloop()
