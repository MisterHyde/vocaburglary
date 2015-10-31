#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFile>
#include <QString>
#include <QTextStream>
#include <QPalette>
#include <QDebug>
#include <QDesktopWidget>
#include <QMessageBox>
#include "managedb.h"
#include "customfunctions.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    Managedb db;
    QList<QStringList> vocRecords;  ///< List with all
    QStringList currIn;         ///< Current word in first language
    QString currAus;        ///< Current word in foreign language
    QString currComIn;      ///< Current comment for the word in first language
    QString currComAus;     ///< Current comment for the word in foreign language
    QString *currCorrect;	///< A pointer to the current record's indicator if the user answered it right
    int listIterator;   ///< Index of the list of voc
    int listCount;      //!< Number of voc
    bool newWords;      //!< True if a new voc pair was insert this variable is set to true so the learning list can be updateted at runtime
    bool firstTry;      //!< True when it was the first try on a voc
    QRect screen;       ///< Holds the size of the screen

    const int gap = 10;

    void updateVocRecords();
    void hideFrames(int number);
    void chopVocRecords();

public slots:
    void startExercise(bool);     ///< Loads a record of voc from the database
    void checkVoc(bool);    ///< Checks if the inserted translation match the word in 'currAus'
    void nextVoc(bool);     ///< Calls 'checkVoc()' and loads a new word
    void addVoc(bool);      ///< Shows the ui for adding a voc
    void insert(bool);      ///< Inserts a pair of voc from the user input to the database
    void showList(bool);    ///< Shows a list with all loaded voc
    void hideIrregular(int check);
    void resizeWindow();
    void exportDBtoJson(bool);  ///< Just a caller function for dbToJson() from the class Managedb
    void importDBfromJson(bool);
};

#endif // MAINWINDOW_H
