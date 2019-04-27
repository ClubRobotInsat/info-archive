#include "PanelRobotState.h"
#include <resources/EmbeddedFiles.h>

PanelRobotState::PanelRobotState() {
	_builder = Gtk::Builder::create_from_string(EmbeddedFiles::readText("glade/state-panel.glade"));
	_builder->get_widget("quit-button", _quitButton);
	_builder->get_widget("tree-view", _treeView);

	// We set columns
	refTreeModel = Gtk::TreeStore::create(m_columns);
	_treeView->set_model(refTreeModel);

	// signals
	_quitButton->signal_clicked().connect(sigc::mem_fun(*this, &PanelRobotState::on_button_quit));
	_treeView->signal_row_activated().connect(sigc::mem_fun(*this, &PanelRobotState::on_treeview_row_activated));
	show_all_children();
}

void PanelRobotState::refreshPanel(JSON json) {
	setTreeView(json);
}

void PanelRobotState::setTreeView(JSON& root) {
	if(!root.empty()) {
		for(auto it = root.begin(); it != root.end(); ++it) {

			// We create father row
			Gtk::TreeModel::Row row = *(refTreeModel->append());
			row[m_columns.col_name] = it.key();

			// We add his children
			addChild(*it, row);
		}
	}
	_treeView->append_column("Name", m_columns.col_name);
}


void PanelRobotState::addChild(JSON& root, Gtk::TreeModel::Row& row) {
	if(!root.empty()) {
		for(auto it = root.begin(); it != root.end(); ++it) {

			// We create child row
			Gtk::TreeModel::Row childrow = *(refTreeModel->append(row.children()));
			childrow[m_columns.col_name] = it.key();
			childrow[m_columns.col_status] = it.value();

			// If we got great children, we add them
			addChild(*it, childrow);
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


// void PanelRobotState::refreshPanel(JSON json) {
//	setElement(json, [](JSON::const_iterator it) {
//		std::cout << *it << std::endl; // Print in console
//	});
//}
//
// template <class UnaryFunction>
// void PanelRobotState::setElement(JSON json, UnaryFunction f) {
//	for(auto it = json.begin(); it != json.end(); ++it) {
//		if(it->is_structured()) {
//			PanelRobotState::setElement(*it, f);
//		} else {
//			f(it);
//		}
//	}
//}


// JSON PanelRobotState::generateTestJSON() {
//	JSON json;
//	json["servo1"] = JSON();
//	json["servo2"] = JSON();
//
//	json["servo1"]["blocked"] = true;
//	json["servo2"]["blocked"] = false;
//	json["servo1"]["mode"] = "Speed";
//	json["servo1"]["color"] = "Color";
//
//	return json.dump();
//}

// int main(){
//     std::cout << "test" << std::endl;
//     JSON json = PanelRobotState::generateTestJSON();
//     PanelRobotState
// }