#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    currIn(""), currAus(""), currComIn(""), currComAus(""),
    listIterator(0), newWords(false), firstTry(true)
{
    ui->setupUi(this);

    hideFrames(-1);
    ui->centralWidget->setGeometry(0, 0, ui->startWidget->width() + 20, ui->startWidget->height() + 20);

    screen = QApplication::desktop()->availableGeometry();

    updateVocRecords();

    connect(ui->pushButtonINSERT, SIGNAL(clicked(bool)), this, SLOT(insert(bool)));
    connect(ui->pushButton_2, SIGNAL(clicked(bool)), this, SLOT(getVocs(bool)));
    connect(ui->checkPushButton, SIGNAL(clicked(bool)), this, SLOT(checkVoc(bool)));
    connect(ui->nextPushButton, SIGNAL(clicked(bool)), this, SLOT(nextVoc(bool)));
    connect(ui->pushButton, SIGNAL(clicked(bool)), this, SLOT(addVoc(bool)));
    connect(ui->pushButton_3, SIGNAL(clicked(bool)), this, SLOT(showList(bool)));
    connect(ui->checkBox, SIGNAL(stateChanged(int)), this, SLOT(hideIrregular(int)));
    //connect(ui->centralWidget, SIGNAL(), this, SLOT(resizeWindow()));
}

void MainWindow::showList(bool)
{
    if(newWords){
        updateVocRecords();
    }

    //TODO: Trying to set the size of the whole window doesn't really works and i don't know for what the x,y coordiantes does
    //ui->centralWidget->setGeometry(10, 10, screen.width() - 10 , screen.height() - 10);
    //qDebug() << "screenheight: " << screen.width() << "; screenheight: " << screen.height();
    //this->setCentralWidget(ui->vocableTable);
    //this->adjustSize();

    QRect screenRect;
    hideFrames(2);
    ui->vocableTable->setGeometry(gap, gap, ui->vocableTable->geometry().width(), ui->centralWidget->height() - ui->startWidget->height() - ui->statusBar->height() + gap);
    ui->vocableTable->setGeometry(gap, gap, ui->centralWidget->width() - 2*gap,ui->startWidget->y() - 2*gap);
    qDebug() << ui->centralWidget->height();
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
    hideFrames(1);

    this->nextVoc(true);
}

// Vergleicht ob der eingegebene Wert mit dem aktuellen Übersetztung, in currIn, übereinstimmt
void MainWindow::checkVoc(bool)
{
    if(ui->translation->text() == currIn){
        ui->successLabel->setText("YEAH!");
        if(firstTry){
            //db.updateRec(currIn, currAus, true);
        }
        nextVoc(false);
    }
    else{
        ui->successLabel->setText("Falsch: " + currIn + ", " + currComIn);
        //db.updateRec(currIn, currAus, false);
        firstTry = false;
    }

    if(currIn == "")
        qDebug() << db.updateRecAusland(ui->translation->text(), currAus);
}

// Loads one word pair out of the vocRecords
void MainWindow::nextVoc(bool)
{
    if(newWords){
        updateVocRecords();
        listIterator = 0;
    }

    firstTry = true;
    currIn = vocRecords.at(1).at(listIterator);
    currAus = vocRecords.at(0).at(listIterator);
    currComIn = vocRecords.at(3).at(listIterator);
    currComAus = vocRecords.at(2).at(listIterator);
    listIterator += 1;

    ui->vocable->setText(currAus + currComAus);
    ui->translation->setText("");
}

// Makes the "addWidget" visible and others invisible
void MainWindow::addVoc(bool)
{
   hideFrames(0);
   hideIrregular(0);
}

// Loads the voc from the database and save them in 'vocRecords', sets the listCount and 'newWords' to false
void MainWindow::updateVocRecords()
{
    vocRecords = db.getVocs();
    listCount = vocRecords.at(0).count();
    newWords = false;
}

// Hides frames
void MainWindow::hideFrames(int number)
{
     ui->addWidget->setVisible(false);
     ui->learnWidget->setVisible(false);
     ui->vocableTable->setVisible(false);

     if(number == 0)
         ui->addWidget->setVisible(true);

     if(number == 1)
         ui->learnWidget->setVisible(true);

     if(number == 2)
         ui->vocableTable->setVisible(true);
}

// Hides irregual input
void MainWindow::hideIrregular(int check)
{
    if(check == 0){
        ui->containerWidget_1->setVisible(false);
        ui->containerWidget_2->setVisible(false);
    }

    if(check == 2){
        ui->containerWidget_1->setVisible(true);
        ui->containerWidget_2->setVisible(true);
    }
}


// Change the size of the widget if the windowsize is beeing changed
void MainWindow::resizeWindow()
{

}
