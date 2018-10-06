#include "mainwindow.hpp"
#include "ui_mainwindow.h"

// PRIVATE

SimulationRenderer *MainWindow::get_SimRend(int t) {
	SimulationRenderer *sr = nullptr;

	switch (t) {
		case 0:
			sr = ui->GL_scene0;
			break;

		default:
			sr = ui->GL_labScene;
			break;
	}

	return sr;
}

SimulationRenderer *MainWindow::get_SimRend() {
	return get_SimRend(current_tab);
}

void MainWindow::make_scene(SimulationRenderer *sr) {
	switch (current_tab) {
		case 0:
			make_scene_0(sr);
			break;

		default:
			make_scene_lab(sr);
			break;
	}
}

// PRIVATE SLOTS

void MainWindow::on_PBrun_clicked() {
	ui->PBreset->setEnabled(false);
	ui->PBclear->setEnabled(false);

	SimulationRenderer *sr = get_SimRend();
	sr->run_simulation();
}

void MainWindow::on_PBpause_clicked() {
	ui->PBreset->setEnabled(true);
	ui->PBclear->setEnabled(true);

	SimulationRenderer *sr = get_SimRend();
	sr->pause_simulation();
}

void MainWindow::on_PBreset_clicked() {
	SimulationRenderer *sr = get_SimRend();
	sr->reset_simulation();
}

void MainWindow::on_PBclear_clicked() {
	SimulationRenderer *sr = get_SimRend();
	sr->clear_simulation();
}

void MainWindow::on_TWscenes_currentChanged(int index) {
	// old renderer: pause simulation
	SimulationRenderer *old_sr = get_SimRend();
	old_sr->pause_simulation();

	current_tab = index;

	// make appropriate scene for the new renderer
	make_scene( get_SimRend() );

	// enable all buttons
	ui->PBreset->setEnabled(true);
	ui->PBclear->setEnabled(true);
}

void MainWindow::on_lEfps_returnPressed() {
	for (int i = 0; i < ui->TWscenes->count(); ++i) {
		SimulationRenderer *sr = get_SimRend(i);
		sr->set_fps( ui->lEfps->text().toFloat() );
	}
}

void MainWindow::on_CBfpsCount_toggled(bool checked) {
	for (int i = 0; i < ui->TWscenes->count(); ++i) {
		SimulationRenderer *sr = get_SimRend(i);
		sr->set_show_fps(checked);
	}
}

// PUBLIC

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent),
	  ui(new Ui::MainWindow)
{
	ui->setupUi(this);

	current_tab = ui->TWscenes->currentIndex();
}

MainWindow::~MainWindow() {
	delete ui;
}
