#include "PanelRobotState.h"
#include <resources/EmbeddedFiles.h>

PanelRobotState::PanelRobotState() {
	_builder = Gtk::Builder::create_from_string(EmbeddedFiles::readText("glade/state-panel.glade"));
	_builder->get_widget("quit-button", _quitButton);
	_builder->get_widget("tree-view", _treeView);

	// We set columns
	refTreeModel = Gtk::TreeStore::create(m_columns);
	_treeView->set_model(refTreeModel);
	_treeView->append_column("Name", m_columns.col_name);
	_treeView->append_column("Status", m_columns.col_status);

	// signals
	_quitButton->signal_clicked().connect(sigc::mem_fun(*this, &PanelRobotState::on_button_quit));
	_treeView->signal_row_activated().connect(sigc::mem_fun(*this, &PanelRobotState::on_treeview_row_activated));
	show_all_children();

	// Test
	setTreeView(generateTestJSON());

	Widget* root;
	_builder->get_widget("root", root);
	add(*root);
}

void PanelRobotState::refreshPanel(JSON json) {
	setTreeView(json);
}

void PanelRobotState::setTreeView(JSON root) {
	if(!root.empty()) {
		for(auto it = root.begin(); it != root.end(); ++it) {
			// We create father row
			Gtk::TreeModel::Row row = *(refTreeModel->append());
			row[m_columns.col_name] = it.key();
			row[m_columns.col_status] = "";
			// We add his children
			addChild(*it, row);
		}
	}
}


void PanelRobotState::addChild(JSON root, Gtk::TreeModel::Row& row) {
	if(!root.empty()) {
		for(JSON::const_iterator it = root.begin(); it != root.end(); ++it) {
			// We create child row
			Gtk::TreeModel::Row childrow = *(refTreeModel->append(row.children()));
			childrow[m_columns.col_name] = it.key();
			childrow[m_columns.col_status] = it.value().dump();
		}
	}
}

// We quit the panel when we click on Quit button
void PanelRobotState::on_button_quit() {
	hide();
}

// We veil the row's details when clicking it
void PanelRobotState::on_treeview_row_activated(const Gtk::TreeModel::Path& path, Gtk::TreeViewColumn* column) {
	Gtk::TreeModel::iterator iter = refTreeModel->get_iter(path);
	if(iter) {
		Gtk::TreeModel::Row row = *iter;
		std::cout << "Row activated: Name = " << row[m_columns.col_name] << std::endl;
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
