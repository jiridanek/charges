#ifndef TEST_CONVERT_HPP
#define TEST_CONVERT_HPP

#include <QObject>
#include <QTest>
#include <iostream>

#include "convert.hpp"


class testConvert : public QObject {
  Q_OBJECT
private slots:
  void converttodouble();
};

void testConvert::converttodouble() {
  QCOMPARE(2.5, convertTo<double>("2.5"));
}

QTEST_MAIN(testConvert)
//int main(){std::cout << "asdfffff"; return 1;}
#endif