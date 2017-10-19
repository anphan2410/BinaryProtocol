#include <QCoreApplication>
#include "src/binaryprotocol.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QList<QPair<QString,QByteArray>> testArray;
    BinaryProtocol aTest;
    QString TestContent;
    QByteArray SampleCommand;
    QByteArray ReGenCommand;
    int tmpInt;

    //The binary protocol commands below are taken from the datasheet of UHV2.
    //Their meanings can be checked in the datasheet too.
    qDebug() << "========BINARYPROTOCOL=======================================================";

    testArray.append(QPair<QString,QByteArray>(QString("Read status at the High Voltage 1, PDF-Page 89|8130344130313F7A"),
                                               aTest.SetBPNo(0).Ch1().HVSwitch().Read().GenMsg()));
    testArray.append(QPair<QString,QByteArray>(QString("Power up channel 1 using HV ON/OFF, PDF-Page 92|8130344130313174"),
                                               aTest.SetBPNo(0).HdrCmd().Ch1().HVSwitch().On().GenMsg()));
    //For This Case I Meas Is 5430 But The DataSheet Gives Wrong Information, Checked With Real Pump.
    testArray.append(QPair<QString,QByteArray>(QString("Read current of the channel 2, PDF-Page 93|8130345430323F6C"),
                                               aTest.SetBPNo(0).Ch2().ReadI().GenMsg()));
    testArray.append(QPair<QString,QByteArray>(QString("Read On the High Voltage 1 the Status Of Start/Protect, PDF-Page 94|8130344330313F78"),
                                               aTest.SetBPNo(0).ProtectSwitch().Ch1().Read().GenMsg()));

    for (quint8 i=0; i<testArray.size(); ++i)
    {
        tmpInt = testArray.at(i).first.indexOf('|');
        TestContent = testArray.at(i).first.left(tmpInt);
        SampleCommand = testArray.at(i).first.mid(tmpInt+1).toLocal8Bit();
        ReGenCommand = testArray.at(i).second;
        qDebug().noquote() << TestContent;
        qDebug().noquote() << "   Sample Command:" << SampleCommand;
        qDebug().noquote() << "   Try Regenerate:" << (ReGenCommand.isEmpty()?"NOT APPLICABLE":ReGenCommand.toHex());
        aTest = BinaryProtocol::fromQByteArray(QByteArray::fromHex(SampleCommand));
        qDebug().noquote() << "   Try Translate :" << aTest.GetMessageTranslation();
        qDebug().noquote() << "          Header :" << QByteArray().append(aTest.GetHdr()).toHex() << " -- " << aTest.GetHeader();
        qDebug().noquote() << "         Command :" << QString::number(aTest.GetCmd(),16) << " -- " << aTest.GetCommand();
        qDebug().noquote() << "         Channel :" << QByteArray().append(aTest.GetCh()).toHex() << " -- " << aTest.GetChannel();
        qDebug().noquote() << "            Data :" << aTest.GetData().toHex() << " -- " << aTest.GetDataTranslation();
    }

    qDebug() << "===END OF TEST================================================================";

    return a.exec();
}
