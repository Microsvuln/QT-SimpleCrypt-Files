#include <QCoreApplication>
#include <QDebug>
#include <iostream>
#include <QFile>
#include <QBuffer>
#include <QByteArray>
#include <QTextStream>
#include <QString>
#include <QDataStream>
#include <QDir>
#include <QDirIterator>
#include <QTextStream>
#include "simplecrypt.h"

using namespace std;

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QStringList fileNames;
    QString fileNames2;
    QString dirPath;
    cout << "Enter Dir Path :";
    std::string myStr ;
    std::getline(std::cin, myStr);
    qInfo() << "you entered :" << QString::fromStdString(myStr);
    dirPath = QString::fromStdString(myStr);
    QDir directory(dirPath);
    QStringList filter;
    filter << "*.jpg";
    QDirIterator it(directory.path(), filter, QDir::AllEntries | QDir::NoSymLinks | QDir::NoDotAndDotDot, QDirIterator::Subdirectories);
    while (it.hasNext()) {
        QFile f(it.next());
        f.open(QIODevice::ReadOnly);
        QDataStream in(&f);
        QByteArray  ba = f.readAll();
        f.close();
        fileNames2 = f.fileName() + ".enc";
        QFile file1(fileNames2);
        file1.open(QIODevice::WriteOnly);
        SimpleCrypt crypto(Q_UINT64_C(0x0c2ad4a4acb9f023)); //some random number
        crypto.setCompressionMode(SimpleCrypt::CompressionAlways); //always compress the data, see section below
        crypto.setIntegrityProtectionMode(SimpleCrypt::ProtectionHash);
        QBuffer tempBuffer(&ba);
        tempBuffer.open(QIODevice::WriteOnly);
        QDataStream s(&tempBuffer);
        QByteArray myCypherText = crypto.encryptToByteArray(tempBuffer.data());
        file1.write(myCypherText.data(), myCypherText.size());
        file1.close();


    }



    return a.exec();
    exit(0);
}
