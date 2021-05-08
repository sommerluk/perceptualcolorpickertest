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

/** @mainpage
 * This library provides various Qt GUI components for choosing colors, with
 * focus on an intuitive and perceptually uniform presentation. The GUI
 * widgets are based internally on the LCh color model, which does reflect
 * the human perception much better than RGB or its transforms like HSV.
 * However, the widgets do not require the user itself to know anything
 * about LCh at all, because the graphical representations is
 * intuitive enough.
 *
 * How to get started? @ref PerceptualColor::ColorDialog provides a
 * perceptual replacement for QColorDialog:
 * @snippet test/testcolordialog.cpp ColorDialog Get color
 *
 * @image html ColorDialog.png "ColorDialog" width=500
 *
 * And there are also individual widgets available. Among others:
 * - @ref PerceptualColor::WheelColorPicker (a full-featured color wheel)
 * - @ref PerceptualColor::ColorPatch (to show a particular color)
 * - @ref PerceptualColor::ChromaHueDiagram (for selecting colors at a
 *   given lightness)
 *
 * @section namespacepollution Namespace pollution and collisions:
 * - Macros are prefixed with <tt>PERCEPTUALCOLOR_</tt> to avoid collisions.
 *   (See <em>Files → File members</em> for a list of all macros.)
 * - To avoid namespace pollution, all symbols of this library live within
 *   the namespace @ref PerceptualColor.
 *
 * This library relies internally on LittleCMS for all the color management.
 * Anyway, you can uses this library without knowing about the internals of
 * LittleCMS.
 *
 * @sa @ref compile
 * @sa @ref hidpisupport
 *
 * @copyright Almost all the code is published under MIT License.
 * Only <tt>cmake/Modules/FindLCMS2.cmake</tt> is licenced under BSD-3-Clause
 * license. The <tt>LICENSES</tt> subfolder contains copies of the licence
 * texts.
 *
 * @internal
 *
 * @todo Test also on Windows. (Does it work well with VisualStudio?
 * Isn’t there a free version of VisualStudio? Test it with this
 * library!)
 *
 * @todo Test opaque RGB color space object with a non-export-all version
 * of this library to make sure it actually works for third-party developers.
 *
 * @todo Would it make sense for @ref PerceptualColor::ChromaHueDiagram and
 * @ref PerceptualColor::ChromaLightnessDiagram to split up their property
 * <tt>currentColor</tt> into two properties: A two-dimensional property
 * for what the user can change, and a one-dimensional property
 * for what only the programmer can change? Or at least provide
 * a Q_INVOKABLE getter and maybe also setter support? So
 * @ref PerceptualColor::WheelColorPicker could use this
 * instead of a lambda expression to set the hue of the
 * @ref PerceptualColor::ChromaLightnessDiagram. And: Also when we don’t do
 * that: When setting <tt>currentColor</tt> to an out-of-gamut color,
 * what happens? Does @ref PerceptualColor::ChromaHueDiagram preserve
 * lightness, while @ref PerceptualColor::ChromaLightnessDiagram preserves
 * hue? Would this make sense?
 *
 * @todo Paint grayed-out handles for all widgets when setEnabled(false)
 * is used! For example 25% lightness instead of black. And 75%
 * lightness instead of white. But: Provide this information
 * in @ref PerceptualColor::AbstractDiagram!
 *
 * @todo Provide a link to (the online version of) this page in README.md
 *
 * @todo The doxygen documentation should not recommand <tt>filename.h</tt>
 * but <tt>PerceptualColor/filename.h</tt>!?
 *
 * @todo Review and unit tests for these classes:
 * @ref PerceptualColor::ChromaLightnessDiagram,
 * @ref PerceptualColor::RgbColorSpace
 *
 * @todo It might be interesting to use <tt>QStyle::PM_FocusFrameHMargin</tt>
 * <em>(Horizontal margin that the focus frame will outset the widget
 * by.)</em> Or: <tt>QStyle::PM_FocusFrameVMargin</tt>. Using this for the
 * distance between the focus indicator and the actual content of the widget
 * maybe give a more <tt>QStyle</tt> compliant look. But: If using this,
 * ensurePolished() must be called before!
 *
 * @todo Use <tt>explicit</tt> on all constructors?
 *
 * @todo Screen picker with magnifier glass in two steps
 * similar to https://colorsnapper.com ?
 *
 * @todo Multi-licensing? Add Boost licence and Unlicense as an additional
 * choise?
 *
 * @todo The image cache for the gamut widgets should be updated
 * asynchronously (in its own thread or even various own threads
 * in parallel). While waiting for the result, an empty image could be used.
 * Or it might be useful to provide first a low-resolution version, and only
 * later-on a high-resolution version. Anyway, KDE provides an interesting
 * recommandation: <tt>int Units::humanMoment = 2000;</tt> <em>Time in
 * milliseconds equivalent to the theoretical human moment, which can be
 * used to determine whether how long to wait until the user should be
 * informed of something, or can be used as the limit for how long something
 * should wait before being automatically initiated. / Some examples: /
 * When the user types text in a search field, wait no longer than this
 * duration after the user completes typing before starting the search /
 * When loading data which would commonly arrive rapidly enough to not
 * require interaction, wait this long before showing a spinner</em> See
 * https://api.kde.org/frameworks/plasma-framework/html/classUnits.html#ab22ad7033b2e3d00a862650e82f5ba5e
 * for details.
 *
 * @todo HLC @ref PerceptualColor::MultiSpinBox Allow entering (on the
 * keyboard) of too big hues (361°), negatif hues (-1°), negatif chroma (-20)
 * and too big chroma (201 or 256) – but do not allow this with the arrows
 * (and how do the arrows react when currently one of these values is
 * shown?). Does this make sense? Anyway do <em>not</em> allow this for
 * lightness, because the lightness is <em>by definition</em> bound
 * to <tt>[0, 100]</tt>.
 *
 * @todo Multi-threaded application of color transforms. It seems okay to
 * create the color transforms in one thread and use the same color
 * transform (once created) from various other threads at the same time
 * as long as the flag <tt>cmsFLAGS_NOCACHE</tt> is used to create the
 * transform.
 *
 * @todo Automatically scale the thickness of the wheel (and maybe even the
 * handle) with varying widget size?
 *
 * @todo Support more color spaces? https://pypi.org/project/colorio/ for
 * example supports a lot of (also perceptually uniform) color spaces…
 *
 * @todo Which symbols should finally be exported?
 *
 * @todo Check in all classes that take a @ref PerceptualColor::RgbColorSpace
 * that the shared pointer is actually not a <tt>nullptr</tt>. If is
 * <em>is</em> a <tt>nullptr</tt> than throw an exception. Throwing the
 * exception early might make error detection easier for users of the library.
 *
 * * @todo Avoid default arguments like <tt>void test(int i = 0)</tt> in
 * public headers, as changes require re-compilation of the client application
 * to take effect, which might lead to a miss-match of behaviour between
 * application and library, if  compile-time and run-time version of the
 * library are not the same. Is the problem  for default constructors
 * like <tt>ClassName() = default</tt> similar?
 *
 * @todo mark all public non-slot functions with Q_INVOKABLE (except property
 * setters and getters)
 *
 * @todo A good widget library should
 * - run on all systems (✓ We do not use system-specific code nor do we
 *   rely on byte order)
 * - support hight-dpi (? work in progress)
 * - stylable by QStyle (? partial)
 * - stylable by style sheets (✗)
 * - accessibility (✗)
 *
 * @todo A good widget library should also be touchscreen-ready.
 * - An alternative to
 *   the spin box? How, for up to 360 values (degrees in step by 1)? Or
 *   should the steps simply be bigger?
 *
 * @todo KDE Frameworks / https://marketplace.qt.io/ ?
 * https://community.kde.org/Incubator
 *
 * @todo Provide property bindings as described in
 * https://www.qt.io/blog/property-bindings-in-qt-6 or not? It is worth
 * when we do not support QML? What are the pitfalls? Imagine a property
 * that holds a percent value from 0 to 100; the setter enforces this
 * range; the binding bypasses the setter and allows every value? And:
 * How can callbacks know about when a setter was called in C++? See
 * also: https://doc.qt.io/qt-5/qtqml-cppintegration-exposecppattributes.html
 * and https://doc.qt.io/qt-5/qtqml-tutorials-extending-qml-example.html and
 * http://blog.aeguana.com/2015/12/12/writing-a-gui-using-qml-for-a-c-project/
 * for interaction between QML and C++. Pitfalls: Example of color() property
 * stored internally at m_color: Much implementation code of the clas will
 * access directly m_color instead of color(), so when using bindings,
 * this code is broken?
 *
 * @todo Provide QML support so that for
 * https://doc.qt.io/qt-5/qml-qtquick-dialogs-colordialog.html (or its
 * Qt6 counterpart) we provide a source compatible alternative, like for
 * QColorWidget? Split the library in three parts (Common, Widgets, QML)?
 *
 * @todo Apparently QWidget cannot be used from QML. (Though there is
 * https://www.kdab.com/declarative-widgets/ – how does that work?) Is it
 * therefore worth to have complete support for signals in all our QWidget
 * code if this is not really necessary for QWidget (for example for
 * properties that can only be changed programatically and not by the
 * user)?
 *
 * @todo Comply with KDE policies: https://community.kde.org/Policies
 *
 * @todo Remove all qDebug calls from the source
 *
 * @todo Use QObject::tr() for translations.
 * Provide po files? How can a library be localized?
 * See https://community.kde.org/Frameworks/Frameworks_Localization_Policy for
 * ideas.
 *
 * @todo Qt6 property bindings (QProperty QPropertyBinding) for all
 * properties?
 *
 * @todo Is Qt 5.6 actually enough?. Even if so, wouldn’t it
 * be better to require the last LTS release (5.15), just to be compatible if
 * in the future we depend on this?
 *
 * @todo Translations: Color picker/Select Color → Farbwähler/Farbauswahl etc…
 *
 * @todo Qt Designer support for the widgets. Quote from a blog from Viking
 * about Qt Designer plugins:
 * The problem is that you have to build it with exactly the same compiler
 * tool chain as designer was built with, and you have to do it in release
 * mode. Unless your Qt is built in debug, then your plugin needs to be
 * built in debug mode as well. So you can’t just always use the same
 * compiler as you build the application with, if you use the system Qt or
 * a downloaded Qt version. It’s easier, though, if all developers work with
 * a custom built Qt. But these days it’s rare to see that.
 *
 * @todo Use <a href="https://lvc.github.io/abi-compliance-checker/">
 * abi-compliance-checker</a> to control ABI compatibility.
 *
 * @todo It might be useful to support for all widgets grayed out appearance
 * when they are disabled. Just fade out, maybe with some transparency, and
 * let colors still be visible, would be a bad idea: It would be highly
 * confusing seeing colors, but colors that are wrong. So everything
 * would have to be gray. For @ref PerceptualColor::ColorPatch and
 * @ref PerceptualColor::GradientSlider this could be simply the default
 * background, similar to grayed out spin boxes. And for the diagram
 * widgets, only the shape would stay visible, with uniform standard
 * gray colors coming from <tt>QStyle</tt>. The handles might not
 * even be displayed at all.
 *
 * @todo Follow KDE's <a href="https://hig.kde.org/index.html">HIG</a>
 *
 * @todo Add screenshots of widgets to the documentation
 *
 * @todo Test linking against lcms.h in version 2.0.0 for compatibility
 * (or require more recent version?)
 *
 * @todo Require (by static cast additional to CMake conditions) a minimum
 * Qt version?
 *
 * @todo Would it be a good idea to implement Q_PROPERTY RESET overall? See
 * also https://phabricator.kde.org/T12359
 *
 * @todo Better design on small widget sizes for the whole library.
 *
 * @todo Anti-aliasing the gamut diagrams? Wouldn't this be bad for
 * performance?
 *
 * @todo Use a cross-hair cursor on @ref PerceptualColor::ChromaHueDiagram
 * and @ref PerceptualColor::ChromaLightnessDiagram when the mouse is
 * hovering over the gamut, to show that this surface can be clicked?
 *
 * @todo Touch-friendly interface: Would it be good to have buttons for
 * plus and minus on the various LCH axis which would be companings
 * for @ref PerceptualColor::ChromaHueDiagram and
 * @ref PerceptualColor::ChromaLightnessDiagram and would allow
 * more exactly choose colors also on touch devices?
 *
 * @todo Would it be a good idea to have plus and minus buttons that
 * manipulate the current color along the depth and vividness axis
 * as proposed in “Extending CIELAB - Vividness, V, depth, D, and clarity, T”
 * by Roy S. Berns?
 *
 * @todo Spell checking for the documentation */

/** @page hidpisupport High DPI support
 * This library supports High DPI out of the box. You do not need to do
 * much to use it. The widgets provide High DPI support automatically.
 *
 * The only problem are icons. Icons are used for
 * @ref PerceptualColor::MultiSpinBox::addActionButton and for
 * the “refresh” icon and (on some widget styles) for the “Ok”
 * button and the “Cancel” button in @ref PerceptualColor::ColorDialog.
 *
 * @section loadicons Load icons
 *
 * This library uses by default a possibly existing icon theme
 * if available in Qt.
 *
 * - Windows and Mac do not provide icon themes by default, while Linux
 *   usually provides them.
 *
 * - You might bundle icons (as resource) with your application.
 *
 * There are different file formats for icon themes:
 *
 * - Loading raster image icons is supported out-of-the-box by Qt.
 *
 * - Loading SVG icons is supported by Qt’s SVG icon
 *   support plugin. (On Linux this is the file
 *   <tt>plugins/iconengines/libqsvgicon.so</tt>). This
 *   plugin is loaded by Qt automatically if present.
 *
 * SVG is pretty much the standard nowadays and the only
 * reliably way to have crisp icons also on desktop scales like 1.25 or 1.5.
 * Make sure that the plugin is present if you want that SVG icons
 * can be loaded. (On Linux, it seems possible to enforce this by linking
 * dynamically to the plugin itself, if you want to. This forces Linux
 * package managers to produce packages of your application that depend
 * not only on Qt base, but also on the SVG plugin.)
 *
 * If no external icon can be loaded by Qt, this library uses hard-coded
 * fallback icon where necessary.
 *
 * @section rendericons Render icons
 *
 * - <a href="https://bugreports.qt.io/browse/QTBUG-89279">Qt6 renders icons
 *   always with high-DPI.</a>
 * - Qt5 renders icons by default in low resolution. This applies even
 *   for SVG icons on high-DPI displays! Application developers can enable
 *   high-DPI icon rendering with the following code (which should be put
 *   by convention <em>before</em> creating the <tt>QCoreApplication</tt>
 *   object):
 *   <br/><tt>QCoreApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);</tt>
 *
 * Exception: The hard-coded fallback icons of this library render <em>always
 * and on all Qt versions (even if no SVG support is available at all
 * in Qt)</em> at high-DPI! */

/** @internal
 *
 * @page datatypes Data types
 *
 * The library uses in general <tt>int</tt> for integer values, because
 * <tt>QSize()</tt> and <tt>QPoint()</tt> also do. As the library relies
 * heavily on the usage of <tt>QSize()</tt> and <tt>QPoint()</tt>, this
 * seems reasonable.
 *
 * For the same reason, it uses generally <tt>qreal</tt>
 * for floating point values, because <tt>QPointF()</tt> also does.
 *
 * Output colors that are shown on the screen, are usually 8-bit-per-channel
 * colors. For internal transformation, usually <tt>qreal</tt>
 * is used for each channel, giving a better precision and reducing rounding
 * errors. */

/** @page compile Build instructions and requirements
 * The library depends on (and therefore you has to link against):
 *
 * |                         | Qt                 | LittleCMS               |
 * | :---------------------- | :----------------- | :---------------------- |
 * | <b>Major release</b>    | 5                  | 2                       |
 * | <b>Minimum version</b>  | ≥ 5.6*             | ≥ 2.0                   |
 * | <b>Required modules</b> | Core, Gui, Widgets | <em>not applicable</em> |
 *
 * <em>* Qt 5.6 introduces <tt>QPaintDevice::devicePixelRatioF()</tt> which is
 * used in this library.</em>
 *
 * This library requires minimum C++17.
 * <!--
 *      Qt 5.6 (which is the mimimum Qt version required
 *      by this library) only requires C++03. Only starting
 *      with Qt 5.7, Qt itself requires C++11. Source:
 *      https://doc.qt.io/qt-5.9/cmake-manual.html#using-qt-5-with-cmake-older-than-3-1-0
 *
 *      Qt 6 requires minimum C++17, as
 *      https://doc-snapshots.qt.io/qt6-dev/cmake-get-started.html
 *      explains.
 *
 *      Our library code uses C++11 features, for example “constexpr”.
 *
 *      In the CMakeLists.txt file, we set -std=c++17 and we set
 *      also -Wpedantic and -pedantic-errors to enforce it. That is
 *      a useful option for this library if we decide to make it Qt-6-only.
 *      But it is even be useful if we support Qt 5, so we have future-proof
 *      requirements that we do not have to raise soon, and that are a
 *      good base for LTS.
 * -->
 *
 * To compile this library, both the input character set and the execution
 * character set have to be UTF8. (See @ref characterset for the reasons
 * behind this choise.)
 *
 * You also need CMake for the build process.
 *
 * @internal
 *
 * @todo Provide detailed build instructions.
 *
 * @todo Provide a CMake find module for this library and install it. */

/** @internal
 *
 * @page pimpl Pointer to implementation idiom
 *
 * This library uses the <em>pointer to implementation</em> idiom
 * (also known as pimpl idiom, d-pointer idiom or opaque-pointer idiom)
 * in almost all classes that are part of the public API, and also in
 * some classes that are part of the private API.
 *
 * This idiom is also used by Qt itself, and Qt even provides some macros
 * and extension points (<tt>Q_DECLARE_PRIVATE</tt>, <tt>Q_D</tt>, a protected
 * member called <tt>d_ptr</tt> in almost all classes…), that help dealing
 * with the pimpl idiom. Though available, these Qt features are not
 * officially documentated; and they would also interfer with private
 * implementations of Qt itself without documented behaviour, which seems
 * inappropriate. Furthermore, the Qt pimpl idiom is complicate because
 * it uses (for performance reasons) inheritance between the private
 * implementation classes. This breaks, however, the encapsulation, because
 * all formerly private elements of a class become protected know. Our class
 * hierarchy is not that deep, so the performance gain might not be worth
 * the additional code complexity. Therefore, this library uses a more simple
 * pimpl idiom without inheritance of the private implementation. It has
 * however all the other features of the Qt pimpl idiom, including
 * <tt>const</tt> propagating access to the private implementation
 * thanks to @ref PerceptualColor::ConstPropagatingUniquePointer and
 * @ref PerceptualColor::ConstPropagatingRawPointer. And, at difference
 * to Qt’s pimpl idiom, it keeps private code strictly private.
 * Note however, that switching later from our current pimpl idiom to
 * the polymorph Qt pimpl idiom would break the binary
 * compatibility. See also the document <em>
 * <a href="https://accu.org/journals/overload/18/100/love_1718/">Interface
 * Versioning in C++</a></em> and KDE’s information document <em>
 * <a
 * href="https://community.kde.org/Policies/Binary_Compatibility_Issues_With_C%2B%2B">
 * Binary Compatibility Issues With C++</a></em> and for details.
 *
 * @note While it might be nice to have the d_pointer and q_pointer
 * be themselfs be declared <tt>const</tt>, because this would
 * clearly communicate that those pointers are not expected to change
 * the adress they point to. Unfortunely, apparently this does not
 * work with neither @ref PerceptualColor::ConstPropagatingUniquePointer nor
 * @ref PerceptualColor::ConstPropagatingRawPointer as it would change also
 * all the access rights to the pointed object to always <tt>const</tt>. */

/** @internal
 *
 * @page measurementdetails Measurement details
 *
 * When this library deals with raster graphics, it simultaniously uses
 * concepts concerning measurement. This page describes the terminology
 * used within the documentation of this library.
 *
 * @section introduction Introduction
 * Today’s displays have a wide range of physical pixel density (pixels
 * per length). Displays with a high physical pixel density are called
 * <b>High-DPI displays</b> or <b>HiDPI displays</b> or <b>Retina displays</b>.
 *
 * @section unitsofmeasurement Units of measurement
 * As Qt docuemntation says:
 *      “<em>Qt uses a model where the application coordinate system is
 *      independent of the display device resolution. The application
 *      operates in </em>device-independent pixels<em>, which are then
 *      mapped to the physical pixels of the display via a scale
 *      factor, known as the </em>device pixel ratio<em>.</em>”
 *
 * So when rendering widgets, there are two different units of measurement
 * to consider:
 * - <b>Device-indepentend pixels</b> are the  unit of measurement for
 *   widgets, windows, screens, mouse events and so on in Qt.
 * - <b>Physical pixels</b> are the unit that measures actual physical
 *   display pixels.
 *
 * The conversion factor between these two units of measurement is
 * <tt>QPaintDevice::devicePixelRatioF()</tt>, a floating point number.
 * It is usually <tt>1.00</tt> on classic low resolution screens. It could be
 * for example <tt>1.25</tt> or <tt>2.00</tt> on displays with a higher
 * pixel density.
 *
 * @section coordinatepointsversuspixelpositions Coordinate points versus pixel positions
 *
 * - <b>Coordinate points</b> are points in the mathematical sense, that
 *   means they have zero surface. Coordinate points should be stored as
 *   <em>floating point numbers</em>.
 * - <b>Pixel positions</b> describe the position of a particular pixel
 *   within the pixel grid. Pixels are surfaces, not points. A pixel is a
 *   square of the width and length <tt>1</tt>. The pixel at position
 *   <tt>QPoint(x, y)</tt> is the square with the top-left edge at coordinate
 *   point <tt>QPoint(x, y)</tt> and the botton-right edge at coordinate
 *   point <tt>QPoint(x+1, y+1)</tt>. Pixel positions should be stored
 *   as <em>integer numbers</em>.
 *
 * Some functions (like mouse events) work with pixel positions, other
 * functions (like antialiased floatting-point drawing operations) work
 * with coordinate points. It’s important to always distinguish correctly
 * these two different concepts. See https://doc.qt.io/qt-6/coordsys.html
 * for more details about integer precision vs floating poin precision
 * on drawing operations. */

/** @internal
 *
 * @page codingstyle Coding style
 *
 * - Document your code.
 * - Provide unit tests for your code.
 * - If working with children within Qt’s object hierarchy, allocate on the
 *   heap and use raw pointers or guarded pointers (`QPointer`). If not,
 *   allocate on the stack or use smart pointers. Prefer Qt’s smart pointers
 *   over the <tt>std</tt> smart pointers of C++. */

/** @brief The namespace of this library.
 *
 * All symbols that are provided in this library are encapsulated within this
 * namespace. */
namespace PerceptualColor
{
} // namespace PerceptualColor
