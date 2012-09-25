#ifndef TEST_EEMPARAMS_HPP
#define TEST_EEMPARAMS_HPP

#include <QObject>
#include <QtTest>
#include <QtCore>

#include <sstream>
#include <istream>

#include "verifythrow.hpp"
#include "eemparams.hpp"
#include "invalidparamline.hpp"


class testEEMparams : public QObject {
  Q_OBJECT
private slots:
  void create();
  void initialize();
  void invalidatom();
};

void testEEMparams::create() {
    EEMparams p;
}

void testEEMparams::initialize() {
    EEMparams p;
    std::istringstream ss("kapa 5.2\nC 3  7.4 11.5\nC 2 2.2 3.3");
    ss >> p;
    QCOMPARE(p.Kappa(), 5.2);
    QCOMPARE(p.A(6, 3), 7.4);
    QCOMPARE(p.B(6, 3), 11.5);
    QCOMPARE(p.A(6, 2), 2.2);
    QCOMPARE(p.B(6, 2), 3.3);
}

void testEEMparams::invalidatom() {
    EEMparams p;
    
    std::stringstream ss("kapa 2.2\nJD 1 1 1");
    QVERIFY_THROW(ss >> p, InvalidParamLine);
}

QTEST_MAIN(testEEMparams)

#endif