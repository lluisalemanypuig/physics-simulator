#pragma once

// C++ includes
#include <memory>
#include <random>

// Qt includes
#include <QMainWindow>

// physim includes
#include <physim/initialiser/initialiser.hpp>

// Custom includes
#include <base/geometry/rgeometry.hpp>
#include <base/model/model.hpp>
#include "simulation_renderer.hpp"
#include "utils.hpp"

namespace Ui {
	class MainWindow;
}

class MainWindow : public QMainWindow {
	Q_OBJECT

	private:
		Ui::MainWindow *ui;
		int current_tab;

		std::shared_ptr<rendered_model> sim_ball;

		std::default_random_engine eng;
		// generator for random numbers between 0 and 1
		std::uniform_real_distribution<float> U01;
		// generator for random numbers between 0 and 10
		std::uniform_real_distribution<float> U010;

	private:
		SimulationRenderer *get_SimRend(int t);
		SimulationRenderer *get_SimRend();
		QProgressBar *get_sim_bar(int t);
		QProgressBar *get_sim_bar();

		void get_init_bounce(physim::init::partinit_free& p);
		void get_init_friction(physim::init::partinit_free& p);
		void get_init_lifetime(physim::init::partinit_free& p);

		// 'make scene' functions
		void make_sim0(SimulationRenderer *sr);
		void make_sim1(SimulationRenderer *sr);
		void make_sim2(SimulationRenderer *sr);
		void make_sim3(SimulationRenderer *sr);
		void make_sim4(SimulationRenderer *sr);
		void make_sim5(SimulationRenderer *sr);
		void make_sim(SimulationRenderer *sr);

		void make_init_with_params(physim::init::initialiser& i);

		// assigns to current renderer parameters of simulation
		void set_params();
		// sets parameters and initialises scene and progress bar
		void init_environment();

	public slots:
		void simulation_done();

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
		void on_Slider_PointSize_sliderMoved(int pos);

	public:
		explicit MainWindow(QWidget *parent = nullptr);
		~MainWindow();
};
