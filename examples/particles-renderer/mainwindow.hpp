#pragma once

// C++ includes
#include <functional>
#include <iostream>
#include <random>
using namespace std;

// Qt includes
#include <QMainWindow>
#include <QFileInfo>

// glm includes
#include <glm/glm.hpp>

// simulator includes
#include <simulator/initialiser/initialiser.hpp>
#include <simulator/initialiser/rect_shower.hpp>
#include <simulator/initialiser/rect_fountain.hpp>
#include <simulator/initialiser/hose.hpp>
#include <simulator/simulator.hpp>
using namespace physim;
using namespace init;

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

		default_random_engine eng;
		// generator for random numbers between 0 and 1
		uniform_real_distribution<float> U01;
		// generator for random numbers between 0 and 10
		uniform_real_distribution<float> U010;

	private:
		SimulationRenderer *get_SimRend(int t);
		SimulationRenderer *get_SimRend();
		QProgressBar *get_sim_bar(int t);
		QProgressBar *get_sim_bar();

		void get_init_bounce(partinit& p);
		void get_init_friction(partinit& p);
		void get_init_lifetime(partinit& p);

		// 'make scene' functions
		void make_sim0(SimulationRenderer *sr);
		void make_sim1(SimulationRenderer *sr);
		void make_sim2(SimulationRenderer *sr);
		void make_sim3(SimulationRenderer *sr);
		void make_sim(SimulationRenderer *sr);

		void make_init_with_params(initialiser& i);

	private slots:
		void on_PBrun_clicked();
		void on_PBpause_clicked();
		void on_PBreset_clicked();
		void on_PBclear_clicked();

		void on_TWscenes_currentChanged(int index);

		void on_lEBounce_returnPressed();
		void on_lEFriction_returnPressed();
		void on_lELifeTime_returnPressed();

		void on_lEfps_returnPressed();
		void on_lETimeStep_returnPressed();
		void on_lETotalTime_returnPressed();

		void on_CoBsolver_currentIndexChanged(const QString &arg1);

	public:
		explicit MainWindow(QWidget *parent = 0);
		~MainWindow();
};
