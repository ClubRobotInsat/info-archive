//
// Created by paul on 18/08/16.
//

#ifndef ROOT_MESSAGE_H
#define ROOT_MESSAGE_H
#include "Header.h"

class Message : public Gtk::TreeModel::ColumnRecord {

public :

    Message();

    Gtk::TreeModelColumn<std::string> _id;
    Gtk::TreeModelColumn<std::string> _cmd;
    Gtk::TreeModelColumn<std::string> _data;

};

#endif //ROOT_MESSAGE_H
