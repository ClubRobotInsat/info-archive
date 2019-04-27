#ifndef ROOT_PANELROBOTSTATE_H
#define ROOT_PANELROBOTSTATE_H

#include "GtkInclude.h"
#include <Json.h>

// template <class UnaryFunction>
class PanelRobotState : public Gtk::Frame {
public:
	PanelRobotState();

	void refreshPanel(JSON json);

protected:
	// Signal handler
	void on_button_quit();
	void on_treeview_row_activated(const Gtk::TreeModel::Path& path, Gtk::TreeViewColumn* column);

	// Tree model columns
	class ModelColumns : public Gtk::TreeModel::ColumnRecord {
	public:
		ModelColumns() {
			add(col_name);
			add(col_details);
			add(col_status);
		}

		Gtk::TreeModelColumn<Glib::ustring> col_name;
		Gtk::TreeModelColumn<Glib::ustring> col_details;
		Gtk::TreeModelColumn<Glib::ustring> col_status;
	};

	ModelColumns m_columns;

	// Child widgets:
	Gtk::Box box;
	Glib::RefPtr<Gtk::TreeStore> refTreeModel;

private:
	Glib::RefPtr<Gtk::Builder> _builder;

	Gtk::Button* _quitButton;
	Gtk::TreeView* _treeView;

	void setTreeView(JSON& root);
	void addChild(JSON& root, Gtk::TreeModel::Row& row);
	//	void setElement(JSON json, UnaryFunction f);
};


// pour tester on fait simulator [ debug
// https://developer.gnome.org/gtkmm-tutorial/stable/sec-treeview-examples.html.en
#endif // ROOT_PANELROBOTSTATE_H
