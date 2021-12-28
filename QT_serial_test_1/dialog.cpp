#include "dialog.h"
#include "ui_dialog.h"


Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);

    foreach(const QSerialPortInfo port_list,QSerialPortInfo::availablePorts())
    {
        ui->comboBox->addItem(port_list.portName());
    }

}

Dialog::~Dialog()
{
    if(serialport_1->isOpen())
       serialport_1->close();
    delete ui;
}

void Dialog::on_pushButton_clicked()
{
    qDebug()<<"portname is"<<ui->comboBox->currentText();

    serialport_1->setPortName(ui->comboBox->currentText());
    serialport_1->open(QSerialPort::ReadOnly);
    serialport_1->setBaudRate(9600);
    serialport_1->setDataBits(QSerialPort::Data8);
    serialport_1->setParity(QSerialPort::NoParity);
    serialport_1->setStopBits(QSerialPort::OneStop);
    serialport_1->setFlowControl(QSerialPort::NoFlowControl);
    serialport_1->flush();
    Data_string.clear();
    serialport_1->clear();
    Data_string=Data_string+serialport_1->readLine();
    QStringList string_array = Data_string.split("\n");
//    Data_string.clear();
//    serialport_1->clear();
    serialport_1->flush();

    ui->textEdit->setText("Registration Number : ");
    QString current_time_2 = "Date : "+QDateTime::currentDateTime().toString("yy-MM-dd");
    ui->textEdit->append(current_time_2);

    Dialog::connect(serialport_1,SIGNAL(readyRead()),this,SLOT(when_serial_recived()));
}
void Dialog::when_serial_recived()
{
    Data_string=Data_string+serialport_1->readLine();
    QStringList string_array = Data_string.split("\n");
    QString current_time = QDateTime::currentDateTime().toString("hh:mm:ss");

     if(string_array.length()>1)
    {
        //qDebug()<<Data_string;
        ui->textEdit->append(string_array[0].remove(-1,1)+"\t"+current_time);
        Data_string.clear();
        ui->textEdit->verticalScrollBar()->setValue(ui->textEdit->maximumHeight());
    }
    else {
    }
//    if(string_array.length()<=1){
//       sentence=sentence+QString::fromStdString(Data_string.toStdString());
//      if(sentence.contains("\n"))
//        qDebug()<<sentence;
//       Data_string.clear();
//    }
//    else {
//       qDebug()<<sentence;
//       sentence.clear();
//    }

}

void Dialog::on_pushButton_2_clicked()
{
    serialport_1->close();
    Data_string.clear();
}

void Dialog::on_pushButton_3_clicked()
{
    if(serialport_1->isOpen())
    serialport_1->close();

    QString save_file_name;
    static QString Data_strings=ui->textEdit->toPlainText();

    QFileDialog* dialog=new QFileDialog(this);
    //dialog->setNameFilter(tr("allfiles(*.txt)"));
    //getOpenFileName(this,tr("Opneimage","",tr("All files(*.txt)")));

    save_file_name=dialog->getSaveFileName(this,"save file name",QDir::currentPath(),"text_file (*.txt)");
    if(!save_file_name.contains(".txt"))
        save_file_name=save_file_name+"(add).txt";
    /*File.open(QFile::WriteOnly|QFile::Append|QFile::Text)) // 쓰기 전용, 텍스트, 이어쓰기
    QTextStream SaveFile(&File);
    SaveStream<<"저장될내용"<<"\n"<<;  // 맨뒤 \n은 줄바꿈
    SaveStream<<"저장될내용"<<"\n"<<; // 이런식으로 계속 추가 하면된다
    File.close(); // 파일닫기
    [출처] QFile을 이용한 파일 입출력(읽기/쓰기), 어플리케이션 실행한 경로 얻기, 폴더 존재 유무 및 폴더 추가/삭제 방법|작성자 browniz1004*/

    QFile f(save_file_name);
    f.open(QIODevice::WriteOnly|QFile::Append|QFile::Text);
    QTextStream stream_1(&f);
    stream_1<<Data_strings<<"\n";
    f.close();
    qDebug()<<save_file_name;
}
