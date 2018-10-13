#include "mainwindow.hpp"
#include <QApplication>

int main(int argc, char *argv[]) {
	/* You see, Qt (either the library or QtCreator
	 * changes the C/C++'s default locale to the
	 * system's locale. And queer things happen when
	 * the locale changes (the system's locale may
	 * be the same as C/C++'s).
	 *
	 * So, if you're executing on a computer that
	 * has a locale that thinks floating point numbers
	 * have a comma as decimal separator then the
	 * code won't work: the object loader, that uses
	 * sscanf, will not parse the vertices' coordiantes
	 * correctly. In fact, it won't parse them at all.
	 *
	 * As an experiment, try parsing a floating point
	 * number with a comma using sscanf, before and after
	 * the 'QApplication a(argc,argv);'. Like this,
	 *
	 * string s = "v 0.987";
	 * float x;
	 * sscanf(s.c_str(), "v %f", &x);
	 *
	 * If done BEFORE, x will contain the value '0.987'.
	 * If done AFTER, x will contain the value '0', because
	 * sscanf will try to parse a floating point number
	 * assuming it is written with a comma: '0,987'.
	 */

	char *curr_locale1 = setlocale(LC_ALL, NULL);

	QApplication a(argc, argv);
	setlocale(LC_ALL, curr_locale1);

	MainWindow w;
	w.show();

	return a.exec();
}
