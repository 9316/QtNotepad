#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QTextStream>
#include <QTextCodec>
#include <QSortFilterProxyModel>
#include <QTextCursor>
#include <QHeaderView>
#include <QMessageBox>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
    ui->label->hide();
    ui->lineEdit->hide();
    ui->pushButton->hide();
    showbutton=false;
    QFont font;
    font.setPointSize(10);
    QHeaderView* headerView = new QHeaderView(Qt::Horizontal);
    headerView->setFont(font);
    headerView->setSizePolicy(QSizePolicy::Expanding ,QSizePolicy::Expanding);
    ui->menuBar->setFont(font);
    setWindowTitle("Notepad");



}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionNew_triggered()
{
    mFilename = "";
    ui->plainTextEdit->setPlainText("");
}

void MainWindow::on_actionOpen_triggered()
{
    QString file =QFileDialog::getOpenFileName(this,"გახსნა"); //QString file-ს გადავეცით მისამართი
    
    if(!file.isEmpty()){ //თუ არ არის ცარიელი ფაილი
        QFile sFile(file); //შევქმენით QFile file ობიექტი რომელსაც გდავეცით file მისამართი
        if(sFile.open(QFile::ReadOnly |QFile::Text)){
            mFilename = file;//mFilenames მიენიჭა file მისამართი
            QTextStream in(&sFile); //ტექსტის ნაკადური კლასის საშვალებით გადავეცით sfileგანთავსებული მისამართი
            QString text = in.readAll(); //წაკითხვა
            sFile.close();

            ui->plainTextEdit->setPlainText(text);
        }
    }
}

void MainWindow::on_actionPaste_triggered()
{
    ui->plainTextEdit->paste();
}

void MainWindow::on_actionSave_triggered()
{
    QFile sFile(mFilename);
    if(sFile.open(QFile::WriteOnly | QFile::Text)){
        QTextStream out(&sFile);

        out<<ui->plainTextEdit->toPlainText();

        sFile.flush();
        sFile.close();
    }
}

void MainWindow::on_actionSave_As_triggered()
{
    QString file =QFileDialog::getSaveFileName(this,"გახსნა"); //QString file-ს გადავეცით მისამართი

    if(!file.isEmpty()){ //თუ არ არის ცარიელი ფაილი
        mFilename = file;
        on_actionSave_triggered();
        }
    }


void MainWindow::on_actionCut_triggered()
{
    ui->plainTextEdit->cut();
}

void MainWindow::on_actionCopy_triggered()
{
    ui->plainTextEdit->copy();
}

void MainWindow::on_actionUndo_triggered()
{
    ui->plainTextEdit->undo();
}

void MainWindow::on_actionRendo_triggered()
{
    ui->plainTextEdit->redo();
}

void MainWindow::on_action_2_triggered()
{
    if(!showbutton){
     ui->lineEdit->show();
    ui->label->show();
     ui->pushButton->show();
     showbutton=true;
    }
    else {
       ui->lineEdit->hide();
       ui->label->hide();
       ui->pushButton->hide();
        showbutton=false;
   }


}


void MainWindow::on_pushButton_clicked()
{
    QString searchString = ui->lineEdit->text();
    QTextDocument *document = ui->plainTextEdit->document();



    if (searchString.isEmpty()) {
        QMessageBox::information(this, tr("ინფორმაცია"),"ველი არ არის შევსებული, გთხოვთ შეავსოთ ველი.");
        ui->lineEdit->setStyleSheet("QLineEdit{background:red};");
    } else {

        QTextCursor highlightCursor(document); //textCursor კლასის მეშვეობით მნიშვნეობას ვანიჭებთ documents
        QTextCursor cursor(document);//cursor ობიექტზე მიმთითებელი დავაფუძნეთ documet-ზე

        cursor.beginEditBlock(); //დაიწყოს რედაქტირების ბლოკი

        QTextCharFormat plainFormat(highlightCursor.charFormat());
        QTextCharFormat colorFormat = plainFormat;
        colorFormat.setForeground(Qt::red);

        while (!highlightCursor.isNull() && !highlightCursor.atEnd()) {
            highlightCursor = document->find(searchString, highlightCursor, QTextDocument::FindWholeWords);

            if (!highlightCursor.isNull()) {
                highlightCursor.movePosition(QTextCursor::WordRight,
                                       QTextCursor::KeepAnchor);
                highlightCursor.mergeCharFormat(colorFormat);
            }
        }

        cursor.endEditBlock();



}
}

void MainWindow::on_lineEdit_cursorPositionChanged(int arg1, int arg2)
{

    ui->lineEdit->setStyleSheet("QLineEdit{background:white};");

}



void MainWindow::on_action_triggered()
{
    QMessageBox::information(this,"ინფორმაცია" , "<b>პროგრამა Notepad დაწერილია ნ.ფ მიერ</b>");
}
