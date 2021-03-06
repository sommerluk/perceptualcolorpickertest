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

// Own header
#include "helper.h"

#include <QPainter>

#include <math.h>

namespace PerceptualColor
{
/** @internal
 *
 * @brief Number of vertical <em>standard</em> wheel steps done by a
 *  wheel event
 *
 * As the QWheelEvent documentation explains, there is a common physical
 * standard wheel step size for mouse wheels: 15°. But there are some
 * mouse models which use non-standard physical wheel step sizes for
 * their mouse wheel, for example because they have a higher wheel
 * resolution.
 *
 * This function converts the values in a QMouseEvent to the
 * <em>standard</em> wheel step count.
 *
 * @param event the QWheelEvent
 * @returns the count of vertical <em>standard</em> wheel steps done
 * within this mouse event. The value is positive for up-steps and
 * negative for down-steps. On a standard mouse wheel, moving the wheel
 * one physical step up will return the value 1. On a non-standard,
 * higher resolution mouse wheel, moving the wheel one physical step up
 * will return a smaller value, for example 0.7 */
qreal standardWheelStepCount(QWheelEvent *event)
{
    // QWheelEvent::angleDelta() returns 8 units for each degree.
    // The standard wheel step is 15°. So on a standard
    // mouse, one wheel step results in (8 × 15) units.
    return event->angleDelta().y() / static_cast<qreal>(8 * 15);
}

/** @internal
 *
 * @brief Type conversion.
 * @param value An LCH value
 * @returns Same LCH value as <tt>cmsCIELCh</tt>. */
cmsCIELCh toCmsCieLch(const LchDouble &value)
{
    cmsCIELCh result;
    result.L = value.l;
    result.C = value.c;
    result.h = value.h;
    return result;
}

/** @internal
 *
 * @brief Type conversion.
 * @param value An LCH value
 * @returns Same LCH value as @ref LchDouble. */
LchDouble toLchDouble(const cmsCIELCh &value)
{
    LchDouble result;
    result.l = value.L;
    result.c = value.C;
    result.h = value.h;
    return result;
}

/** @internal
 *
 * @brief Background for semi-transparent colors.
 *
 * When showing a semi-transparent color, there has to be a background
 * on which it is shown. This function provides a suitable background
 * for showcasing a color.
 *
 * @param devicePixelRatioF The desired device-pixel ratio.
 *
 * @returns An image of a mosaic of neutral gray rectangles of different
 * lightness. You can use this as tiles to paint a background. The image
 * has its device pixel ratio set to the value that was given in the
 * parameter.
 *
 * @note The image is considering the given device-pixel ratio to deliver
 * sharp (and correctly scaled) images also for HiDPI devices.
 * The painting does not use floating point drawing, but rounds
 * to full integers. Therefore, the result is always a sharp image.
 * This function takes care that each square has the same pixel size,
 * without scaling errors or anti-aliasing errors.
 *
 * @sa @ref AbstractDiagram::transparencyBackground()
 *
 * @todo Provide color management support? Currently, we use the same
 * value for red, green and blue, this might <em>not</em> be perfectly
 * neutral gray depending on the color profile of the monitor… */
QImage transparencyBackground(qreal devicePixelRatioF)
{
    // The valid lightness range is [0, 255]. The median is 127/128.
    // We use two color with equal distance to this median to get a
    // neutral gray.
    constexpr int lightnessDistance = 15;
    constexpr int lightnessOne = 127 - lightnessDistance;
    constexpr int lightnessTwo = 128 + lightnessDistance;
    constexpr int squareSizeInLogicalPixel = 10;
    const int squareSize = qRound(squareSizeInLogicalPixel * devicePixelRatioF);

    QImage temp(squareSize * 2, squareSize * 2, QImage::Format_RGB32);
    temp.fill(QColor(lightnessOne, lightnessOne, lightnessOne));
    QPainter painter(&temp);
    QColor foregroundColor(lightnessTwo, lightnessTwo, lightnessTwo);
    painter.fillRect(0, 0, squareSize, squareSize, foregroundColor);
    painter.fillRect(squareSize, squareSize, squareSize, squareSize, foregroundColor);
    temp.setDevicePixelRatio(devicePixelRatioF);
    return temp;
}

/** @internal
 *
 * @brief Round floating point numbers to a certain number of digits
 *
 * @param value the value that will be rounded
 * @param precision the number of decimal places to which rounding takes place
 * @returns the rounded value */
double roundToDigits(double value, int precision)
{
    const double multiplier = std::pow(
        // Make sure that pow returns a double:
        static_cast<double>(10),
        precision);
    return std::round(value * multiplier) / multiplier;
}

/** @internal
 *
 * @brief Invisible marker for rich text
 *
 * Some parts of Qt accept both, plain text and rich text, within the same
 * property. Example: <tt>QWhatsThis</tt> uses <tt>Qt::mightBeRichText()</tt>
 * to decide if a text is treated as rich text or as plain text. But
 * <tt>Qt::mightBeRichText()</tt> is only a raw guess. This situation
 * is not comfortable: You never really know in advance if text will be
 * treated as rich text or as plain text.
 *
 * This function provides a solution. It provides a rich text marker. If
 * your text starts with this marker, it will always be treated
 * as rich text. The marker itself will not be visible in the rendered
 * rich text.
 *
 * Usage example:
 * @snippet test/testhelper.cpp richTextMarkerExample
 *
 * @returns Invisible marker for rich text */
QString richTextMarker()
{
    return QStringLiteral(u"<a/>");
}

} // namespace PerceptualColor
