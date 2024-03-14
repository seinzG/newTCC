#include "wrapper.h"

Wrapper::Wrapper() {}

QString Wrapper::callScript(QString script, QString args) {
    QProcess process;
    process.setProcessChannelMode(QProcess::MergedChannels);
    QString exe = "python "+script+" "+args;
    process.start(exe);
    process.waitForFinished(-1);
    QString output(process.readAllStandardOutput());
    return output;
}

QString Wrapper::createToken() {
    return callScript("createToken.py", "");
}

QStringList Wrapper::getSheets() {
    QString sResult = callScript("getSheets.py", "");
    sResult.remove('\r');
    QStringList slResult = sResult.left(sResult.length()-1).split("\n");
    if (slResult.length() >= 3) {
        //Summary
        slResult.removeFirst();
        //Test Coverage
        slResult.removeFirst();
        //Update History
        slResult.removeFirst();
    }
    return slResult;
}

QStringList Wrapper::getTestCaseIDs(QString testSuite) {
    QString sResult = callScript("getTestCaseID.py", testSuite);
    sResult.remove('\'');
    sResult = sResult.mid(1,(sResult.length() - 2));
    sResult = sResult.left(sResult.length()-1);
    return sResult.split("] [");
}

QStringList Wrapper::parseResult(QString result) {
    result.remove('\'');
    result = result.left(result.length() - 2);
    return result.split("] [");
}

QString Wrapper::Add(QString args) {
    QString result = callScript("add.py", args);
    return result;
}

QString Wrapper::Update(QString args) {
    QString result = callScript("update.py", args);
    return result;
}

QString Wrapper::Delete(QString args) {
    QString result = callScript("delete.py", args);
    return result;
}

QString Wrapper::updateHistory(QString args) {
    QString result = callScript("updateHistory.py", args);
    return result;
}

QString Wrapper::getData(QString testSuite, QString testCaseID) {
    QString args = testSuite + " " + testCaseID;
    return callScript("read.py", args);
}
