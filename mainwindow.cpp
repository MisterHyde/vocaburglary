#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    currIn(""), currAus(""), currComIn(""), currComAus(""),
    listIterator(0), newWords(false), firstTry(true), usingAndroid(true)
{
    ui->setupUi(this);
    spacerSize = ui->verticalSpacer->sizeHint();
    spacerSize2 = ui->verticalSpacer_2->sizeHint();
    spacerSize3 = ui->verticalSpacer_3->sizeHint();

    db = new Managedb(this);

    hideFrames(nothing);

#ifndef ANDROID
    ui->centralWidget->setGeometry(0, 0, ui->startWidget->width() + 20, ui->startWidget->height() + 20);
#endif

    screen = QApplication::desktop()->availableGeometry();
    window = ui->centralWidget->geometry();
    languageDirection = true;

    updateVocRecords(true);

    connect(ui->pushButtonINSERT, SIGNAL(clicked(bool)), this, SLOT(insert(bool)));
    connect(ui->pushButtonStartExercise, SIGNAL(clicked(bool)), this, SLOT(startExercise(bool)));
    connect(ui->pushButtonCheck, SIGNAL(clicked(bool)), this, SLOT(checkVoc(bool)));
    connect(ui->pushButtonNext, SIGNAL(clicked(bool)), this, SLOT(nextVoc(bool)));
    connect(ui->pushButtonStartInsert, SIGNAL(clicked(bool)), this, SLOT(addVoc(bool)));
    connect(ui->pushButtonStartLearn, SIGNAL(clicked(bool)), this, SLOT(showList(bool)));
    connect(ui->checkBox, SIGNAL(stateChanged(int)), this, SLOT(hideIrregular(int)));
    connect(ui->pushButtonExportDB, SIGNAL(clicked(bool)), this, SLOT(exportDBtoJson(bool)));
    connect(ui->pushButton, SIGNAL(clicked(bool)), this, SLOT(importDBfromJson(bool)));
    connect(ui->pushButtonBack, SIGNAL(clicked()), this, SLOT(showStartWidget()));
    connect(ui->pushButtonChLang, SIGNAL(clicked(bool)), this, SLOT(changeLanguage(bool)));
    connect(ui->pushButtonRandomize, SIGNAL(clicked(bool)), this, SLOT(updateVocRecords(bool)));
    connect(db, SIGNAL(quit()), this, SLOT(close()));

    //connect(ui->centralWidget, SIGNAL(), this, SLOT(resizeWindow()));

    //CustomFuctions::parseXml("DCE_most_frequent_words_07_08_basic_list_v2.xml");
}

void MainWindow::showList(bool)
{
    if(newWords){
        updateVocRecords(true);
        newWords = false;
    }

    //TODO: Trying to set the size of the whole window doesn't really works and i don't know for what the x,y coordiantes does
    //ui->centralWidget->setGeometry(10, 10, screen.width() - 10 , screen.height() - 10);
    //qDebug() << "screenheight: " << screen.width() << "; screenheight: " << screen.height();
    //this->setCentralWidget(ui->vocableTable);
    //this->adjustSize();

    //QRect screenRect;
    hideFrames(listBox);
    // Strange things I do here trying to make a dynamic window size...
    //ui->vocableTable->setGeometry(gap, gap, ui->vocableTable->geometry().width(), ui->centralWidget->height() - ui->startWidget->height() - ui->statusBar->height() + gap);
    //ui->vocableTable->setGeometry(gap, gap, ui->centralWidget->width() - 2*gap,ui->startWidget->y() - 2*gap);
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

    // arrange the size of the columns
    //ui->vocableTable->setColumnWidth(0, 150);
    //ui->vocableTable->setColumnWidth(1, 150);
    QHeaderView* header = ui->vocableTable->horizontalHeader();
    header->setStretchLastSection(true);

    QTableWidget *tw = ui->vocableTable;
    ui->vocableTable->setMaximumSize(CustomFunctions::myGetQTableWidgetSize(tw));
    //ui->vocableTable->set
    //QRect g = ui->verticalSpacer->geometry();
    //ui->verticalSpacer->setGeometry(QRect(ui->verticalSpacer->x(), ui->verticalSpacer->y()));
    connect(ui->vocableTable, SIGNAL(cellChanged(int,int)), this, SLOT(changedWord(int,int)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

// Slot der aktiviert wird wenn der Button "INSERT" gedrückt wurde.
// Ruft die Methode von managedb auf, die den insert statement ausführt, und leert die Eingabefelder
void MainWindow::insert(bool)
{
    //TODO If entry exists show a message on the status bar
    bool success = db->insertRec(ui->inIn->text(), ui->inAus->text(), ui->inKommentar->text(), ui->ausKommentar->text());

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
    if(vocRecords.size() == 0){
        hideFrames(nothing);
        ui->statusBar->showMessage(tr("Error: No words in cache..."));
        return;
    }

    hideFrames(learnBox);

    this->nextVoc(true);
}

// Loads one word pair out of the vocRecords
void MainWindow::nextVoc(bool)
{
    // If there are new words inserted in this session reload the vocabular list
    if(newWords){
        ui->statusBar->showMessage(tr("New words occured reload list"), 10000);
        updateVocRecords(true);
        listIterator = 0;
    }

    if(listIterator == vocRecords.size()){
        listIterator = 0;
        QMessageBox box;
        box.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
        box.setText(tr("Congratulations!\n You've completed the list. Want some new?"));
        // If the "Ok" button was clicked mix the vocabulary new and start over again
        if(box.exec() == QMessageBox::Ok){
            chopVocRecords();
        }
        else{
            hideFrames(nothing);
        }
    }

    firstTry = true;
    // Save the current record in the attributes
    currInOrigin = vocRecords.at(listIterator).at(0);
    // Fill the attributes depending on the languageDirection
    if(languageDirection){
        QString buff = currInOrigin.trimmed();
        currIn = buff.split(QRegExp("\\,"));
        currAus = vocRecords.at(listIterator).at(1);
        currComIn = vocRecords.at(listIterator).at(2);
        currComAus = vocRecords.at(listIterator).at(3);
        currCorrect = &vocRecords[listIterator][6];
    }
    else{
        currIn = QStringList(vocRecords.at(listIterator).at(1));
        currAus = vocRecords.at(listIterator).at(0);
        currComIn = vocRecords.at(listIterator).at(3);
        currComAus = vocRecords.at(listIterator).at(2);
        currCorrect = &vocRecords[listIterator][6];
    }
    // Till the end of the list load the next word
//    if(listIterator <= vocRecords.size()-1){ // TODO Doppeltgemoppelt?
        ui->vocable->setText(currAus + currComAus);
        ui->translation->setText("");
        ui->labelRightCount->setText("Right counter: " + vocRecords.at(listIterator).at(4));
        ui->labelWrongCount->setText("Wrong counter " + vocRecords.at(listIterator).at(5));
        listIterator += 1;

        // TODO Doppeltgemoppelt?
        // If the last record done ask the user if he wants to start over again or leave

//    }
//    else{
//        listIterator = 0;
//        QMessageBox box;
//        box.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
//        box.setText(tr("Congratulations!\n You've completed the list. Want some new?"));
//        // If the "Ok" button was clicked mix the vocabulary new and start over again
//        if(box.exec() == QMessageBox::Ok){
//            chopVocRecords();
//        }
//        else{
//            hideFrames(nothing);
//        }
//    }
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
            ui->successLabel->setText("YEAH!" + ui->translation->text() + tr(" war richtig :-)"));
        }
        if(firstTry){
            //updateRank needs as second parameter the foreign word
            if(languageDirection)
                db->updateRank(currInOrigin, currAus, true);
            else
                db->updateRank(currInOrigin, currIn.first(), true);
        }
        nextVoc(false);
    }
    else{
        if(!currComIn.isEmpty())
            currComIn = ", " + currComIn;
        ui->successLabel->setText("Falsch: " + CustomFunctions::undoStringList(currIn) + currComIn);
        //updateRank needs as second parameter the foreign word
        if(languageDirection)
            db->updateRank(currInOrigin, currAus, false);
        else
            db->updateRank(currInOrigin, currIn.first(), false);

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
void MainWindow::updateVocRecords(bool)
{
    vocRecords = db->getVocs();
    if(vocRecords.isEmpty())
        qDebug() << "Hier, MainWindow::updateVocRecords(), ist ein Fehler passiert";
    listCount = vocRecords.count();
    newWords = false;
}

// Hides frames
void MainWindow::hideFrames(Boxes number)
{
     ui->addWidget->setVisible(false);
     ui->learnWidget->setVisible(false);
     ui->vocableTable->setVisible(false);
     ui->pushButtonBack->setVisible(false);
     ui->startWidget->setVisible(false);

     switch(number){
        case addBox:
         ui->addWidget->setVisible(true);
         ui->pushButtonBack->setVisible(true);
         ui->verticalSpacer->changeSize(-1, -1, QSizePolicy::Expanding);
         ui->verticalSpacer_2->changeSize(-1, -1, QSizePolicy::Expanding);
         ui->verticalSpacer_3->changeSize(-1, -1, QSizePolicy::Expanding);
         break;
        case learnBox:
         ui->learnWidget->setVisible(true);
         ui->pushButtonBack->setVisible(true);
         ui->pushButtonBack->setGeometry(window.x() - ui->pushButtonBack->height(), window.y() - ui->pushButtonBack->width(), ui->pushButtonBack->width(), ui->pushButton->height());
         ui->verticalSpacer->changeSize(-1,-1, QSizePolicy::Expanding);
         //ui->verticalSpacer_2->changeSize(-1,-1, QSizePolicy::Expanding);
         //ui->verticalSpacer_3->changeSize(-1,-1, QSizePolicy::Expanding);
         break;
        case listBox:
         disconnect(ui->vocableTable, SIGNAL(cellChanged(int,int)), this, SLOT(changedWord(int,int)));
         ui->vocableTable->setVisible(true);
         ui->vocableTable->setGeometry(window);
         ui->pushButtonBack->setVisible(true);
         ui->verticalSpacer->changeSize(-1,-1, QSizePolicy::Minimum);
         ui->verticalSpacer_2->changeSize(-1,-1, QSizePolicy::Minimum);
         ui->verticalSpacer_3->changeSize(-1,-1, QSizePolicy::Minimum);
         break;
        default:
         ui->verticalSpacer->changeSize(-1,-1, QSizePolicy::Expanding);
         ui->verticalSpacer_2->changeSize(-1,-1, QSizePolicy::Expanding);
         ui->verticalSpacer_3->changeSize(-1,-1, QSizePolicy::Expanding);
         ui->startWidget->setVisible(true);
     }

     spacerSize = ui->verticalSpacer->sizeHint();
     spacerSize2 = ui->verticalSpacer_2->sizeHint();
     spacerSize3 = ui->verticalSpacer_3->sizeHint();
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
    db->dbToJson();
}

void MainWindow::importDBfromJson(bool)
{
    db->jsonToDb();
    this->newWords=true;
}

// Creates a new vocRecords which contains just the wrong words
void MainWindow::chopVocRecords()
{
    for(int i=0; i<vocRecords.size(); i++){
        if(vocRecords.at(i).at(5) == "true"){
            vocRecords.removeAt(i);
        }
    }
    if(vocRecords.size() == 0){
        hideFrames(nothing);
    }
}

//void MainWindow::resizeEvent(QResizeEvent *event){
////    ui->centralWidget->setGeometry(this->x(),this->y(),event->size().width(),event->size().height);
//    //ui->mainLayout->setGeometry(0, 0, event->size().width(), event->size().height());
//    ui->mainLayout->setGeometry(QRect(0, 0, event->size().width(), event->size().height()));
//    qDebug() << "<name>:" << "x" << "y" << "width" << "height";
//    qDebug() << "Main Window:" << "0" << "0" << event->size().width() << event->size().height();
//    qDebug() << "vocableTable" << ui->vocableTable->x() << ui->vocableTable->y() << ui->vocableTable->width() << ui->vocableTable->height();
//    qDebug() << "startWidget:" << ui->startWidget->x() << ui->startWidget->y() << ui->startWidget->width() << ui->startWidget->height();
//}

void MainWindow::showStartWidget()
{
    hideFrames(nothing);
}


void MainWindow::changeLanguage(bool)
{
    // Change the text displayed by the pushed button
    QString newText = "";
    QStringList oldText = ui->pushButtonChLang->text().split(" ");

    if(languageDirection){
        newText = oldText.at(0) + " <- " + oldText.at(2);
    }
    else{
        newText = oldText.at(0) + " -> " + oldText.at(2);
    }

    ui->pushButtonChLang->setText(newText);
    languageDirection = !languageDirection;

    listIterator = 0;
    nextVoc(true);
}

void MainWindow::changedWord(int row, int column)
{
    QString newText, oldIn, oldOut;
    newText = ui->vocableTable->item(row, column)->text();
    // TODO Check for double commas

    int index = 0;

    if(column == 1){
        index = 0;
    }
    else{
        index = 1;
    }

    oldOut = ui->vocableTable->item(row, index)->text();
    oldIn = vocRecords.at(row).at(index);
    QStringList buff = vocRecords.at(row);
    //buff.insert(index,newText);
    buff.replace(index, newText);

    if(newText.isEmpty()){
        vocRecords.removeAt(row);
    }
    else{
        vocRecords.insert(row, buff);
    }

    db->updateRecAusland(newText, oldIn, oldOut, column);
}
