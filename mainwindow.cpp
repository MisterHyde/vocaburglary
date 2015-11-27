#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    currIn(""), currAus(""), currComIn(""), currComAus(""),
    listIterator(0), newWords(false), firstTry(true)
{
    ui->setupUi(this);

    hideFrames(nothing);
    ui->centralWidget->setGeometry(0, 0, ui->startWidget->width() + 20, ui->startWidget->height() + 20);

    screen = QApplication::desktop()->availableGeometry();

    updateVocRecords();

    connect(ui->pushButtonINSERT, SIGNAL(clicked(bool)), this, SLOT(insert(bool)));
    connect(ui->pushButtonStartExercise, SIGNAL(clicked(bool)), this, SLOT(startExercise(bool)));
    connect(ui->pushButtonCheck, SIGNAL(clicked(bool)), this, SLOT(checkVoc(bool)));
    connect(ui->pushButtonNext, SIGNAL(clicked(bool)), this, SLOT(nextVoc(bool)));
    connect(ui->pushButtonStartInsert, SIGNAL(clicked(bool)), this, SLOT(addVoc(bool)));
    connect(ui->pushButtonStartLearn, SIGNAL(clicked(bool)), this, SLOT(showList(bool)));
    connect(ui->checkBox, SIGNAL(stateChanged(int)), this, SLOT(hideIrregular(int)));
    connect(ui->pushButtonExportDB, SIGNAL(clicked(bool)), this, SLOT(exportDBtoJson(bool)));
    connect(ui->pushButton, SIGNAL(clicked(bool)), this, SLOT(importDBfromJson(bool)));
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
    hideFrames(listBox);
    // Strange things I do here trying to make a dynamic window size...
    ui->vocableTable->setGeometry(gap, gap, ui->vocableTable->geometry().width(), ui->centralWidget->height() - ui->startWidget->height() - ui->statusBar->height() + gap);
    ui->vocableTable->setGeometry(gap, gap, ui->centralWidget->width() - 2*gap,ui->startWidget->y() - 2*gap);
    //qDebug() << "height: " << ui->centralWidget->height();
    // Inserts in a QTableWidget as many items as the variable 'listCount' tells
    ui->vocableTable->setRowCount(listCount);
    for(int i=0; i<listCount; i++){
        QTableWidgetItem *in = new QTableWidgetItem(vocRecords.at(i).at(0));
        QTableWidgetItem *aus = new QTableWidgetItem(vocRecords.at(i).at(1));
        ui->vocableTable->setItem(i, 0, aus);
        ui->vocableTable->setItem(i, 1, in);
        //qDebug() << i << vocRecords.at(i).at(0) << vocRecords.at(i).at(0);
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

// Shows the exercise frame and loads the first word pair
void MainWindow::startExercise(bool)
{
    hideFrames(learnBox);

    this->nextVoc(true);
}

// Loads one word pair out of the vocRecords
void MainWindow::nextVoc(bool)
{
    // If the last record done ask the user if he wants to start over again or leave
    if(listIterator == listCount){
        listIterator = 0;
        QMessageBox box;
        box.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
        box.setText(tr("Congratulations!\n You've completed the list. Want some new?"));
        // If the "Ok" button was clicked mix the vocabulary new and start over again
        if(box.exec() == 0x00000400){
            chopVocRecords();
        }
        else{
            hideFrames(nothing);
        }
    }

    // If there are new words inserted in this session reload the vocabular list
    if(newWords){
        updateVocRecords();
        listIterator = 0;
    }

    firstTry = true;
    // Save the current record in the attributes
    QString buff = vocRecords.at(listIterator).at(0);
    buff = buff.remove(" ");
    currIn = buff.split(QRegExp("\\,"));
    currAus = vocRecords.at(listIterator).at(1);
    currComIn = vocRecords.at(listIterator).at(2);
    currComAus = vocRecords.at(listIterator).at(3);
    currCorrect = &vocRecords[listIterator][4];
    // If the list is at the end set the iterator to the begin else to the next record
    listIterator += 1;

    ui->vocable->setText(currAus + currComAus);
    ui->translation->setText("");
}

// Compare if the user input is equal to the right translation, saved in "currIn"
void MainWindow::checkVoc(bool)
{
    // Checks if the the user input is containted in the answer string
    if(currIn.contains(ui->translation->text())){
        *currCorrect = QString("true");
        // If size is bigger than one show which other meanings the word has
        if(currIn.size() > 1){
            QString buff = "";
            for(int i=0;i<currIn.size();++i){
                if(currIn.at(i) != ui->translation->text()){
                   buff += currIn.at(i);
                   if(i != currIn.size()-1)
                       buff += ", ";
                }
            }
            ui->successLabel->setText("YEAH but also: " + buff);
        }
        else{
            ui->successLabel->setText("YEAH!");
        }
        if(firstTry){
            //db.updateRec(currIn, currAus, true);
        }
        nextVoc(false);
    }
    else{
        if(!currComIn.isEmpty())
            currComIn = ", " + currComIn;
        ui->successLabel->setText("Falsch: " + CustomFuctions::undoStringList(currIn) + currComIn);
        //db.updateRec(currIn, currAus, false);
        firstTry = false;
    }
}

// Makes the "addWidget" visible and others invisible
void MainWindow::addVoc(bool)
{
   hideFrames(addBox);
   hideIrregular(0);
}

// Loads the voc from the database and save them in 'vocRecords', sets the listCount and 'newWords' to false
void MainWindow::updateVocRecords()
{
    vocRecords = db.getVocs();
    if(vocRecords.isEmpty())
        qDebug() << "Hier ist ein Fehler passiert";
    listCount = vocRecords.count();
    newWords = false;
}

// Hides frames
void MainWindow::hideFrames(boxes number)
{
     ui->addWidget->setVisible(false);
     ui->learnWidget->setVisible(false);
     ui->vocableTable->setVisible(false);

     if(number == addBox)
         ui->addWidget->setVisible(true);

     if(number == learnBox)
         ui->learnWidget->setVisible(true);

     if(number == listBox)
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

void MainWindow::exportDBtoJson(bool)
{
    db.dbToJson();
}

void MainWindow::importDBfromJson(bool)
{
    db.jsonToDb();
}

// Creates a new vocRecords which contains just the wrong words
void MainWindow::chopVocRecords()
{
    for(int i=0; i<listCount; i++){
        if(vocRecords.at(i).at(5) == "true"){
            vocRecords.removeAt(i);
        }
    }
    if(vocRecords.size() == 0){
        hideFrames(nothing);
    }
}

void MainWindow::resizeEvent(QResizeEvent *event){
//    ui->centralWidget->setGeometry(this->x(),this->y(),event->size().width(),event->size().height);
    //ui->mainLayout->setGeometry(0, 0, event->size().width(), event->size().height());
    ui->mainLayout->setGeometry(QRect(0, 0, event->size().width(), event->size().height()));
    qDebug() << "<name>:" << "x" << "y" << "width" << "height";
    qDebug() << "Main Window:" << "0" << "0" << event->size().width() << event->size().height();
    qDebug() << "vocableTable" << ui->vocableTable->x() << ui->vocableTable->y() << ui->vocableTable->width() << ui->vocableTable->height();
    qDebug() << "startWidget:" << ui->startWidget->x() << ui->startWidget->y() << ui->startWidget->width() << ui->startWidget->height();
}

//
