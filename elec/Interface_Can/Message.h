//
// Created by paul on 18/08/16.
//

#ifndef ROOT_MESSAGE_H
#define ROOT_MESSAGE_H
#include "Header.h"

class Message : public Gtk::TreeModel::ColumnRecord {

public :

    Message();

    Gtk::TreeModelColumn<unsigned int> _id;
    Gtk::TreeModelColumn<uint8_t> _data;
    Gtk::TreeModelColumn<bool> _ackReceived;

};

#endif //ROOT_MESSAGE_H
