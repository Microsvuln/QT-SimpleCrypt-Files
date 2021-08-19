#include <QCoreApplication>
#include <QDebug>
#include <iostream>
#include <QFile>
#include <QBuffer>
#include <QByteArray>
#include <QTextStream>
#include <QString>
#include <QDataStream>
#include "simplecrypt.h"
#include "simplecryptdevice.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QFile file("G:/Projects/Qt C++/qtcrypt-files/nn/0517.jpg");
    file.open(QIODevice::ReadOnly);
    QDataStream in(&file);
    QByteArray  ba = file.readAll();
    ///// qInfo() << ba;

    file.close();

    QFile file1("G:/Projects/Qt C++/qtcrypt-files/nn/0517.jpg.enc");
    file1.open(QIODevice::WriteOnly);
    SimpleCrypt crypto(Q_UINT64_C(0x0c2ad4a4acb9f023)); //some random number
    crypto.setCompressionMode(SimpleCrypt::CompressionAlways); //always compress the data, see section below
    crypto.setIntegrityProtectionMode(SimpleCrypt::ProtectionHash);
    QBuffer tempBuffer(&ba);
    tempBuffer.open(QIODevice::WriteOnly);
    QDataStream s(&tempBuffer);

    ///// QDataStream d1(&file1);
    QByteArray myCypherText = crypto.encryptToByteArray(tempBuffer.data());
    ///// qInfo() << myCypherText;
    //// s << myCypherText;4
    file1.write(myCypherText.data(), myCypherText.size());
    ////s << myCypherText;
    file1.close();


    QFile file2("G:/Projects/Qt C++/qtcrypt-files/nn/0517-dec.jpg");
    file2.open(QIODevice::WriteOnly);
    SimpleCrypt crypto2(Q_UINT64_C(0x0c2ad4a4acb9f023)); //same random number: key should match encryption key
    QByteArray plaintext = crypto2.decryptToByteArray(myCypherText);
    QBuffer buffer2(&plaintext);
    buffer2.open(QIODevice::ReadOnly);
    QDataStream s2(&buffer2);
    file2.write(plaintext.data(), plaintext.size());
    //etc.
    buffer2.close();
    file2.close();
    //// qInfo() << myCypherText;
    return a.exec();
    exit(0);
}
