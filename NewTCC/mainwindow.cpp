#include "mainwindow.h"
#include "./ui_mainwindow.h"

#define ARG(str) "\""+str+"\""

#define TESTSUITE_ARGS		"\"1"+TestSuite+"\" "+\
                            "\"2"+TestCaseID+"\" "+\
                            "\"3"+Functionality+"\" "+\
                            "\"4"+TestDescription+"\" "+\
                            "\"5"+Steps+"\" "+\
                            "\"6"+ER+"\" "+\
                            "\"7"+ValidTCs+"\" "+\
                            "\"8" +Automation+"\" "+\
                            "\"9"+RI+"\" "+\
                            "\"A"+Note+"\"";

#define HIST_ARGS			"\"1"+TestSuite+"\" "+\
                            "\"2"+Functionality+"\" "+\
                            "\"3"+RI+"\" "+\
                            "\"4"+TestCaseID+"\" "+\
                            "\"5"+Type+"\" "+\
                            "\"6"+Date+"\" "+\
                            "\"7"+Author;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    qDebug() << __PRETTY_FUNCTION__ << endl;

    ui->setupUi(this);
    this->initialize();
}

MainWindow::~MainWindow()
{
    qDebug() << __PRETTY_FUNCTION__ << endl;

    QSettings settings("Intel", "Terralogic");
    settings.setValue("Author", ui->author_line->text());
    delete this->testCaseID_line;
    delete this->testCaseID_cb;
    settings.value("Author");
    delete ui;
}

bool MainWindow::initialize() {
    qDebug() << __PRETTY_FUNCTION__ << endl;

    pyW = new Wrapper();
    pyW->createToken();

    QSettings settings("Intel", "Terralogic");
    if (settings.value("Author") != "") {
        QString author = settings.value("Author").toString();
        ui->author_line->setText(author);
    }

    QStringList formType = {"Added New", "Updated", "Deleted"};
    ui->type_cb->addItems(formType);

    QStringList sheets = pyW->getSheets();
    ui->testSuite_cb->addItems(sheets);

    QDateTime date = QDateTime::currentDateTime();
    ui->dateEdit->setDateTime(date);

    this->testCaseID_line = new QLineEdit();
    QString newTestCaseID = this->createTestCaseID();
    this->testCaseID_line->setText(newTestCaseID);
    this->testCaseID_cb = new QComboBox();
    ui->stackedWidget->addWidget(testCaseID_line);
    ui->stackedWidget->addWidget(testCaseID_cb);
    ui->stackedWidget->setCurrentWidget(testCaseID_line);
    int swHeight = ui->stackedWidget->fontMetrics().height();
    ui->stackedWidget->setFixedHeight(swHeight + 10);

    //get font metric height
    int fmhTestDes = ui->testDes_le->fontMetrics().height();
    ui->testDes_le->setFixedHeight(fmhTestDes*2 + 10);
    int fmhExpectResult = ui->er_le->fontMetrics().height();
    ui->er_le->setFixedHeight(fmhExpectResult*3 + 10);
    int fmhNote = ui->note_le->fontMetrics().height();
    ui->note_le->setFixedHeight(fmhNote*3 + 10);

    QStringList validTCs = {"Yes", "No"};
    ui->valid_cb->addItems(validTCs);

    QStringList autoAvailability = {"Not yet", "Can", "Can not", "Done", "Delete"};
    ui->autoAvai_cb->addItems(autoAvailability);

    QObject::connect(ui->pushButton, &QPushButton::pressed, this, &MainWindow::close);
    QObject::connect(ui->pushButton_2, &QPushButton::pressed, this, &MainWindow::Finished);
    QObject::connect(ui->type_cb, &QComboBox::currentTextChanged, this, &MainWindow::changeType);
    QObject::connect(ui->testSuite_cb, &QComboBox::currentTextChanged, this, &MainWindow::changeTestSuite);
    return true;
}

void MainWindow::changeType() {
    qDebug() << __PRETTY_FUNCTION__ << endl;

    if (ui->type_cb->currentText() == "Added New") {
        this->change2New();
        return;
    }
    else
    if ( ui->type_cb->currentText() == "Updated") {
        this->change2Update();
        return;
    }
    this->change2Delete();
    return;
}

void MainWindow::change2New() {
    qDebug() << __PRETTY_FUNCTION__ << endl;

    ui->stackedWidget->setCurrentWidget(this->testCaseID_line);
    QLineEdit* le = static_cast<QLineEdit*>(ui->stackedWidget->currentWidget());
    le->setText(this->createTestCaseID());

    ui->func_le->setReadOnly(false);
    ui->func_le->setText("");

    ui->testDes_le->setReadOnly(false);
    ui->testDes_le->setText("");

    ui->steps_le->setReadOnly(false);
    ui->steps_le->setText("");

    ui->er_le->setReadOnly(false);
    ui->er_le->setText("");

    ui->ri_le->setReadOnly(false);
    ui->ri_le->setText("");

    ui->note_le->setReadOnly(false);
    ui->note_le->setText("");

    ui->valid_cb->setDisabled(false);
    ui->valid_cb->setCurrentText("Yes");

    ui->autoAvai_cb->setDisabled(false);
    ui->autoAvai_cb->setCurrentText("Not yet");

    ui->pushButton_2->setText("Add");
}

void MainWindow::change2Update() {
    qDebug() << __PRETTY_FUNCTION__ << endl;

    this->updateOrDelete();

    ui->func_le->setReadOnly(false);
    ui->testDes_le->setReadOnly(false);
    ui->steps_le->setReadOnly(false);
    ui->er_le->setReadOnly(false);
    ui->ri_le->setReadOnly(false);
    ui->note_le->setReadOnly(false);
    ui->valid_cb->setDisabled(false);
    ui->autoAvai_cb->setDisabled(false);

    ui->pushButton_2->setText("Update");
}

void MainWindow::change2Delete() {
    qDebug() << __PRETTY_FUNCTION__ << endl;

    this->updateOrDelete();

    ui->func_le->setReadOnly(true);
    ui->testDes_le->setReadOnly(true);
    ui->steps_le->setReadOnly(true);
    ui->er_le->setReadOnly(true);
    ui->ri_le->setReadOnly(true);
    ui->note_le->setReadOnly(true);
    ui->valid_cb->setDisabled(true);
    ui->autoAvai_cb->setDisabled(true);

    ui->pushButton_2->setText("Delete");
}

void MainWindow::updateOrDelete() {
    qDebug() << __PRETTY_FUNCTION__ << endl;

    ui->stackedWidget->setCurrentWidget(this->testCaseID_cb);
    QComboBox* cb = static_cast<QComboBox*>(ui->stackedWidget->currentWidget());
    cb->clear();
    QStringList testCaseIDs = pyW->getTestCaseIDs(ui->testSuite_cb->currentText());
    QObject::connect(ui->testSuite_cb, &QComboBox::currentTextChanged, this, &MainWindow::changeTestSuite);
    QObject::connect(this->testCaseID_cb, &QComboBox::currentTextChanged, this, &MainWindow::changeTestCaseID);
    cb->addItems(testCaseIDs);
}

void MainWindow::changeTestSuite() {
    qDebug() << __PRETTY_FUNCTION__ << endl;

    if (ui->type_cb->currentText() == "Added New") {
        static_cast<QLineEdit*>(ui->stackedWidget->currentWidget())->setText(this->createTestCaseID());
    }
    else {
        QComboBox* cb = static_cast<QComboBox*>(ui->stackedWidget->currentWidget());
        cb->clear();
        QStringList testCaseIDs = pyW->getTestCaseIDs(ui->testSuite_cb->currentText());
        cb->addItems(testCaseIDs);
    }
}

void MainWindow::changeTestCaseID() {
    qDebug() << __PRETTY_FUNCTION__ << endl;

    QString testSuite = ui->testSuite_cb->currentText();

    QString testCaseID = static_cast<QComboBox*>(ui->stackedWidget->currentWidget())->currentText();
    if (testCaseID == "") {
        return;
    }

    QString data = pyW->getData(ARG(testSuite), ARG(testCaseID));
    data = data.left(data.length()-1);
    qDebug() << "------------------------------------------" << endl;
    qDebug() << data << endl;
    qDebug() << "------------------------------------------" << endl;
    QRegExp separator("\',\\s\'|\',\\s\"|\",\\s\'|\",\\s\"");
    QStringList dataList = data.split(separator);
    qDebug() << dataList << endl;
    ui->func_le->setText(dataList[1]);

    ui->testDes_le->setAcceptRichText(true);
    ui->testDes_le->setText("");
    for (auto line : dataList[2].split("\\n")) {
        ui->testDes_le->append(line);
    }
    ui->steps_le->setText("");
    ui->steps_le->setAcceptRichText(true);
    for (auto line : dataList[3].split("\\n")) {
        ui->steps_le->append(line);
    }
    ui->er_le->setText("");
    ui->er_le->setAcceptRichText(true);
    for (auto line : dataList[4].split("\\n")) {
        ui->er_le->append(line);
    }
    ui->valid_cb->setCurrentText(dataList[5]);
    ui->autoAvai_cb->setCurrentText(dataList[6]);
    ui->ri_le->setText("");
    if (dataList.length()>=8)
        ui->ri_le->setText(dataList[7]);
    ui->note_le->setText("");
    if (dataList.length()==9) {
        for (auto line : dataList[8].split("\\n")) {
            ui->note_le->append(line);
        }
    }
}

QString MainWindow::createTestCaseID() {
    qDebug() << __PRETTY_FUNCTION__ << endl;

    QString args = ARG(ui->testSuite_cb->currentText());
    QStringList testCaseIDs = pyW->getTestCaseIDs(args);
    QString latestID = testCaseIDs.last();
    qint16 length = latestID.length();
    if (latestID.at(length-1) <'0' || latestID.at(length-1) > '9') {
        latestID = testCaseIDs[testCaseIDs.length()-2];
        length = latestID.length();
    }
    qint16 index = -1;
    qint16 num = 0;
    for (int i = length-1; i >= 0; --i) {
        if (latestID.at(i) < '0' || latestID.at(i) > '9') {
            index = i;
            break;
        }
    }
    if (index != -1) {
        for (int i = index+1; i < length; ++i) {
            num += num*10 + latestID.at(i).toLatin1()-'0';
        }
        return latestID.left(index+1)+QString::number(num+1);
    }
    else
        return "";
}

void MainWindow::Finished() {
    {
        qDebug() << __PRETTY_FUNCTION__ << endl;

        QString Author = ui->author_line->text();
        QString Type = ui->type_cb->currentText();
        QString Date = ui->dateEdit->text();
        QString TestSuite = ui->testSuite_cb->currentText();
        QString TestCaseID;
        QString Functionality = ui->func_le->text();
        QString TestDescription = ui->testDes_le->toPlainText();
        QString Steps = ui->steps_le->toPlainText();
        QString ER = ui->er_le->toPlainText();
        QString RI = ui->ri_le->text();
        QString Note = ui->note_le->toPlainText();
        QString ValidTCs = ui->valid_cb->currentText();
        QString Automation = ui->autoAvai_cb->currentText();

        QString result;

        if (ui->pushButton_2->text() == "Add") {
            TestCaseID = static_cast<QLineEdit*>(ui->stackedWidget->currentWidget())->text();
            QString add_args = TESTSUITE_ARGS;
            result = pyW->Add(add_args);
        }
        if (ui->pushButton_2->text() == "Update") {
            TestCaseID = static_cast<QComboBox*>(ui->stackedWidget->currentWidget())->currentText();
            QString update_args = TESTSUITE_ARGS;
            result = pyW->Update(update_args);
        }

        if (ui->pushButton_2->text() == "Delete") {
            TestCaseID = static_cast<QComboBox*>(ui->stackedWidget->currentWidget())->currentText();
            QString delete_args = TESTSUITE_ARGS;
            result = pyW->Delete(delete_args);
        }
        if (result.trimmed() == "Success") {
            QString hist_args = HIST_ARGS;
            result = pyW->updateHistory(hist_args);
        }
        QMessageBox msgBox;
        msgBox.setText(result.trimmed());
        msgBox.exec();
    }
}
