﻿// SPDX-License-Identifier: MIT
/*
 * Copyright (c) 2020 Lukas Sommer sommerluk@gmail.com
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following
 * conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 */

#include "PerceptualColor/perceptualcolorglobal.h"
#include "perceptualcolorinternal.h"

// First included header is the public header of the class we are testing;
// this forces the header to be self-contained.
#include "PerceptualColor/rgbcolorspacefactory.h"

#include <QtTest>

#include "PerceptualColor/chromahuediagram.h"
#include "PerceptualColor/colorwheel.h"
#include "rgbcolorspace.h"

static void snippet01()
{
    //! [Create]
    QSharedPointer<PerceptualColor::RgbColorSpace> myColorSpace =
        // Create the color space object with the factory class.
        // This call might be slow.
        PerceptualColor::RgbColorSpaceFactory::createSrgb();

    // These calls are fast:

    PerceptualColor::ChromaHueDiagram *myDiagram =
        // Create a widget with the color space:
        new PerceptualColor::ChromaHueDiagram(myColorSpace);

    PerceptualColor::ColorWheel *myWheel =
        // Create another widget with the very same color space:
        new PerceptualColor::ColorWheel(myColorSpace);
    //! [Create]

    delete myDiagram;
    delete myWheel;
}

namespace PerceptualColor
{
class TestRgbColorSpaceFactory : public QObject
{
    Q_OBJECT

public:
    TestRgbColorSpaceFactory(QObject *parent = nullptr)
        : QObject(parent)
    {
    }

private Q_SLOTS:
    void initTestCase()
    {
        // Called before the first test function is executed
    }

    void cleanupTestCase()
    {
        // Called after the last test function was executed
    }

    void init()
    {
        // Called before each test function is executed
    }

    void cleanup()
    {
        // Called after every test function
    }

    void testCreate()
    {
        QSharedPointer<PerceptualColor::RgbColorSpace> temp = RgbColorSpaceFactory::createSrgb();
        QCOMPARE(temp.isNull(), false); // This is no nullptr!
        // Make a random call, just to be sure that a method call will not crash:
        temp->profileInfoDescription();
        // Make sure the returned value is actually the sRGB gamut.
        QCOMPARE(temp->profileInfoDescription(), QStringLiteral("sRGB color space"));
    }

    void testSnipped01()
    {
        snippet01();
    }
};

} // namespace PerceptualColor

QTEST_MAIN(PerceptualColor::TestRgbColorSpaceFactory)

// The following “include” is necessary because we do not use a header file:
#include "testrgbcolorspacefactory.moc"
