#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    currIn(""), currAus(""), currComIn(""), currComAus(""),
    listIterator(0), newWords(false), firstTry(true)
{
    ui->setupUi(this);

    ui->addFrame->setVisible(false);
    ui->learnFrame->setVisible(false);
    ui->vocableTable->setVisible(false);
    ui->centralWidget->setGeometry(0, 0, ui->startFrame->width() + 20, ui->startFrame->height() + 20);

    screen = QApplication::desktop()->availableGeometry();

    updateVocRecords();

    connect(ui->pushButtonINSERT, SIGNAL(clicked(bool)), this, SLOT(insert(bool)));
    connect(ui->pushButton_2, SIGNAL(clicked(bool)), this, SLOT(getVocs(bool)));
    connect(ui->checkPushButton, SIGNAL(clicked(bool)), this, SLOT(checkVoc(bool)));
    connect(ui->nextPushButton, SIGNAL(clicked(bool)), this, SLOT(nextVoc(bool)));
    connect(ui->pushButton, SIGNAL(clicked(bool)), this, SLOT(addVoc(bool)));
    connect(ui->pushButton_3, SIGNAL(clicked(bool)), this, SLOT(showList(bool)));
}

void MainWindow::showList(bool)
{
    if(newWords){
        updateVocRecords();
    }

    //TODO: Trying to set the size of the whole window doesn't really works and i don't know for what the x,y coordiantes does
    ui->centralWidget->setGeometry(10, 10, screen.width(), screen.height());
    this->setCentralWidget(ui->vocableTable);
    this->adjustSize();

    QRect screenRect;
    ui->addFrame->setVisible(false);
    ui->learnFrame->setVisible(false);
    ui->vocableTable->setVisible(true);
    ui->vocableTable->setGeometry(gap, gap, ui->vocableTable->geometry().width(), ui->centralWidget->height() - ui->startFrame->height() - ui->statusBar->height() + gap);

    // Inserts in a QTableWidget as many items as the variable 'listCount' tells
    ui->vocableTable->setRowCount(listCount);
    for(int i=0; i<listCount; i++){
        QTableWidgetItem *in = new QTableWidgetItem(vocRecords.at(0).at(i));
        QTableWidgetItem *aus = new QTableWidgetItem(vocRecords.at(1).at(i));
        ui->vocableTable->setItem(i, 0, aus);
        ui->vocableTable->setItem(i, 1, in);
    }

    ui->vocableTable->setColumnWidth(0, 150);
    ui->vocableTable->setColumnWidth(1, 150);
}

MainWindow::~MainWindow()
{
    delete ui;
}

// Slot der aktiviert wird wenn der Button "INSERT" gedrückt wurde.
// Ruft die Methode von managedb auf, die den insert statement ausführt, und leert die Eingabefelder
void MainWindow::insert(bool)
{
    bool success = db.insertRec(ui->inIn->text(), ui->inAus->text(), ui->inKommentar->text(), ui->ausKommentar->text());

    if(success){
        ui->inIn->setText("");
        ui->inAus->setText("");
        ui->inKommentar->setText("");
        ui->ausKommentar->setText("");
        newWords = true;
    }
}

// Makes the
void MainWindow::getVocs(bool)
{
    ui->addFrame->setVisible(false);
    ui->learnFrame->setVisible(true);
    ui->vocableTable->setVisible(false);

    this->nextVoc(true);
}

// Vergleicht ob der eingegebene Wert mit dem aktuellen Übersetztung, in currIn, übereinstimmt
void MainWindow::checkVoc(bool)
{
    if(ui->translation->text() == currIn){
        ui->successLabel->setText("YEAH!");
        if(firstTry){
            db.updateRec(currIn, currAus, true);
        }
        nextVoc(false);
    }
    else{
        ui->successLabel->setText("Falsch: " + currIn + ", " + currComIn);
        db.updateRec(currIn, currAus, false);
        firstTry = false;
    }
}

// Loads one word pair out of the vocRecords
void MainWindow::nextVoc(bool)
{
    if(newWords){
        updateVocRecords();
        listIterator = 0;
    }

    firstTry = true;
    currIn = vocRecords.at(0).at(listIterator);
    currAus = vocRecords.at(1).at(listIterator);
    currComIn = vocRecords.at(2).at(listIterator);
    currComAus = vocRecords.at(3).at(listIterator);
    listIterator += 1;

    ui->vocable->setText(currAus + currComAus);
    ui->translation->setText("");
}

// Makes the "addFrame" visible and others invisible
void MainWindow::addVoc(bool)
{
    ui->addFrame->setVisible(true);
    ui->learnFrame->setVisible(false);
    ui->vocableTable->setVisible(false);
}

// Loads the voc from the database and save them in 'vocRecords', sets the listCount and 'newWords' to false
void MainWindow::updateVocRecords()
{
    vocRecords = db.getVocs();
    listCount = vocRecords.at(0).count();
    newWords = false;
}
