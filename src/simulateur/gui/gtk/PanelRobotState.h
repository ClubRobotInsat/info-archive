#ifndef ROOT_PANELROBOTSTATE_H
#define ROOT_PANELROBOTSTATE_H

#include "GtkInclude.h"
#include <Json.h>

class PanelRobotState : public Gtk::Frame {
public:
	PanelRobotState();

	void refreshPanel(JSON json);
	JSON generateTestJSON();

protected:
	// Signal handler
	void on_button_quit();
	void on_treeview_row_activated(const Gtk::TreeModel::Path& path, Gtk::TreeViewColumn* column);

	// Tree model columns
	class ModelColumns : public Gtk::TreeModel::ColumnRecord {
	public:
		ModelColumns() {
			add(col_name);
			add(col_status);
		}

		Gtk::TreeModelColumn<Glib::ustring> col_name;
		Gtk::TreeModelColumn<Glib::ustring> col_status;
	};

	ModelColumns m_columns;

	// Child widgets:
	Glib::RefPtr<Gtk::TreeStore> refTreeModel;

private:
	Glib::RefPtr<Gtk::Builder> _builder;

	Gtk::Button* _quitButton;
	Gtk::TreeView* _treeView;

	void setTreeView(JSON root);
	void addChild(JSON root, Gtk::TreeModel::Row& row);
};

#endif // ROOT_PANELROBOTSTATE_H
