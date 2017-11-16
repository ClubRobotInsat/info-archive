## Un petit serveur pour les websockets en c++.

# Exemple

    SimuWebSocketServer server{5000}; // create a server listening on port 5000;
    server.start(); // start the server 
    server.broadcast("hey"); // send message to the connected clients.
    server.stop(); // stop the server
    
## Json Message API

# Exemple

    MessageBatch messages; // create a simple buffer for the message

    Message message; // create a Json message object
    message.add("hey", 1); // add key-value to the object. "hey": 1
    message.add("blah", "hey");
    
    messages.add(message); // add message to the buffer
    
    messages.getAndClear(); // return a string from the message buffer then clean it
    
# Creer des classes JSONAble pour pouvoir les ajouter au message
  
Simplement creer une class qui contient un heritage public de "l'interface" JSONAble et qui redefinira une fonction toJson() qui retourne un string avec la representation en json de l'objet

    class JVector3 : public JSONAble
    {
    private:
        double x, y, z;
    public:
        JVector3(double x, double y, double z)
        {
            this->x = x;
            this->y = y;
            this->z = z;
        }
    
        std::string toJSON() const
        {
            Message msg;
            msg.add("x", this->x);
            msg.add("y", this->y);
            msg.add("z", this->z);
            return msg.toJSON();
        }
    };

      
