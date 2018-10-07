#pragma once

// C++ includes
#include <iostream>
using namespace std;

// Qt includes
#include <QMainWindow>

// glm includes
#include <glm/vec3.hpp>

// Custom includes
#include "simulation_renderer.hpp"

namespace Ui {
	class MainWindow;
}

class MainWindow : public QMainWindow {
	Q_OBJECT

	private:
		Ui::MainWindow *ui;
		int current_tab;

	private:
		SimulationRenderer *get_SimRend(int t);
		SimulationRenderer *get_SimRend();

		void make_scene_0(SimulationRenderer *sr);
		void make_scene_lab(SimulationRenderer *sr);
		void make_scene(SimulationRenderer *sr);

	private slots:
		void on_PBrun_clicked();
		void on_PBpause_clicked();
		void on_PBreset_clicked();
		void on_PBclear_clicked();

		void on_TWscenes_currentChanged(int index);

		void on_lEfps_returnPressed();
		void on_CBfpsCount_toggled(bool checked);

	public:
		explicit MainWindow(QWidget *parent = 0);
		~MainWindow();
};
