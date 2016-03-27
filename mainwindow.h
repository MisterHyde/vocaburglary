#ifndef MAINWINDOW_H
#define MAINWINDOW_H
//#define ANDROID
#include <QMainWindow>
#include <QFile>
#include <QString>
#include <QTextStream>
#include <QPalette>
#include <QDebug>
#include <QDesktopWidget>
#include <QMessageBox>
#include <QResizeEvent>
#include <iostream>
#include <QSizePolicy>
#include "customfunctions.h"
#ifdef ANDROID
#include "managedjsondb.h"
#else
#include "managedb.h"
#endif

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
    enum Boxes {nothing = -1, addBox, learnBox, listBox, buttonBox, irregularBox};

    Ui::MainWindow *ui;

#ifdef ANDROID
    ManagedJsonDb db;
#else
    Managedb db;
#endif
    QList<QStringList> vocRecords;  ///< List with all
    // Instead of using for the current word varables for all information use a pointer to the place in the data structure
    QStringList currIn;         ///< Current list of translations
    QString currInOrigin;   ///< Holds the string which is stored in the database
    QString currAus;        ///< Current word in foreign language
    QString currComIn;      ///< Current comment for the word in first language
    QString currComAus;     ///< Current comment for the word in foreign language
    QString *currCorrect;	///< A pointer to the current record's indicator if the user answered it right
    int listIterator;   ///< Index of the list of voc
    int listCount;      //!< Number of voc
    bool newWords;      //!< True if a new voc pair was insert this variable is set to true so the learning list can be updateted at runtime
    bool firstTry;      //!< True when it was the first try on a voc
    QRect screen;       ///< Holds the size of the screen
    QRect window;
    bool usingAndroid;
    QSize spacerSize;
    QSize spacerSize2;
    QSize spacerSize3;
    bool languageDirection; ///< true if from auslandisch to inlandisch else false

    const int gap = 10;

    void hideFrames(Boxes number);
    void chopVocRecords();
    //void resizeEvent(QResizeEvent *event);

private slots:
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
    void showStartWidget();
    void changeLanguage(bool); ///< Change the asked language
    void updateVocRecords(bool);
    void changedWord(int row, int column); ///< Slot which writes the changed word to the datarecord
};

#endif // MAINWINDOW_H
