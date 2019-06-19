#ifndef ROOT_PANELROBOTSTATE_H
#define ROOT_PANELROBOTSTATE_H

#include "GtkInclude.h"
#include <Json.h>

class PanelRobotState : public Gtk::Frame {
public:
	PanelRobotState();

	void refreshPanel(const JSON& json);
	JSON generateTestJSON();

protected:
	// Signal handler
	void on_treeview_row_activated(const Gtk::TreeModel::Path& path, Gtk::TreeViewColumn* column);

	// Tree model columns
	class ModelColumns : public Gtk::TreeModel::ColumnRecord {
	public:
		ModelColumns() {
			add(col_name);
			add(col_status);
		}

		// Define data type for each column
		Gtk::TreeModelColumn<Glib::ustring> col_name;
		Gtk::TreeModelColumn<Glib::ustring> col_status;
	};

	ModelColumns _columns;

	// Child widgets:
	Glib::RefPtr<Gtk::TreeStore> _refTreeModel;

private:
	Glib::RefPtr<Gtk::Builder> _builder;

	Gtk::TreeView* _treeView;

	void updateRow(Gtk::TreeModel::Row row, const JSON::const_iterator& it);
	void setTreeView(const JSON& root);
	void addChild(const JSON& root, Gtk::TreeModel::Row& row);
};

#endif // ROOT_PANELROBOTSTATE_H
