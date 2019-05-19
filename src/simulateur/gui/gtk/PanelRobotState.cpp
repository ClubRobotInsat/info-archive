#include "PanelRobotState.h"

#include <resources/EmbeddedFiles.h>

PanelRobotState::PanelRobotState() {
	_builder = Gtk::Builder::create_from_string(EmbeddedFiles::readText("glade/state-panel.glade"));
	_builder->get_widget("tree-view", _treeView);

	// We set columns
	_refTreeModel = Gtk::TreeStore::create(_columns);
	_treeView->set_model(_refTreeModel);
	_treeView->append_column("Name", _columns.col_name);
	_treeView->append_column("Status", _columns.col_status);

	// signals
	_treeView->signal_row_activated().connect(sigc::mem_fun(*this, &PanelRobotState::on_treeview_row_activated));

	// Test
	// setTreeView(generateTestJSON());

	Widget* root;
	_builder->get_widget("root", root);
	add(*root);
}

void PanelRobotState::refreshPanel(const JSON& json) {
	setTreeView(json);
}

void PanelRobotState::updateRow(Gtk::TreeModel::Row row, const JSON::const_iterator& it) {
	row[_columns.col_name] = it.key();

	if(!it.value().is_structured()) {
		row[_columns.col_status] = it.value().dump();
	} else {
		row[_columns.col_status] = "";
		addChild(*it, row);
	}
}

void PanelRobotState::setTreeView(const JSON& root) {
	if(!root.empty()) {
		for(JSON::const_iterator it = root.begin(); it != root.end(); ++it) {
			bool found = false;

			for(auto& child : _refTreeModel->children()) {
				if(child[_columns.col_name] == it.key()) {
					found = true;
					updateRow(child, it);
				}
			}

			// We create parent row
			if(!found) {
				Gtk::TreeModel::Row row = *(_refTreeModel->append());
				updateRow(row, it);
			}
		}
	}
}

void PanelRobotState::addChild(const JSON& root, Gtk::TreeModel::Row& row) {
	if(!root.empty()) {
		for(JSON::const_iterator it = root.begin(); it != root.end(); ++it) {
			bool found = false;

			for(auto& child : row.children()) {
				if(child[_columns.col_name] == it.key()) {
					found = true;
					updateRow(child, it);
				}
			}

			// We create child row
			if(!found) {
				Gtk::TreeModel::Row childrow = *(_refTreeModel->append(row.children()));
				updateRow(childrow, it);
			}
		}
	}
}

// We veil the row's details when clicking it
void PanelRobotState::on_treeview_row_activated(const Gtk::TreeModel::Path& path, Gtk::TreeViewColumn* column) {
	Gtk::TreeModel::iterator iter = _refTreeModel->get_iter(path);
	if(iter) {
		Gtk::TreeModel::Row row = *iter;
		// std::cout << "Row activated: Name = " << row[m_columns.col_name] << std::endl;
	}
}

// Generate a JSON for testing
JSON PanelRobotState::generateTestJSON() {

	JSON json;
	json["servo1"] = JSON();
	json["servo2"] = JSON();

	json["servo1"]["blocked"] = true;
	json["servo2"]["blocked"] = false;
	json["servo1"]["mode"] = "Speed";
	json["servo1"]["color"] = "Color";

	return json;
}
