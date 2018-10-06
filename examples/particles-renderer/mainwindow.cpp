#include "mainwindow.h"
#include "ui_mainwindow.h"

// PRIVATE

// PRIVATE SLOTS

void MainWindow::on_PBrun_clicked() {
	ui->GL_scene1->display_simulation();
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

void MainWindow::on_TWscenes_currentChanged(int index) {
	current_tab = index;

	// free the geometry
	// free the particles
	// clear simulator

	// make appropriate scene, depending on the index
	// enable/disable widgets depending on the index
}
