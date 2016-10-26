//
// Created by paul on 18/08/16.
//

#ifndef ROOT_MESSAGE_H
#define ROOT_MESSAGE_H
#include "Header.h"

/**
 * A class that is used to store data that will be sent to a Gtk::Treeview
 */
class Message : public Gtk::TreeModel::ColumnRecord {

public:
	Message();

	Gtk::TreeModelColumn<std::string> _id;
	Gtk::TreeModelColumn<std::string> _cmd;
	Gtk::TreeModelColumn<std::string> _data;
	Gtk::TreeModelColumn<std::string> _time;
	/**
	 * A column that will be used to store data about the color.
	 * Used mainly for rendering line in different colors based on if the message was sent to the CAN or received
	 * from it.
	 */
	Gtk::TreeModelColumn<std::string> _color;
};

#endif // ROOT_MESSAGE_H
